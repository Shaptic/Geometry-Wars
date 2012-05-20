#include "Enemy.h"

/*=====================THIS IS MAX LINE LENGTH================================*/

CEnemy::CEnemy(CDisplay& m_Display, CTimer& m_Timer):
CEntity(m_Display, m_Timer), ENEMY_SPEED(2.5f)
{
    /* No sprite name supplied, so we must attempt to
     * load a default.
     */
    this->LoadEntity("Images"FN_SLASH"Enemy_Fireball.png");

    /* Default health */
    this->health = 100;

    this->Init();
}

CEnemy::CEnemy(CDisplay& m_Display, CTimer& m_Timer, const std::string& filename):
CEntity(m_Display, m_Timer), ENEMY_SPEED(2.5f)
{
    /* First, we must load the current sprite for
     * whatever level we are on. 
     */
    this->LoadEntity(filename.c_str());

    /* Default health */
    this->health = 100;

    this->Init();
}

CEnemy::CEnemy(CDisplay& m_Display, CTimer& m_Timer,
    const std::string& filename, const int health):
CEntity(m_Display, m_Timer), ENEMY_SPEED(2.5f)
{
    /* No sprite name supplied, so we must attempt to
     * load a default.
     */
    this->LoadEntity(filename.c_str());

    /* Set health */
    this->health = health;

    this->Init();
}

void CEnemy::Update(const CPlayer& m_Player)
{
    this->AI(m_Player);
    this->Display.Blit(this->health_bar, (int)this->GetX(), (int)this->GetY() - 5);
    this->Blit();
}

bool CEnemy::Die(const int m_damage)
{
    this->health -= m_damage;

    /* Update the health bar to show accurate health */
    int health = this->health / 4;  // The bar is 1/4th of the size of actual health
    int damage = (100 - this->health) / 4;

    SDL_FreeSurface(this->health_bar);
    this->health_bar = create_surface(25, 5);

    fill_rect(this->health_bar, create_rect(0, 0, health, 5), create_color(GREEN));
    SDL_Surface* tmp = create_surface(damage, 5, create_color(RED));
    this->Display.Blit(this->health_bar, tmp, 25 - damage, 0);
    SDL_FreeSurface(tmp);

    if(this->health <= 0)
        return true;
    else
        return false;
}

void CEnemy::Init()
{
    /* Pick a direction on the screen to spawn at */
    int seed = rand() % 4;

    switch(seed)
    {
    case 0:
        /* Assuming 0 means top. 
         * So the sprite moves to somewhere
         * (within 48 pixels of) the top border
         */
        this->Move_Force(rand() % this->Display.GetWidth(),
            0 - rand() % 48);
        break;

    case 1:
        /* Same as top, but now within
         * 48 pixels of bottom border.
         */
        this->Move_Force(rand() % this->Display.GetWidth(),
            this->Display.GetHeight() + rand() % 48);
        break;

    case 2:
        /* Now we go 48 pixels from the right
         * side of the screen.
         */
        this->Move_Force(this->Display.GetWidth() + rand() % 48, 
            rand() % this->Display.GetHeight());
        break;

    case 3:
        /* Again, but from the left */
        this->Move_Force(0 - rand() % 48,
            rand() % this->Display.GetHeight());
        break;
    }

#ifndef _DEBUG
    /* Are we going to drop a power-up on death? */
    if(rand() % 10 == 4)    // 10% chance of getting a powerup
    {
        if(rand() % 3 == 1) // 3% chance of getting the nuke
        {
            this->powerup.duration = -1;
            this->powerup.ability  = CPlayer::PowerUp::NUKE;
        }
        else if(rand() % 10 == 7) // 1% chance of getting an extra life
        {
            this->powerup.duration = -1;
            this->powerup.ability  = CPlayer::PowerUp::EXTRA_LIFE;
        }
        else    // Default 10% chance powerups
        {
            this->powerup.ability  = (CPlayer::PowerUp::ABILITY)
                (CPlayer::PowerUp::AMMO_PACK + rand() % CPlayer::PowerUp::TOTAL_POWERUPS);
            this->powerup.duration = 60 * 20;    // 20 seconds
        }
    }
    else
    {
        this->powerup.ability = CPlayer::PowerUp::NO_POWERUP;
        this->powerup.duration= -1;
    }
#else
    /* Testing power ups in debug builds */
    this->powerup.ability = CPlayer::PowerUp::BURST_FIRE;
    this->powerup.duration= 60;
#endif // _DEBUG

    /* A full health bar */
    this->health_bar = create_surface(25, 5, create_color(GREEN));
}

void CEnemy::AI(const CPlayer& m_Player)
{
    if(m_Player.GetLives() <= 0)
        return;

    /* Analyze the current player position
     * and move towards the player. 
     * TODO: Improve this
     */
    const int target_x  = (int)m_Player.GetX();
    const int target_y  = (int)m_Player.GetY();

    const int our_x     = (int)this->GetX();
    const int our_y     = (int)this->GetY();

    if(our_x > target_x)
        this->Move_Rate(-ENEMY_SPEED, 0.0f);
    else
        this->Move_Rate(ENEMY_SPEED, 0.0f);

    if(our_y > target_y)
        this->Move_Rate(0.0f, -ENEMY_SPEED);
    else
        this->Move_Rate(0.0f, ENEMY_SPEED);
}