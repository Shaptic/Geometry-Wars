#ifndef PARTICLE_ENGINE_H
#define PARTICLE_ENGINE_H

#include <cstdlib>

#include <list>

#include "Display.h"
#include "Entity.h"
#include "Player.h"
#include "Timer.h"

class Bit: public BaseObject
{
public:
    Bit(Display* Screen, Timer* timer,
        const int x, const int y,
        const int m_dx, const int m_dy);

    Bit(Display* Screen, Timer* timer,
        const int x, const int y,
        const int m_dx, const int m_dy,
        const int m_lifetime);
    
    ~Bit();

    void Update();
    bool IsAlive();

private:
    int lifetime;
    int dx, dy;
};

class ParticleEngine
{
public:
    ParticleEngine(Display* Screen, Timer* timer);
    ~ParticleEngine();

    void ExplodeObject(const int x, const int y);
    void AddPlayerTrail(CPlayer* player,
        const int dx, const int dy);
    void UpdateParticles();

private:
    Display*        Screen;
    Timer*          timer;
    std::list<Bit*> particles;
    std::list<Bit*> trail;
};

#endif // PARTICLE_ENGINE_H