#include <QMessageBox>
#include <QTextStream>
#include <QHeaderView>
#include <QStandardItem>
#include <QFileIconProvider>
#include <QIcon>
#include <QDateTime>
#include <QtAlgorithms>

#include <QDebug>

#include "stdlib.h"
#include "filesystem.h"

FileSystem::FileSystem(QWidget *parent) :
    QWidget(parent)
{
    initStatus();
    initSearch();
    initSystem();

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(statusLayout);
    mainLayout->addLayout(searchLayout);
    mainLayout->addLayout(systemLayout);
    setLayout(mainLayout);
}

FileSystem::~FileSystem()
{
    delete currentDir;
    delete fileList;
}

void FileSystem::initStatus()
{
    dirLabel = new QLabel;
    usageLabel = new QLabel;
    cloudButton = new QPushButton;
    cloudButton->setText("Pull from Cloud");    

    updateStatus();

    statusLayout = new QHBoxLayout;
    statusLayout->addWidget(dirLabel);
    statusLayout->addWidget(usageLabel);
    statusLayout->addWidget(cloudButton);

    connect(cloudButton, SIGNAL(clicked()),
            this, SLOT(cloudClicked()));
}

void FileSystem::initSearch()
{
    upButton = new QPushButton;
    upButton->setText("Up");
    searchButton = new QPushButton;
    searchButton->setText("Search");
    searchLine = new QLineEdit;
    clearButton = new QPushButton;
    clearButton->setText("Clear");
    
    updateSearch();

    searchLayout = new QHBoxLayout;
    searchLayout->addWidget(upButton);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(searchLine);
    searchLayout->addWidget(clearButton);

    connect(upButton, SIGNAL(clicked()),
            this, SLOT(upClicked()));
    connect(searchButton, SIGNAL(clicked()),
            this, SLOT(searchClicked()));
    connect(searchLine, SIGNAL(textChanged(const QString &)),
            this, SLOT(lineEdited(const QString &)));
    //connect(searchLine, SIGNAL(returnPressed()),
    //        this, SLOT(searchClicked()));
    connect(clearButton, SIGNAL(clicked()),
            this, SLOT(clearClicked()));
}

void FileSystem::initSystem()
{
    fileList = new QList<QString>;

    systemModel = new QStandardItemModel;
    systemModel->setColumnCount(3);
    systemModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Type")));
    systemModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Name")));
    systemModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Modified")));

    systemView = new QTableView;
    systemView->setModel(systemModel);
    systemView->verticalHeader()->setVisible(false);
    
    updateSystem();

    systemLayout = new QHBoxLayout;
    systemLayout->addWidget(systemView);

    connect(systemView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(itemClicked(QModelIndex)));
    connect(systemView->horizontalHeader(), 
            SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
            this, SLOT(indicatorClicked(int, Qt::SortOrder)));
}

void FileSystem::updateStatus()
{
    dirLabel->setText("Local Memory/");

    FILE *handler = popen("df -h | egrep '/$' | awk '{print $5}'", "r");
    QTextStream stream(handler, QIODevice::ReadOnly);
    QString text = stream.readLine();
    usageLabel->setText(text.append(" Used"));
    pclose(handler);
}

void FileSystem::updateSearch()
{
    upButton->setEnabled(false);
    searchButton->setEnabled(false);
    searchLine->clear();
    clearButton->setEnabled(false);
}

void FileSystem::updateSystem()
{
    clearFiles();

    QString currentPath = "Local Memory";
    currentDir = new QDir(".");
    if (!currentDir->exists(currentPath))
    {
        currentDir->mkdir(currentPath);
    }
    currentDir->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    currentDir->setSorting(QDir::Time);
    currentDir->cd(currentPath);
    clearSystem();
    showDir();

    systemView->horizontalHeader()->setSortIndicatorShown(true);
    systemView->horizontalHeader()->setSortIndicator(1, Qt::AscendingOrder);
    systemView->horizontalHeader()->setSortIndicator(2, Qt::DescendingOrder);
    systemView->setColumnWidth(0, 55);
    systemView->setColumnWidth(1, 180);
    systemView->setColumnWidth(2, 110);

    isSystemView = true;
}

