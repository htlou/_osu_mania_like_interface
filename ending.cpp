#include "ending.h"
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>

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

QString Int2String_(int num, int x){ // num位数，x数字，取x后num位转换为QString，不足补0
    QString ans("");
    while(num--){
        ans.prepend((char)(x % 10 + 48));
        x /= 10;
    }
    return ans;
}

EndingScene::EndingScene(QObject *parent,int v):QGraphicsScene(parent),Score(v){
    QPixmap _background(QPixmap(":/img/resources/background-1.png"));
    _background = _background.scaled(SCREEN_WIDTH, SCREEN_HEIGHT, Qt::KeepAspectRatioByExpanding);
    background = new QGraphicsPixmapItem(_background);
    background->setPos(0,0);

    QGraphicsBlurEffect* blurEffect = new QGraphicsBlurEffect;
    blurEffect->setBlurRadius(110);
    background->setGraphicsEffect(blurEffect);
    this->addItem(background);
    // 左边放曲绘，先空着


    // rect items
    QGraphicsRectItem *rect1 = new QGraphicsRectItem(SCREEN_WIDTH*0.5, SCREEN_HEIGHT*0.3, SCREEN_WIDTH*0.4, SCREEN_HEIGHT*0.2);
    rect1->setBrush(Qt::black);
    rect1->setPen(Qt::NoPen);
    QGraphicsDropShadowEffect *shadowEffect1 = new QGraphicsDropShadowEffect;
    shadowEffect1->setBlurRadius(10);  // 设置阴影模糊半径
    shadowEffect1->setColor(Qt::black);  // 设置阴影颜色
    shadowEffect1->setOffset(5, 5);  // 设置阴影偏移量
    rect1->setGraphicsEffect(shadowEffect1);
    addItem(rect1);
    GenOpacityAnimationIn(rect1,0.5);
    rect1->setOpacity(0.5);

    QGraphicsRectItem *rect2 = new QGraphicsRectItem(SCREEN_WIDTH*0.5, SCREEN_HEIGHT*0.525, SCREEN_WIDTH*0.4, SCREEN_HEIGHT*0.1);
    rect2->setBrush(Qt::black);
    rect2->setPen(Qt::NoPen);
    QGraphicsDropShadowEffect *shadowEffect2 = new QGraphicsDropShadowEffect;
    shadowEffect2->setBlurRadius(10);  // 设置阴影模糊半径
    shadowEffect2->setColor(Qt::black);  // 设置阴影颜色
    shadowEffect2->setOffset(5, 5);  // 设置阴影偏移量
    rect2->setGraphicsEffect(shadowEffect2);
    addItem(rect2);
    GenOpacityAnimationIn(rect2,0.5);
    rect2->setOpacity(0.5);

    // show score
    Final_Score = new QGraphicsSimpleTextItem(Int2String_(7,Score));
    QFont font = DefaultFont;
    font.setPointSize(40);
    Final_Score -> setFont(font);
    Final_Score->setPos(SCREEN_WIDTH*0.55, SCREEN_HEIGHT*0.4 - 40);
    Final_Score->setBrush(Qt::white);
    addItem(Final_Score);
    GenOpacityAnimationIn(Final_Score,1);

    // show max combo
    QString m_combo = "72"; // max combo的数值需要从GameScene获取
    Max_combo = new QGraphicsSimpleTextItem(m_combo);
    Max_combo -> setFont(DefaultFont);
    Max_combo->setPos(SCREEN_WIDTH*0.55, SCREEN_HEIGHT*0.54);
    Max_combo->setBrush(Qt::white);
    addItem(Max_combo);
    GenOpacityAnimationIn(Max_combo,1);

    comboText = new QGraphicsSimpleTextItem(QString("Max Combo"));
    comboText -> setFont(SmallFont);
    comboText->setPos(SCREEN_WIDTH*0.55, SCREEN_HEIGHT*0.54+50);
    comboText->setBrush(Qt::white);
    addItem(comboText);
    GenOpacityAnimationIn(comboText,1);

    // show accuracy
    QString acc = "96.24%"; // max combo的数值需要从GameScene获取
    Accuracy = new QGraphicsSimpleTextItem(acc);
    Accuracy -> setFont(DefaultFont);
    Accuracy->setPos(SCREEN_WIDTH*0.75, SCREEN_HEIGHT*0.54);
    Accuracy->setBrush(Qt::white);
    addItem(Accuracy);
    GenOpacityAnimationIn(Accuracy,1);

    AccText = new QGraphicsSimpleTextItem(QString("Accuracy"));
    AccText -> setFont(SmallFont);
    AccText->setPos(SCREEN_WIDTH*0.75, SCREEN_HEIGHT*0.54+50);
    AccText->setBrush(Qt::white);
    addItem(AccText);
    GenOpacityAnimationIn(AccText,1);

    // show ranking
    int rank = 0;
    char cRank = (rank == 0) ? 'S' : ('A' + rank - 1);
    QPixmap rankPic(":/img/resources/ranking-"+QString(cRank)+".png");
    QGraphicsPixmapItem *rankItem = new QGraphicsPixmapItem(rankPic);
    rankItem->setScale(SCREEN_HEIGHT*0.15/rankPic.height());
    rankItem->setPos(SCREEN_WIDTH*0.8, SCREEN_HEIGHT*0.36);
    addItem(rankItem);
    GenOpacityAnimationIn(rankItem,1);

    // show return instruction
    QGraphicsRectItem *rect3 = new QGraphicsRectItem(SCREEN_WIDTH*0.5, SCREEN_HEIGHT*0.65, SCREEN_WIDTH*0.4, SCREEN_HEIGHT*0.1);
    rect3->setBrush(Qt::black);
    rect3->setPen(Qt::NoPen);
    rect3->setOpacity(0.5);
    QGraphicsDropShadowEffect *shadowEffect3 = new QGraphicsDropShadowEffect;
    shadowEffect3->setBlurRadius(10);  // 设置阴影模糊半径
    shadowEffect3->setColor(Qt::black);  // 设置阴影颜色
    shadowEffect3->setOffset(5, 5);  // 设置阴影偏移量
    rect3->setGraphicsEffect(shadowEffect3);
    addItem(rect3);
    GenOpacityAnimationIn(rect3,0.5);

    Instruction = new QGraphicsSimpleTextItem(QString("Press any key to return"));
    Instruction -> setFont(DefaultFont);
    Instruction->setPos(SCREEN_WIDTH*0.53, SCREEN_HEIGHT*0.67);
    Instruction->setBrush(Qt::white);
    addItem(Instruction);
    GenOpacityAnimationIn(Instruction,1);
    // Instruction -> setFont(QFont("Arial",20));

    //Like Phigros, a logo indicating S,A,B,C is needed

    // Instruction -> setPos(700,100);

    // this -> addItem(Instruction);
}

void EndingScene::keyPressEvent(QKeyEvent *event){
    emit quit_();
}
