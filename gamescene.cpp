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
#include "menubutton.h"

template<class T>
void GenOpacityAnimationIn(T* w,qreal x){
    QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect;
    w->setGraphicsEffect(opacityEffect);
    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    opacityAnimation->setDuration(800);
    opacityAnimation->setKeyValueAt(0.0,0.0);
    opacityAnimation->setKeyValueAt(1.0,x);
    opacityAnimation->start();
}

template<class T>
void GenOpacityAnimationOut(T* w){
    QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect;
    w->setGraphicsEffect(opacityEffect);
    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    opacityAnimation->setDuration(800);
    opacityAnimation->setKeyValueAt(0.0,0.8);
    opacityAnimation->setKeyValueAt(1.0,0.0);
    opacityAnimation->start();
}

GameScene::GameScene(QString Route, QObject *parent)
    : QGraphicsScene(parent), _parent((MyMainWindow*)parent), status(0), trackWidth(100), trackInterval(20), velocity(VELOCITY), track_x(150)
{
    // set the size of the scene & the background
    Path = ":/datamusic/data/"+Route;
    DefaultPath = ":/resources";
    Route_ = Route;

    setSceneRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    s_width = sceneRect().size().toSize().width(); s_height = sceneRect().size().toSize().height();
    trackHeight = SCREEN_HEIGHT * 6 / 8;
    //QGraphicsRectItem* background = new QGraphicsRectItem(sceneRect());
    //background->setBrush(Qt::black);
    //this->addItem(background);

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
    start_time = clock();
}

