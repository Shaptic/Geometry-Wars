#ifndef __GARBAGE_DISPOSAL_H__
#define __GARBAGE_DISPOSAL_H__

#include "Enemy.h"
#include "Bullet.h"

class CGarbageDisposal
{
public:
    CGarbageDisposal();
    ~CGarbageDisposal();

    void TrashEnemy(CEnemy* Enemy);
    void TrashBullet(CBullet* Bullet);
    void TrashEnemies(CEnemies Enemies);
    void TrashBullets(CBullets Bullets);

    void EmptyTrash(CEnemies& Enemies, CBullets& Bullets);

private:
    std::vector<CEnemy*>    EnemyTrash;
    std::vector<CBullet*>   BulletTrash;
};



#endif // __GARBAGE_DISPOSAL_H__