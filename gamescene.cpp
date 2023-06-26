#include "gamescene.h"
#include "globalVariables.h"

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
    : QGraphicsScene(parent), status(0), trackWidth(100), trackInterval(20), velocity(1), track_x(150)
{
    // set the size of the scene & the background
    start_time = clock();
    Path = ":/data/data";
    DefaultPath = ":/resources";

    setSceneRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
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
    track_x += 150;
    qDebug() << "This is track_x" << track_x;

    connect(player, SIGNAL(errorOccurred(QMediaPlayer::Error, QString)), this, SLOT(onError(QMediaPlayer::Error, QString)));
}

QString Int2String(int num, int x){ // num位数，x数字，取x后num位转换为QString，不足补0
    QString ans("");
    while(num--){
        ans.prepend((char)(x % 10 + 48));
        x /= 10;
    }
    return ans;
}

void GameScene::Change_Number(){
    //Score_.display(score);
    Score_ -> setText(Int2String(7, score));
    Combo_ -> setText(Int2String(3, combo));
    qDebug() << score << " Fuck";
}

void GameScene::Correct_Perfect(){
    score += 114514;
    combo ++;
    Change_Number();
    qDebug() << "Perfect!\n";
}

void GameScene::Correct_Good(){
    score += 66666;
    combo ++;
    Change_Number();
    qDebug() << "Good!\n";
}

void GameScene::Correct_Normal(){
    score += 23333;
    combo ++;
    Change_Number();
    qDebug() << "Normal!\n";
}

void GameScene::Miss(){
    score ++;
    combo = 0;
    Change_Number();
    qDebug() << "Miss!\n";
}

void GameScene::keyPressEvent(QKeyEvent* event) {
    if (event->isAutoRepeat()) {
        return;
    }
    // QVector<decltype(Qt::Key_D)> keyVal {Qt::Key_D, Qt::Key_F, Qt::Key_H, Qt::Key_J};
    if (event->key() == Qt::Key_Shift && !status) {
        clear(); // refresh the scene
        startGame();
        status = 1;
    }
    else if(event->key()== Qt::Key_Exit && status){
        //应该是一个暂停界面
    }
    else {
        for (int i = 0; i < nTracks; ++i) {
            if (event->key() == keyVal[i]) {
                // detect collision
                // activate key shape
                keyItems[i]->setBrush(QColor(10, 10, 255, 200));

                // maybe some press effect here

                int now_time = clock() - start_time - pause_time;
                while(ptr[i] < tm[i].size() && now_time - tm[i][ptr[i]].first > eps){
                    ptr[i] ++;
                }

                qDebug() << i << " " << ptr[i] << " " << now_time;
                int p = abs(now_time - tm[i][ptr[i]].first);
                if(p <= eps){
                    if(tm[i][ptr[i]].second == -1){
                        if(p <= eps_perfect)Correct_Perfect();
                        else if(p <= eps_good)Correct_Good();
                        else Correct_Normal();

                        ptr[i]++;
                    }
                }
                else{
                    if(now_time > tm[i][ptr[i]].first)tm[i][ptr[i]].second = -1;
                    Miss();
                }
            }
        }
    }
}

void GameScene::keyReleaseEvent(QKeyEvent* event) {
    if (event->isAutoRepeat()) {
        return;
    }
    for (int i = 0; i < nTracks; ++i) {
        if (event->key() == keyVal[i]) {
            // deactivate key shape (color)
            keyItems[i]->setBrush(QColor(10, 10, 255, 128));

            int now_time = clock() - start_time - pause_time;
            int R = tm[i][ptr[i]].second;
            if(R == -1)return;
            int p = abs(now_time - R);
            if(p <= eps){
                if(p <= eps_perfect)
                    Correct_Perfect();
                else if(p <= eps_good)
                    Correct_Good();
                else Correct_Normal();
            }
            else{
                Miss();
            }
        }
    }
}

