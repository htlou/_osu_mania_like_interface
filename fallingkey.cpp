#include "fallingkey.h"
#include "QtGui/qbrush.h"

#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QPainter>
#include <QDebug>

FallingKey::FallingKey(qreal _x, qreal _y0, qreal _y, qreal _w, qreal _h, QColor _c, int _v, QGraphicsItem* parent) : QGraphicsRectItem(parent),
    duration(_y / _v), x(_x), min_y(_y0), max_y(_y), w(_w), h(_h), color(_c)
{
    QRectF rect(x, -h, w, h);

    setRect(rect);
    setPen(Qt::NoPen);
    setBrush(color);
    setOpacity(0);
}

void FallingKey::startFalling() {
    qDebug() << "start falling";
    setOpacity(1);
    QSequentialAnimationGroup* group = new QSequentialAnimationGroup(this);
    QPropertyAnimation* ani1 = new QPropertyAnimation(this, "y");
    ani1->setDuration(duration);
    ani1->setStartValue(min_y);
    ani1->setEndValue(max_y+2*h);
    group->addAnimation(ani1);
    QPropertyAnimation* ani2 = new QPropertyAnimation(this, "opacity");
    ani2->setDuration(10);
    ani2->setStartValue(1);
    ani2->setEndValue(0);
    ani2->setEasingCurve(QEasingCurve::OutQuart);
    group->addAnimation(ani2);
    connect(group, &QSequentialAnimationGroup::finished,
            this, &FallingKey::deleteLater);
    group->start();
}
