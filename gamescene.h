#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "fallingkey.h"
#include <QGraphicsScene>
#include <QObject>
#include <QDebug>
#include <QKeyEvent>
#include <QFile>
#include <QTimer>
#include <QElapsedTimer>

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene(QObject *parent = 0);

public slots:
    void timerFallingKey();

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

private:
    void startGame();
    void setBackgroundItem();
    // read file
    void Read_Music_Data(const QString & Path);
    int ReadInt(QFile* file);
    // falling items
    void setFallingItems();
    // end of the game
    void endInterface(); // wait for further implementation ...

    int s_width;
    int s_height;
    int status; // 0 for menu
    int nTracks; // number of tracks
    int Total_time;
    qreal trackWidth, trackInterval, trackHeight, track_x;
    qreal velocity; // falling velocity, per msec
    QTimer s_timer;
    QElapsedTimer e_timer;
    QVector<QGraphicsRectItem*> keyItems;   // deprecate in future versions
    QVector<decltype(Qt::Key_D)> keyVal {Qt::Key_D, Qt::Key_F, Qt::Key_H, Qt::Key_J};
    QVector<QPair<int, int> > tm[11];
    QMultiMap<int, FallingKey*> fallingKeys;
};

#endif // GAMESCENE_H
