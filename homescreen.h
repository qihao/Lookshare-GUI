#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

#include "status.h"
#include "notepad.h"
#include "filesystem.h"

class MasterWindow;

class HomeScreen : public QWidget
{
    Q_OBJECT
    
public:
    explicit HomeScreen(QWidget *parent = 0);
    ~HomeScreen();

public slots:
    void notepadClicked();
    void statusClicked();
    void systemClicked();

private:
    QPushButton *linkButton;
    QPushButton *statusButton;
    QPushButton *internetButton;
    QPushButton *systemButton;
    QPushButton *dewButton;
    QPushButton *notepadButton;
    QPushButton *calendarButton;
    QPushButton *stockButton;
    QPushButton *clockButton;
    QPushButton *toggleButton;
    QGridLayout *gridLayout;

public:
    // keep a handle of MasterWindow to access stacked layout
    MasterWindow *masterWindow;
};

#endif // HOMESCREEN_H
