#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QPushButton>

class Notepad : public QWidget
{
    Q_OBJECT
    
public:
    explicit Notepad(QWidget *parent = 0);
    ~Notepad();

    // called by HomeScreen
    void update();

public slots:
    void saveClicked();
    
private:
    QPlainTextEdit *textEdit;
    QPushButton *saveButton;

    int fileNumber;
    QString currentPath;
};

#endif // NOTEPAD_H
