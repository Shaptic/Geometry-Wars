#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <string>

#include "Entity.h"
#include "Player.h"

class CEnemy: public CEntity
{
public:
    CEnemy(CDisplay& m_Display, CTimer& m_Timer);
    CEnemy(CDisplay& m_Display, CTimer& m_Timer, const std::string& sprite_filename);
    CEnemy(CDisplay& m_Display, CTimer& m_Timer, const std::string& sprite_filename, int health);

    void Update(const CPlayer& m_Player);
    bool Die(const int damage);
    CPlayer::PowerUp&   GetPowerUp(){return this->powerup;}

private:
    const float ENEMY_SPEED;

    void Init();
    void AI(const CPlayer& m_Player);

    CPlayer::PowerUp powerup;

    int health;

    SDL_Surface* health_bar;
};

typedef std::list<CEnemy*> CEnemies;

#endif // __ENEMY_H__