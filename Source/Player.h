#ifndef PLAYER_H
#define PLAYER_H

#include <list>

#include "Entity.h"
#include "Bullet.h"
#include "PowerUps.h"

/* Between shots, in frames, not seconds */
static const int REGULAR_SHOT_DELAY = 5;
static const int LOWERED_SHOT_DELAY = 2;
static int SHOT_DELAY               = REGULAR_SHOT_DELAY;

/* The angle that each shot (not counting the first shot)
 * is fired at from the "barrel"
 */
static const double SHOT_ANGLE = 10.0;

/* The maximum amount of shots that can be fired 
 * in a single burst.
 */
static const int MAX_SHOTS = 5;

class CPlayer: public BaseObject
{
public:
    CPlayer(Display* Screen, Timer* timer);
    ~CPlayer();

    void Shoot(std::list<Bullet*>& bullets);
    void Kill();
    bool CanShoot();
    bool CanDie();
    bool IsDead();
    int  GetLives(){return this->lives;}
    void Blit();
    void SetPowerUp(PowerUp* m_powerup);

private:
    unsigned int lives;
    unsigned int to_shoot;   // Amount of shots / burst
    unsigned int shot_delay; // Delay between bursts
    int mouse_aim_x, mouse_aim_y;   // Mouse location

    PowerUp* current_powerup;
    std::list<PowerUp*> all_powerups;
};

#endif // PLAYER_H