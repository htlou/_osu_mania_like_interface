#ifndef SELECTION_H
#define SELECTION_H
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QFile>
#include <QGraphicsItemAnimation>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include "globalvariations.h"
#include "myMainWindow.h"

class selection_button{
public:
    QString m_text; // music name
    QString Path; // music path
    qreal orig;

    int posy;
    QGraphicsRectItem *rect;
    QGraphicsSimpleTextItem *txt;
    selection_button(QString text, QString path, int Y);
};

class selection_scene : public QGraphicsScene{
Q_OBJECT
signals:
    void selected();
    void backSig();

public:
    int Num; // music num
    int Nump = 5;
    int central;
    int Central_py = SCREEN_HEIGHT / 2 - 35;
    int Lbound = 220;
    bool flag_000 = 0;
    qreal Opacity[7] = {0,1,1,1,1,1,0};
    qreal Scale[7] = {1,1,1,1.40,1,1,1};
    int Modifier[7] = {0,0,0,-14,0,0,0};
    QVector<selection_button> vec; // list of choices
    selection_scene(MyMainWindow *_parent);
    void addItem_(selection_button* button, qreal op);
    void removeItem_(selection_button* button);
    void genAnimationUp();
    void genAnimationDown();
    QGraphicsItemAnimation *animation[7];
    QGraphicsItemAnimation *animation0[7];
    QGraphicsOpacityEffect *opacityEffect[7];
    QGraphicsOpacityEffect *opacityEffect0[7];
    QGraphicsSimpleTextItem *MusWriter;
    QGraphicsSimpleTextItem *Difficulty;
    QGraphicsSimpleTextItem *Length;
    QGraphicsSimpleTextItem *YrScore;
    QGraphicsSimpleTextItem *Instruction1;
    QGraphicsSimpleTextItem *Instruction2;
    QTimeLine *timeline[7];
    QGraphicsPixmapItem *background;
    QElapsedTimer *timer;

    void keyPressEvent(QKeyEvent *event)override;
    void UpdateInfo();

public slots:
    void backSlot();

private:
    MyMainWindow *parent;
};

#endif // SELECTION_H
