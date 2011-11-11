#include "Enemy.h"

Enemy::Enemy(Display* Screen, Timer* timer, CPlayer* Player,
    const std::string& filename): BaseObject(Screen, timer)
{
    this->LoadEntity(filename.c_str());

    this->powerup = new PowerUp;

    if(rand() % 10 == 4)
    {
        switch(rand() % TOTAL_POWERUPS)
        {
        case 0:
            this->powerup->duration = POWERUP_DURATION;
            this->powerup->ability  = SHIELD;
            break;

        case 1:
            this->powerup->duration = POWERUP_DURATION;
            this->powerup->ability  = MORE_SHOTS;
            break;

        case 2:
            this->powerup->duration = POWERUP_DURATION;
            this->powerup->ability  = LOW_SHOT_DELAY;
            break;

        case 3:
            /* Extra life has a 1/100 chance of spawning */
            if(rand() % 10 == 6)
            {
                this->powerup->duration = 1;
                this->powerup->ability  = EXTRA_LIFE;
            }
            else
            {
                this->powerup->duration = 1;
                this->powerup->ability  = NO_POWERUP;
            }
            break;

        case 4:
            /* EMP has a 1/20 chance of spawning */
            if(rand() % 2 == 1)
            {
                this->powerup->duration = 1;
                this->powerup->ability  = EMP;
            }
            else
            {
                this->powerup->duration = 1;
                this->powerup->ability  = NO_POWERUP;
            }
            break;

        default:
            this->powerup->duration = 1;
            this->powerup->ability  = NO_POWERUP;
            break;
        }
    }
    else
    {
        this->powerup->duration = 1;
        this->powerup->ability = NO_POWERUP;
    }

    this->Player    = Player;

    /* We must pick a random side offscreen to
     * spawn, top, bottom, left, right
     */
    int side = rand() % 4;

    switch(side)
    {
    case 0:     // TOP
        this->x = 1.0f * (rand() % 800);
        this->y = 1.0f * -(rand() & 50);
        break;

    case 1:     // BOTTOM
        this->x = 1.0f * (rand() % 800);
        this->y = 1.0f * (600 + rand() % 50);
        break;

    case 2:     // LEFT
        this->x = 1.0f * -(rand() % 50);
        this->y = 1.0f * (rand() % 600);
        break;

    case 3:     // RIGHT
        this->x = 1.0f * (800 + rand() % 50);
        this->y = 1.0f * (rand() % 600);
        break;
    }
}

Enemy::~Enemy()
{
    this->Player->SetPowerUp(this->powerup);
}

void Enemy::Update()
{
    float dx, dy;

    if(this->Player->GetX() > this->GetX())
        dx = ENEMY_SPEED;
    else
        dx = -ENEMY_SPEED;

    if(this->Player->GetY() > this->GetY())
        dy = ENEMY_SPEED;
    else
        dy = -ENEMY_SPEED;

    this->Move_Rate(dx, dy);
    this->Blit();
}