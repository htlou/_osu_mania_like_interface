#include "board.h"

// implementations for class DetectLine
DetectLine::DetectLine() {
    QPixmap normal(QPixmap(":/element/resources/detect_line.png"));
    setPixmap(normal);
    setScale(TRACK_WIDTH / boundingRect().width());
}

void DetectLine::onKeyPress() {
    QPixmap active(QPixmap(":/element/resources/detect_line@glow.png"));
    setPixmap(active);
    setScale(TRACK_WIDTH / boundingRect().width());
}

void DetectLine::onKeyRelease() {
    QPixmap normal(QPixmap(":/element/resources/detect_line.png"));
    setPixmap(normal);
    setScale(TRACK_WIDTH / boundingRect().width());
}

// implementations for class Board (set background track)
Board::Board() {
    QPixmap trackBg(QPixmap(":/element/resources/track_background.png"));
    setPixmap(trackBg);
    setOpacity(0.8);
    QTransform trans;
    trans.scale(TRACK_WIDTH*4/boundingRect().width(), TRACK_HEIGHT/boundingRect().height());
    setTransform(trans);
    // centralize
    setPos(SCREEN_WIDTH / 2 - TRACK_WIDTH * 2, 0);
}
