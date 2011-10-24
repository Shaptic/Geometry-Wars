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
}

CPlayer::~CPlayer()
{
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