QString Int2String0(int x){ // num位数，x数字，取x后num位转换为QString，不足补0
    QString ans("");
    if(!x) ans = QString("0");
    while(x){
        ans.prepend((char)(x % 10 + 48));
        x /= 10;
    }
    return ans;
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

void GameScene::Correct_Perfect(int pos){
    combo ++;
    score += ((combo < 10) ? combo : 10) * 1000;
    Change_Number();
    qDebug() << "Perfect!\n";
    //qDebug() << hint;
    // show hint
    if (hint[pos]) {
        qDebug() << "delete!";
        delete hint[pos];
        delete hintVanishTimer;
    }
    hint[pos] = new stageHint("hint-perfect");
    hint[pos]->setPos(TRACK_WIDTH*(pos-2+0.50)+SCREEN_WIDTH/2, TRACK_HEIGHT-120);
    addItem(hint[pos]);
    hintVanishTimer = new QTimer();
    hintVanishTimer->start(500);
    //connect(hintVanishTimer, &QTimer::timeout, this, &GameScene::hintVanishSlot);
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    opacityAnimation->setDuration(500);
    opacityAnimation->setKeyValueAt(0.0,0);
    opacityAnimation->setKeyValueAt(0.001,1);
    opacityAnimation->setKeyValueAt(0.999,1);
    opacityAnimation->setKeyValueAt(1.0,0);
    hint[pos] -> setGraphicsEffect(opacityEffect);
    opacityAnimation -> start();
}

void GameScene::Correct_Good(int pos){
    combo ++;
    score += ((combo < 10) ? combo : 10) * 800;
    Change_Number();
    qDebug() << "Good!\n";
    // show hint
    if (hint[pos]) {
        qDebug() << "delete!";
        delete hint[pos];
        delete hintVanishTimer;
    }
    hint[pos] = new stageHint("hint-good");
    hint[pos]->setPos(TRACK_WIDTH*(pos-2+0.50)+SCREEN_WIDTH/2, TRACK_HEIGHT-120);
    addItem(hint[pos]);
    hintVanishTimer = new QTimer();
    hintVanishTimer->start(500);
    //connect(hintVanishTimer, &QTimer::timeout, this, &GameScene::hintVanishSlot);
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    opacityAnimation->setDuration(500);
    opacityAnimation->setKeyValueAt(0.0,0);
    opacityAnimation->setKeyValueAt(0.001,1);
    opacityAnimation->setKeyValueAt(0.999,1);
    opacityAnimation->setKeyValueAt(1.0,0);
    hint[pos] -> setGraphicsEffect(opacityEffect);
    opacityAnimation -> start();
}

void GameScene::Correct_Normal(int pos){
    combo ++;
    score += ((combo < 10) ? combo : 10) * 500;
    Change_Number();
    qDebug() << "Normal!\n";
    // show hint
    if (hint[pos]) {
        qDebug() << "delete!";
        delete hint[pos];
        delete hintVanishTimer;
    }
    hint[pos] = new stageHint("hint-normal");
    hint[pos]->setPos(TRACK_WIDTH*(pos-2+0.50)+SCREEN_WIDTH/2, TRACK_HEIGHT-120);
    addItem(hint[pos]);
    hintVanishTimer = new QTimer();
    hintVanishTimer->start(500);
    //connect(hintVanishTimer, &QTimer::timeout, this, &GameScene::hintVanishSlot);
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    opacityAnimation->setDuration(500);
    opacityAnimation->setKeyValueAt(0.0,0);
    opacityAnimation->setKeyValueAt(0.001,1);
    opacityAnimation->setKeyValueAt(0.999,1);
    opacityAnimation->setKeyValueAt(1.0,0);
    hint[pos] -> setGraphicsEffect(opacityEffect);
    opacityAnimation -> start();
}

void GameScene::Miss(int pos){
    score ++;
    combo = 0;
    Change_Number();
    qDebug() << "Miss!\n";
    // show hint
    if (hint[pos]) {
        qDebug() << "delete!";
        delete hint[pos];
        delete hintVanishTimer;
    }
    hint[pos] = new stageHint("hint-miss");
    hint[pos]->setPos(TRACK_WIDTH*(pos-2 + 0.50)+SCREEN_WIDTH/2, TRACK_HEIGHT-120);
    addItem(hint[pos]);
    hintVanishTimer = new QTimer();
    hintVanishTimer->start(500);
    //connect(hintVanishTimer, &QTimer::timeout, this, SLOT(hintVanishSlot(pos)));
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    opacityAnimation->setDuration(500);
    opacityAnimation->setKeyValueAt(0.0,0);
    opacityAnimation->setKeyValueAt(0.001,1);
    opacityAnimation->setKeyValueAt(0.999,1);
    opacityAnimation->setKeyValueAt(1.0,0);
    hint[pos] -> setGraphicsEffect(opacityEffect);
    opacityAnimation -> start();
}

void GameScene::hintVanishSlot(int pos)
{
    hint[pos]->vanish();
    hint[pos] = nullptr;
    delete hintVanishTimer;
}

void GameScene::keyPressEvent(QKeyEvent* event) {

    if(e_timer.elapsed() - pauseTime > Total_time + 100)return;
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

                int now_time = e_timer.elapsed() - pauseTime;
                //now_time = 1ll * now_time * 1000 / CLOCKS_PER_SEC;
                qDebug()<< i <<" " << now_time<<" "<< "pressed";
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
                        if(p <= eps_perfect)Correct_Perfect(i);
                        else if(p <= eps_good)Correct_Good(i);
                        else Correct_Normal(i);
                        ptr[i]++;

                    }
                    else{
                        pressed_and_long[i] = 1;
                        qDebug() << "Long Start!";
                    }
                }
                else{
                    if(now_time > tm[i][ptr[i]].first + eps)tm[i][ptr[i]].second = -1;
                    Miss(i);
                }
            }
        }
    }
}

