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

    PauseScene(QObject *parent = nullptr): QGraphicsScene(parent){
        PauseSceneButton *gameButton = new PauseSceneButton("Back to game", nullptr);
        PauseSceneButton *settingsButton = new PauseSceneButton("Settings", nullptr);
        PauseSceneButton *menuButton = new PauseSceneButton("Exit game& Return to menu", nullptr);

        addItem(gameButton);
        addItem(settingsButton);
        addItem(menuButton);

        QPixmap _background(":/img/resources/bg2.jpg");
        _background.scaled(SCREEN_WIDTH, SCREEN_HEIGHT, Qt::KeepAspectRatioByExpanding);
        background = new QGraphicsPixmapItem(_background);
        this->addItem(background);


        //这里xy位置不对，调一下
        int y = this->sceneRect().height()/2+gameButton->boundingRect().height() * 2;
        int x = 2*this->sceneRect().width()/3;

        background->setPos(x,y);


        gameButton->setPos(x, y);
        y += gameButton->boundingRect().height() * 2;
        settingsButton->setPos(x, y);
        y += settingsButton->boundingRect().height() * 2;
        menuButton->setPos(x, y);

        connect(gameButton, &PauseSceneButton::button_pressed, this, &PauseScene::back_game_slot);
        connect(settingsButton, &PauseSceneButton::button_pressed, this, &PauseScene::enter_settings_slot);
        connect(menuButton, &PauseSceneButton::button_pressed, this, &PauseScene::back_menu_slot);
    }

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
