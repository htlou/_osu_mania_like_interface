#include "selection.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTimeLine>

selection_button::selection_button(QString txt_, QString pth, int Y){
    m_text = txt_; Path = pth; posy = Y;
    rect = new QGraphicsRectItem(0,0,300,40);
    txt = new QGraphicsSimpleTextItem(m_text);
}

void selection_scene::addItem_(selection_button* button, qreal op){
    button -> rect -> setPos(Lbound, button -> posy);
    button -> txt -> setPos(10+Lbound, button -> posy + 10);
    button -> rect -> setOpacity(op);
    button -> txt -> setOpacity(op);


    addItem(button -> rect);
    addItem(button -> txt);

}

void selection_scene::removeItem_(selection_button* button){
    removeItem(button -> rect);
    removeItem(button -> txt);
}

selection_scene::selection_scene(){
    QFile file(":/data/tree.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
    QTextStream stream(&file);
    central = 0;

    setSceneRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    Lbound = 220;
    Central_py = (SCREEN_HEIGHT - 40) / 2;

    QPixmap bgPic(QPixmap(":/img/resources/fail-background.png"));
    bgPic.scaled(SCREEN_WIDTH, SCREEN_HEIGHT, Qt::KeepAspectRatioByExpanding);
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(bgPic);
    background->setPos(-(bgPic.width()-SCREEN_WIDTH)/2, -(bgPic.height()-SCREEN_HEIGHT)/2);
    addItem(background);

    Num = stream.readLine().toInt();
    for(int i = 0; i < Num; i ++){
        QString line1 = stream.readLine();
        QString line2 = stream.readLine();
        qDebug() << line1 << "\n" << line2 << "\n";
        vec.push_back(selection_button(line1,line2,(i - central) * 100 + Central_py));
    }
    }
    for(int i = 0; i < Num; i ++) addItem_(&vec[i], i <= 3 ? Opacity[i + 3] : 0);

    QGraphicsRectItem* SelectRect = new QGraphicsRectItem(0,0,320,46);
    SelectRect -> setPos(Lbound-10,Central_py-3);

    addItem(SelectRect);
}

void selection_scene::genAnimationUp(){
    for(int i = central - 3 >= 0 ? central - 3 : 0; i <= central + 2 && i < Num; i ++){
        int id = i - central + 3;
        animation[id] = new QGraphicsItemAnimation;
        animation[id] -> setItem(vec[i].rect);
        animation0[id] = new QGraphicsItemAnimation;
        animation0[id] -> setItem(vec[i].txt);
        opacityEffect[id] = new QGraphicsOpacityEffect;
        opacityEffect0[id] = new QGraphicsOpacityEffect;
        vec[i].rect->setGraphicsEffect(opacityEffect[id]);
        vec[i].txt->setGraphicsEffect(opacityEffect0[id]);

        QPropertyAnimation *opacityAnimation = new QPropertyAnimation(opacityEffect[id], "opacity");
        opacityAnimation->setDuration(400);
        opacityAnimation->setKeyValueAt(0.0,Opacity[id]);
        opacityAnimation->setKeyValueAt(1.0,Opacity[id + 1]);

        QPropertyAnimation *opacityAnimation0 = new QPropertyAnimation(opacityEffect0[id], "opacity");
        opacityAnimation0->setDuration(400);
        opacityAnimation0->setKeyValueAt(0.0,Opacity[id]);
        opacityAnimation0->setKeyValueAt(1.0,Opacity[id + 1]);

        opacityAnimation -> start();
        opacityAnimation0 -> start();

        timeline[id] = new QTimeLine(400);
        timeline[id] -> setLoopCount(1);
        animation[id] -> setTimeLine(timeline[id]);
        animation0[id] -> setTimeLine(timeline[id]);

        animation[id]-> setPosAt(0,QPointF(Lbound,(id-3)*100+Central_py));
        animation[id]-> setPosAt(1,QPointF(Lbound,(id-2)*100+Central_py));
        animation0[id]-> setPosAt(0,QPointF(Lbound + 10,(id-3)*100+10+Central_py));
        animation0[id]-> setPosAt(1,QPointF(Lbound + 10,(id-2)*100+10+Central_py));

        timeline[id] -> start();
    }
}

