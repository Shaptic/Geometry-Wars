#ifndef BULLET_H
#define BULLET_H

#include <cmath>
#include "Entity.h"

static const double PI          = std::atan(1.0) * 4;

class CBullet : public CBaseObject
{
public:
    CBullet(CDisplay& Screen, CTimer& timer, int x, int y);
    CBullet(CDisplay& Screen, CTimer& timer, int x, int y, double angle);
    ~CBullet();

    void Update();

private:
    int   dest_x, dest_y;
    float dx, dy;
    float unit_vector;

    const float BULLET_SPEED;
};

#endif // BULLET_H