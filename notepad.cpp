#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "masterwindow.h"

Notepad::Notepad(QWidget *parent) :
    QWidget(parent)
{
    textEdit = new QPlainTextEdit(this);
    textEdit->setFont(QFont("Arial", 12));

    topLayout = new QHBoxLayout; 
    topLayout->addWidget(textEdit, 0, Qt::AlignHCenter);

    saveButton = new QPushButton;
    saveButton->setText("Save");

    bottomLayout = new QHBoxLayout; 
    bottomLayout->addWidget(saveButton, 0, Qt::AlignHCenter);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);

    update();

    connect(saveButton, SIGNAL(clicked()),
            this, SLOT(saveClicked()));
}

Notepad::~Notepad()
{
}

void Notepad::update()
{
    currentPath = "Local Memory/data/";
    textEdit->clear();
}

void Notepad::saveClicked()
{
    QString fileName = getFilename();
    QFile file(fileName);
    file.open(QFile::WriteOnly | QFile::Text);
    QTextStream stream(&file);
    stream << textEdit->toPlainText();
    file.close();

    masterWindow->storage->update();
    masterWindow->storage->system->showDataDir();
    masterWindow->stackLayout->setCurrentIndex(4);
}

QString Notepad::getFilename()
{
    QDir currentDir(".");
    if (!currentDir.exists(currentPath))
    {
        currentDir.mkpath(currentPath);
    }

    currentDir.cd(currentPath);
    if (currentDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot).isEmpty())
    {
        return "0.txt";
    }
    else
    {
        for (int i = 0; i < INT_MAX; i++)
        {
            QString fileName = currentPath + QString("%1.txt").arg(i);
            QFile file(fileName);
            if (!file.exists())
            {
                return fileName;
            }
        }
    }
}