void GameScene::startGame() {
    start_time = clock();
    Read_Chart_Data((Path+"/1/chart.txt"));
    setBackgroundItem();
    Read_BGM_Data((Path + "/1/audio.mp3"));
//    Read_Img_Data((Path + "/1/BG.jpg"));也可以用这个放置曲绘，没想好
    setFallingItems();
}

void GameScene::setBackgroundItem() {
    // background picture
//    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(QPixmap(":/img/resources/bg1.jpg")); // sources can be enriched afterwards

    // 分辨率缩放的解决方案
    QPixmap bgPic(Path + "/1/BG.jpg");
    bgPic.scaled(SCREEN_WIDTH, SCREEN_HEIGHT, Qt::KeepAspectRatioByExpanding);
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(bgPic);
    background->setPos(-(bgPic.width()-SCREEN_WIDTH)/2, -(bgPic.height()-SCREEN_HEIGHT)/2);
    addItem(background);

    // osu!mania keys and tracks (set default to 4 tracks)
    // nTracks = 4;
    qDebug() << nTracks << " This is ntrack";
    QVector<QColor> vec_color{QColor(0, 0, 255, 64), }; // prepare different colors, unfinished
    for (int i = 0; i < nTracks; ++i) {
        // add track element (photoshop needed to get neater tracks)
        // qreal trackWidth = 100, trackInterval = 20, trackHeight = (qreal)s_height * 3 / 4;
        int kx = track_x + i * trackWidth + i * trackInterval ;
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

    QFont font("Arial", 30); // 初始化得分与Combo
    Score_ -> setPos(900,100);
    Score_ -> setBrush(Qt::red);
    Score_ -> setFont(font);
    Combo_ -> setPos(930,150);
    Combo_ -> setBrush(Qt::red);
    Combo_ -> setFont(font);

    this -> addItem(Score_);
    this -> addItem(Combo_);

}

// set falling items
void GameScene::setFallingItems() {
    for (int i = 0; i < nTracks; ++i) {
        if (!tm[i].empty()) {
            // create falling items
            for (auto p : tm[i]) {
                FallingKey* fk;
                if (p.second == -1)
                    fk = new FallingKey(track_x + (i) * (trackWidth + trackInterval), y_offset, trackHeight, trackWidth, trackWidth/2, QColor(50, 50, 200, 156), velocity);
                else
                    fk = new FallingKey(track_x + (i) * (trackWidth + trackInterval), y_offset, trackHeight, trackWidth, trackWidth + velocity * (p.second - p.first), QColor(50, 50, 200, 156), velocity);
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

    for(auto p = fallingKeys.begin(); p != fallingKeys.end() ; ++p) {
        //i++;
        if (i == nTracks || (p.key() - e_timer.elapsed()) * velocity > trackHeight) break;
        else {
            nErase++;
            FallingKey* fk = p.value();
            fk->startFalling();
        }
        i ++;
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
    nTracks = Track_num;
    Total_time = ReadInt(&file);
    for(int i = 0; i < Track_num; i ++){
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
        ptr[i] = 0;
    }
    for(int i = 0; i < Track_num; i ++)
        qDebug() << tm[i].size() << " "; qDebug() << "\n";

    for(int i = 0; i < Track_num; i ++){
        for(int j = 0; j < tm[i].size(); j ++)
            qDebug() << tm[i][j].first << " " << tm[i][j].second << "\n";
    }
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
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl("qrc"+Path));
    audioOutput->setVolume(50);
    player->play();
    if(player->isPlaying())qDebug()<<"playing bgm";
}

void GameScene::onError(QMediaPlayer::Error error, QString)
{
    qDebug() << "播放出错：" << player->errorString();
}

void GameScene::endInterface() {
    qDebug() << "End of the game";
}

GameScene::~GameScene(){
    Total_time = 0;
    // TODO:理想的情况下手动关闭窗口后程序应当停止运行，但我搞不出来TAT
}
