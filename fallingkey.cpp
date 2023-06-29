#include "fallingkey.h"

#include <QPainter>
#include <QDebug>

FallingKey::FallingKey(int _trackid, int _starttime, int _endtime, QGraphicsScene *_parent)
    : startTime(_starttime), endTime(_endtime), trackID(_trackid), parent(_parent)
{
    stylePath = ":/element/resources/mania-note1.png";
    QPixmap stylePic((QPixmap(stylePath)));
    setPixmap(stylePic);
    if (endTime == -1) {
        setScale((double)TRACK_WIDTH * 0.75 / boundingRect().width());
        longKey = -1;
        // set initial position
        setPos(SCREEN_WIDTH / 2 + (trackID-1.875)*TRACK_WIDTH, -boundingRect().height() * (double)TRACK_WIDTH * 0.75 / boundingRect().width());
    } else {
        longKey = (endTime - startTime) / (INTERVAL * VELOCITY); // 计算长键的长度
        QTransform trans;
        trans.scale((double)TRACK_WIDTH * 0.75 / boundingRect().width(), longKey / boundingRect().height());
        setTransform(trans);
        // set initial position
        setPos(SCREEN_WIDTH / 2 + (trackID-1.875)*TRACK_WIDTH, -longKey);
    }
    // set visible boundary -- not realised yet
//    QPainterPath path;
//    path.addRect(SCREEN_WIDTH-(trackID-2)*TRACK_WIDTH, 0, TRACK_WIDTH, TRACK_HEIGHT);
//    m_boundaryPath = path;
//    update();

    //setOpacity(0);
    isFalling = false;
}

// override paint to hide long key (when the key falls over the boundary)
//void FallingKey::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    // set visible boundary
//    if(!m_boundaryPath.isEmpty()){
//        QPainterPath path = mapFromScene(m_boundaryPath);
//        if(!path.isEmpty())
//            painter->setClipPath(path);
//    }
//    QGraphicsPixmapItem::paint(painter, option, widget);
//}

void FallingKey::startFalling() {
    qDebug() << "start falling";
    setOpacity(1);
    isFalling = true;
    // set up timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &FallingKey::fall);
    m_timer->start(INTERVAL);
}

void FallingKey::pauseFalling()
{
    qDebug() << "falling paused";
    m_timer->stop();
}

void FallingKey::resumeFalling()
{
    qDebug() << "falling continued";
    m_timer->start(INTERVAL);
}

void FallingKey::fall()
{
    // qDebug() << "key on" << trackID << "fall";
    setY(pos().y() + VELOCITY);
    if (pos().y() >= TRACK_HEIGHT && longKey == -1) {
        m_timer->stop();
        qDebug() << "end of falling";
        emit endOfFalling();
        // deleteLater();
        setOpacity(0);
    } else if (longKey != -1) {
        if (pos().y() >= TRACK_HEIGHT) {
            m_timer->stop();
            emit endOfFalling();
            // deleteLater();
            setOpacity(0);
        }
    }
}
