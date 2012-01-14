#include "GarbageDisposal.h"

CGarbageDisposal::CGarbageDisposal()
{}

CGarbageDisposal::~CGarbageDisposal()
{
    for(size_t i = 0; i < this->EnemyTrash.size(); i++)
    {
        delete this->EnemyTrash[i];
    }

    for(size_t i = 0; i < this->BulletTrash.size(); i++)
    {
        delete this->BulletTrash[i];
    }

    this->EnemyTrash.clear();
    this->BulletTrash.clear();
}

void CGarbageDisposal::TrashBullet(CBullet* Bullet)
{
    this->BulletTrash.push_back(Bullet);
}

void CGarbageDisposal::TrashEnemy(CEnemy* Enemy)
{
    this->EnemyTrash.push_back(Enemy);
}

void CGarbageDisposal::TrashBullets(CBullets Bullets)
{
    for(CBullets::iterator i = Bullets.begin();
        i != Bullets.end(); i++)
    {
        this->BulletTrash.push_back(*i);
    }
}

void CGarbageDisposal::TrashEnemies(CEnemies Enemies)
{
    for(CEnemies::iterator i = Enemies.begin();
        i != Enemies.end(); i++)
    {
        this->EnemyTrash.push_back(*i);
    }
}

void CGarbageDisposal::EmptyTrash(CEnemies& Enemies, CBullets& Bullets)
{
    for(size_t i = 0; i < this->EnemyTrash.size(); i++)
    {
        if(this->EnemyTrash[i] != NULL)
        {
            Enemies.remove(this->EnemyTrash[i]);
            delete this->EnemyTrash[i];
        }
    }

    for(size_t i = 0; i < this->BulletTrash.size(); i++)
    {
        if(this->BulletTrash[i] != NULL)
        {
            Bullets.remove(this->BulletTrash[i]);
            delete this->BulletTrash[i];
        }
    }

    this->EnemyTrash.clear();
    this->BulletTrash.clear();
}
