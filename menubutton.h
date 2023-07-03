#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include "globalvariations.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QObject>
#include <QPropertyAnimation>

class MenuButton : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    MenuButton(const QString &name);
    void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event)override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
    QRectF boundingRect() const override;

signals:
    void clicked();
    void myScaleChanged();

private:
    bool m_pressed, m_hover;
    QPixmap _pixmap;
    // qreal posx, posy, w, h;
};

#endif // MENUBUTTON_H
