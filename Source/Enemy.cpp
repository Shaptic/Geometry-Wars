#include "Enemy.h"

Enemy::Enemy(Display* Screen, Timer* timer, CPlayer* Player): BaseObject(Screen, timer)
{
    this->Player    = Player;
    this->x         = 1.0f * (rand() % 800);
    this->y         = 1.0f * (rand() % 600);
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