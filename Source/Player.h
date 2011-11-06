#ifndef PLAYER_H
#define PLAYER_H

#include <list>

#include "Entity.h"
#include "Bullet.h"
#include "PowerUps.h"

/* Between shots, in frames, not seconds */
static const int SHOT_DELAY = 5;

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
    bool CanShoot();
    void Blit();
    void SetPowerUp(POWER_UP m_powerup);

private:
    int to_shoot;   // Amount of shots / burst
    int shot_delay; // Delay between bursts
    int mouse_aim_x, mouse_aim_y;   // Mouse location

    POWER_UP powerup;
};

#endif // PLAYER_H