#include "Particles.h"

CParticle::CParticle(CDisplay& Display, CTimer& Timer,
    const SDL_Color& color,
    int x, int y):
CEntity(Display, Timer)
{
    this->Move_Force(x, y);

    /* Choose either neg or pos direction */
    int sign = rand() % 2;
    if(sign == 1)
    {
        this->dx = 5 + rand() % 15;
    }
    else
    {
        this->dx = -5 - (rand() % 15);
    }

    sign = rand() % 2;
    if(sign == 1)
    {
        this->dy = -5 - rand() % 15;
    }
    else
    {
        this->dy = 5 + rand() % 15;
    }

    this->lifetime = 100 + rand() % 100;

    this->SetEntity(create_surface(5, 5, color));
}

bool CParticle::Update()
{
    lifetime--;

    if(lifetime > 0 && !this->IsOffscreen())
    {
        this->Move_Rate(this->dx, this->dy);
        this->Blit();
        return true;
    }
    else
        return false;
}

/* ============== PARTICLE ENGINE ============== */

CParticleEngine::~CParticleEngine()
{
    for(std::list<CParticle*>::iterator i = this->Particles.begin();
        i != this->Particles.end();)
    {
        delete (*i);
    }

    this->Particles.clear();
}

void CParticleEngine::GenerateExplosion(const CEntity* Entity)
{
    /* Set the amount of particles to create */
    int limit = 20 + rand() % 20;

    /* Obtain pixel color at the center of the sprite for explosions */
    Uint8 r, g, b;
    SDL_GetRGB(get_pixel(Entity->GetEntity(),
        Entity->GetCollisionBoundaries()->w / 2,
        Entity->GetCollisionBoundaries()->h / 2),
        Entity->GetEntity()->format, &r, &g, &b);

    SDL_Color color = create_color(r, g, b);

    for(short i = 0; i < limit; i++)
    {
        this->Particles.push_back(new CParticle(this->Display, this->Timer,
            color, (int)Entity->GetX(), (int)Entity->GetY()));
    }
}

void CParticleEngine::Update()
{
    for(std::list<CParticle*>::iterator i = this->Particles.begin();
        i != this->Particles.end();)
    {
        if((*i)->Update())
            i++;
        else
        {
            delete (*i);
            i = this->Particles.erase(i);
        }
    }
}