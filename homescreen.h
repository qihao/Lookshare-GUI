#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

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
    void storageClicked();

private:
    QPushButton *linkButton;
    QPushButton *statusButton;
    QPushButton *internetButton;
    QPushButton *storageButton;
    QPushButton *dewButton;
    QPushButton *notepadButton;
    QPushButton *calendarButton;
    QPushButton *stockButton;
    QPushButton *clockButton;
    QPushButton *toggleButton;
    QGridLayout *gridLayout;

public:
    MasterWindow *masterWindow;
};

#endif // HOMESCREEN_H
