#include "Bullet.h"

Bullet::Bullet(Display* Screen, Timer* timer, int x, int y): BaseObject(Screen, timer, x, y)
{
    /* Target */
    SDL_GetMouseState(&this->dest_x, &this->dest_y);

    /* v means velocity vector (vx, vy)
     * x1 is the destination vector (x1, y1)
     * x0 is the shot location vector (x0, y0)
     * c is the speed
     * Then:
     *      v   = c * (x1 - x0) / |x1 - x0|
     * So:
     *      vx  = c * (dest_x - start_x) / sqrt((dest_x - start_x)^2 + (dest_y - start_y)^2)
     *      vy  = c * (dest_y - start_y) / sqrt((dest_x - start_x)^2 + (dest_y - start_y)^2)
     */

    this->unit_vector   = sqrt(1.0f * ((dest_x - x)*(dest_x - x) + (dest_y - y)*(dest_y - y)));
    this->dx            = BULLET_SPEED * (dest_x - x) / this->unit_vector;
    this->dy            = BULLET_SPEED * (dest_y - y) / this->unit_vector;
}

Bullet::Bullet(Display* Screen, Timer* timer, int x, int y, double angle): BaseObject(Screen, timer, x, y)
{
    /* Target */
    SDL_GetMouseState(&this->dest_x, &this->dest_y);

    this->unit_vector   = sqrt(1.0f * ((dest_x - x)*(dest_x - x) + (dest_y - y)*(dest_y - y)));
    this->dx            = BULLET_SPEED * (dest_x - x) / this->unit_vector;
    this->dy            = BULLET_SPEED * (dest_y - y) / this->unit_vector;

    /* Modify vector to go off by x degrees, using matrix multiplication.
     * [x'] = [cos(angle)     -sin(angle)] [x]
     * [y']   [sin(angle)     cos(angle) ] [y]
     * Meaning:
     * x' = xcos(angle)-ysin(angle)
     * y' = xsin(angle)+ycos(angle)
     */
    angle               = angle * (PI / 180);   // Angles must be in radians
    this->dx            = 1.0f * (this->dx * std::cos(angle)) - (this->dy * std::sin(angle));
    this->dy            = 1.0f * (this->dx * std::sin(angle)) + (this->dy * std::cos(angle));    
}

Bullet::~Bullet()
{
}

void Bullet::Update()
{
    this->Move_Rate(this->dx, this->dy);
}