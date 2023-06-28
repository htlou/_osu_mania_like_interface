#include <QGraphicsView>
#include <QApplication>
#include <mainmenu.h>
#include <gamescene.h>
#include "globalvariations.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QSize screen = QGuiApplication::primaryScreen()->size();
    SCREEN_WIDTH = screen.width();
    SCREEN_HEIGHT = screen.height();

    MyMainWindow mainWindow;
    mainWindow.showFullScreen();
//    GameScene *scene = new GameScene();
//    QGraphicsView *view = new QGraphicsView(scene);
//    view->setRenderHint(QPainter::Antialiasing);
//    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    view->setFixedSize(scene->sceneRect().size().toSize());
//    view->setWindowFlags(Qt::Window);
//    view->showFullScreen();

    return a.exec();
}
