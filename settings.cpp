#include "settings.h"
#include "globalvariations.h"
#include <QResizeEvent>
#include <QGraphicsPixmapItem>

template<class T>
void GenOpacityAnimation_(T* w){
    QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect;
    w->setGraphicsEffect(opacityEffect);
    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    opacityAnimation->setDuration(500);
    opacityAnimation->setKeyValueAt(0.0,0.0);
    opacityAnimation->setKeyValueAt(1.0,1.0);
    opacityAnimation->start();
}

keysets::keysets(const QString &str1, const QString& str2, int px, int py, QFont font__){
    button = new settings_button(str1,200,50,nullptr);
    button -> can_selected = 1;
    txt = new QGraphicsSimpleTextItem(str2);
    txt -> setBrush(Qt::white);
    p_x = px, p_y = py;
    font_ = font__; flag = 0;
    connect(button,&settings_button::button_pressed,this,&keysets::being_pressed);
}

void keysets::being_pressed(){
    flag = 1;
    txt -> setBrush(Qt::white);
    emit pressed();
}

void keysets::release_(){
    flag = 0;
    txt -> setBrush(Qt::white);
    button -> m_pressed = 0;
    button -> m_selected = 0;
    button -> update();
}

void settings_scene::pl1(){
    L2 -> release_(), L3 -> release_(), R1 -> release_();
    nowchange = 1;
}
void settings_scene::pl2(){
    L1 -> release_(), L3 -> release_(), R1 -> release_();
    nowchange = 2;
}
void settings_scene::pl3(){
    L1 -> release_(), L2 -> release_(), R1 -> release_();
    nowchange = 3;
}
void settings_scene::pr1(){
    L2 -> release_(), L3 -> release_(), L1 -> release_();
    nowchange = 4;
}

QRectF settings_button::boundingRect() const{
    return QRectF(0, 0, W, H);
}

void settings_button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QColor color = m_hover ? QColor(255, 255, 255, 50) : QColor(0, 0, 0, 50);
    if (m_pressed || (can_selected && m_selected)) {
        color = QColor(255, 255, 255, 100);
    }

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(Qt::white, 0));
    painter->setBrush(color);

    painter->drawRoundedRect(boundingRect(), 10, 10);
    painter->setPen(QPen(Qt::white));

    QFont font___ = DefaultFont;
    font___.setPointSize(16);
    painter->setFont(font___);
    painter->drawText(boundingRect(), Qt::AlignCenter, m_text);
}

void settings_scene::addkeysets(keysets *w){
    int px = w->p_x, py = w->p_y;
    addItem(w->button);
    addItem(w->txt);
    (w->button)->setPos(px,py);
    (w->txt)->setPos(px+240,py);
    (w->txt)->setFont(w->font_);
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
}

void settings_button::mousePressEvent(QGraphicsSceneMouseEvent *event){
    m_pressed = true;
    m_selected = true;
    update();
    //    QGraphicsObject::mousePressEvent(event);
}

void settings_button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    m_pressed = false;
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

void settings_scene::senderrormsg(int Pos){
    errormsg = new QGraphicsSimpleTextItem("Repeated Keys. Please check again.");
    QFont Font = DefaultFont;
    Font.setPointSize(15);
    errormsg -> setBrush(Qt::red);
    errormsg -> setFont(Font);
    errormsg -> setPos(150, 75 + 160 + Pos * 150);

    addItem(errormsg);
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    opacityAnimation->setDuration(1000);
    opacityAnimation->setKeyValueAt(0.0,0);
    opacityAnimation->setKeyValueAt(0.2,1);
    opacityAnimation->setKeyValueAt(0.8,1);
    opacityAnimation->setKeyValueAt(1.0,0);
    errormsg -> setGraphicsEffect(opacityEffect);
    opacityAnimation -> start();


}

void settings_scene::return_slot(){
    qDebug() << "return from settings";
    for(int i = 0; i < 4; i ++)
        for(int j = i + 1; j < 4; j ++)
            if(key_val_[i] == key_val_[j]){
                senderrormsg(i);
                return;
            }
    emit exit_settings();
}

