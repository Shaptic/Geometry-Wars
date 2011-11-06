#include "Player.h"


CPlayer::CPlayer(Display* Screen, Timer* timer): BaseObject(Screen, timer)
{
    this->SetEntity(create_surface(20, 20, create_color(GREEN)));
    this->SetCollisionBoundaries(create_rect(0, 0, 20, 20));
    this->SetMovementBoundaries_Max(800 - 20, 600 - 20);
    this->SetMovementBoundaries_Min(0, 0);

    SDL_GetMouseState(&this->mouse_aim_x, &this->mouse_aim_y);
    this->shot_delay = 0;

    this->x = 1.0f * (Screen->width / 2);
    this->y = 1.0f * (Screen->height / 2);

    this->to_shoot = 1;

    this->powerup = NO_POWERUP;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Shoot(std::list<Bullet*>& shots)
{
    if(!this->CanShoot())
        return;

    /* We need to calculate the slope that the bullet needs
     * to have in order to be going in the direction of the 
     * mouse pointer.
     */
    double angle_iter = SHOT_ANGLE;

    for(short i = 0; i <= this->to_shoot; i++)
    {
        if(i % 2 == 0)
            angle_iter = -(i/2) * SHOT_ANGLE;
        else
            angle_iter = (i/2) * SHOT_ANGLE;

        Bullet* shot = new Bullet(
            this->display, this->timer,
            (int)this->x + (this->collision_box.w / 2) - 5,
            (int)this->y + (this->collision_box.h / 2) - 5,
            angle_iter);

        shot->SetEntity(create_surface(10, 10, create_color(BLUE)));
        shots.push_back(shot);
    }
}

bool CPlayer::CanShoot()
{
    if(this->shot_delay == 0)
    {
        this->shot_delay = SHOT_DELAY;
        return true;
    }

    else
        return false;
}

void CPlayer::Blit()
{
    /* Reduce the shot delay. */
    if(this->shot_delay > 0)
        this->shot_delay--;

    /* Update the entity on the screen. */
    if(this->main_entity != NULL)
        this->display->Blit(this->main_entity, (int)this->x, (int)this->y);
}

void CPlayer::SetPowerUp(POWER_UP m_powerup)
{
    printf("0x%x; %d\n", m_powerup, m_powerup & MORE_SHOTS);
    if(this->powerup & NO_POWERUP)
        this->powerup &= ~NO_POWERUP;
    if(m_powerup == NO_POWERUP)
        this->powerup = NO_POWERUP;
    else
        this->powerup |= m_powerup;

    if(this->powerup & MORE_SHOTS)
        this->to_shoot += 2;
}