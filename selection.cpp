#include "selection.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTimeLine>

selection_button::selection_button(QString txt_, QString pth, int Y){
    m_text = txt_; Path = pth; posy = Y;
    rect = new QGraphicsRectItem(0,0,300,40);
    txt = new QGraphicsSimpleTextItem(m_text);
}

int selection_scene :: ReadInt(QFile* file){
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

void selection_scene::addItem_(selection_button* button){
    button -> rect -> setPos(220, button -> posy);
    button -> txt -> setPos(230, button -> posy + 10);


    addItem(button -> rect);
    addItem(button -> txt);

}

void selection_scene::removeItem_(selection_button* button){
    removeItem(button -> rect);
    removeItem(button -> txt);
}

selection_scene::selection_scene(){
    QFile file(":/data/tree.txt");
    //Num = ReadInt(&file);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
    QTextStream stream(&file);
    central = 0;
    Num = stream.readLine().toInt();
    for(int i = 0; i < Num; i ++){
        QString line1 = stream.readLine();
        QString line2 = stream.readLine();
        qDebug() << line1 << "\n" << line2 << "\n";
        vec.push_back(selection_button(line1,line2,(i - central) * 100 + 700));
    }
    }
    for(int i = 0; i <= 2; i ++) addItem_(&vec[i]);
}

void selection_scene::genAnimationUp(){
    for(int i = central - 3 >= 0 ? central - 3 : 0; i <= central + 2 && i < Num; i ++){
        int id = i - central + 3;
        animation[id] = new QGraphicsItemAnimation;
        animation[id] -> setItem(vec[i].rect);
        animation0[id] = new QGraphicsItemAnimation;
        animation0[id] -> setItem(vec[i].txt);


        timeline[id] = new QTimeLine(400);
        timeline[id] -> setLoopCount(1);
        animation[id] -> setTimeLine(timeline[id]);
        animation0[id] -> setTimeLine(timeline[id]);

        animation[id]-> setPosAt(0,QPointF(220,(id-3)*100+700));
        animation[id]-> setPosAt(1,QPointF(220,(id-2)*100+700));
        animation0[id]-> setPosAt(0,QPointF(230,(id-3)*100+710));
        animation0[id]-> setPosAt(1,QPointF(230,(id-2)*100+710));

        qDebug() << "test1111";
        timeline[id] -> start();
    }
}

void selection_scene::keyPressEvent(QKeyEvent* event){
    if (event->isAutoRepeat()) {
        return;
    }
    if(event -> key() == Qt::Key_Up){
        if(central > 0){
        genAnimationUp();
        for(int i = central - 2 >= 0? central - 2 : 0; i <= central + 2 && i < Num; i ++){
            removeItem_(&vec[i]);
        }
        //Handle Graphics



        central --;
        for(int i = central - 2 >= 0? central - 2 : 0; i <= central + 2 && i < Num; i ++){
            vec[i].posy = (i - central) * 100 + 700;
            addItem_(&vec[i]);
        }

        }
    }
    else if(event -> key() == Qt::Key_Down){
        if(central + 1 < Num){
        for(int i = central - 2 >= 0? central - 2 : 0; i <= central + 2 && i < Num; i ++){
            removeItem_(&vec[i]);
        }
        central ++;
        for(int i = central - 2 >= 0? central - 2 : 0; i <= central + 2 && i < Num; i ++){
//            qDebug() << i <<" " << central << "pressDown!";
            vec[i].posy = (i - central) * 100 + 700;
            addItem_(&vec[i]);
        }
        }
    }
    else if(event -> key() == Qt::Key_Enter){
        //should be a signal to emit start game.
    }
}

