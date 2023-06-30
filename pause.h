#ifndef PAUSE_H
#define PAUSE_H
#include <QMainWindow>
#include <globalvariations.h>
#include <QObject>
#include <QtWidgets>
#include <QPushButton>
#include <QGraphicsScene>
#include <QObject>
#include <QDebug>
#include <QKeyEvent>
#include <QFile>
#include <QTimer>
#include <QElapsedTimer>
#include <QWidget>
#include <QGraphicsObject>

class PauseSceneButton : public QGraphicsObject{
    Q_OBJECT
public:
    QString m_text;
    bool m_hover, m_pressed;

    PauseSceneButton(const QString &text, QGraphicsObject *parent = nullptr)
        : QGraphicsObject(parent), m_text(text), m_hover(false), m_pressed(false) {
        setAcceptHoverEvents(true);
        setAcceptedMouseButtons(Qt::LeftButton);
        setCursor(Qt::PointingHandCursor);
        setZValue(1);
    }
    QRectF boundingRect()const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event)override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)override;


signals:
    void button_pressed();
};




class PauseScene: public QGraphicsScene{
    Q_OBJECT

signals:
    void back_game_sig();
    void enter_settings_sig();
    void back_menu_sig();

public slots:
    void back_game_slot(){
        qDebug()<<"back to game...";
        emit back_game_sig();
    }
    void enter_settings_slot(){
        qDebug()<<"setting sent...";
        emit enter_settings_sig();
    }
    void back_menu_slot(){
        qDebug()<<"back to menu...";
        emit back_menu_sig();
    }

public:
    QGraphicsPixmapItem *background;

    PauseScene(QObject *parent = nullptr);
};

class MyPauseWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyPauseWindow(QMainWindow *parent = nullptr);
    void resizeEvent(QResizeEvent *event);

public slots:
    void back_game_0();
    void enter_settings_0();
    void back_menu_0();


private:
    QGraphicsView *m_view;
    PauseScene *m_pauseScene= new PauseScene(this);

signals:
    void goongame();
    void closeGameAndPauseWindow();
};



#endif // PAUSE_H
