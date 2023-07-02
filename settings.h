#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include "globalvariations.h"
#include <QResizeEvent>


class settings_button : public QGraphicsObject{
    Q_OBJECT
public:
    QString m_text;
    int W,H;
    bool m_hover, m_pressed;
    bool m_selected = 0;
    bool can_selected = 0;

    settings_button(const QString &text, int Wi, int He, QGraphicsObject *parent = nullptr)
        : QGraphicsObject(parent), m_text(text), m_hover(false), m_pressed(false) {
        W = Wi, H = He;
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

class VolumeBar : public QObject{
    Q_OBJECT

public:
    //    settings_button* A2,D2;
    settings_button* A1;
    settings_button* D1;
    QGraphicsSimpleTextItem* Vol;
    QGraphicsSimpleTextItem* Instruction;
    VolumeBar();
    //    void Add2();
    void Add1();
    //    void Dec2();
    void Dec1();
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
    VolumeBar* VBar;
    int nowchange;

    QGraphicsPixmapItem *background;
    QGraphicsSimpleTextItem *headline;
    void pl1();
    void pl2();
    void pl3();
    void pr1();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void senderrormsg(int x);
    void AddVolumeBar(VolumeBar *v);

    QGraphicsSimpleTextItem *errormsg;

    void addkeysets(keysets* w);
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
