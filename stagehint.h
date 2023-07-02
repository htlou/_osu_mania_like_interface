#ifndef STAGEHINT_H
#define STAGEHINT_H

#include "globalvariations.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPropertyAnimation>

class stageHint : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal MyScale READ MyScale WRITE setMyScale NOTIFY MyScaleChanged)
    Q_PROPERTY(qreal MyOpacity READ MyOpacity WRITE setMyOpacity NOTIFY MyOpacityChanged)
public:
    stageHint(const QString &name);
    void setMyScale(qreal scale);
    void setMyOpacity(qreal _opacity);
    void startAnimation();
    qreal MyScale();
    qreal MyOpacity();

public slots:
    void vanish();

signals:
    void MyScaleChanged();
    void MyOpacityChanged();

private:
    QPropertyAnimation *zoomInAnimation, *zoomOutAnimation;
};

#endif // STAGEHINT_H
