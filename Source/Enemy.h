#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "Player.h"
#include "PowerUps.h"

static const float ENEMY_SPEED = 2.5f;

class Enemy: public BaseObject
{
public:
    Enemy(Display* Screen, Timer* timer, CPlayer* Player);
    ~Enemy();

    void Update();
    PowerUp* GetPowerUp(){return this->powerup;}
    void SetPowerUp(int ability)
    {
        delete this->powerup;
        this->powerup = new PowerUp;
        this->powerup->duration = POWERUP_DURATION;
        this->powerup->ability = (POWERUPS)ability;
    }

    PowerUp* powerup;

private:
    CPlayer* Player;
};

#endif // ENEMY_H