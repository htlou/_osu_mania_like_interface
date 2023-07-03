#ifndef ENDING_H
#define ENDING_H
#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "globalvariations.h"

class EndingScene : public QGraphicsScene{
Q_OBJECT
signals:
    void quit_();

public:
    EndingScene(QObject *parent = nullptr, int FinalScore = 0);
    int Score;
    QGraphicsSimpleTextItem *comboText, *AccText;
    QGraphicsSimpleTextItem* Instruction;
    QGraphicsSimpleTextItem *Final_Score, *Max_combo, *Accuracy;
    QGraphicsPixmapItem *background;

    void keyPressEvent(QKeyEvent *event);
};

#endif // ENDING_H
