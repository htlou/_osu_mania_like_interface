#include "mainmenu.h"
#include <gamescene.h>

QRectF MainMenuButton::boundingRect() const{
    return QRectF(-100, -25, 200, 50);
}

void MainMenuButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    Q_UNUSED(option);
//    Q_UNUSED(widget);

    // 不需要绘制任何内容，因为QPushButton已经处理了绘制逻辑
    // 上面那句话现在是扯淡了
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

void MainMenuButton::mousePressEvent(QGraphicsSceneMouseEvent *event){
    m_pressed = true;
    update();
//    QGraphicsObject::mousePressEvent(event);
}

void MainMenuButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    m_pressed = false;
    update();
    emit button_pressed();
//    QGraphicsObject::mouseReleaseEvent(event);
}

void MainMenuButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    m_hover = true;
    update();
//    QGraphicsObject::hoverEnterEvent(event);
}

void MainMenuButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    m_hover = false;
    update();
//    QGraphicsItem::hoverLeaveEvent(event);
}

MyMainWindow::MyMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 创建场景和视图
    m_mainMenu = new MainMenu(this);
    m_view = new QGraphicsView(m_mainMenu, this);

    // 将视图设置为主窗口的中心窗口
    setCentralWidget(m_view);

    // 连接主菜单的信号和槽函数/*
    connect(m_mainMenu, &MainMenu::enter_select_sig, this, &MyMainWindow::enter_select_0);
    connect(m_mainMenu, &MainMenu::enter_settings_sig, this, &MyMainWindow::enter_settings_0);
    connect(m_mainMenu, &MainMenu::quit_game_sig, this, &MyMainWindow::quit_game_0);
}

void MyMainWindow::enter_select_0()
{
    qDebug()<<"select ongoing...";
    // 在此处理进入游戏的逻辑
//    QGraphicsView view;
//    GameScene scene;
//    view.setRenderHint(QPainter::Antialiasing);
//    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    view.setScene(&scene);
//    view.setFixedSize(scene.sceneRect().size().toSize());
//    view.show();
    GameScene *scene = new GameScene(this);
    QGraphicsView *view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(scene->sceneRect().size().toSize());
    view->setWindowFlags(Qt::Window);
    view->showFullScreen();
//    centralWidget()->layout()->addWidget(view);
        // 隐藏主菜单视图

}

void MyMainWindow::enter_settings_0()
{
    // 在此处理打开设置的逻辑

    qDebug()<<"setting ongoing...";
}

void MyMainWindow::quit_game_0()
{
    // 在此处理退出游戏的逻辑
    qDebug()<<"quit ongoing...";
    QApplication::quit();
}

