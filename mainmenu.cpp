#include "mainmenu.h"
#include "globalvariations.h"
#include <gamescene.h>
#include <settings.h>
#include <QTimeLine>

MainMenu::MainMenu(QObject *parent): QGraphicsScene(parent){

    // add background for the start menu
    QPixmap _background(QPixmap(":/img/resources/background-1.png"));
    _background = _background.scaled(SCREEN_WIDTH, SCREEN_HEIGHT, Qt::KeepAspectRatioByExpanding);
    background = new QGraphicsPixmapItem(_background);
    //background->setOffset(-_background.width()/2, -_background.height()/2);
    background->setPos(0, 0);
    //background->setPos(-(_background.width()-SCREEN_WIDTH)/2, -(_background.height()-SCREEN_HEIGHT)/2);
    addItem(background);

//    MainMenuButton *startButton = new MainMenuButton("Start", nullptr);
//    MainMenuButton *settingsButton = new MainMenuButton("Settings", nullptr);
//    MainMenuButton *quitButton = new MainMenuButton("Quit", nullptr);
//    addItem(startButton);
//    addItem(settingsButton);
//    addItem(quitButton);

    qDebug() << SCREEN_WIDTH << " " << SCREEN_HEIGHT << " THIS IS SCREEN";

    menubar = new QGraphicsRectItem(0, SCREEN_HEIGHT*0.35, SCREEN_WIDTH, SCREEN_HEIGHT*0.2);
    menubar->setBrush(Qt::black);
    menubar->setOpacity(0.3);
    addItem(menubar);
    logo = new QGraphicsPixmapItem(QPixmap(":/element/resources/psu-icon.png"));
    logo->setOffset(-logo->pixmap().width()/2, -logo->pixmap().height()/2);
    logo->setScale(SCREEN_WIDTH * 0.2 / logo->pixmap().width());
    logo->setPos(SCREEN_WIDTH * 0.3, SCREEN_HEIGHT * 0.45);
    addItem(logo);
    start = new MenuButton("menu-play");
    start->setScale(SCREEN_WIDTH * 0.12 / start->pixmap().width());
    start->setPos(SCREEN_WIDTH * 0.45, SCREEN_HEIGHT * 0.35);
    qDebug() << start ->scale()  << "this is scale";
    addItem(start);
    connect(start, &MenuButton::clicked, this, &MainMenu::GenAnimation);
    setting = new MenuButton("key-setting");
    setting->setScale(SCREEN_WIDTH * 0.12 / setting->pixmap().width());
    setting->setPos(SCREEN_WIDTH * 0.58, SCREEN_HEIGHT * 0.35);
    addItem(setting);
    connect(setting, &MenuButton::clicked, this, &MainMenu::enter_settings_slot);
    quit = new MenuButton("menu-quit");
    quit->setScale(SCREEN_WIDTH * 0.12 / quit->pixmap().width());
    quit->setPos(SCREEN_WIDTH * 0.71, SCREEN_HEIGHT * 0.35);
    addItem(quit);
    connect(quit, &MenuButton::clicked, this, &MainMenu::quit_game_slot);

//    int y = 0;
//    startButton->setPos(0, y);
//    y += startButton->boundingRect().height() * 2;
//    settingsButton->setPos(0, y);
//    y += settingsButton->boundingRect().height() * 2;
//    quitButton->setPos(0, y);

//    connect(startButton, &MainMenuButton::button_pressed, this, &MainMenu::enter_select_slot);
//    connect(settingsButton, &MainMenuButton::button_pressed, this, &MainMenu::enter_settings_slot);
//    connect(quitButton, &MainMenuButton::button_pressed, this, &MainMenu::quit_game_slot);
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

void MainMenu::GenAnimation(){
    QGraphicsItemAnimation* animation = new QGraphicsItemAnimation;
    animation -> setItem(logo);
    QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect;
    logo->setGraphicsEffect(opacityEffect);
    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    opacityAnimation->setDuration(800);
    opacityAnimation->setKeyValueAt(0.0,1.0);
    opacityAnimation->setKeyValueAt(1.0,0.0);
    opacityAnimation -> start();

    QTimeLine* timeline = new QTimeLine(800);
    animation -> setTimeLine(timeline);

    animation-> setPosAt(0,QPointF(SCREEN_WIDTH * 0.30, SCREEN_HEIGHT * 0.45));
    animation-> setPosAt(1,QPointF(SCREEN_WIDTH * 0.10, SCREEN_HEIGHT * 0.45));

    timeline -> start();
    removeItem(start);
    removeItem(setting);
    removeItem(quit);

    QGraphicsOpacityEffect* opacityEffect_ = new QGraphicsOpacityEffect;
    menubar->setGraphicsEffect(opacityEffect_);
    QPropertyAnimation *opacityAnimation_ = new QPropertyAnimation(opacityEffect_, "opacity");
    opacityAnimation_->setDuration(800);
    opacityAnimation_->setKeyValueAt(0.0,1.0);
    opacityAnimation_->setKeyValueAt(1.0,0.0);
    opacityAnimation_-> start();

    connect(opacityAnimation,&QPropertyAnimation::finished,this,&MainMenu::enter_select_slot);
}

void MainMenu::quit_game_slot(){
    qDebug()<<"quit sent...";
    emit quit_game_sig();
}
