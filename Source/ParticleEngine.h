#ifndef PARTICLE_ENGINE_H
#define PARTICLE_ENGINE_H

#include <cstdlib>

#include <list>

#include "Display.h"
#include "Entity.h"
#include "Player.h"
#include "Timer.h"

static const int EMP_TIME = 5;

class Bit: public BaseObject
{
public:
    Bit(Display* Screen, Timer* timer,
        const int x, const int y,
        const int m_dx, const int m_dy,
        const int m_lifetime);
    
    Bit(Display* Screen, Timer* timer,
        const int x, const int y,
        const int m_dx, const int m_dy,
        const int m_lifetime,
        const SDL_Color& color);

    Bit(Display* Screen, Timer* timer,
        const int x, const int y,
        const int m_dx, const int m_dy,
        const SDL_Color& color);

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

    //void GenerateEMP(const int x, const int y);
    void ExplodeObject(BaseObject* obj);
    void ExplodePlayer(const int x, const int y);
    void AddPlayerTrail(CPlayer* player,
        const int dx, const int dy);
    void UpdateParticles();

private:
    Display*        Screen;
    Timer*          timer;
    std::list<Bit*> particles;
    std::list<Bit*> trail;

    /* Unused
     * SDL_Surface*    emp;
    
     * bool            show_emp;
     * unsigned int    show_emp_time;
     */
};

#endif // PARTICLE_ENGINE_H