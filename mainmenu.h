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
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
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
    void quit_game_slot(){
        qDebug()<<"quit sent...";
        emit quit_game_sig();
    }

public:
    MainMenu(QObject *parent = nullptr): QGraphicsScene(parent){
        MainMenuButton *startButton = new MainMenuButton("Start", nullptr);
        MainMenuButton *settingsButton = new MainMenuButton("Settings", nullptr);
        MainMenuButton *quitButton = new MainMenuButton("Quit", nullptr);

        addItem(startButton);
        addItem(settingsButton);
        addItem(quitButton);

        int y = -startButton->boundingRect().height() * 2;
        startButton->setPos(0, y);
        y += startButton->boundingRect().height() * 2;
        settingsButton->setPos(0, y);
        y += settingsButton->boundingRect().height() * 2;
        quitButton->setPos(0, y);

        connect(startButton, &MainMenuButton::button_pressed, this, &MainMenu::enter_select_slot);
        connect(settingsButton, &MainMenuButton::button_pressed, this, &MainMenu::enter_settings_slot);
        connect(quitButton, &MainMenuButton::button_pressed, this, &MainMenu::quit_game_slot);
    }

};

class MyMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyMainWindow(QWidget *parent = nullptr);

public slots:
    void enter_select_0();
    void enter_settings_0();
    void quit_game_0();


private:
    QGraphicsView *m_view;
    MainMenu *m_mainMenu;
};



#endif // MAINMENU_H
