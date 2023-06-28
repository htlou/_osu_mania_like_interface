#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "globalvariations.h"
#include <QResizeEvent>


class settings_button : public QGraphicsObject{
    Q_OBJECT
public:
    QString m_text;
    bool m_hover, m_pressed;

    settings_button(const QString &text, QGraphicsObject *parent = nullptr)
        : QGraphicsObject(parent), m_text(text), m_hover(false), m_pressed(false) {
        setAcceptHoverEvents(true);
        setAcceptedMouseButtons(Qt::LeftButton);
        setCursor(Qt::PointingHandCursor);
        setZValue(1);
    }
    QRectF boundingRect()const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event)override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)override;


signals:
    void button_pressed();
};


class keysets : public QObject{
Q_OBJECT

signals:
    void pressed();

public:
    settings_button* button;
    QGraphicsSimpleTextItem* txt;
    QFont font_;
    int p_x,p_y;
    bool flag;
    keysets(const QString &str1, const QString& str2, int posx,int posy, QFont font__);
    void being_pressed();
    void release_();
};

class settings_scene : public QGraphicsScene {
Q_OBJECT

signals:
    void exit_settings();

public slots:
    void return_slot(){
        qDebug() << "return from settings";
        emit exit_settings();
    }


public:
    keysets* L1;
    keysets* L2;
    keysets* L3;
    keysets* R1;
    keysets* R2;
    keysets* R3;
    int nowchange;

    QGraphicsPixmapItem *background;
    void pl1();
    void pl2();
    void pl3();
    void pr1();
    void pr2();
    void pr3();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void senderrormsg();

    QGraphicsSimpleTextItem *errormsg;

    void addkeysets(keysets* w){
        int px = w->p_x, py = w->p_y;
        addItem(w->button);
        addItem(w->txt);
        (w->button)->setPos(px,py);
        (w->txt)->setPos(px+30,py+80);
        (w->txt)->setFont(w->font_);
    }
    settings_scene(QObject *parent = nullptr): QGraphicsScene(parent){
        QPixmap _background(":/img/resources/bg1.jpg");
        _background.scaled(SCREEN_WIDTH, SCREEN_HEIGHT, Qt::KeepAspectRatioByExpanding);
        background = new QGraphicsPixmapItem(_background);
        background->setPos(0,0);
        this->addItem(background);

        settings_button* return_button = new settings_button("Exit", nullptr);
        addItem(return_button);
        connect(return_button, &settings_button::button_pressed,this,&settings_scene::return_slot);

        QFont font__ = QFont("Arial",22);

        nowchange = -1;
        L1 = new keysets("Left Key 1","Key_"+QString((char)(key_val_[0]-65+'A')),100,100,font__);
        L2 = new keysets("Left Key 2","Key_"+QString((char)(key_val_[1]-65+'A')),350,100,font__);
        L3 = new keysets("Left Key 3","Key_"+QString((char)(key_val_[2]-65+'A')),600,100,font__);
        R1 = new keysets("Right Key 1","Key_"+QString((char)(key_val_[3]-65+'A')),100,300,font__);
        R2 = new keysets("Right Key 2","Key_"+QString((char)(key_val_[4]-65+'A')),350,300,font__);
        R3 = new keysets("Right Key 3","Key_"+QString((char)(key_val_[5]-65+'A')),600,300,font__);

        addkeysets(L1),addkeysets(L2),addkeysets(L3),addkeysets(R1),addkeysets(R2),addkeysets(R3);

        connect(L1,&keysets::pressed,this,&settings_scene::pl1);
        connect(L2,&keysets::pressed,this,&settings_scene::pl2);
        connect(L3,&keysets::pressed,this,&settings_scene::pl3);
        connect(R1,&keysets::pressed,this,&settings_scene::pr1);
        connect(R2,&keysets::pressed,this,&settings_scene::pr2);
        connect(R3,&keysets::pressed,this,&settings_scene::pr3);
        //位置是随便放的
    }
};

class settings_window : public QMainWindow{
Q_OBJECT

signals:
    void quit_settings_();

public:
    settings_window(QMainWindow *parent = nullptr);
    void resizeEvent(QResizeEvent *event);
    int volume;
    int Keyvalue[10];
    QGraphicsView *view;
    settings_scene *scene;

    void volumebar();
    void quit_settings();
};




#endif // SETTINGS_H
