#ifndef MPDF_H
#define MPDF_H

#include <QWidget>
#include <QPushButton>
#include <QSpacerItem>
#include <QLabel>
#include <QVector>
#include <QLineEdit>
#include <QSignalMapper>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>

extern "C"
{
    #include <fitz.h>
}

class Storage;

class Mpdf : public QWidget
{
    Q_OBJECT
    
public:
    explicit Mpdf(QWidget *parent = 0);
    ~Mpdf();

    void loadPdf(QString path);
    void deletePdf();

signals:
    void clicked(int number);

private:
    QSignalMapper *signalMapper;

public slots:
    void closeClicked();
    void zoominClicked();
    void zoomoutClicked();
    void prevClicked();
    void numberClicked(int number);
    void nextClicked();
    void searchClicked();
    void goClicked();

private:
    QPushButton *closeButton;
    QSpacerItem *spacerItem1;
    QLabel *pageLabel;
    QPushButton *prevButton;
    QVector<QPushButton *> *numberButtonVec;
    QPushButton *nextButton;
    QPushButton *searchButton;
    QPushButton *zoominButton;
    QPushButton *zoomoutButton;
    QSpacerItem *spacerItem2;
    QLineEdit *searchLine;
    QPushButton *goButton;
    QLabel *pdfLabel;
    QScrollArea *scrollArea;

    QHBoxLayout *top1Layout;
    QHBoxLayout *top2Layout;
    QHBoxLayout *top3Layout;
    QHBoxLayout *pdfLayout;
    QVBoxLayout *mainLayout;

    bool searchView;
    QString currentPath;
    fz_context *ctx;
    fz_document *doc;
    int pageCount;
    int currentPage;
    int currentZoom;

    void initTop1();
    void initTop2();
    void initTop3();
    void initPdf();

    void showPage();
    void enablePageNumber();
    QString removeBackRef(QString s);

    void mouseMoveEvent(QMouseEvent *event);
    void showTop();
    void hideTop();
    void showSearch();
    void hideSearch();

public:
    Storage *storage;
};

#endif // MPDF_H
