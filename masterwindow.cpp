#include "masterwindow.h"

MasterWindow::MasterWindow(QWidget *parent)
    : QWidget(parent)
{
    resize(400, 300);
    setWindowTitle("Lookshare");
 
    start = new QWidget;
    home = new HomeScreen;
    status = new Status;
    notepad = new Notepad;
    storage = new Storage;

    home->masterWindow = this;
    notepad->masterWindow = this;

    stackLayout = new QStackedLayout;
    stackLayout->addWidget(start);   // 0
    stackLayout->addWidget(home);    // 1
    stackLayout->addWidget(status);  // 2
    stackLayout->addWidget(notepad); // 3
    stackLayout->addWidget(storage); // 4

    frame = new QFrame;
    frame->setFrameShape(QFrame::HLine);
    frame->setFrameShadow(QFrame::Raised);
    homeButton = new QPushButton;
    homeButton->setText("Home");

    bottomLayout = new QVBoxLayout;
    bottomLayout->addWidget(frame);
    bottomLayout->addWidget(homeButton, 0, Qt::AlignHCenter);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(stackLayout);
    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);

    stackLayout->setCurrentIndex(0);

    connect(homeButton, SIGNAL(clicked()),
            this, SLOT(homeClicked()));
}

MasterWindow::~MasterWindow()
{
}

void MasterWindow::homeClicked()
{

    if (stackLayout->currentIndex() == 4)
    {
        if (storage->stackLayout->currentIndex() == 1)
        {
            storage->mplayer->deleteVideo();
        }
        if (storage->stackLayout->currentIndex() == 2)
        {
            storage->mpdf->deletePdf();
        }
    }

    stackLayout->setCurrentIndex(1);
}

