#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <list>

#include "SDL/SDL_mixer.h"

#include "Entity.h"
#include "Bullet.h"
#include "Settings.h"

class CPlayer: public CEntity
{
public:
    CPlayer(CDisplay& m_Display, CTimer& m_Timer, CSettingsManager& Settings);
    ~CPlayer();

    struct PowerUp
    {
        enum ABILITY
        {
            /* Powerups are organized by 
             * one-time-use ones and duration ones.
             */
            NO_POWERUP  = -1,
            EXTRA_LIFE  = 0,
            NUKE,
            AMMO_PACK,
            INVINCIBLE,
            BURST_FIRE,
            SCATTER_GUN,
            TOTAL_POWERUPS
        } ability;

        unsigned int duration;
    };

    /* Self-explanatory */
    int GetSpeed() const{return this->speed;}
    int GetAmmoCount() const{return this->bullets;}
    int GetLives() const{return this->lives;}
    int GetDamage() const{return this->damage;}

    /* Increase ammo count */
    void IncreaseAmmo(int amount);

    /* Lose a life */
    void Die(){this->lives--;}

    /* Add a powerup to the player */
    void AddPowerUp(const PowerUp& powerup);

    /* Take care of shooting */
    void Shoot(CBullets& bullets);
    bool CanShoot();

    /* Checks if the player has a certain ability */
    bool CheckPowerUp(const PowerUp::ABILITY ability);

    /* Every frame */
    void Update();

    /* Reset for a new game */
    void Reset();

private:
    /* Frames to pass between each shot */
    static const int DEF_SHOT_DELAY     = 4;
    static const int ACCL_SHOT_DELAY    = 1;
    static const int DEF_PLAYER_SPEED   = 8;

    int shot_delay;
    unsigned int speed;
    unsigned int bullets;
    unsigned int lives;
    unsigned int damage;

    std::list<PowerUp> PlayerPowerUps;

    Mix_Chunk*  NormalShot;
};

#endif // __PLAYER_H__