#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QDesktopWidget>
//#include <QTimer>

#include "masterwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    QPixmap pixmap("logo.png");
    QSplashScreen splash(pixmap.scaled(400, 300, Qt::KeepAspectRatio));
    splash.move(QApplication::desktop()->availableGeometry().center() 
            - splash.rect().center());
    splash.show();
    //QTimer::singleShot(3000, &splash, SLOT(close()));

    MasterWindow window;
    window.move(QApplication::desktop()->availableGeometry().center() 
            - window.rect().center());
    window.show();
    //QTimer::singleShot(1000, &window, SLOT(show()));
    
    splash.finish(&window);

    return app.exec();
}
