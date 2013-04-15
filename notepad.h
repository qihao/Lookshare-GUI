#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class MasterWindow;

class Notepad : public QWidget
{
    Q_OBJECT
    
public:
    explicit Notepad(QWidget *parent = 0);
    ~Notepad();

    void update();

public slots:
    void saveClicked();
    
private:
    QPlainTextEdit *textEdit;
    QPushButton *saveButton;

    QHBoxLayout *topLayout;
    QHBoxLayout *bottomLayout;
    QVBoxLayout *mainLayout; 

    QString currentPath;

    QString getFilename();

public:
    MasterWindow *masterWindow;
};

#endif // NOTEPAD_H
