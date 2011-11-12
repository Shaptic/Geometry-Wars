#include "Enemy.h"

CEnemy::CEnemy(CDisplay& m_Screen, CTimer& m_Timer, CPlayer& m_Player,
    const std::string& filename): Player(m_Player), CBaseObject(m_Screen, m_Timer),
    ENEMY_SPEED(2.5f)
{
    this->LoadEntity(filename.c_str());

    this->powerup = new PowerUp;

    if(rand() % 10 == 4)
    {
        switch(rand() % PowerUp::TOTAL_POWERUPS)
        {
        case 0:
            this->powerup->duration = POWERUP_DURATION;
            this->powerup->ability  = PowerUp::SHIELD;
            break;

        case 1:
            this->powerup->duration = POWERUP_DURATION;
            this->powerup->ability  = PowerUp::MORE_SHOTS;
            break;

        case 2:
            this->powerup->duration = POWERUP_DURATION;
            this->powerup->ability  = PowerUp::LOW_SHOT_DELAY;
            break;

        case 3:
            /* Extra life has a 1/100 chance of spawning */
            if(rand() % 10 == 6)
            {
                this->powerup->duration = 1;
                this->powerup->ability  = PowerUp::EXTRA_LIFE;
            }
            else
            {
                this->powerup->duration = 1;
                this->powerup->ability  = PowerUp::NO_POWERUP;
            }
            break;

        case 4:
            /* EMP has a 1/20 chance of spawning */
            if(rand() % 2 == 1)
            {
                this->powerup->duration = 1;
                this->powerup->ability  = PowerUp::EMP;
            }
            else
            {
                this->powerup->duration = 1;
                this->powerup->ability  = PowerUp::NO_POWERUP;
            }
            break;

        default:
            this->powerup->duration = 1;
            this->powerup->ability  = PowerUp::NO_POWERUP;
            break;
        }
    }
    else
    {
        this->powerup->duration = 1;
        this->powerup->ability = PowerUp::NO_POWERUP;
    }

    /* We must pick a random side offscreen to
     * spawn, top, bottom, left, right
     */
    int side = rand() % 4;

    switch(side)
    {
    case 0:     // TOP
        this->x = 1.0f * (rand() % this->Display.GetWidth());
        this->y = 1.0f * -(rand() & 50);
        break;

    case 1:     // BOTTOM
        this->x = 1.0f * (rand() % this->Display.GetWidth());
        this->y = 1.0f * (this->Display.GetHeight() + rand() % 50);
        break;

    case 2:     // LEFT
        this->x = 1.0f * -(rand() % 50);
        this->y = 1.0f * (rand() % this->Display.GetHeight());
        break;

    case 3:     // RIGHT
        this->x = 1.0f * (this->Display.GetWidth() + rand() % 50);
        this->y = 1.0f * (rand() % this->Display.GetHeight());
        break;
    }
}

CEnemy::~CEnemy()
{
    this->Player.SetPowerUp(this->powerup);
}

void CEnemy::Update()
{
    float dx, dy;

    if(this->Player.GetX() > this->GetX())
        dx = ENEMY_SPEED;
    else
        dx = -ENEMY_SPEED;

    if(this->Player.GetY() > this->GetY())
        dy = ENEMY_SPEED;
    else
        dy = -ENEMY_SPEED;

    this->Move_Rate(dx, dy);
    this->Blit();
}