void GameScene::keyReleaseEvent(QKeyEvent* event) {
    if(e_timer.elapsed() - pauseTime > Total_time + 100)return;
    if (event->isAutoRepeat()) {
        return;
    }
    for (int i = 0; i < nTracks; ++i) {
        if (event->key() == keyVal[i]) {
            pressed_and_long[i] = 0;
            // deactivate key shape (color)
            detectLines[i]->onKeyRelease();

            int now_time = e_timer.elapsed() - pauseTime;
            //now_time = 1ll * now_time * 1000 / CLOCKS_PER_SEC;

            int R = tm[i][ptr[i]].second;
            if(R == -1){
                if(tm[i][ptr[i]].first + eps < now_time)
                ptr[i]++;
                return;
            }
            int p = abs(now_time - R), p1 = now_time - tm[i][ptr[i]].first;
            if(p <= eps && p1 > eps){
                if(p <= eps_perfect)
                    Correct_Perfect(i);
                else if(p <= eps_good)
                    Correct_Good(i);
                else Correct_Normal(i);
                ptr[i] ++;
            }
            else if(p1 > eps && p > eps){
                qDebug()<<"chevoi?";
                Miss(i);
                ptr[i] ++;
            }
        }
    }
}

void GameScene::EnableAnimation(){
    GenOpacityAnimationIn(trackBoard,0.8);
    for(int i = 0; i < 4; i ++)
        GenOpacityAnimationIn(detectLines[i],1);

    timer500 = new QTimer;
    timer500 -> start(800);
    connect(timer500,&QTimer::timeout,this,&GameScene::startGameReal);
}

void GameScene::startGame(QString Route) {
    eps = eps0;
    eps_good = eps1;
    eps_perfect = eps2;
//这里是我们用GlobalVariations替换的位置
    Read_Chart_Data((Path+"/chart.txt"));
    setBackgroundItem(); // 曲绘目前放在这里
    Read_BGM_Data((Path+"/audio.mp3"));

    EnableAnimation();
//    Read_Img_Data((Path + "/"+Route+"/BG.jpg"));也可以用这个放置曲绘，没想好
}

void GameScene::startGameReal(){
    timer500 -> stop();
    setFallingItems();
    // set timer events

    keyFallingTimer = new QTimer(this);
    keyFallingTimer->start(INTERVAL); // 默认 0.01s 触发判定是否有键下落

    chkMiss = new QTimer(this);
    chkMiss -> start(5); //每个10ms检查是否有错过的按键

    AllTimer = new QTimer(this);
    AllTimer -> start(Total_time);

    e_timer.start();
    start_time = clock();
    connect(keyFallingTimer, &QTimer::timeout, this, &GameScene::timerFallingKey);
    connect(AllTimer,&QTimer::timeout,this,&GameScene::endGame);
    connect(chkMiss,&QTimer::timeout,this,&GameScene::checkMiss);
}

