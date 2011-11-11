#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "Player.h"
#include "PowerUps.h"

class CEnemy: public CBaseObject
{
public:
    CEnemy(CDisplay& Screen, CTimer& Timer, CPlayer& m_Player, const std::string& filename);
    ~CEnemy();

    void Update();
    PowerUp* GetPowerUp(){return this->powerup;}
    void SetPowerUp(PowerUp::POWERUPS ability)
    {
        delete this->powerup;
        this->powerup = new PowerUp;
        this->powerup->duration = POWERUP_DURATION;
        this->powerup->ability = ability;
    }

    PowerUp* powerup;

private:
    const float ENEMY_SPEED;

    CPlayer& Player;
};

#endif // ENEMY_H