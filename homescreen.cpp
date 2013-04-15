#include "homescreen.h"
#include "masterwindow.h"

HomeScreen::HomeScreen(QWidget *parent)
    : QWidget(parent)
{
    linkButton = new QPushButton;
    linkButton->setText("Link");
    statusButton = new QPushButton;
    statusButton->setText("Status");
    internetButton = new QPushButton;
    internetButton->setText("Internet");
    storageButton = new QPushButton;
    storageButton->setText("Storage");
    dewButton = new QPushButton;
    dewButton->setText("Dew");
    notepadButton = new QPushButton;
    notepadButton->setText("Notepad");
    calendarButton = new QPushButton;
    calendarButton->setText("Calendar");
    stockButton = new QPushButton;
    stockButton->setText("Stocks");
    clockButton = new QPushButton;
    clockButton->setText("Clock");

    gridLayout = new QGridLayout;
    gridLayout->addWidget(linkButton, 0, 0, Qt::AlignHCenter);
    gridLayout->addWidget(statusButton, 0, 1, Qt::AlignHCenter);
    gridLayout->addWidget(internetButton, 0, 2, Qt::AlignHCenter);
    gridLayout->addWidget(storageButton, 1, 0, Qt::AlignHCenter);
    gridLayout->addWidget(dewButton, 1, 1, Qt::AlignHCenter);
    gridLayout->addWidget(notepadButton, 1, 2, Qt::AlignHCenter);
    gridLayout->addWidget(calendarButton, 2, 0, Qt::AlignHCenter);
    gridLayout->addWidget(stockButton, 2, 1, Qt::AlignHCenter);
    gridLayout->addWidget(clockButton, 2, 2, Qt::AlignHCenter);
    setLayout(gridLayout);

    connect(statusButton, SIGNAL(clicked()),
            this, SLOT(statusClicked()));
    connect(notepadButton, SIGNAL(clicked()),
            this, SLOT(notepadClicked()));
    connect(storageButton, SIGNAL(clicked()),
            this, SLOT(storageClicked()));
}

HomeScreen::~HomeScreen()
{
}

void HomeScreen::statusClicked()
{
    masterWindow->status->update();
    masterWindow->stackLayout->setCurrentIndex(2);
}

void HomeScreen::notepadClicked()
{
    masterWindow->notepad->update();
    masterWindow->stackLayout->setCurrentIndex(3);
}

void HomeScreen::storageClicked()
{
    masterWindow->storage->update();
    masterWindow->stackLayout->setCurrentIndex(4);
}
