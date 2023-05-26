#ifndef FALLINGKEY_H
#define FALLINGKEY_H
#include <QGraphicsRectItem>
#include <QObject>

class FallingKey : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    Q_PROPERTY(qreal y READ y WRITE setY)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
public:
    FallingKey(qreal _x, qreal _y, qreal _w, qreal _h, QColor _c, int _v, QGraphicsItem* parent = 0);
    virtual void startFalling();
private:
    int duration;
    int velocity;
    qreal x, max_y, w, h;
    QColor color;
};

#endif // FALLINGKEY_H
