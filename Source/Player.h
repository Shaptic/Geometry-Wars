#ifndef PLAYER_H
#define PLAYER_H

#include <list>

#include "Entity.h"
#include "Bullet.h"
#include "PowerUps.h"

class CPlayer: public CBaseObject
{
public:
    CPlayer(CDisplay& Screen, CTimer& timer);
    ~CPlayer();

    void Reset();
    void Shoot(std::list<CBullet*>& bullets);
    void Kill();
    bool CanShoot();
    bool CanDie();
    bool IsDead();
    int  GetLives(){return this->lives;}
    void Blit();
    void SetPowerUp(PowerUp* m_powerup);

private:
    /* Between shots, in frames, not seconds */
    static const int REGULAR_SHOT_DELAY = 5;
    static const int LOWERED_SHOT_DELAY = 2;
    
    /* The angle that each shot (not counting the first shot)
     * is fired at from the "barrel"
     */
    const double SHOT_ANGLE;

    /* The maximum amount of shots that can be fired 
     * in a single burst.
     */
    static const int MAX_SHOTS = 5;

    int lives;                      // Strange negative lives glitch makes signed int needed
    unsigned int SHOT_DELAY;
    unsigned int to_shoot;          // Amount of shots / burst
    unsigned int shot_delay;        // Delay between bursts
    int mouse_aim_x, mouse_aim_y;   // Mouse location

    std::list<PowerUp*> all_powerups;
};

#endif // PLAYER_H