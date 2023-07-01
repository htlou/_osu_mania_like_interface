#include "mainmenu.h"
#include "globalvariations.h"
#include <gamescene.h>
#include <settings.h>

MainMenu::MainMenu(QObject *parent): QGraphicsScene(parent){

    // add background for the start menu
    QPixmap _background(QPixmap(":/img/resources/menu-background-1.jpg"));
    _background = _background.scaled(SCREEN_WIDTH, SCREEN_HEIGHT, Qt::KeepAspectRatioByExpanding);
    background = new QGraphicsPixmapItem(_background);
    //background->setOffset(-_background.width()/2, -_background.height()/2);
    //background->setPos(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    background->setPos(-(_background.width()-SCREEN_WIDTH), 0);
    addItem(background);

    MainMenuButton *startButton = new MainMenuButton("Start", nullptr);
    MainMenuButton *settingsButton = new MainMenuButton("Settings", nullptr);
    MainMenuButton *quitButton = new MainMenuButton("Quit", nullptr);
    addItem(startButton);
    addItem(settingsButton);
    addItem(quitButton);

    int y = 0;
    startButton->setPos(0, y);
    y += startButton->boundingRect().height() * 2;
    settingsButton->setPos(0, y);
    y += settingsButton->boundingRect().height() * 2;
    quitButton->setPos(0, y);

    connect(startButton, &MainMenuButton::button_pressed, this, &MainMenu::enter_select_slot);
    connect(settingsButton, &MainMenuButton::button_pressed, this, &MainMenu::enter_settings_slot);
    connect(quitButton, &MainMenuButton::button_pressed, this, &MainMenu::quit_game_slot);
}


QRectF MainMenuButton::boundingRect() const{
    return QRectF(-100, -25, 200, 50);
}

void MainMenuButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    Q_UNUSED(option);
//    Q_UNUSED(widget);

    QColor color = m_hover ? QColor(255, 255, 255, 50) : QColor(0, 0, 0, 50);
    if (m_pressed) {
        color = QColor(255, 255, 255, 100);
    }

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(Qt::white, 2));
    painter->setBrush(color);

    painter->drawRoundedRect(boundingRect(), 10, 10);
    painter->setPen(QPen(Qt::white));
    painter->drawText(boundingRect(), Qt::AlignCenter, m_text);
}

void MainMenuButton::mousePressEvent(__attribute__ ((unused))QGraphicsSceneMouseEvent *event){
    m_pressed = true;
    update();
//    QGraphicsObject::mousePressEvent(event);
}

void MainMenuButton::mouseReleaseEvent(__attribute__ ((unused))QGraphicsSceneMouseEvent *event){
    m_pressed = false;
    update();
    emit button_pressed();
//    QGraphicsObject::mouseReleaseEvent(event);
}

void MainMenuButton::hoverEnterEvent(__attribute__ ((unused))QGraphicsSceneHoverEvent *event){
    m_hover = true;
    update();
//    QGraphicsObject::hoverEnterEvent(event);
}

void MainMenuButton::hoverLeaveEvent(__attribute__ ((unused))QGraphicsSceneHoverEvent *event) {
    m_hover = false;
    update();
//    QGraphicsItem::hoverLeaveEvent(event);
}
