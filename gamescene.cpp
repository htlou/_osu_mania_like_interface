#include "gamescene.h"

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
//#include "pause.h"
#include "pauseWidget.h"
#include "ending.h"

GameScene::GameScene(QString Route, QObject *parent)
    : QGraphicsScene(parent), _parent((MyMainWindow*)parent), status(0), trackWidth(100), trackInterval(20), velocity(VELOCITY), track_x(150)
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

    // set track size
    TRACK_WIDTH = SCREEN_WIDTH / 8;
    TRACK_HEIGHT = SCREEN_HEIGHT * 5 / 6;
//    // set a text for start -- deprecated
//    QGraphicsTextItem* startPrompt = new QGraphicsTextItem("Press left shift to begin");
//    startPrompt->setDefaultTextColor(Qt::white);
//    QFont font_start("Helvetica", 32);
//    startPrompt->setFont(font_start);
//    startPrompt->setPos((s_width - startPrompt->boundingRect().width()) / 2, (s_height - startPrompt->boundingRect().height()) / 2);
//    this->addItem(startPrompt);
    // track x&y
    y_offset = s_height/8;
    x_offset = s_width/3;
    track_x = (s_width - trackInterval) / 2 - 2 * trackWidth - trackInterval - x_offset;
    track_x += 150;
    qDebug() << "This is track_x" << track_x;

    myProcess = new QProcess(this);

    connect(player, SIGNAL(errorOccurred(QMediaPlayer::Error, QString)), this, SLOT(onError(QMediaPlayer::Error, QString)));
    // start the game
    status = 1;
    startGame(Route);
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
    Time -> setText(Int2String(7,e_timer.elapsed()));
}

void GameScene::Correct_Perfect(){
    combo ++;
    score += ((combo < 10) ? combo : 10) * 1000;
    Change_Number();
    qDebug() << "Perfect!\n";
}

void GameScene::Correct_Good(){
    combo ++;
    score += ((combo < 10) ? combo : 10) * 800;
    Change_Number();
    qDebug() << "Good!\n";
}

