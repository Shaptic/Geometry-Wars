#ifndef __BULLET_H__
#define __BULLET_H__

#include <list>

#include "Entity.h"
#include "Events.h"

class CBullet: public CEntity
{
public:
    CBullet(CDisplay& m_Display, CTimer& m_Timer,
        const int x, const int y);
    ~CBullet(){}

    void Update();

private:
    static const int BULLET_SPEED = 16;

    /* Bullet target */
    int dest_x, dest_y;

    float unit_vector;
    float dx, dy;
};

typedef std::list<CBullet*> CBullets;

#endif // __BULLET_H__