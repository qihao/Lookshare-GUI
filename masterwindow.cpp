#include "masterwindow.h"

MasterWindow::MasterWindow(QWidget *parent)
    : QWidget(parent)
{
    resize(400, 300);
    setWindowTitle("Lookshare");
    
    toggleButton = new QPushButton;
    toggleButton->setText("X");
    spacerItem = new QSpacerItem(25, 25, 
            QSizePolicy::Expanding, QSizePolicy::Minimum);

    topLayout = new QHBoxLayout;
    topLayout->addWidget(toggleButton);
    topLayout->addItem(spacerItem);

    start = new QWidget;
    home = new HomeScreen;
    status = new Status;
    notepad = new Notepad;
    system = new FileSystem;

    home->masterWindow = this;

    stackLayout = new QStackedLayout;
    stackLayout->addWidget(start);
    stackLayout->addWidget(home);
    stackLayout->addWidget(status);
    stackLayout->addWidget(notepad);
    stackLayout->addWidget(system);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(stackLayout);
    setLayout(mainLayout);

    currentIndex = 0;
    stackLayout->setCurrentIndex(currentIndex);

    connect(toggleButton, SIGNAL(clicked()),
            this, SLOT(toggleClicked()));
}

MasterWindow::~MasterWindow()
{
}

void MasterWindow::toggleClicked()
{
/*
    if (currentIndex == 1)
    {
        currentIndex = 0;
    }
    else
    {
        currentIndex = 1;
    }
*/
    currentIndex = 1;
    stackLayout->setCurrentIndex(currentIndex);
}
