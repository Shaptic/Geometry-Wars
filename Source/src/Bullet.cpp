#include "Bullet.h"

CBullet::CBullet(CDisplay& m_Display, CTimer& m_Timer,
    const int x, const int y):
    CEntity(m_Display, m_Timer)
{
    this->Move_Force(x, y);

    /* Acquire target */
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

void CBullet::Update()
{
    this->Move_Rate(this->dx, this->dy);
    this->Blit();
}