void GameScene::Correct_Normal(){
    combo ++;
    score += ((combo < 10) ? combo : 10) * 500;
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

    if(event->key()== Qt::Key_Escape && status){
        pauseGame();
    }
    else {
        for (int i = 0; i < nTracks; ++i) {
            if (event->key() == keyVal[i]) {
                // detect collision
                // activate key shape
                // keyItems[i]->setBrush(QColor(10, 10, 255, 200));

                // maybe some press effect here
                detectLines[i]->onKeyPress();

                int now_time = clock() - start_time - pause_time;
                now_time = 1ll * now_time * 1000 / CLOCKS_PER_SEC;
                while(ptr[i] < tm[i].size() && now_time - tm[i][ptr[i]].first > eps){
                    ptr[i] ++;
                }

                // qDebug() << i << " " << ptr[i] << " " << now_time;
//                qDebug() << "ptr[" << i << "]:  " << ptr[i];
//                qDebug() << "tm[" << i << "].size() = " << tm[i].size();
//                qDebug() << now_time;
                int p = abs(now_time - tm[i][ptr[i]].first);
                if(p <= eps){
                    if(tm[i][ptr[i]].second == -1){
                        if(p <= eps_perfect)Correct_Perfect();
                        else if(p <= eps_good)Correct_Good();
                        else Correct_Normal();
                        ptr[i]++;

                    }
                    else{
                        pressed_and_long[i] = 1;
                        qDebug() << "Long Start!";
                    }
                }
                else{
                    if(now_time > tm[i][ptr[i]].first + eps)tm[i][ptr[i]].second = -1;
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
            pressed_and_long[i] = 0;
            // deactivate key shape (color)
            detectLines[i]->onKeyRelease();

            int now_time = clock() - start_time - pause_time;
            now_time = 1ll * now_time * 1000 / CLOCKS_PER_SEC;

            int R = tm[i][ptr[i]].second;
            if(R == -1){
                if(tm[i][ptr[i]].first + eps < now_time)
                ptr[i]++;
                return;
            }
            int p = abs(now_time - R), p1 = now_time - tm[i][ptr[i]].first;
            if(p <= eps && p1 > eps){
                if(p <= eps_perfect)
                    Correct_Perfect();
                else if(p <= eps_good)
                    Correct_Good();
                else Correct_Normal();
                ptr[i] ++;
            }
            else if(p1 > eps && p > eps){
                qDebug()<<"chevoi?";
                Miss();
                ptr[i] ++;
            }
        }
    }
}

void GameScene::startGame(QString Route) {
    eps = eps0;
    eps_good = eps1;
    eps_perfect = eps2;
//这里是我们用GlobalVariations替换的位置

    start_time = clock();
    Read_Chart_Data((Path+"/"+Route+"/chart.txt"));
    setBackgroundItem(); // 曲绘目前放在这里
    Read_BGM_Data((Path + "/"+Route+"/audio.mp3"));
//    Read_Img_Data((Path + "/"+Route+"/BG.jpg"));也可以用这个放置曲绘，没想好
    setFallingItems();
    // set timer events
    keyFallingTimer = new QTimer(this);
    keyFallingTimer->start(INTERVAL); // 默认 0.01s 触发判定是否有键下落

    chkMiss = new QTimer(this);
    chkMiss -> start(INTERVAL); //每个10ms检查是否有错过的按键

    AllTimer = new QTimer(this);
    AllTimer -> start(Total_time);
    qDebug() << "FUUFUFUFUFUFU" << Total_time;

    e_timer.start();
    connect(keyFallingTimer, &QTimer::timeout, this, &GameScene::timerFallingKey);
    connect(AllTimer,&QTimer::timeout,this,&GameScene::endGame);
    connect(chkMiss,&QTimer::timeout,this,&GameScene::checkMiss);
}

void GameScene::setBackgroundItem() {
    // show background picture
    QPixmap bgPic(QPixmap(":/img/resources/fail-background.png"));
    bgPic.scaled(SCREEN_WIDTH, SCREEN_HEIGHT, Qt::KeepAspectRatioByExpanding);
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(bgPic);
    background->setPos(-(bgPic.width()-SCREEN_WIDTH)/2, -(bgPic.height()-SCREEN_HEIGHT)/2);
    addItem(background);

    // osu!mania keys and tracks (set default to 4 tracks)
    // nTracks = 4;
    qDebug() << nTracks << " This is ntrack";
    qDebug() << "This is track height: " << TRACK_HEIGHT;

    // show board and fixed boundary lines
    Board *trackBoard = new Board;
    addItem(trackBoard);
    for (int i = 0; i < 4; ++i) {
        DetectLine* dl = new DetectLine;
        dl->setPos(TRACK_WIDTH*(i-2)+SCREEN_WIDTH/2, TRACK_HEIGHT);
        detectLines.append(dl);
        addItem(dl);
    }

    QFont font("Arial", 30); // 初始化得分与Combo
    Score_ -> setPos(SCREEN_WIDTH-300,100);
    Score_ -> setBrush(Qt::white);
    Score_ -> setFont(font);
    Combo_ -> setPos(SCREEN_WIDTH-300,150);
    Combo_ -> setBrush(Qt::white);
    Combo_ -> setFont(font);
    Time -> setPos(SCREEN_WIDTH-300,200);
    Time -> setBrush(Qt::white);
    Time -> setFont(font);

    this -> addItem(Score_);
    this -> addItem(Combo_);
    this -> addItem(Time);

}

// set falling items
void GameScene::setFallingItems() {
    for (int i = 0; i < nTracks; ++i) {
        if (!tm[i].empty()) {
            // create falling items
            for (auto p : tm[i]) {
                FallingKey *key = new FallingKey(i, p.first, p.second, this);
                addItem(key);
                fallingKeys.insert(p.first, key);
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

    for(auto p = fallingKeys.begin();p != fallingKeys.end() ; ++p) {
        if ((p.key() - e_timer.elapsed() + pauseTime) * VELOCITY / INTERVAL > TRACK_HEIGHT) break;
            nErase++;
        FallingKey* fk = p.value();
            if (!fk->isFalling) fk->startFalling();
        queueFalling.push_back(fk);
        i++;
    }
    // qDebug() << e_timer.elapsed()-pause_time << " " << i<<" by timerFallingKey";
    if (nErase)
        for (int i = 0; i < nErase; ++i) fallingKeys.erase(fallingKeys.begin());
}

// delete the key at the end of the falling event
void GameScene::handleEndOfFalling()
{
    qDebug() << "handle end of falling";
    qDebug() << e_timer.elapsed() - pause_time << " " << e_timer.elapsed() << " " << pause_time;
    queueFalling.pop_front();
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
    memset(pressed_and_long,0,sizeof(pressed_and_long));

    //Total_time = 11000; // Just for debugging
    //decide KeyVal
    for(int st = 2 - (6 - nTracks) / 2, i = st; i < st + nTracks; i ++){
            keyVal[i - st] = key_val_[i];
    }

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
    keyFallingTimer->stop();
    keyFallingTimer->deleteLater();
    // TODO:理想的情况下手动关闭窗口后程序应当停止运行，但我搞不出来TAT
}

void GameScene::handleCloseGameAndPauseWindow(){

    this->clear();
    this->deleteLater();
    emit closethis();
}

void GameScene::GoOnGame(){
    for(int st = 2 - (6 - nTracks) / 2, i = st; i < st + nTracks; i ++){
        keyVal[i - st] = key_val_[i];
    }
    // delete pause interface
    btnContinue->deleteLater();
    btnBack->deleteLater();
    delete pauseBGText;
    delete pauseBGRect;
    // continue music playing
    player->play();
    // restart timer events
    keyFallingTimer->start(INTERVAL);
    pauseTime += e_timer.elapsed() - pauseClock;
    // deal with key falling
    for (int i = 0; i < queueFalling.size(); ++i) {
        if (queueFalling[i]->isFalling) {
            queueFalling[i]->resumeFalling();
        }
    }
    connect(keyFallingTimer, &QTimer::timeout, this, &GameScene::timerFallingKey);
}

void GameScene::pauseGame()
{
    // stop timer, record pause time
    qDebug()<<"stop!!!";
    double start_pause = clock();
    keyFallingTimer->stop();
    AllTimer -> stop();
    player->pause();
    // pause the falling keys
    for (int i = 0; i < queueFalling.size(); ++i) {
        if (queueFalling[i]->isFalling) {
            queueFalling[i]->pauseFalling();
        }
    }
    pauseClock = e_timer.elapsed();
    // pause interface
    // background color (half transparent)
    QRect BGRect(0, SCREEN_HEIGHT*0.3, SCREEN_WIDTH, SCREEN_HEIGHT*0.3);
    pauseBGRect = new QGraphicsRectItem(BGRect);
    pauseBGRect->setBrush(Qt::black);
    pauseBGRect->setOpacity(0.3);
    addItem(pauseBGRect);
    // button
    btnContinue = new PauseButton("pause-continue");
    btnContinue->setPos(SCREEN_WIDTH *0.35, SCREEN_HEIGHT * 0.55);
    addItem(btnContinue);
    btnBack = new PauseButton("pause-back");
    btnBack->setPos(SCREEN_WIDTH *0.65, SCREEN_HEIGHT * 0.55);
    addItem(btnBack);
    connect(btnContinue, &PauseButton::button_clicked, this, &GameScene::GoOnGame);
    connect(btnBack, &PauseButton::button_clicked, this, &GameScene::handleCloseGameAndPauseWindow);
    // PAUSE text
    QPixmap pauseText(":/element/resources/pause-text.png");
    pauseBGText = new QGraphicsPixmapItem(pauseText);
    pauseBGText->setOffset(-pauseText.width()/2, -pauseText.height()/2);
    pauseBGText->setScale(SCREEN_WIDTH*0.3 / pauseText.width());
    pauseBGText->setPos(SCREEN_WIDTH*0.5, SCREEN_HEIGHT*0.4);
    addItem(pauseBGText);
    pause_time += clock() - start_pause;
}

void GameScene::endGame(){
    EndingScene* eScene = new EndingScene(this,score);
    QGraphicsView* eview = new QGraphicsView(eScene, _parent); // 这里必须要加第二个参数_parent，否则会弹出一个新界面
    eview->setRenderHint(QPainter::Antialiasing);
    eview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    eview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    eview->setFixedSize(eScene->sceneRect().size().toSize());
    //eview->showFullScreen();
    // remove game view
    QWidget *w = _parent->layout->currentWidget();
    _parent->layout->removeWidget(w);
    w->deleteLater();
    // qDebug() << "game scene succefully removed!";
    _parent->layout->addWidget(eview);
    _parent->layout->currentWidget()->setFocus();
    //the necessary ending scene is completed.
    keyFallingTimer -> stop();
    AllTimer -> stop();
    chkMiss -> stop();

    connect(eScene,&EndingScene::quit_,this,&GameScene::endgame0);
}

void GameScene::endgame0(){
    emit end000();
}

void GameScene::checkMiss(){
    int now_time = clock() - start_time - pause_time;
    now_time = 1ll * now_time * 1000 / CLOCKS_PER_SEC;
    for(int i = 0; i < nTracks; i ++){
//        qDebug() << i << " " << now_time << " " << tm[i][ptr[i]].first << " " << pressed_and_long[i];
        if((now_time - tm[i][ptr[i]].first) > eps && !pressed_and_long[i]){
            Miss();
            ptr[i] ++;
        }
    }
}