void selection_scene::genAnimationDown(){
    for(int i = central - 2 >= 0 ? central - 2 : 0; i <= central + 3 && i < Num; i ++){
        int id = i - central + 3;
        animation[id] = new QGraphicsItemAnimation;
        animation[id] -> setItem(vec[i].rect);
        animation0[id] = new QGraphicsItemAnimation;
        animation0[id] -> setItem(vec[i].txt);
        opacityEffect[id] = new QGraphicsOpacityEffect;
        opacityEffect0[id] = new QGraphicsOpacityEffect;
        vec[i].rect->setGraphicsEffect(opacityEffect[id]);
        vec[i].txt->setGraphicsEffect(opacityEffect0[id]);

        QPropertyAnimation *opacityAnimation = new QPropertyAnimation(opacityEffect[id], "opacity");
        opacityAnimation->setDuration(400);
        opacityAnimation->setKeyValueAt(0.0,Opacity[id]);
        opacityAnimation->setKeyValueAt(1.0,Opacity[id - 1]);

        QPropertyAnimation *opacityAnimation0 = new QPropertyAnimation(opacityEffect0[id], "opacity");
        opacityAnimation0->setDuration(400);
        opacityAnimation0->setKeyValueAt(0.0,Opacity[id]);
        opacityAnimation0->setKeyValueAt(1.0,Opacity[id - 1]);

        opacityAnimation -> start();
        opacityAnimation0 -> start();

        timeline[id] = new QTimeLine(400);
        timeline[id] -> setLoopCount(1);
        animation[id] -> setTimeLine(timeline[id]);
        animation0[id] -> setTimeLine(timeline[id]);

        animation[id]-> setPosAt(0,QPointF(Lbound,(id-3)*100+Central_py));
        animation[id]-> setPosAt(1,QPointF(Lbound,(id-4)*100+Central_py));
        animation0[id]-> setPosAt(0,QPointF(Lbound + 10,(id-3)*100+10+Central_py));
        animation0[id]-> setPosAt(1,QPointF(Lbound + 10,(id-4)*100+10+Central_py));

        timeline[id] -> start();
    }
}

void selection_scene::keyPressEvent(QKeyEvent* event){
    if (event->isAutoRepeat()) {
        return;
    }
    if(event -> key() == Qt::Key_W){
        if(central > 0){
        genAnimationUp();
        central --;
        for(int i = 0;i < Num; i ++){
            vec[i].posy = (i - central) * 100 + Central_py;
            if(abs(i - central) <= 2){
                vec[i] .rect ->setOpacity(1.0);
                vec[i].txt->setOpacity(1.0);
            }
            else if(abs(i - central) >= 4){
                vec[i].rect->setOpacity(0);
                vec[i].txt->setOpacity(0);
            }
        }
        }
    }
    else if(event -> key() == Qt::Key_S){
        if(central + 1 < Num){
        genAnimationDown();
        central ++;
        for(int i = 0;i < Num; i ++){
            vec[i].posy = (i - central) * 100 + Central_py;
            if(abs(i - central) <= 2){
                vec[i] .rect ->setOpacity(1.0);
                vec[i].txt->setOpacity(1.0);
            }
            else if(abs(i-central) >= 4){
                vec[i].rect->setOpacity(0);
                vec[i].txt->setOpacity(0);
            }
        }
        }
    }
    else if(event -> key() == Qt::Key_Space){
        SelectedPath = vec[central].Path;
        qDebug() << "now Entered" << " " << SelectedPath;
        emit selected();
    }
}

