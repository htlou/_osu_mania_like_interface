#ifndef SELECTION_H
#define SELECTION_H
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QFile>
#include <QGraphicsItemAnimation>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include "globalvariations.h"

class selection_button{
public:
    QString m_text; // music name
    QString Path; // music path

    int posy;
    QGraphicsRectItem *rect;
    QGraphicsSimpleTextItem *txt;
    selection_button(QString text, QString path, int Y);
};

class selection_scene : public QGraphicsScene{
Q_OBJECT
signals:
    void selected();

public:
    int Num; // music num
    int Nump = 5;
    int central;
    int Central_py = 700;
    int Lbound = 220;
    qreal Opacity[7] = {0,1,1,1,1,1,0};
    QVector<selection_button> vec; // list of choices
    selection_scene();
    void addItem_(selection_button* button, qreal op);
    void removeItem_(selection_button* button);
    void genAnimationUp();
    void genAnimationDown();
    QGraphicsItemAnimation *animation[7];
    QGraphicsItemAnimation *animation0[7];
    QGraphicsOpacityEffect *opacityEffect[7];
    QGraphicsOpacityEffect *opacityEffect0[7];
    QTimeLine *timeline[7];
    QGraphicsPixmapItem *background;

    void keyPressEvent(QKeyEvent *event);

};

#endif // SELECTION_H
