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
    void return_slot();

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
    settings_scene(QObject *parent = nullptr);
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
