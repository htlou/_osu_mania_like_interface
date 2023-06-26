#include <QGraphicsView>
#include <QApplication>
#include "mainmenu.h"
#include <gamescene.h>
#include "globalVariables.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSize screen = QGuiApplication::primaryScreen()->size();
    SCREEN_WIDTH = screen.width();
    SCREEN_HEIGHT = screen.height();
//    SCREEN_SCALE = qMin(SCREEN_WIDTH/2560., SCREEN_HEIGHT/1440.);
//    BACKGROUND_SCALE = qMax(SCREEN_WIDTH/2560., SCREEN_HEIGHT/1440.);
//    MARGIN_HORIZONTAL = round((SCREEN_WIDTH-2560*SCREEN_SCALE)/2);
//    MARGIN_VERTICAL = round((SCREEN_HEIGHT-1440*SCREEN_SCALE)/2);
//    GEM_SIZE = 128*SCREEN_SCALE;

    MyMainWindow mainWindow;
    mainWindow.show();
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