void FileSystem::update()
{
    updateStatus();
    updateSearch();
    updateSystem();
}

void FileSystem::cloudClicked()
{
    QMessageBox::information(this, "Cloud", "Cloud not enabled");
}

void FileSystem::upClicked()
{    
    clearSystem();
    currentDir->cdUp();
    showDir();
    showDirLabel();
    setUpButton();
}

void FileSystem::searchClicked()
{
    clearFiles();

    QString text = searchLine->text();
    dirLabel->setText("Search for '" + text + "'");

    QString cmd; 
    if (!text.startsWith("find"))
    {
        cmd = "find ./Local\\ Memory/ ";
        QList<QString> files = text.split(QRegExp("\\s+"));
        for (int i = 0; i < files.size(); i++ )
        {
            if (i == files.size() - 1)
            {
                cmd.append("-name '" + files.at(i) + "'");
            }
            else
            {
                cmd.append("-name '" + files.at(i) + "' -or ");
            }
        }  
    }
    else
    {
        cmd = text;
    }

    FILE *handler = popen(cmd.toStdString().c_str(), "r");      
    QTextStream stream(handler, QIODevice::ReadOnly);

    while (!stream.atEnd())
    {
        QString line = stream.readLine();

        if (!line.isEmpty() && line.compare("./Local Memory/") != 0)
        {
            fileList->append(line);
        }
    }

    qSort(fileList->begin(), fileList->end(), byTime);
    reverseFiles();

    pclose(handler);

    systemView->horizontalHeader()->setSortIndicatorShown(true);
    systemView->horizontalHeader()->setSortIndicator(1, Qt::AscendingOrder);
    systemView->horizontalHeader()->setSortIndicator(2, Qt::DescendingOrder);

    clearSystem();
    showFiles();    
    upButton->setEnabled(false);
    isSystemView = false;

qDebug() << *fileList;
}

void FileSystem::lineEdited(const QString &s)
{
    if (s.isEmpty())
    {
        searchButton->setEnabled(false);
        clearButton->setEnabled(false);

        if (!isSystemView)
        {
            clearFiles();
            clearSystem();
            showDir();
            showDirLabel();
            setUpButton();
            isSystemView = true;
        }
    }
    else
    {
        searchButton->setEnabled(true);
        clearButton->setEnabled(true);
    }
}

void FileSystem::clearClicked()
{
    searchLine->clear();
    searchButton->setEnabled(false);
    clearButton->setEnabled(false);

    if (!isSystemView)
    {
        clearFiles();
        clearSystem();
        showDir();
        showDirLabel();
        setUpButton();
        isSystemView = true;
    }
}

void FileSystem::itemClicked(QModelIndex i)
{
    QFileInfo fileInfo;
    if (isSystemView)
    {
        fileInfo = currentDir->entryInfoList().at(i.row()); 
    }
    else
    {
        fileInfo.setFile(fileList->at(i.row()));
    }

    if (fileInfo.isDir())
    {
        clearFiles();
        clearSystem();
        currentDir->cd(fileInfo.absoluteFilePath());
        showDir();
        showDirLabel();
        setUpButton();       
        isSystemView = true;
    }
    else
    {
        openFile(fileInfo);
    }
}

void FileSystem::indicatorClicked(int i, Qt::SortOrder order)
{
    if (isSystemView)
    {
        if (i == 0)
        {
            if (order == Qt::AscendingOrder)
            {
                currentDir->setSorting(QDir::Type);
            }
            else
            {
                currentDir->setSorting(QDir::Type | QDir::Reversed);
            }        
        
            clearSystem();
            showDir();
        }

        systemView->sortByColumn(i, order);
    }
    else if (!fileList->isEmpty())
    {
        if (i == 0)
        {
            qSort(fileList->begin(), fileList->end(), byType);    
        } 
        else if (i == 1)
        {
            qSort(fileList->begin(), fileList->end(), byName);
        }
        else
        {
            qSort(fileList->begin(), fileList->end(), byTime);
        }

        if (order == Qt::DescendingOrder)
        {              
              reverseFiles();
        }

        clearSystem();
        showFiles();
    }
}

void FileSystem::clearSystem()
{
    systemModel->removeRows(0, systemModel->rowCount());
}

void FileSystem::clearFiles()
{
    fileList->clear();
}

