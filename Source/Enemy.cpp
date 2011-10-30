#include "Enemy.h"

Enemy::Enemy(Display* Screen, Timer* timer, CPlayer* Player): BaseObject(Screen, timer)
{
    if(rand() % 10 == 4)
        this->isSpecial = true;
    else
        this->isSpecial = false;

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