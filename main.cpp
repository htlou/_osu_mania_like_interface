#include <QGraphicsView>
#include <QApplication>
#include <gamescene.h>
#include "globalvariations.h"
#include "myMainWindow.h"

int main(int argc, char *argv[])
{
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");
    QApplication a(argc, argv);
//<<<<<<< HEAD
    //qDebug() << CLOCKS_PER_SEC;
//=======
//>>>>>>> 8922cff90688750062f9f9932c848851f63c2a62

    QSize screen = QGuiApplication::primaryScreen()->size();
    SCREEN_WIDTH = screen.width();
    SCREEN_HEIGHT = screen.height();

    MyMainWindow mainWindow(&a);
    mainWindow.showFullScreen();

    return a.exec();
}
