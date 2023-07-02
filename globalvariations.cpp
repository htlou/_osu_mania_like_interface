#include "globalvariations.h"

int SCREEN_HEIGHT = 1, SCREEN_WIDTH = 1;
double SCREEN_SCALE = 1;
int key_val_[6]={Qt::Key_S,Qt::Key_D,Qt::Key_F,Qt::Key_H,Qt::Key_J,Qt::Key_K};
int music_vol=50;
int eps0=240,eps1=160,eps2=80;
double VELOCITY = 10, LATENCY = 1, INTERVAL = 10;
int TRACK_WIDTH = 1, TRACK_HEIGHT = 1;
QFont DefaultFont = QFont("Arial",20);
QString SelectedPath = QString("Default");

GlobalVariations::GlobalVariations()
{

}
