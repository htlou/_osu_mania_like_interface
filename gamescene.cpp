#include "gamescene.h"

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QFont>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QTime>
#include <QTimer>
#include <QFile>
#include <QTimerEvent>
#include <QDir>

#include <QGraphicsTextItem>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent), status(0), trackWidth(100), trackInterval(20), velocity(1)
{
    // set the size of the scene & the background
    start_time = clock();
    Path = ":/data/data";
    DefaultPath = ":/resources";

    setSceneRect(0, 0, 1920, 1080);
    s_width = sceneRect().size().toSize().width(); s_height = sceneRect().size().toSize().height();
    trackHeight = s_height * 6 / 8;
    QGraphicsRectItem* background = new QGraphicsRectItem(sceneRect());
    background->setBrush(Qt::black);
    this->addItem(background);
    // set a text for start
    QGraphicsTextItem* startPrompt = new QGraphicsTextItem("Press left shift to begin");
    startPrompt->setDefaultTextColor(Qt::white);
    QFont font_start("Helvetica", 32);
    startPrompt->setFont(font_start);
    startPrompt->setPos((s_width - startPrompt->boundingRect().width()) / 2, (s_height - startPrompt->boundingRect().height()) / 2);
    this->addItem(startPrompt);
    // track x&y
    y_offset = s_height/8;
    x_offset = s_width/3;
    track_x = (s_width - trackInterval) / 2 - 2 * trackWidth - trackInterval - x_offset;
}

void GameScene::keyPressEvent(QKeyEvent* event) {
    if (event->isAutoRepeat()) {
        return;
    }
    // QVector<decltype(Qt::Key_D)> keyVal {Qt::Key_D, Qt::Key_F, Qt::Key_H, Qt::Key_J};
    if (event->key() == Qt::Key_Shift && !status) {
        clear(); // refresh the scene
        startGame();
    }
    else if(event->key()== Qt::Key_Exit && status){
        //应该是一个暂停界面
    }
    else {
        for (int i = 0; i < 4; ++i) {
            if (event->key() == keyVal[i]) {
                // detect collision
                // activate key shape
                keyItems[i]->setBrush(QColor(10, 10, 255, 200));
            }
        }
    }
}

void GameScene::keyReleaseEvent(QKeyEvent* event) {
    if (event->isAutoRepeat()) {
        return;
    }
    for (int i = 0; i < 4; ++i) {
        if (event->key() == keyVal[i]) {
            // deactivate key shape (color)
            keyItems[i]->setBrush(QColor(10, 10, 255, 128));
        }
    }
}

void GameScene::startGame() {
    setBackgroundItem();
    Read_Chart_Data((Path+"/1/chart.txt"));
    Read_BGM_Data((Path + "/1/audio.mp3"));
//    Read_Img_Data((Path + "/1/BG.jpg"));也可以用这个放置曲绘，没想好
    setFallingItems();
}

void GameScene::setBackgroundItem() {
    // background picture
//    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(QPixmap(":/img/resources/bg1.jpg")); // sources can be enriched afterwards
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(QPixmap((Path + "/1/BG.jpg")));
    addItem(background);
    background->setPos(0, 0);
    background->setScale(s_width / background->boundingRect().width());
    // osu!mania keys and tracks (set default to 4 tracks)
    nTracks = 4;
    QVector<QColor> vec_color{QColor(0, 0, 255, 64), }; // prepare different colors, unfinished
    for (int i = 0; i < nTracks; ++i) {
        // add track element (photoshop needed to get neater tracks)
        // qreal trackWidth = 100, trackInterval = 20, trackHeight = (qreal)s_height * 3 / 4;
        int kx = track_x + i * trackWidth + i * trackInterval;
        QRectF r(kx, y_offset, trackWidth, trackHeight);
        QGraphicsRectItem *tmptrack = new QGraphicsRectItem(r);
        tmptrack->setPen(Qt::NoPen);
        tmptrack->setBrush(QColor(0, 0, 255, 64));
        addItem(tmptrack);
        // add key item
        QRectF keyRect(kx, trackHeight, trackWidth, trackWidth);
        QGraphicsRectItem *tmpkey = new QGraphicsRectItem(keyRect);
        tmpkey->setPen(Qt::NoPen);
        tmpkey->setBrush(QColor(10, 10, 255, 128));
        addItem(tmpkey);
        keyItems.push_back(tmpkey);
    }
    // set timer event
    s_timer.setInterval(100); // 0.1s 触发判定是否有键下落
    s_timer.start();
    e_timer.start();
    connect(&s_timer, &QTimer::timeout, this, &GameScene::timerFallingKey);
}