void FileSystem::showDirLabel()
{
    QDir dir(".");
    dirLabel->setText(dir.relativeFilePath(currentDir->path()).append("/"));
}

void FileSystem::showDir()
{
    QList<QFileInfo> infoList = currentDir->entryInfoList();

    QFileIconProvider iconPro;
    foreach (QFileInfo fileInfo, infoList)
    {
        QIcon icon = iconPro.icon(fileInfo);
        QString name = fileInfo.fileName();
        QString date = fileInfo.lastModified().toString(Qt::SystemLocaleDate);

        QList<QStandardItem *> itemList;
        itemList.append(new QStandardItem(QIcon(icon), QString("")));
        itemList.append(new QStandardItem(QString(name)));
        itemList.append(new QStandardItem(QString(date)));

        systemModel->appendRow(itemList);
    }
}

void FileSystem::showFiles()
{    
    QFileIconProvider iconPro;
    foreach (QString filePath, *fileList)
    {
        QFileInfo fileInfo(filePath);
        QIcon icon = iconPro.icon(fileInfo);
        QString name = fileInfo.fileName();
        QString date = fileInfo.lastModified().toString(Qt::SystemLocaleDate);

        QList<QStandardItem *> itemList;
        itemList.append(new QStandardItem(QIcon(icon), QString("")));
        itemList.append(new QStandardItem(QString(name)));
        itemList.append(new QStandardItem(QString(date)));

        systemModel->appendRow(itemList);
    }
}

void FileSystem::openFile(const QFileInfo &fileInfo)
{
    QString suffix = fileInfo.suffix();
    QString path = escapePath(fileInfo.filePath());

    if (suffix.compare("cpp") == 0 || 
        suffix.compare("h") == 0 || 
        suffix.compare("rtf") == 0 || 
        suffix.compare("txt") == 0)
    {
        system(path.prepend("gedit ").toStdString().c_str());
    } 
    else if (suffix.compare("pdf") == 0)
    {
        system(path.prepend("evince ").toStdString().c_str());
    } 
    else if (suffix.compare("odt") == 0 || 
        suffix.compare("doc") == 0 || 
        suffix.compare("docx") == 0)
    {
        system(path.prepend("soffice ").toStdString().c_str());
    }
    else if (suffix.compare("html") == 0)
    {
        system(path.prepend("firefox ").toStdString().c_str());
    }
    else if (suffix.compare("jpg") == 0 || 
        suffix.compare("png") == 0 || 
        suffix.compare("tiff") == 0 || 
        suffix.compare("gif") == 0)
    {
        system(path.prepend("xdg-open ").toStdString().c_str());
    }
    else
    {
        QMessageBox::information(this, "File", "File suffix not supported");
    }
}

void FileSystem::setUpButton()
{
    if (dirLabel->text().compare("Local Memory/") != 0)
    {
        upButton->setEnabled(true);
    }
    else
    {
        upButton->setEnabled(false);
    }
}

QString FileSystem::escapePath(QString s)
{
    s.replace(" ", "\\ ");
    return s;
}

void FileSystem::reverseFiles()
{    
    for (int i = 0; i < fileList->size() / 2; i++)
    {
        fileList->swap(i, fileList->size() - i - 1);
    }
}

bool FileSystem::byType(QString s1, QString s2)
{
    QFileInfo f1(s1);
    QFileInfo f2(s2);
        
    if (f1.isDir() && f2.isDir())
    {
        return f1.fileName() < f2.fileName();
    }
    else if (f1.isDir() && !f2.isDir())
    {
        return true;
    }
    else if (!f1.isDir() && f2.isDir())
    {
        return false;
    }
    else
    {
        if (f1.suffix() == f2.suffix())
        {
            return f1.baseName() < f2.baseName();
        }
        else
        {
            return f1.suffix() < f2.suffix();
        }
    }
}

bool FileSystem::byName(QString s1, QString s2)
{
    QFileInfo f1(s1);
    QFileInfo f2(s2);

    return f1.fileName() < f2.fileName();
}
    
bool FileSystem::byTime(QString s1, QString s2)
{
    QFileInfo f1(s1);
    QFileInfo f2(s2);

    return f1.lastModified() < f2.lastModified();
}

