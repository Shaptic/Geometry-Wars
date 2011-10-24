#ifndef BULLET_H
#define BULLET_H

#include <cmath>
#include "Entity.h"

static const double PI          = std::atan(1.0) * 4;
static const float BULLET_SPEED = 10.0f;

class Bullet : public BaseObject
{
public:
    Bullet(Display* Screen, Timer* timer, int x, int y);
    Bullet(Display* Screen, Timer* timer, int x, int y, double angle);
    ~Bullet();

    void Update();

private:
    int   dest_x, dest_y;
    float dx, dy;
    float unit_vector;
};

#endif // BULLET_H