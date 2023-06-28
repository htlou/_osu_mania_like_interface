#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QtWidgets>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QDebug>
#include <QKeyEvent>
#include <QStackedLayout>
#include "settings.h"

class MyMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyMainWindow(QApplication *_app, QWidget *parent = nullptr);
    QStackedLayout *layout;
    QApplication *app;
signals:
    void closeGameAndPauseWindow();
    void goongame();
public slots:
    void enter_select_0();
    void enter_settings_0();
    void quit_game_0();
//    void close_game_window();
    void return_from_settings();
    // void pause_game();
    void close_from_pause();

private:
    QGraphicsView* initStartMenu();
    QGraphicsView* initGame();
    // QGraphicsView *m_view;
    // MainMenu *m_mainMenu;
};


#endif // MYMAINWINDOW_H
