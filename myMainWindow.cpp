#include "myMainWindow.h"
#include "mainmenu.h"
#include "gamescene.h"
#include "selection.h"

MyMainWindow::MyMainWindow(QApplication *_app, QWidget *parent)
    : QMainWindow(parent), app(_app)
{
    layout = new QStackedLayout;
    layout->addWidget(initStartMenu());
}

QGraphicsView* MyMainWindow::initStartMenu()
{
    // 创建场景和视图
    MainMenu* m_mainMenu = new MainMenu(this);
    QGraphicsView* m_view = new QGraphicsView(m_mainMenu, this);

    // 将视图设置为主窗口的中心窗口
    setCentralWidget(m_view);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setFixedSize(m_mainMenu->sceneRect().size().toSize());

    // 连接主菜单的信号和槽函数/*
    connect(m_mainMenu, &MainMenu::enter_select_sig, this, &MyMainWindow::enter_select_0);
    connect(m_mainMenu, &MainMenu::enter_settings_sig, this, &MyMainWindow::enter_settings_0);
    connect(m_mainMenu, &MainMenu::quit_game_sig, this, &MyMainWindow::quit_game_0);

    return m_view;
}

QGraphicsView* MyMainWindow::initGame()
{
    QGraphicsScene *scene = new GameScene(QString("1"),this);
    QGraphicsView *view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(scene->sceneRect().size().toSize());
    // handle closethis emitted from scene
    connect((GameScene*)scene, &GameScene::closethis, this, &MyMainWindow::close_from_pause);
    //view->setWindowFlags(Qt::Window); // 不要再创建一个顶级窗口了
    return view;
}

void MyMainWindow::enter_select_0()
{
    qDebug()<<"select ongoing...";
    selection_scene *scene__ = new selection_scene();

    QGraphicsView* s_view = new QGraphicsView(scene__, this);
    s_view->setRenderHint(QPainter::Antialiasing);
    s_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    s_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    s_view->setFixedSize(SCREEN_WIDTH,SCREEN_HEIGHT);
    //s_view -> showFullScreen();

    QWidget* currentWidget = layout->currentWidget();
    layout->removeWidget(currentWidget);
    currentWidget->deleteLater();
    layout->addWidget(s_view);
    layout->currentWidget()->setFocus();
    /*QWidget *w = layout->currentWidget();
    layout->removeWidget(w);
    w->deleteLater();
    QGraphicsView* game_view = initGame();
    layout->addWidget(game_view);
    layout->currentWidget()->setFocus(); // 修复进入游戏后不响应键盘事件的问题*/
}

void MyMainWindow::enter_settings_0()
{
    // 在此处理打开设置的逻辑

    qDebug()<<"setting ongoing...";
    settings_scene* _setting = new settings_scene(this);
    QGraphicsView* s_view = new QGraphicsView(_setting, this);
    s_view->setRenderHint(QPainter::Antialiasing);
    s_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    s_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    s_view->setFixedSize(_setting->sceneRect().size().toSize());
    // show the setting window
    QWidget* currentWidget = layout->currentWidget();
    layout->removeWidget(currentWidget);
    currentWidget->deleteLater();
    layout->addWidget(s_view);
    layout->currentWidget()->setFocus();
    // handle return
    connect(_setting, &settings_scene::exit_settings, this, &MyMainWindow::return_from_settings);
//    settings_window *setting_ = new settings_window();
//    setting_->showFullScreen();
//    connect(setting_, &settings_window::quit_settings_,this,&MyMainWindow::return_from_settings);
}

void MyMainWindow::quit_game_0()
{
    // 在此处理退出游戏的逻辑
    qDebug()<<"quit ongoing...";
    QApplication::quit();
}

//void MyMainWindow::close_game_window(){
//    qDebug()<<"test";
//    view->close();

//    delete view;
//}

void MyMainWindow :: return_from_settings(){
    qDebug() << "return from settings";
    // clear interface
    QWidget* currentWidget = layout->currentWidget();
    layout->removeWidget(currentWidget);
    currentWidget->deleteLater();
    // back to the start menu
    layout->addWidget(initStartMenu());
    layout->currentWidget()->setFocus();
}

void MyMainWindow::close_from_pause()
{
    QWidget* w = layout->currentWidget();
    layout->removeWidget(w);
    layout->addWidget(initStartMenu());
    layout->currentWidget()->setFocus();
}
