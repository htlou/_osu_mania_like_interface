#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "fallingkey.h"
#include "globalvariations.h"
#include "myMainWindow.h"
//#include "pause.h"
#include "pauseWidget.h"
#include "board.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QDebug>
#include <QKeyEvent>
#include <QFile>
#include <QTimer>
#include <QElapsedTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QLCDNumber>
#include <QLabel>
#include <QProcess>

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene(QString Route, QObject *parent = 0);
    ~GameScene();
signals:
   void closethis();

public slots:
    void timerFallingKey();
    void onError(QMediaPlayer::Error error, QString);
    void handleCloseGameAndPauseWindow();
    void GoOnGame();
    void handleEndOfFalling();

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

signals:
    void gamePauseSig();

private:
    QProcess *myProcess;
    MyMainWindow *_parent;

    void startGame(QString Route);
    void pauseGame();
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

    void Correct_Perfect();
    void Correct_Good();
    void Correct_Normal();
    void Miss();
    void Show_combo();
    void Show_score();
    void Change_Number();
    void endGame();

    int s_width;
    int s_height;
    int x_offset;
    int y_offset;
    int status; // 0 for menu
    int nTracks; // number of tracks
    int Total_time;
    int pause_time = 0; // to detect pause
    int pauseTime = 0, pauseClock = 0;  // pauseTime: accumulate pause time; pauseClock: save the tick when the game is paused
    int score = 0, combo = 0; // Score
    int ptr[11]; // To evaluate the press event, Pointer of notes
    qreal trackWidth, trackInterval, trackHeight, track_x;
    qreal velocity; // falling velocity, per msec
    QTimer *keyFallingTimer;
    QTimer *AllTimer;
    QElapsedTimer e_timer;
    QVector<QGraphicsRectItem*> keyItems;   // deprecate in future versions
    int keyVal[6] {Qt::Key_S, Qt::Key_D, Qt::Key_F, Qt::Key_H, Qt::Key_J, Qt::Key_K};
    QVector<QPair<int, int> > tm[11];
    QMultiMap<int, FallingKey*> fallingKeys;
    QVector<DetectLine*> detectLines;
    QQueue<FallingKey*> queueFalling;

    QGraphicsSimpleTextItem* Score_ = new QGraphicsSimpleTextItem("0000000");
    QGraphicsSimpleTextItem* Combo_ = new QGraphicsSimpleTextItem("000");

    QGraphicsSimpleTextItem* Time = new QGraphicsSimpleTextItem("0000000");//see the time.only to debugging

    double start_time = 0; // timer
    QString Path; // Music Data Path
    QString DefaultPath;//default Data Path, storing something like default key sound
    int eps = 400; //key pressed in this time will be calculated(otherwise neglected)
    int eps_good = 300; // eps_good
    int eps_perfect = 200; // eps_perfect

    QMediaPlayer *player = new QMediaPlayer;
    QAudioOutput *audioOutput = new QAudioOutput;

    // pause interface items
    PauseButton *btnContinue, *btnBack;
    QGraphicsRectItem *pauseBGRect;
    QGraphicsPixmapItem *pauseBGText;

};

QString Int2String(int num,int x);

#endif // GAMESCENE_H
