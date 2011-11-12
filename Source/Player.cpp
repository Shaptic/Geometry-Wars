#include "Player.h"

CPlayer::CPlayer(CDisplay& Screen, CTimer& timer): CBaseObject(Screen, timer), SHOT_ANGLE(10.0)
{
    this->SetEntity(create_surface(20, 20, create_color(GREEN)));
    this->SetCollisionBoundaries(create_rect(0, 0, 20, 20));
    this->SetMovementBoundaries_Max(Screen.GetWidth() - 20, Screen.GetHeight() - 20);
    this->SetMovementBoundaries_Min(0, 0);

    SDL_GetMouseState(&this->mouse_aim_x, &this->mouse_aim_y);
    this->shot_delay = 0;

    this->x = 1.0f * (Screen.GetWidth()  / 2);
    this->y = 1.0f * (Screen.GetHeight() / 2);

    this->to_shoot = 1;
    this->SHOT_DELAY = REGULAR_SHOT_DELAY;

    this->lives             = 1;
}

CPlayer::~CPlayer()
{
    for(std::list<PowerUp*>::iterator i = this->all_powerups.begin();
        i != this->all_powerups.end(); i++)
    {
        delete (*i);
    }

    this->all_powerups.clear();
}

void CPlayer::Reset()
{
    SDL_GetMouseState(&this->mouse_aim_x, &this->mouse_aim_y);
    this->shot_delay = 0;

    this->x = 1.0f * (this->Display.GetWidth()  / 2);
    this->y = 1.0f * (this->Display.GetHeight() / 2);

    this->to_shoot = 1;
    this->SHOT_DELAY = REGULAR_SHOT_DELAY;

    this->lives             = 1;
}

void CPlayer::Kill()
{
    this->lives--;
}

void CPlayer::Shoot(std::list<CBullet*>& shots)
{
    if(!this->CanShoot())
        return;
    else
        this->shot_delay = SHOT_DELAY;

    /* We need to calculate the slope that the bullet needs
     * to have in order to be going in the direction of the 
     * mouse pointer.
     */
    double angle_iter = SHOT_ANGLE;

    for(unsigned short i = 0; i < this->to_shoot; i++)
    {
        if(i == 0)
            angle_iter = 0;
        else if(i % 2 == 0)
            angle_iter = -(i / 2) * SHOT_ANGLE;
        else
            angle_iter = ((i + 2) / 2) * SHOT_ANGLE;

        CBullet* shot = new CBullet(
            this->Display, this->Timer,
            (int)this->x + (this->collision_box.w / 2) - 5,
            (int)this->y + (this->collision_box.h / 2) - 5,
            angle_iter);

        shot->SetEntity(create_surface(10, 10, create_color(BLUE)));
        shots.push_back(shot);
    }
}

bool CPlayer::CanShoot()
{
    return (this->shot_delay == 0);
}

bool CPlayer::IsDead()
{
    return (this->lives <= 0);
}

void CPlayer::Blit()
{
    /* Reduce the shot delay. */
    if(this->shot_delay > 0)
        this->shot_delay--;

    /* Check for invalid powerups */
    for(std::list<PowerUp*>::iterator i = this->all_powerups.begin();
        i != this->all_powerups.end(); i++)
    {
        (*i)->duration--;
        if((*i)->duration == 0)
        {
            if((*i)->ability == PowerUp::LOW_SHOT_DELAY)
                SHOT_DELAY = REGULAR_SHOT_DELAY;
            else if((*i)->ability == PowerUp::MORE_SHOTS)
                this->to_shoot -= this->to_shoot > 2 ? 2 : 0;

            delete (*i);
            this->all_powerups.erase(i);
            break;
        }
    }

    /* Update the entity on the screen. */
    if(this->main_entity != NULL)
        this->Display.Blit(this->main_entity, (int)this->x, (int)this->y);
}

void CPlayer::SetPowerUp(PowerUp* powerup)
{
    if(powerup->ability == PowerUp::LOW_SHOT_DELAY)
        SHOT_DELAY = LOWERED_SHOT_DELAY;

    else if(powerup->ability == PowerUp::MORE_SHOTS)
    {
        /* If we are already at the maximum amount of
         * shots that we can take, simply extend the
         * amount of time that the increased shot
         * count lasts.
         */
        if(this->to_shoot >= MAX_SHOTS)
        {
            for(std::list<PowerUp*>::iterator i = this->all_powerups.begin();
                i != this->all_powerups.end(); i++)
            {
                if((*i)->ability == PowerUp::MORE_SHOTS)
                {
                    (*i)->duration += POWERUP_DURATION;
                    break;
                }
            }
            return;
        }
        else    // Otherwise just add 2 shots
        {
            this->to_shoot += 2;
        }
    }

    else if(powerup->ability == PowerUp::EXTRA_LIFE)
        this->lives++;

    this->all_powerups.push_back(powerup);
}