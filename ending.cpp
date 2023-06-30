#include "ending.h"

QString Int2String_(int num, int x){ // num位数，x数字，取x后num位转换为QString，不足补0
    QString ans("");
    while(num--){
        ans.prepend((char)(x % 10 + 48));
        x /= 10;
    }
    return ans;
}

EndingScene::EndingScene(QObject *parent,int v):QGraphicsScene(parent),Score(v){
    QPixmap _background(QPixmap(":/img/resources/fail-background.png"));
    _background.scaled(SCREEN_WIDTH, SCREEN_HEIGHT, Qt::KeepAspectRatioByExpanding);
    background = new QGraphicsPixmapItem(_background);
    background->setPos(0,0);
    this->addItem(background);

    YrScore = new QGraphicsSimpleTextItem((QString)("Your Score"));
    Final_Score = new QGraphicsSimpleTextItem(Int2String_(Score,7));
    Instruction = new QGraphicsSimpleTextItem("Press Any Key to return...");

    YrScore -> setFont(QFont("Arial",20));
    Final_Score -> setFont(QFont("Arial",20));
    Instruction -> setFont(QFont("Arial",20));

    //Like Phigros, a logo indicating S,A,B,C is needed

    YrScore -> setPos(300,300);
    Final_Score -> setPos(400,300);
    Instruction -> setPos(700,100);

    this -> addItem(YrScore);
    this -> addItem(Final_Score);
}

void EndingScene::keyPressEvent(QKeyEvent *event){
    emit quit_();
}
