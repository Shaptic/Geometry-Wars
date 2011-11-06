#include "ParticleEngine.h"

Bit::Bit(Display* Screen, Timer* timer, const int x, const int y, const int m_dx, const int m_dy):
    BaseObject(Screen, timer, x, y)
{
    this->dx = m_dx;
    this->dy = m_dy;

    /* The time this particle will last (ms)*/
    this->lifetime = 25 + rand() % 25;

    this->SetEntity(create_surface(5, 5, create_color(YELLOW)));
}

Bit::Bit(Display* Screen, Timer* timer, const int x, const int y, const int m_dx, const int m_dy, const int lifetime):
    BaseObject(Screen, timer, x, y)
{
    this->dx = m_dx;
    this->dy = m_dy;

    /* The time this particle will last (ms)*/
    this->lifetime = lifetime;

    this->SetEntity(create_surface(5, 5, create_color(GREEN)));
}

Bit::~Bit()
{
}

void Bit::Update()
{
    if(this->lifetime > 0)
    {
        this->Move_Rate(this->dx, this->dy);
        this->lifetime--;
        this->Blit();
    }
}

bool Bit::IsAlive()
{
    return (this->lifetime > 0);
}


ParticleEngine::ParticleEngine(Display* Screen, Timer* timer)
{
    this->Screen    = Screen;
    this->timer     = timer;
}

ParticleEngine::~ParticleEngine()
{
    this->particles.clear();
}

void ParticleEngine::ExplodeObject(const int x, const int y)
{
    /* We will spawn anywhere from 10-30 "bits" that
     * are left-over from the destroyed enemy.
     */
    const int bits = 10 + (rand() % 20);
    int dx, dy;

    for(short i = 0; i < bits; i++)
    {
        /* We create a particle with a random velocity. */
        if(rand() % 2 == 0) // Make veloctiy negative
            dx = -(5 + (rand() % 5));
        else
            dx = (5 + (rand() % 5));

        if(rand() % 2 == 0) // Again for y direction
            dy = -(5 + (rand() % 5));
        else
            dy = (5 + (rand() % 5));

        this->particles.push_back(new Bit(this->Screen, this->timer, x, y, dx, dy));
    }
}

void ParticleEngine::AddPlayerTrail(CPlayer* player, const int dx, const int dy)
{
    if(dx == 0 && dy > 0)   // Going north
    {
        this->trail.push_back(new Bit(this->Screen, this->timer,
            (int)player->GetX() + rand() % player->GetCollisionBoundaries()->w,
            (int)player->GetY() - player->GetCollisionBoundaries()->h - rand() % 100,
            dx, dy, 20));
    }
    else if(dx > 0 && dy == 0)   // Player is going east
    {
        this->trail.push_back(new Bit(this->Screen, this->timer,
            (int)player->GetX() - rand() % 100,
            (int)player->GetY() + rand() % player->GetCollisionBoundaries()->h,
            dx, dy, 20));
    }
    else if(dx == 0 && dy < 0)  // Going south
    {
        this->trail.push_back(new Bit(this->Screen, this->timer,
            (int)player->GetX() + rand() % player->GetCollisionBoundaries()->w,
            (int)player->GetY() + rand() % 100,
            dx, dy, 20));
    }
    else if(dx < 0 && dy == 0)   // Player is going west
    {
        this->trail.push_back(new Bit(this->Screen, this->timer,
            (int)player->GetX() + rand() % 100,
            (int)player->GetY() + rand() % player->GetCollisionBoundaries()->h,
            dx, dy, 20));
    }
    else if(dx > 0 && dy < 0)   // Player is going north-east
    {
        /* Equation of the line is:
         * y - player.y = -1(x - player.x)
         * y = -x + (player.x + player.y)
         */
        int x = (int)player->GetX() - (rand() % 75);
        int y = -x + ((int)player->GetX() + (int)player->GetY());

        if(rand() % 2 == 1)
            y += rand() % 20;
        else
            y -= rand() % 20;

        this->trail.push_back(new Bit(this->Screen, this->timer,
            x, y, dx, dy, 20));
    }
    else if(dx > 0 && dy > 0)   // Player is going south-east
    {
        /* Equation of the line is:
         * y - player.y = -1(x - player.x)
         * y = -x + (player.x + player.y)
         * But voodoo magic is neccessary to do it right,
         * we get the distance from the ship, double it,
         * and get the right vector.
         */
        int x = (int)player->GetX() - (rand() % 75);
        int y = -x + ((int)player->GetX() + (int)player->GetY());
        y -= (y - (int)player->GetY()) * 2;

        if(rand() % 2 == 1)
            y += rand() % 20;
        else
            y -= rand() % 20;

        this->trail.push_back(new Bit(this->Screen, this->timer,
            x, y, dx, dy, 20));
    }
    else if(dx < 0 && dy > 0)  // Going south-west
    {
        int x = (int)player->GetX() + (rand() % 75);
        int y = -x + ((int)player->GetX() + (int)player->GetY());

        if(rand() % 2 == 1)
            y += rand() % 20;
        else
            y -= rand() % 20;

        this->trail.push_back(new Bit(this->Screen, this->timer,
            x, y, dx, dy, 20));
    }
    else if(dx < 0 && dy < 0)   // Going north-west
    {
        int x = ((int)player->GetX() + player->GetCollisionBoundaries()->w) + (rand() % 75);
        int y = -x + ((int)player->GetX() + (int)player->GetY()) - player->GetCollisionBoundaries()->h;
        y -= (y - (int)player->GetY()) * 2;

        if(rand() % 2 == 1)
            y += rand() % 20;
        else
            y -= rand() % 20;

        this->trail.push_back(new Bit(this->Screen, this->timer,
            x, y, dx, dy, 20));
    }
}

void ParticleEngine::UpdateParticles()
{
    std::vector<std::list<Bit*>::iterator> remover1;
    std::vector<std::list<Bit*>::iterator> remover2;

    for(std::list<Bit*>::iterator i = this->particles.begin();
        i != this->particles.end(); i++)
    {
        if((*i)->IsAlive())
            (*i)->Update();
        else
            remover1.push_back(i);
    }

    for(std::list<Bit*>::iterator i = this->trail.begin();
        i != this->trail.end(); i++)
    {
        if((*i)->IsAlive())
            (*i)->Update();
        else
            remover2.push_back(i);
    }

    for(unsigned int i = 0; i < remover1.size(); i++)
    {
        delete *remover1[i];
        this->particles.erase(remover1[i]);
    }
    for(unsigned int i = 0; i < remover2.size(); i++)
    {
        delete *remover2[i];
        this->trail.erase(remover2[i]);
    }

    remover1.clear();
    remover2.clear();
}