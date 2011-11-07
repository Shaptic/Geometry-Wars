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

    this->current_powerup   = NULL;
}

CPlayer::~CPlayer()
{
    this->all_powerups.clear();
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

    /* Check for invalid powerups */
    for(std::list<PowerUp*>::iterator i = this->all_powerups.begin();
        i != this->all_powerups.end(); i++)
    {
        (*i)->duration--;
        if((*i)->duration == 0)
        {
            if((*i)->ability == LOW_SHOT_DELAY)
                SHOT_DELAY = REGULAR_SHOT_DELAY;
            else if((*i)->ability == MORE_SHOTS)
                this->to_shoot -= this->to_shoot > 2 ? 2 : 0;

            delete (*i);
            this->all_powerups.erase(i);
            break;
        }
    }

    /* Update the entity on the screen. */
    if(this->main_entity != NULL)
        this->display->Blit(this->main_entity, (int)this->x, (int)this->y);
}

void CPlayer::SetPowerUp(PowerUp* powerup)
{
    if(powerup->ability == LOW_SHOT_DELAY)
        SHOT_DELAY = LOWERED_SHOT_DELAY;
    else if(powerup->ability == MORE_SHOTS && this->to_shoot < MAX_SHOTS)
        this->to_shoot += 2;

    this->current_powerup = powerup;
    this->all_powerups.push_back(powerup);
}