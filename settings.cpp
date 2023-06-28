#include "settings.h"
#include "globalvariations.h"
#include <QResizeEvent>
#include <QGraphicsPixmapItem>

keysets::keysets(const QString &str1, const QString& str2, int px, int py, QFont font__){
    button = new settings_button(str1,nullptr);
    txt = new QGraphicsSimpleTextItem(str2);
    p_x = px, p_y = py;
    font_ = font__; flag = 0;
    connect(button,&settings_button::button_pressed,this,&keysets::being_pressed);
}

void keysets::being_pressed(){
    flag = 1;
    txt -> setBrush(Qt::red);
    emit pressed();
}

void keysets::release_(){
    flag = 0;
    txt -> setBrush(Qt::black);
    button -> m_pressed = false;
    button -> update();
}

void settings_scene::pl1(){
    L2 -> release_(), L3 -> release_(), R1 -> release_(), R2 -> release_(), R3 -> release_();
    nowchange = 1;
}
void settings_scene::pl2(){
    L1 -> release_(), L3 -> release_(), R1 -> release_(), R2 -> release_(), R3 -> release_();
    nowchange = 2;
}
void settings_scene::pl3(){
    L2 -> release_(), L1 -> release_(), R1 -> release_(), R2 -> release_(), R3 -> release_();
    nowchange = 3;
}
void settings_scene::pr1(){
    L2 -> release_(), L3 -> release_(), L1 -> release_(), R2 -> release_(), R3 -> release_();
    nowchange = 4;
}
void settings_scene::pr2(){
    L2 -> release_(), L3 -> release_(), R1 -> release_(), L1 -> release_(), R3 -> release_();
    nowchange = 5;
}
void settings_scene::pr3(){
    L2 -> release_(), L3 -> release_(), R1 -> release_(), R2 -> release_(), L1 -> release_();
    nowchange = 6;
}

QRectF settings_button::boundingRect() const{
    return QRectF(0, 0, 200, 50);
}

void settings_button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

void settings_scene::keyReleaseEvent(QKeyEvent *event){
    //nothing
}

void settings_scene::keyPressEvent(QKeyEvent *event){
    if(nowchange <= 0) return;
    if(event -> key() < 'A' || event -> key() > 'Z') return;
    key_val_[nowchange - 1] = event -> key();
    if(nowchange == 1){
        (L1 -> txt) -> setText("Key_" + QString((char)(event -> key() - 'A' + 65)));
        L1 -> release_();
        nowchange = -1;
    }
    if(nowchange == 2){
        (L2 -> txt) -> setText("Key_" + QString((char)(event -> key() - 'A' + 65)));
        L2 -> release_();
        nowchange = -1;
    }
    if(nowchange == 3){
        (L3 -> txt) -> setText("Key_" + QString((char)(event -> key() - 'A' + 65)));
        L3 -> release_();
        nowchange = -1;
    }
    if(nowchange == 4){
        (R1 -> txt) -> setText("Key_" + QString((char)(event -> key() - 'A' + 65)));
        R1 -> release_();
        nowchange = -1;
    }
    if(nowchange == 5){
        (R2 -> txt) -> setText("Key_" + QString((char)(event -> key() - 'A' + 65)));
        R2 -> release_();
        nowchange = -1;
    }
    if(nowchange == 6){
        (R3 -> txt) -> setText("Key_" + QString((char)(event -> key() - 'A' + 65)));
        R3 -> release_();
        nowchange = -1;
    }
}

void settings_button::mousePressEvent(QGraphicsSceneMouseEvent *event){
    m_pressed = true;
    update();
    //    QGraphicsObject::mousePressEvent(event);
}

void settings_button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    //m_pressed = false;
    //update();
    emit button_pressed();
    //    QGraphicsObject::mouseReleaseEvent(event);
}

void settings_button::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    m_hover = true;
    update();
    //    QGraphicsObject::hoverEnterEvent(event);
}

void settings_button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    m_hover = false;
    update();
    //    QGraphicsItem::hoverLeaveEvent(event);
}

void settings_scene::senderrormsg(){
    errormsg = new QGraphicsSimpleTextItem("Repeated Keys. Please check again.");
    errormsg -> setBrush(Qt::red);
    errormsg -> setPos(50,700);
    addItem(errormsg);
}

settings_window :: settings_window(QMainWindow *parent){
    scene = new settings_scene(this);
    view = new QGraphicsView(scene, this);
    scene->setSceneRect(0, 0, 800, 600);
    setFixedSize(800, 600);
}

void settings_window::resizeEvent(QResizeEvent *event){
    QSize size = event->size();
    view->resize(size);
    view->move(0,0);
    scene->setSceneRect(0, 0, size.width(), size.height());

    connect(scene,&settings_scene::exit_settings,this,&settings_window::quit_settings);
}

void settings_window::quit_settings(){
    for(int i = 0; i < 6; i ++)
        for(int j = i + 1; j < 6; j ++)
            if(key_val_[i] == key_val_[j]){
                scene -> senderrormsg();
                return;
            }
    emit quit_settings_();
    close();
}
