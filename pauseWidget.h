#ifndef PAUSEWIDGET_H
#define PAUSEWIDGET_H

#include "globalvariations.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>

class PauseButton : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    //Q_PROPERTY(type name READ name WRITE setName NOTIFY nameChanged)
public:
    PauseButton(const QString &name);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

signals:
    void button_clicked();

private:
    QString sourceName;

};

#endif // PAUSEWIDGET_H
