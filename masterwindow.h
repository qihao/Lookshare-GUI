#ifndef MASTERWINDOW_H
#define MASTERWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedLayout>

#include "homescreen.h"
#include "status.h"
#include "notepad.h"
#include "filesystem.h"

class MasterWindow : public QWidget
{
    Q_OBJECT
    
public:
    MasterWindow(QWidget *parent = 0);
    ~MasterWindow();

public slots:
    void toggleClicked();

private:
    QPushButton *toggleButton;
    QSpacerItem *spacerItem;
    HomeScreen *home;

    QHBoxLayout *topLayout;
    QVBoxLayout *mainLayout;

public:
    // called by HomeScreen
    QWidget *start;
    Status *status;
    Notepad *notepad;
    FileSystem *system;

    int currentIndex;
    QStackedLayout *stackLayout;
};

#endif // MASTERWINDOW_H
