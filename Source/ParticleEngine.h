#ifndef PARTICLE_ENGINE_H
#define PARTICLE_ENGINE_H

#include <cstdlib>

#include <list>

#include "Display.h"
#include "Entity.h"
#include "Player.h"
#include "Timer.h"

static const int EMP_TIME = 5;

class CBit: public CBaseObject
{
public:
    CBit(CDisplay& Screen, CTimer& timer,
        const int x, const int y,
        const int m_dx, const int m_dy,
        const int m_lifetime);
    
    CBit(CDisplay& Screen, CTimer& timer,
        const int x, const int y,
        const int m_dx, const int m_dy,
        const int m_lifetime,
        const SDL_Color& color);

    CBit(CDisplay& Screen, CTimer& timer,
        const int x, const int y,
        const int m_dx, const int m_dy,
        const SDL_Color& color);

    ~CBit();

    void Update();
    bool IsAlive();

private:
    int lifetime;
    int dx, dy;
};

class CParticleEngine
{
public:
    CParticleEngine(CDisplay& Screen, CTimer& timer);
    ~CParticleEngine();

    //void GenerateEMP(const int x, const int y);
    void ExplodeObject(CBaseObject* obj);
    void ExplodePlayer(const int x, const int y);
    void AddPlayerTrail(CPlayer& player,
        const int dx, const int dy);
    void UpdateParticles();

private:
    CDisplay&        Screen;
    CTimer&          Timer;
    std::list<CBit*> particles;
    std::list<CBit*> trail;

    /* Unused
     * SDL_Surface*    emp;
    
     * bool            show_emp;
     * unsigned int    show_emp_time;
     */
};

#endif // PARTICLE_ENGINE_H