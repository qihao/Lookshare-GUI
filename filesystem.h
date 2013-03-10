#ifndef FileSystem_H
#define FileSystem_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDir>
#include <QTableView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class FileSystem : public QWidget
{
    Q_OBJECT
public:
    explicit FileSystem(QWidget *parent = 0);
    ~FileSystem();


  //called by HomeScreen
    void update();
    
public slots:
    void cloudClicked();
    void upClicked();
    void searchClicked();
    void lineEdited(const QString &s);
    void clearClicked();
    void itemClicked(QModelIndex i);
    void indicatorClicked(int i, Qt::SortOrder order);

private:
    QLabel *dirLabel;
    QLabel *usageLabel;
    QPushButton *cloudButton;

    QPushButton *upButton;
    QPushButton *searchButton;
    QLineEdit *searchLine;
    QPushButton *clearButton;

    QDir *currentDir;
    QTableView *systemView;
    QStandardItemModel *systemModel;

    QHBoxLayout *statusLayout;
    QHBoxLayout *searchLayout;
    QHBoxLayout *systemLayout;
    QVBoxLayout *mainLayout;
    
    bool isSystemView;
    QList<QString> *fileList;

    void initStatus();
    void initSearch();
    void initSystem();

    void updateStatus();
    void updateSearch();
    void updateSystem();

    void clearSystem();
    void clearFiles(); 
    void showDirLabel();
    void showDir();
    void showFiles();
    void openFile(const QFileInfo &fileInfo);
    void setUpButton();

    // utility functions
    QString escapePath(QString s);
    void reverseFiles();
    static bool byType(QString s1, QString s2);
    static bool byName(QString s1, QString s2);
    static bool byTime(QString s1, QString s2);
};

#endif // System_H
