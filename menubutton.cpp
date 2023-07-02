#include "menubutton.h"
#include "QtGui/qpainter.h"
#include "globalvariations.h"

MenuButton::MenuButton(const QString &name)
    : _pixmap(":/element/resources/"+name+".png")
{
    //QPixmap start(":/element/resources/play.png");
    QPixmap start(":/element/resources/"+name+".png");
    setPixmap(start);
    setOffset(-start.width()/2, -start.height()/2);

    setAcceptHoverEvents(true);
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape); //设置按钮的响应范围为边界矩形的范围

}

void MenuButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //    Q_UNUSED(option);
    //    Q_UNUSED(widget);

    QColor color = m_hover ? QColor(255, 255, 255, 50) : QColor(0, 0, 0, 50);
    if (m_pressed) {
        color = QColor(255, 255, 255, 100);
    }

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(color);

//    qreal borderWidth = 10; // 增加的边界像素值
    // QRectF rect(posx, posy, w, h);
    painter->drawRoundedRect(boundingRect(), 10, 10);
    painter->setPen(QPen(Qt::white));
    painter->drawPixmap(boundingRect().toRect(), _pixmap);
    //painter->drawText(boundingRect(), Qt::AlignCenter, );
}

QRectF MenuButton::boundingRect() const{
    return QRectF(0, 0, SCREEN_HEIGHT*0.2, SCREEN_HEIGHT*0.2);
}

void MenuButton::mousePressEvent(__attribute__ ((unused))QGraphicsSceneMouseEvent *event){
    m_pressed = true;
    update();
    //    QGraphicsObject::mousePressEvent(event);
}

void MenuButton::mouseReleaseEvent(__attribute__ ((unused))QGraphicsSceneMouseEvent *event){
    m_pressed = false;
    update();
    emit clicked();
    //    QGraphicsObject::mouseReleaseEvent(event);
}

void MenuButton::hoverEnterEvent(__attribute__ ((unused))QGraphicsSceneHoverEvent *event){
    m_hover = true;
    update();
    //    QGraphicsObject::hoverEnterEvent(event);
}

void MenuButton::hoverLeaveEvent(__attribute__ ((unused))QGraphicsSceneHoverEvent *event) {
    m_hover = false;
    update();
    //    QGraphicsItem::hoverLeaveEvent(event);
}
