#ifndef GLOBALVARIATIONS_H
#define GLOBALVARIATIONS_H

#include <Qt>
#include <QString>
#include <QFont>

extern int SCREEN_WIDTH, SCREEN_HEIGHT;
extern double SCREEN_SCALE;
extern int key_val_[4];
extern int music_vol;
extern int eps0,eps1,eps2;
extern double VELOCITY, LATENCY, INTERVAL;
extern int TRACK_WIDTH, TRACK_HEIGHT; // to be specified after entering the game scene
extern QString SelectedPath;
extern QFont DefaultFont;
extern QString rootdir;

class GlobalVariations
{
public:
    GlobalVariations();
};

#endif // GLOBALVARIATIONS_H
