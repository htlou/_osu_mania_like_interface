#ifndef GLOBALVARIATIONS_H
#define GLOBALVARIATIONS_H

#include <Qt>

extern int SCREEN_WIDTH, SCREEN_HEIGHT;
extern int key_val_[6];
extern int music_vol;
extern int eps0,eps1,eps2;
extern double VELOCITY, LATENCY, INTERVAL;
extern int TRACK_WIDTH, TRACK_HEIGHT; // to be specified after entering the game scene

class GlobalVariations
{
public:
    GlobalVariations();
};

#endif // GLOBALVARIATIONS_H
