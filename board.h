#ifndef BOARD_H
#define BOARD_H

#include "globalvariations.h"
#include <QObject>
#include <QGraphicsPixmapItem>

class Board : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Board();
};

// The line at the end of the track
class DetectLine : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    DetectLine();
    void onKeyPress();
    void onKeyRelease();

};

#endif // BOARD_H
