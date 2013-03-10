#include <QDir>
#include <QFile>
#include <QTextStream>

#include "notepad.h"

Notepad::Notepad(QWidget *parent) :
    QWidget(parent)
{
    textEdit = new QPlainTextEdit(this);
    textEdit->setGeometry(QRect(25, 25, 350, 150));
    textEdit->setFont(QFont("Arial", 12));

    saveButton = new QPushButton(this);
    saveButton->setGeometry(QRect(150, 200, 100, 25));
    saveButton->setText("Save");

    update();

    connect(saveButton, SIGNAL(clicked()),
            this, SLOT(saveClicked()));
}

Notepad::~Notepad()
{
}

void Notepad::update()
{
    textEdit->clear();

    currentPath = "./Local Memory/data/";
    QDir currentDir(".");
    // date folder exists
    if (!currentDir.exists(currentPath))
    {
        currentDir.mkpath(currentPath);
        fileNumber = 0;
    }
    else// not exists
    {
        currentDir.cd(currentPath);
        // data folder empty
	if (currentDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot).isEmpty())
        {
            fileNumber = 0;
        }
        else// not empty
        {
            int max = 0;
            foreach (QString s, currentDir.entryList())
            {
                if (s.toInt() > max)
                {
                    max = s.toInt();
                }
            }
        	
            fileNumber = max + 1;
        }
    }    
}

void Notepad::saveClicked()
{
    QString fileName = currentPath + QString("%1.txt").arg(fileNumber);
    QFile file(fileName);
    file.open(QFile::WriteOnly | QFile::Text);
    QTextStream stream(&file);
    stream << textEdit->toPlainText();
    file.close();

    fileNumber++;
}
