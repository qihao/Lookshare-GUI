#ifndef STATUS_H
#define STATUS_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>

class Status : public QWidget
{
    Q_OBJECT
    
public:
    explicit Status(QWidget *parent = 0);
    ~Status();

    void update();

private:
    QLabel *wifiLabel;
    QLabel *storageLabel;
    QLabel *batteryLabel;
    QLabel *deviceLabel;
    QLabel *wifiText;
    QLabel *storageText;
    QLabel *batteryText;
    QLabel *deviceText;

    QGridLayout *gridLayout;

    void updateCmd(const char* cmd, QLabel *text);
};

#endif // STATUS_H
