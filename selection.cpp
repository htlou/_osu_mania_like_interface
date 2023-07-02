#include "selection.h"
#include "menubutton.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTimeLine>

selection_button::selection_button(QString txt_, QString pth, int Y){
    m_text = txt_; Path = pth; posy = Y; orig = 1;
    rect = new QGraphicsRectItem(0,0,340,70);
    QColor bgCol = QColor(0, 0, 0, 50);
    QFont font = DefaultFont;
    font.setPointSize(25);
    rect -> setPen(QPen(bgCol,0));
    rect -> setBrush(bgCol);
    txt = new QGraphicsSimpleTextItem(m_text);
    txt -> setBrush(Qt::white);
    txt -> setFont(font);
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

selection_scene::selection_scene(MyMainWindow *_parent)
    : parent(_parent)
{
    QFile file(":/data/tree.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
    QTextStream stream(&file);
    central = 0;

    setSceneRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    Lbound = 220;
    Central_py = (SCREEN_HEIGHT - 40) / 2;

    QPixmap bgPic(QPixmap(":/img/resources/background-1.png"));
    bgPic = bgPic.scaled(SCREEN_WIDTH, SCREEN_HEIGHT, Qt::KeepAspectRatioByExpanding);
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(bgPic);
    background->setPos(0, 0);
    //background->setPos(-(bgPic.width()-SCREEN_WIDTH)/2, -(bgPic.height()-SCREEN_HEIGHT)/2);
    addItem(background);

    Num = stream.readLine().toInt();
    for(int i = 0; i < Num; i ++){
        QString line1 = stream.readLine();
        QString line2 = stream.readLine();
        qDebug() << line1 << "\n" << line2 << "\n";
        vec.push_back(selection_button(line1,line2,(i - central) * 100 + Central_py + (i == 0) * (-14)));
    }
    }
    vec[0].rect -> setScale(Scale[3]);
    vec[0].txt -> setScale(Scale[3]);
    vec[0].orig = 1.40;
    for(int i = 0; i < Num; i ++) addItem_(&vec[i], i <= 3 ? Opacity[i + 3] : 0);


    QFont font = DefaultFont;
    font.setPointSize(28);
    font.setBold(1);
    MusWriter = new QGraphicsSimpleTextItem("Music Writer:");
    Length = new QGraphicsSimpleTextItem("Length:");
    Difficulty = new QGraphicsSimpleTextItem("Difficulty:");

    MusWriter -> setFont(font);
    Length -> setFont(font);
    Difficulty -> setFont(font);
    MusWriter -> setBrush(Qt::white);
    Length -> setBrush(Qt::white);
    Difficulty -> setBrush(Qt::white);
    MusWriter -> setPos(SCREEN_WIDTH * 0.55, SCREEN_HEIGHT * 0.20);
    Length -> setPos(SCREEN_WIDTH * 0.55, SCREEN_HEIGHT * 0.20 + 80);
    Difficulty -> setPos(SCREEN_WIDTH * 0.55, SCREEN_HEIGHT * 0.20 + 160);

    addItem(MusWriter);
    addItem(Length);
    addItem(Difficulty);

    UpdateInfo();


    // draw back-to-menu button
    MenuButton* backBtn = new MenuButton("back-icon");
    backBtn->setPos(20, 20);
    backBtn->setScale(0.25);
    addItem(backBtn);
    connect(backBtn, &MenuButton::clicked, this, &selection_scene::backSlot);

//    QGraphicsRectItem* SelectRect = new QGraphicsRectItem(0,0,320,46);
//    SelectRect -> setPos(Lbound-10,Central_py-3);

//    addItem(SelectRect);
}

void selection_scene::genAnimationUp(){
    for(int i = central - 3 >= 0 ? central - 3 : 0; i <= central + 2 && i < Num; i ++){
        int id = i - central + 3;
        animation[id] = new QGraphicsItemAnimation;
        animation[id] -> setItem(vec[i].rect);
        animation0[id] = new QGraphicsItemAnimation;
        animation0[id] -> setItem(vec[i].txt);
        if(id == 5 || id == 0){
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
        }

        timeline[id] = new QTimeLine(400);
        timeline[id] -> setLoopCount(1);
        animation[id] -> setTimeLine(timeline[id]);
        animation0[id] -> setTimeLine(timeline[id]);

        animation[id]-> setPosAt(0,QPointF(Lbound,(id-3)*100+Central_py+Modifier[id]));
        animation[id]-> setPosAt(1,QPointF(Lbound,(id-2)*100+Central_py+Modifier[id+1]));
        animation0[id]-> setPosAt(0,QPointF(Lbound + 10,(id-3)*100+10+Central_py+Modifier[id]));
        animation0[id]-> setPosAt(1,QPointF(Lbound + 10,(id-2)*100+10+Central_py+Modifier[id+1]));
        animation[id] -> setScaleAt(0,Scale[id]/vec[i].orig,Scale[id]/vec[i].orig);
        animation[id] -> setScaleAt(1,Scale[id + 1]/vec[i].orig, Scale[id + 1]/vec[i].orig);

        animation0[id] -> setScaleAt(0,Scale[id]/vec[i].orig,Scale[id]/vec[i].orig);
        animation0[id] -> setScaleAt(1,Scale[id + 1]/vec[i].orig, Scale[id + 1]/vec[i].orig);

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

        animation[id]-> setPosAt(0,QPointF(Lbound,(id-3)*100+Central_py+Modifier[id]));
        animation[id]-> setPosAt(1,QPointF(Lbound,(id-4)*100+Central_py+Modifier[id-1]));
        animation0[id]-> setPosAt(0,QPointF(Lbound + 10,(id-3)*100+10+Central_py+Modifier[id]));
        animation0[id]-> setPosAt(1,QPointF(Lbound + 10,(id-4)*100+10+Central_py+Modifier[id-1]));


        animation[id] -> setScaleAt(0,Scale[id]/vec[i].orig,Scale[id]/vec[i].orig);
        animation[id] -> setScaleAt(1,Scale[id - 1]/vec[i].orig, Scale[id - 1]/vec[i].orig);

        animation0[id] -> setScaleAt(0,Scale[id]/vec[i].orig,Scale[id]/vec[i].orig);
        animation0[id] -> setScaleAt(1,Scale[id - 1]/vec[i].orig, Scale[id - 1]/vec[i].orig);

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
        UpdateInfo();
        for(int i = 0;i < Num; i ++){
            vec[i].posy = (i - central) * 100 + Central_py;
            if(abs(i - central) <= 2){
                vec[i].rect ->setOpacity(1.0);
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
        UpdateInfo();
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
    else if(event -> key() == Qt::Key_Escape){
        emit backSig();
    }
}

void selection_scene::UpdateInfo(){
    QString Path = ":/data/data";
    QString Route = vec[central].Path;
    QFile file(Path+"/"+Route+"/info.txt");
    qDebug() << Path + "/" + Route + "/info.txt";
    if(file.open(QIODevice :: ReadOnly | QIODevice :: Text)){
        QTextStream stream(&file);
        QString MW = stream.readLine();
        QString L = stream.readLine();
        QString DI = stream.readLine();

        qDebug() << MW;
        MusWriter -> setText("Music Writer: " + MW);
        Length -> setText("Length: " + L + " sec");
        Difficulty -> setText("Difficulty Level:" + DI);
        //addItem(MusWriter);
        //addItem(Length);
        //addItem(Difficulty);
    }
}

void selection_scene::backSlot()
{
    emit backSig();
}