void GameScene::setBackgroundItem() {
    // show background picture
    QPixmap bgPic(QPixmap(":/img/resources/background-1.png"));
    bgPic = bgPic.scaled(SCREEN_WIDTH, SCREEN_HEIGHT, Qt::KeepAspectRatioByExpanding);
    background = new QGraphicsPixmapItem(bgPic);
    //const QRectF rect0 = QRectF(TRACK_HEIGHT,0,SCREEN_WIDTH,SCREEN_HEIGHT-TRACK_HEIGHT);

    //background->setPos(-(bgPic.width()-SCREEN_WIDTH)/2, -(bgPic.height()-SCREEN_HEIGHT)/2);
    addItem(background);

    // osu!mania keys and tracks (set default to 4 tracks)
    // nTracks = 4;
    qDebug() << nTracks << " This is ntrack";
    qDebug() << "This is track height: " << TRACK_HEIGHT;

    // show board and fixed boundary lines
    trackBoard = new Board;
    addItem(trackBoard);
    qreal h0 = 26;
    for (int i = 0; i < 4; ++i) {
        DetectLine* dl = new DetectLine;
        dl->setPos(TRACK_WIDTH*(i-2)+SCREEN_WIDTH/2, TRACK_HEIGHT);
        detectLines.append(dl);
        addItem(dl);
    }
    // draw score board
    scoreBoard = new QGraphicsRectItem((SCREEN_WIDTH+TRACK_WIDTH*4)/2,0,(SCREEN_WIDTH-TRACK_WIDTH*4)/2,200);
    scoreBoard->setBrush(Qt::black);
    scoreBoard->setOpacity(0.5);
    addItem(scoreBoard);

    QPixmap veil;
    h0 = 26.0 * TRACK_WIDTH / detectLines[0] -> boundingRect().width();
    veil = bgPic.copy(0,TRACK_HEIGHT+h0,SCREEN_WIDTH,SCREEN_HEIGHT-TRACK_HEIGHT);
    Veil = new QGraphicsPixmapItem(veil);
    background->setPos(0, 0);
    Veil -> setPos(0,TRACK_HEIGHT+h0);
    Veil -> setZValue(26);
    addItem(Veil);

    QFont font(DefaultFont); // 初始化得分与Combo
    Score_ -> setPos(SCREEN_WIDTH-300,25);
    Score_ -> setBrush(Qt::white);
    Score_ -> setFont(font);
    Combo_ -> setPos(SCREEN_WIDTH-300,75);
    Combo_ -> setBrush(Qt::white);
    Combo_ -> setFont(font);
//    stageHint* combo_icon = new stageHint("combo");
//    combo_icon->setPos(SCREEN_WIDTH/2, SCREEN_HEIGHT*0.10);
//    combo_icon->setScale(0.3);
//    addItem(combo_icon);
    Time -> setPos(SCREEN_WIDTH-300,125);
    Time -> setBrush(Qt::white);
    Time -> setFont(font);

    this -> addItem(Score_);
    this -> addItem(Combo_);
    this -> addItem(Time);
    // draw pause button
    pauseBtn = new MenuButton("pause-button");
    pauseBtn->setPos(20, 20);
    pauseBtn->setScale(0.25);
    pauseBtn -> setZValue(30);
    addItem(pauseBtn);
    connect(pauseBtn, &MenuButton::clicked, this, &GameScene::pauseSlot);

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
    qDebug() << e_timer.elapsed() - pauseTime << " " << e_timer.elapsed() << " " << pauseTime;
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
    FullTime = Total_time;
    memset(pressed_and_long,0,sizeof(pressed_and_long));

    //Total_time = 11000; // Just for debugging
    //decide KeyVal
    for(int i = 0; i < nTracks; i ++){
            keyVal[i] = key_val_[i];
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
    audioOutput->setVolume(music_vol);
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
    for(int i = 0; i < nTracks; i ++){
        keyVal[i] = key_val_[i];
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
    FullTime = -pauseTime + e_timer.elapsed();
    AllTimer->start(Total_time - FullTime);
    is_paused = 0;
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
    //start_pause = clock();
    is_paused = 1;
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
    pauseBGRect -> setZValue(150);
    addItem(pauseBGRect);
    // button
    btnContinue = new PauseButton("pause-continue");
    btnContinue->setPos(SCREEN_WIDTH *0.35, SCREEN_HEIGHT * 0.55);
    btnContinue -> setZValue(151);
    addItem(btnContinue);
    btnBack = new PauseButton("pause-back");
    btnBack->setPos(SCREEN_WIDTH *0.65, SCREEN_HEIGHT * 0.55);
    btnBack -> setZValue(151);
    addItem(btnBack);
    connect(btnContinue, &PauseButton::button_clicked, this, &GameScene::GoOnGame);
    connect(btnBack, &PauseButton::button_clicked, this, &GameScene::handleCloseGameAndPauseWindow);
    // PAUSE text
    QPixmap pauseText(":/element/resources/pause-text.png");
    pauseBGText = new QGraphicsPixmapItem(pauseText);
    pauseBGText->setOffset(-pauseText.width()/2, -pauseText.height()/2);
    pauseBGText->setScale(SCREEN_WIDTH*0.3 / pauseText.width());
    pauseBGText->setPos(SCREEN_WIDTH*0.5, SCREEN_HEIGHT*0.4);
    pauseBGText->setZValue(151);
    addItem(pauseBGText);
}

void GameScene::endGame(){
    QFile file(rootdir+"/data/"+Route_+"/info.txt");
    //qDebug() << file.open(QIODevice :: ReadWrite | QIODevice :: Text);
    if(file.open(QIODevice :: ReadWrite | QIODevice :: Text)){
        qDebug() << "rewrite info.txt";

        QTextStream stream(&file);
        QString MW = stream.readLine();
        QString L = stream.readLine();
        QString DI = stream.readLine();
        QString Ys = stream.readLine();

        int w = Ys.toInt();
        qDebug() << score << " " << w;
        if(score > w){
            Ys = Int2String0(score);
        }

        file.resize(0);
        QTextStream out(&file);
        out << MW << '\n' << L << '\n' << DI << '\n' << Ys << '\n';

        //qDebug() << MW;

        //addItem(MusWriter);
        //addItem(Length);
        //addItem(Difficulty);
    }

    QGraphicsItemAnimation* animation = new QGraphicsItemAnimation;
    animation -> setItem(trackBoard);
    QTimeLine* timeline = new QTimeLine(800);
    animation -> setTimeLine(timeline);
    animation-> setPosAt(0,QPointF(SCREEN_WIDTH / 2 - TRACK_WIDTH * 2, 0));
    animation-> setPosAt(1,QPointF(SCREEN_WIDTH / 2 - TRACK_WIDTH * 2, -TRACK_HEIGHT));
    timeline -> start();
    GenOpacityAnimationOut(trackBoard);
    for(int i = 0; i < 4; i ++){
        QGraphicsItemAnimation* animation = new QGraphicsItemAnimation;
        animation -> setItem(detectLines[i]);
        QTimeLine* timeline = new QTimeLine(800);
        animation -> setTimeLine(timeline);
        animation-> setPosAt(0,QPointF(TRACK_WIDTH*(i-2)+SCREEN_WIDTH/2, TRACK_HEIGHT));
        animation-> setPosAt(1,QPointF(TRACK_WIDTH*(i-2)+SCREEN_WIDTH/2, 0));
        timeline -> start();
        GenOpacityAnimationOut(detectLines[i]);
    }

    removeItem(Veil);
    removeItem(scoreBoard);
    removeItem(Score_);
    removeItem(Time);
    removeItem(Combo_);
    removeItem(pauseBtn);

    QGraphicsBlurEffect* BlurEffect = new QGraphicsBlurEffect;
    background->setGraphicsEffect(BlurEffect);
    QPropertyAnimation *BlurAnimation = new QPropertyAnimation(BlurEffect, "blurRadius");
    BlurAnimation->setDuration(800);
    BlurAnimation->setKeyValueAt(0.0,0.0);
    BlurAnimation->setKeyValueAt(1.0,110.0);
    BlurAnimation->start();


    timer500 = new QTimer;
    timer500 -> start(800);
    connect(timer500,&QTimer::timeout,this,&GameScene::endGameReal);
}

void GameScene::endGameReal(){
    timer500 -> stop();
    EndingScene* eScene = new EndingScene(this,score);
    QGraphicsView* eview = new QGraphicsView(eScene, _parent); // 这里必须要加第二个参数_parent，否则会弹出一个新界面
    eview->setRenderHint(QPainter::Antialiasing);
    eview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    eview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    eview->setFixedSize(eScene->sceneRect().size().toSize());
    //eview->showFullScreen();
    // remove game view
    player->stop();
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
    if(is_paused) return;
    int now_time = e_timer.elapsed() - pauseTime;
    //now_time = 1ll * now_time * 1000 / CLOCKS_PER_SEC;
    for(int i = 0; i < nTracks; i ++){
        //qDebug() << i << " " << now_time << " " << tm[i][ptr[i]].first << " " << pressed_and_long[i];
        if((now_time - tm[i][ptr[i]].first) > eps && !pressed_and_long[i]){
            Miss(i);
            ptr[i] ++;
        }
    }
}

void GameScene::pauseSlot()
{
    pauseGame();
}
