#include "pauseWidget.h"

PauseButton::PauseButton(const QString &name)
    : sourceName(name)
{
    QPixmap textPic((QPixmap(":/element/resources/"+name+".png")));
    setPixmap(textPic);
    setOffset(-textPic.width()/2, -textPic.height()/2);
    setScale(SCREEN_HEIGHT / 16 / boundingRect().height());
    setAcceptHoverEvents(true);
}

void PauseButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "hover event";
    QPixmap hoverPic(":/element/resources/"+sourceName+"-hover.png");
    setPixmap(hoverPic);
    setOffset(-hoverPic.width()/2, -hoverPic.height()/2);
    setScale(SCREEN_HEIGHT / 16 / boundingRect().height());
}

void PauseButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QPixmap hoverPic(":/element/resources/"+sourceName+".png");
    setPixmap(hoverPic);
    setOffset(-hoverPic.width()/2, -hoverPic.height()/2);
    setScale(SCREEN_HEIGHT / 16 / boundingRect().height());
}

void PauseButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit button_clicked();
}
