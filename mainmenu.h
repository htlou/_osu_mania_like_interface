#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
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
#include <gamescene.h>
#include "menubutton.h"

// Only for test purpose, to be deprecated in final version
class MainMenuButton : public QGraphicsObject{
    Q_OBJECT
public:
    QString m_text;
    bool m_hover, m_pressed;

    MainMenuButton(const QString &text, QGraphicsObject *parent = nullptr)
        : QGraphicsObject(parent), m_text(text), m_hover(false), m_pressed(false) {
        setAcceptHoverEvents(true);
        setAcceptedMouseButtons(Qt::LeftButton);
        setCursor(Qt::PointingHandCursor);
        setZValue(1);
    }
    QRectF boundingRect()const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event)override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)override;


signals:
    void button_pressed();
};

class MainMenu: public QGraphicsScene{
    Q_OBJECT

signals:
    void enter_select_sig();
    void enter_settings_sig();
    void quit_game_sig();

public slots:
    void enter_select_slot(){
        qDebug()<<"select sent...";
        emit enter_select_sig();
    }
    void enter_settings_slot(){
        qDebug()<<"setting sent...";
        emit enter_settings_sig();
    }
    void quit_game_slot();

public:
    QGraphicsPixmapItem *background;
    QGraphicsPixmapItem *logo;
    MenuButton *start;
    MenuButton *setting;
    MenuButton *quit;
    QGraphicsRectItem *menubar;
    void GenAnimation();
    void GenAnimation2();

    explicit MainMenu(QObject *parent = nullptr);

};




#endif // MAINMENU_H