// set falling items
void GameScene::setFallingItems() {
    for (int i = 0; i < nTracks; ++i) {
        if (!tm[i].empty()) {
            // create falling items
            for (auto p : tm[i]) {
                FallingKey* fk;
                if (p.second == -1)
                    fk = new FallingKey(track_x + (i-1) * (trackWidth + trackInterval), y_offset, trackHeight, trackWidth, trackWidth/2, QColor(50, 50, 200, 156), velocity);
                else
                    fk = new FallingKey(track_x + (i-1) * (trackWidth + trackInterval), y_offset, trackHeight, trackWidth, trackWidth + velocity * (p.second - p.first), QColor(50, 50, 200, 156), velocity);
                // 长键还没有想好怎么判定和消失
                addItem(fk);
                fallingKeys.insert(p.first, fk);
            }
        }
    }
    qDebug() << "keys prepared";
    qDebug() << fallingKeys.size() << " " << fallingKeys.begin().key();
    if (e_timer.elapsed() > Total_time) {
        endInterface();
    }
}

// handle falling animation; functions on certain interval
void GameScene :: timerFallingKey() {
    int i = 0;
    int nErase = 0;

    for(auto p = fallingKeys.begin(); ; ++p) {
        i++;
        if (i == 4 || (p.key() - e_timer.elapsed()) * velocity > trackHeight) break;
        else {
            nErase++;
            FallingKey* fk = p.value();
            fk->startFalling();
        }
    }
    qDebug() << e_timer.elapsed() << " " << i;
    if (nErase)
        for (int i = 0; i < nErase; ++i) fallingKeys.erase(fallingKeys.begin());
}

// read file
int GameScene :: ReadInt(QFile* file){
    char c = 0; int ans = 0;
    file -> getChar(&c);
    while(c < '0' || c > '9')file -> getChar(&c);
    while(c >= '0' && c <= '9'){
        ans = ans * 10 + c - '0';
        bool flag = file -> getChar(&c);
        if(flag == 0) return ans;
    }
    return ans;
}

void GameScene :: Read_Chart_Data(const QString & Path){
    qDebug()<<Path;
    QFile file(Path);
    if(file.open(QIODevice :: ReadOnly | QIODevice :: Text)){
    int Track_num = ReadInt(&file);
    Total_time = ReadInt(&file);
    for(int i = 1; i <= Track_num; i ++){
        int m = ReadInt(&file);
        for(int j = 1; j <= m; j ++){
            int x = ReadInt(&file), l, r;
            if(x == 1){
                l = ReadInt(&file);
                tm[i].push_back(qMakePair(l, -1));
            }
            else{
                l = ReadInt(&file), r = ReadInt(&file);
                tm[i].push_back(qMakePair(l, r));
            }
        }
        tm[i].push_back(qMakePair(Total_time + 114514, -1));
        // ptr[i] = 0;
    }
    for(int i = 1; i <= Track_num; i ++)
        qDebug() << tm[i].size() << " "; qDebug() << "\n";
    //qDebug() << "Read End!";
    file.close();
    }
    else {
    qDebug() << "File open error: " << file.errorString();
    }
}

void GameScene :: Read_Img_Data(const QString & Path){
    QGraphicsPixmapItem *foreground_img = new QGraphicsPixmapItem(QPixmap(Path));
    addItem(foreground_img);
    foreground_img->setPos(s_width/3, s_height/6);
    foreground_img->setScale(s_width / (2*foreground_img->boundingRect().width()));
}

void GameScene::Read_BGM_Data(const QString & Path){
    //
}
void GameScene::endInterface() {
    qDebug() << "End of the game";
}

GameScene::~GameScene(){
    Total_time = 0;
    // TODO:理想的情况下手动关闭窗口后程序应当停止运行，但我搞不出来TAT
}
