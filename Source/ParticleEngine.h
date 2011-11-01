#ifndef PARTICLE_ENGINE_H
#define PARTICLE_ENGINE_H

#include <cstdlib>

#include <vector>

#include "Display.h"
#include "Entity.h"
#include "Timer.h"

class Bit: public BaseObject
{
public:
    Bit(Display* Screen, Timer* timer,
        const int x, const int y,
        const int m_dx, const int m_dy);
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
    void UpdateParticles();

private:
    Display*            Screen;
    Timer*              timer;
    std::vector<Bit*>   particles;
};

#endif // PARTICLE_ENGINE_H