void settings_scene::AddVolumeBar(VolumeBar *v){
    addItem(v -> Instruction);
    addItem(v -> D1);
    addItem(v -> A1);
    addItem(v -> Vol);
}

settings_scene::settings_scene(QObject *parent): QGraphicsScene(parent){
    QPixmap _background(QPixmap(":/img/resources/fail-background.png"));
    _background = _background.scaled(SCREEN_WIDTH, SCREEN_HEIGHT, Qt::KeepAspectRatioByExpanding);
    background = new QGraphicsPixmapItem(_background);
    background->setPos(0,0);
    this->addItem(background);

    settings_button* return_button = new settings_button("Exit",150,150,nullptr);
    return_button -> setPos(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 200);
    addItem(return_button);
    connect(return_button, &settings_button::button_pressed,this,&settings_scene::return_slot);

    VBar = new VolumeBar;
    AddVolumeBar(VBar);

    QFont font__ = DefaultFont;
    font__.setPointSize(48);
    headline = new QGraphicsSimpleTextItem(QString("SETTINGS"));
    headline -> setFont(font__);
    headline -> setPos(SCREEN_WIDTH/2 - 225, SCREEN_HEIGHT / 30);
    headline -> setBrush(Qt::white);
    addItem(headline);

    font__.setPointSize(28);

    nowchange = -1;
    L1 = new keysets("Left Key 1","Key_"+QString((char)(key_val_[0]-65+'A')),100,160,font__);
    L2 = new keysets("Left Key 2","Key_"+QString((char)(key_val_[1]-65+'A')),100,310,font__);
    L3 = new keysets("Right Key 1","Key_"+QString((char)(key_val_[2]-65+'A')),100,460,font__);
    R1 = new keysets("Right Key 2","Key_"+QString((char)(key_val_[3]-65+'A')),100,610,font__);

    addkeysets(L1),addkeysets(L2),addkeysets(L3),addkeysets(R1);

    connect(L1,&keysets::pressed,this,&settings_scene::pl1);
    connect(L2,&keysets::pressed,this,&settings_scene::pl2);
    connect(L3,&keysets::pressed,this,&settings_scene::pl3);
    connect(R1,&keysets::pressed,this,&settings_scene::pr1);
    //位置是随便放的
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
    for(int i = 0; i < 4; i ++)
        for(int j = i + 1; j < 4; j ++)
            if(key_val_[i] == key_val_[j]){
                scene -> senderrormsg(i);
                return;
            }
    emit quit_settings_();
    close();
}

QString Int2String2(int x){
    QString ans("");
    if(x == 0) ans = QString("0");
    while(x){
        ans.prepend((char)(x % 10 + 48));
        x /= 10;
    }
    return ans;
}

VolumeBar::VolumeBar(){
    D1 = new settings_button("-",50,50,nullptr);
    A1 = new settings_button("+",50,50,nullptr);
    D1 -> setPos(100,880);
    A1 -> setPos(600,880);
    Vol = new QGraphicsSimpleTextItem(Int2String2(music_vol/5));
    Instruction = new QGraphicsSimpleTextItem("Music Volume");
    QFont Font = DefaultFont; Font.setPointSize(44);
    Vol -> setFont(Font);
    Vol -> setPos(340,860);
    Font.setPointSize(34);
    Instruction -> setFont(Font);
    Instruction -> setPos(100, 800);

    connect(D1,&settings_button::button_pressed,this,&VolumeBar::Dec1);
    connect(A1,&settings_button::button_pressed,this,&VolumeBar::Add1);
}

void VolumeBar::Dec1(){
    if(music_vol == 0)return;
    music_vol -= 5;
    Vol -> setText(Int2String2(music_vol/5));
    if(music_vol <= 45) Vol -> setPos(360,860);
    else Vol -> setPos(340, 860);
}

void VolumeBar::Add1(){
    if(music_vol == 100) return;
    music_vol += 5;
    Vol -> setText(Int2String2(music_vol/5));
    if(music_vol <= 45) Vol -> setPos(360,860);
    else Vol -> setPos(340, 860);
}


