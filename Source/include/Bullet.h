/********************************************
 *              BULLET.H                    *
 *                                          *
 * A generic class for shooting bullets at  *
 * targets on the screen. The position      *
 * is determined by calculating the         *
 * unit vector toward the destination       *
 * based on the current location of the     *
 * mouse (target) and the current position  *
 * of the ship (x, y).                      *
 * Calls to CBullet::Update() simply update *
 * the (x, y) positioning of the bullet.    *
 * Collision detection should be handled    *
 * externally.                              *
 *******************************************/
 
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

    /* Bullet destination, position */
    float unit_vector;
    float dx, dy;
};

/* Makes it easier to have a container of CBullets. */
typedef std::list<CBullet*> CBullets;

#endif // __BULLET_H__