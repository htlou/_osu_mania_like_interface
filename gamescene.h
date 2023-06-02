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
#include <QMediaPlayer>
#include <QAudioOutput>

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene(QObject *parent = 0);
    ~GameScene();

public slots:
    void timerFallingKey();
    void onError(QMediaPlayer::Error error, QString);

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

private:
    void startGame();
    void setBackgroundItem();
    // read file
    void Read_Chart_Data(const QString & Path);
    void Read_Img_Data(const QString &Path);
    void Read_BGM_Data(const QString &Path);
    int ReadInt(QFile* file);
    // falling items
    void setFallingItems();
    // end of the game
    void endInterface(); // wait for further implementation ...

    int s_width;
    int s_height;
    int x_offset;
    int y_offset;
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

    double start_time = 0; // timer
    QString Path; // Music Data Path
    QString DefaultPath;//default Data Path, storing something like default key sound
    int eps = 400; //key pressed in this time will be calculated(otherwise neglected)
    int eps_good = 300; // eps_good
    int eps_perfect = 200; // eps_perfect

    QMediaPlayer *player = new QMediaPlayer;
    QAudioOutput *audioOutput = new QAudioOutput;


};

#endif // GAMESCENE_H
