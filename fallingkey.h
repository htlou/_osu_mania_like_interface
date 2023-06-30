#ifndef FALLINGKEY_H
#define FALLINGKEY_H
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QObject>
#include <QTimer>
#include "globalvariations.h"

class FallingKey : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    FallingKey(int _trackid, int _starttime, int _endtime, QGraphicsScene *_parent=nullptr);
    virtual void startFalling();
    virtual void pauseFalling();
    virtual void resumeFalling();
//    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    bool isFalling; // 键是否正在下落；用于和主界面交互，方便暂停下落
    bool isEndFalling; // 是否结束下落，用于和主界面交互
signals:
    void endOfFalling();
public slots:
    virtual void fall();
private:
    int startTime, endTime;
    int duration;
    int trackID;
    int longKey; // -1 - 短键；>0 - 长键，长键的长度
    QGraphicsScene *parent;
    QTimer *m_timer;
    QString stylePath;
    QPainterPath m_boundaryPath;
};

#endif // FALLINGKEY_H
