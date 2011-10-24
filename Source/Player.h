#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

/* Between shots, in frames, not seconds */
static const int SHOT_DELAY = 1;

class CPlayer : public BaseObject
{
public:
    CPlayer(Display* Screen, Timer* timer);
    ~CPlayer();

    bool CanShoot();
    void Blit();

private:
    int shot_delay;
    int mouse_aim_x, mouse_aim_y;
    
};

#endif // PLAYER_H