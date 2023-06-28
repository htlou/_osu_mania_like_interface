#include "pause.h"
#include "settings.h"

QRectF PauseSceneButton::boundingRect() const{
    return QRectF(0, 0, 200, 50);
}

PauseScene::PauseScene(QObject *parent): QGraphicsScene(parent){
    PauseSceneButton *gameButton = new PauseSceneButton("Back to game", nullptr);
    PauseSceneButton *settingsButton = new PauseSceneButton("Settings", nullptr);
    PauseSceneButton *menuButton = new PauseSceneButton("Exit game& Return to menu", nullptr);

    addItem(gameButton);
    addItem(settingsButton);
    addItem(menuButton);

    QPixmap _background(QPixmap(":/img/resources/bg2.jpg"));
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


void PauseSceneButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

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





void PauseSceneButton::mousePressEvent(QGraphicsSceneMouseEvent *event){
    m_pressed = true;
    update();
    //    QGraphicsObject::mousePressEvent(event);
}

void PauseSceneButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    m_pressed = false;
    update();
    emit button_pressed();
    //    QGraphicsObject::mouseReleaseEvent(event);
}

void PauseSceneButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    m_hover = true;
    update();
    //    QGraphicsObject::hoverEnterEvent(event);
}

void PauseSceneButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    m_hover = false;
    update();
    //    QGraphicsItem::hoverLeaveEvent(event);
}

MyPauseWindow::MyPauseWindow(QMainWindow *parent)
    : QMainWindow(parent)
{
    // 创建场景和视图
    m_pauseScene = new PauseScene(this);
    m_view = new QGraphicsView(m_pauseScene, this);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    m_pauseScene->setSceneRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    m_view->showFullScreen();


    // 连接主菜单的信号和槽函数/*
    connect(m_pauseScene, &PauseScene::back_game_sig, this, &MyPauseWindow::back_game_0);
    connect(m_pauseScene, &PauseScene::enter_settings_sig, this, &MyPauseWindow::enter_settings_0);
    connect(m_pauseScene, &PauseScene::back_menu_sig, this, &MyPauseWindow::back_menu_0);
}

void MyPauseWindow::back_game_0()
{
    qDebug()<<"return ongoing...";
    emit goongame();
    close();
    // 隐藏主菜单视图
}

void MyPauseWindow::enter_settings_0()
{
    // 在此处理打开设置的逻辑
    qDebug()<<"setting ongoing...";
    settings_window *setting_ = new settings_window();
    setting_->showFullScreen();
}

void MyPauseWindow::back_menu_0()
{
    // 在此处理退出游戏的逻辑
    qDebug()<<"back ongoing...";
    emit closeGameAndPauseWindow();
    close();
}

void MyPauseWindow::resizeEvent(QResizeEvent *event)
{
    QSize size = event->size();
    m_view->resize(size);
    m_view->move(0, 0);
    m_pauseScene->setSceneRect(0, 0, size.width(), size.height());
}
