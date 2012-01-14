#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include <vector>

#include "Display.h"
#include "Timer.h"
#include "Entity.h"

class CParticle: public CEntity
{
public:
    CParticle(CDisplay& Display, CTimer& Timer,
        const SDL_Color& color, const int x, const int y);
    ~CParticle(){}

    bool Update();

private:
    int dx, dy;
    int lifetime;
};

class CParticleEngine
{
public:
    CParticleEngine(CDisplay& Display, CTimer& Timer);
    ~CParticleEngine();

    void GenerateExplosion(const CEntity* Entity);
    void Update();

private:
    CDisplay& Display;
    CTimer&   Timer;

    std::vector<CParticle*> Particles;
};

#endif // __PARTICLES_H__