#include <QPixmap>
#include <QMessageBox>

#include "stdlib.h"
#include "mpdf.h"
#include "storage.h"

Mpdf::Mpdf(QWidget *parent) :
    QWidget(parent)
{
    initTop1();
    initPdf();
    initTop2();
    initTop3();

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(top1Layout);
    mainLayout->addLayout(top2Layout);
    mainLayout->addLayout(top3Layout);
    mainLayout->addLayout(pdfLayout);
    setLayout(mainLayout);
   
    hideTop();

    setMouseTracking(true);
}

Mpdf::~Mpdf()
{
    deletePdf();
}

void Mpdf::initTop1()
{
    closeButton = new QPushButton;
    closeButton->setText("X");
    closeButton->setMaximumWidth(30);

    spacerItem1 = new QSpacerItem(30, 30, 
            QSizePolicy::Expanding, QSizePolicy::Minimum);

    top1Layout = new QHBoxLayout;
    top1Layout->addWidget(closeButton);
    top1Layout->addItem(spacerItem1);

    connect(closeButton, SIGNAL(clicked()),
            this, SLOT(closeClicked()));
}

void Mpdf::initTop2()
{
    pageLabel = new QLabel;
    pageLabel->setText("Pages");

    prevButton = new QPushButton;
    prevButton->setText("prev");
    prevButton->setMinimumWidth(30);
    prevButton->setFlat(true);

    top2Layout = new QHBoxLayout;
    top2Layout->addWidget(pageLabel);
    top2Layout->addWidget(prevButton);

    numberButtonVec = new QVector<QPushButton *>;
    signalMapper = new QSignalMapper(this);

    for (int i = 0; i < 9; i++)
    {
        QPushButton *numberButton = new QPushButton;
        numberButton->setText(QString::number(i + 1));
        numberButton->setMinimumWidth(15);
        numberButton->setFlat(true);
        numberButtonVec->push_back(numberButton);
        top2Layout->addWidget(numberButton);

        signalMapper->setMapping(numberButton, i);
        connect(numberButton, SIGNAL(clicked()),
            signalMapper, SLOT(map()));
    }
    
    nextButton = new QPushButton;
    nextButton->setText("next");
    nextButton->setMinimumWidth(30);
    nextButton->setFlat(true);
    top2Layout->addWidget(nextButton);

    searchButton = new QPushButton;
    searchButton->setText("Search");
    top2Layout->addWidget(searchButton);

    connect(prevButton, SIGNAL(clicked()),
            this, SLOT(prevClicked()));
    connect(signalMapper, SIGNAL(mapped(int)),
            this, SLOT(numberClicked(int)));
    connect(nextButton, SIGNAL(clicked()),
            this, SLOT(nextClicked()));
    connect(searchButton, SIGNAL(clicked()),
            this, SLOT(searchClicked()));
}

void Mpdf::initTop3()
{
    zoominButton = new QPushButton(scrollArea);
    zoominButton->setText("+");
    zoominButton->setMaximumWidth(30);

    zoomoutButton = new QPushButton(scrollArea);
    zoomoutButton->setText("-");
    zoomoutButton->setMaximumWidth(30);

    spacerItem2 = new QSpacerItem(30, 30, 
            QSizePolicy::Expanding, QSizePolicy::Minimum);

    searchLine = new QLineEdit;
    goButton = new QPushButton;
    goButton->setText("Go");

    top3Layout = new QHBoxLayout;
    top3Layout->addWidget(zoominButton);
    top3Layout->addWidget(zoomoutButton);
    top3Layout->addItem(spacerItem2);
    top3Layout->addWidget(searchLine);
    top3Layout->addWidget(goButton);

    searchView = false;

    connect(zoominButton, SIGNAL(clicked()),
            this, SLOT(zoominClicked()));
    connect(zoomoutButton, SIGNAL(clicked()),
            this, SLOT(zoomoutClicked()));
    connect(goButton, SIGNAL(clicked()),
            this, SLOT(goClicked())); 
}

void Mpdf::initPdf()
{
    pdfLabel = new QLabel;
    scrollArea = new QScrollArea;
    scrollArea->setWidget(pdfLabel);
    scrollArea->setWidgetResizable(true);

    pdfLayout = new QHBoxLayout;
    pdfLayout->addWidget(scrollArea);

    ctx = NULL;
    doc = NULL;
}

void Mpdf::loadPdf(QString path)
{
    currentPath = removeBackRef(path);
    ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    doc = fz_open_document(ctx, currentPath.toStdString().c_str());
    pageCount = fz_count_pages(doc);
 
    currentPage = 1;
    currentZoom = 60;

    enablePageNumber();
    showPage();
    hideTop();
}

void Mpdf::deletePdf()
{
    if (!doc)
    {
        return;
    }

    fz_close_document(doc);
    doc = NULL;

    if (!ctx)
    {
        return;
    }
    
    fz_free_context(ctx);
    ctx = NULL;

    system("ls *.png | egrep '^[0-9]' | xargs rm -rf");
}

