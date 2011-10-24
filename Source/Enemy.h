#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "Player.h"

static const float ENEMY_SPEED = 2.5f;

class Enemy: public BaseObject
{
public:
    Enemy(Display* Screen, Timer* timer, CPlayer* Player);
    ~Enemy();

    void Update();

private:
    CPlayer* Player;
};

#endif // ENEMY_H