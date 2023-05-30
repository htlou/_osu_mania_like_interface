#include <QGraphicsView>
#include <QApplication>
#include <mainmenu.h>
#include <gamescene.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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
