#ifndef MASTERWINDOW_H
#define MASTERWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedLayout>

#include "homescreen.h"
#include "status.h"
#include "notepad.h"
#include "storage.h"

class MasterWindow : public QWidget
{
    Q_OBJECT
    
public:
    MasterWindow(QWidget *parent = 0);
    ~MasterWindow();

public slots:
    void homeClicked();

private:
    HomeScreen *home;

    QFrame *frame;
    QPushButton *homeButton;

    QVBoxLayout *bottomLayout;
    QVBoxLayout *mainLayout;

public:
    QWidget *start;
    Status *status;
    Notepad *notepad;
    Storage *storage;

    QStackedLayout *stackLayout;
};

#endif // MASTERWINDOW_H