void Mpdf::closeClicked()
{
    deletePdf();
    storage->back();
}

void Mpdf::zoominClicked()
{
    currentZoom = currentZoom + 10;
    showPage();
}

void Mpdf::zoomoutClicked()
{
    if (currentZoom > 10)
    {
        currentZoom = currentZoom - 10;
        showPage();
    }  
}

void Mpdf::prevClicked()
{
    currentPage = ((currentPage - 1) / 9 - 1) * 9 + 1;
    for (int i = 0; i < 9; i++)
    {
        numberButtonVec->at(i)->setText(QString::number(currentPage + i));
    }

    enablePageNumber();
    showPage();
}

void Mpdf::numberClicked(int number)
{
    currentPage = (currentPage - 1) / 9 * 9 + number + 1;
    showPage();
}

void Mpdf::nextClicked()
{
    currentPage = ((currentPage - 1) / 9 + 1) * 9 + 1;
    for (int i = 0; i < 9; i++)
    {
        numberButtonVec->at(i)->setText(QString::number(currentPage + i));
    }

    enablePageNumber();
    showPage();
}

void Mpdf::searchClicked()
{
    if (searchView)
    {
        hideSearch();
    }
    else
    {
        showSearch();
    }
}

void Mpdf::goClicked()
{
    QMessageBox::information(this, "Search", "Search not enabled");    
}

void Mpdf::showPage()
{
    fz_page *page = fz_load_page(doc, currentPage - 1);

    fz_matrix transform;
    fz_rotate(&transform, 0);
    fz_pre_scale(&transform, currentZoom / 100.0f, currentZoom / 100.0f);

    fz_rect bounds;
    fz_bound_page(doc, page, &bounds);
    fz_transform_rect(&bounds, &transform);

    fz_irect bbox;
    fz_round_rect(&bbox, &bounds);
    fz_pixmap *pix = fz_new_pixmap_with_bbox(ctx, fz_device_rgb, &bbox);
    fz_clear_pixmap_with_value(ctx, pix, 0xff);

    fz_device *dev = fz_new_draw_device(ctx, pix);
    fz_run_page(doc, page, dev, &transform, NULL);
    fz_free_device(dev);
    
    QString qpng = QString("%1.png").arg(currentPage);
    const char *ccpng = qpng.toStdString().c_str();
    char *cpng = new char[strlen(ccpng) + 1];
    strcpy(cpng, ccpng);
    
    fz_write_png(ctx, pix, cpng, 0);
    QPixmap qpix(qpng);
    pdfLabel->setPixmap(qpix);

    fz_drop_pixmap(ctx, pix);
    fz_free_page(doc, page);
}

void Mpdf::enablePageNumber()
{
    if (currentPage == 1)
    {
        prevButton->setEnabled(false);
    }
    else
    {
        prevButton->setEnabled(true);
    }

    if (pageCount - currentPage + 1 <= 9)
    {
        for (int i = 0; i < pageCount - currentPage + 1; i++)
        {
            numberButtonVec->at(i)->setEnabled(true);
        }
        for (int i = pageCount - currentPage + 1; i < 9; i++)
        {
            numberButtonVec->at(i)->setEnabled(false);
        }
        nextButton->setEnabled(false);
    }
    else
    {
        for (int i = 0; i < 9; i++)
        {
            numberButtonVec->at(i)->setEnabled(true);
        }
        nextButton->setEnabled(true);
    }  
}

QString Mpdf::removeBackRef(QString s)
{
    s.replace("\\ ", " ");
    return s;
}

void Mpdf::mouseMoveEvent(QMouseEvent *event)
{
    if (event->pos().y() <= 70)
    {
        showTop();
    }
    else if (event->pos().y() >= 100)
    {
        hideTop();
    }
}

void Mpdf::showTop()
{
    pageLabel->setVisible(true);
    prevButton->setVisible(true);

    for (int i = 0; i < 9; i++)
    {
        numberButtonVec->at(i)->setVisible(true);
    }

    nextButton->setVisible(true);
    searchButton->setVisible(true);
    zoominButton->setVisible(true);
    zoomoutButton->setVisible(true);
}
    
void Mpdf::hideTop()
{
    pageLabel->setVisible(false);
    prevButton->setVisible(false);

    for (int i = 0; i < 9; i++)
    {
        numberButtonVec->at(i)->setVisible(false);
    }

    nextButton->setVisible(false);
    searchButton->setVisible(false);
    zoominButton->setVisible(false);
    zoomoutButton->setVisible(false);
    searchLine->setVisible(false);
    goButton->setVisible(false);
    searchView = false;
}

void Mpdf::showSearch()
{
    searchLine->setVisible(true);
    goButton->setVisible(true);
    searchView = true;
}
    
void Mpdf::hideSearch()
{
    searchLine->setVisible(false);
    goButton->setVisible(false);
    searchView = false;
}

