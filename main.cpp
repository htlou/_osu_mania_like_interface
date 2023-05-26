#include <QGraphicsView>
#include <QApplication>
#include <gamescene.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsView view;
    GameScene scene;
    view.setRenderHint(QPainter::Antialiasing);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setScene(&scene);
    view.setFixedSize(scene.sceneRect().size().toSize());
    view.show();
    return a.exec();
}
