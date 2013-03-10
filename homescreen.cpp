#include "homescreen.h"
#include "masterwindow.h"

HomeScreen::HomeScreen(QWidget *parent) :
    QWidget(parent)
{
    linkButton = new QPushButton;
    statusButton = new QPushButton;
    internetButton = new QPushButton;
    systemButton = new QPushButton;
    dewButton = new QPushButton;
    notepadButton = new QPushButton;
    calendarButton = new QPushButton;
    stockButton = new QPushButton;
    clockButton = new QPushButton;

    linkButton->setText("Link");
    statusButton->setText("Status");
    internetButton->setText("Internet");
    systemButton->setText("Local File Storage");
    dewButton->setText("Dew");
    notepadButton->setText("Notepad");
    calendarButton->setText("Calendar");
    stockButton->setText("Stocks");
    clockButton->setText("Clock");

    gridLayout = new QGridLayout;
    gridLayout->setHorizontalSpacing(25);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    gridLayout->addWidget(linkButton, 0, 0, 1, 1);
    gridLayout->addWidget(statusButton, 0, 1, 1, 1);
    gridLayout->addWidget(internetButton, 0, 2, 1, 1);
    gridLayout->addWidget(systemButton, 1, 0, 1, 1);
    gridLayout->addWidget(dewButton, 1, 1, 1, 1);
    gridLayout->addWidget(notepadButton, 1, 2, 1, 1);
    gridLayout->addWidget(calendarButton, 2, 0, 1, 1);
    gridLayout->addWidget(stockButton, 2, 1, 1, 1);
    gridLayout->addWidget(clockButton, 2, 2, 1, 1);
    setLayout(gridLayout);

    connect(statusButton, SIGNAL(clicked()),
            this, SLOT(statusClicked()));
    connect(notepadButton, SIGNAL(clicked()),
            this, SLOT(notepadClicked()));
    connect(systemButton, SIGNAL(clicked()),
            this, SLOT(systemClicked()));
}

HomeScreen::~HomeScreen()
{
}

void HomeScreen::statusClicked()
{
    masterWindow->status->update();
    masterWindow->currentIndex = 2;
    masterWindow->stackLayout->setCurrentIndex(masterWindow->currentIndex);
}

void HomeScreen::notepadClicked()
{
    masterWindow->notepad->update();
    masterWindow->currentIndex = 3;
    masterWindow->stackLayout->setCurrentIndex(masterWindow->currentIndex);
}

void HomeScreen::systemClicked()
{
    masterWindow->system->update();
    masterWindow->currentIndex = 4;
    masterWindow->stackLayout->setCurrentIndex(masterWindow->currentIndex);
}
