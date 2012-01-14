#ifndef __CUSTOM_MENU_H__
#define __CUSTOM_MENU_H__

#include "Entity.h"
#include "Timer.h"
#include "Menus.h"
#include "Particles.h"

class CCustomMenu: public CMenu
{
public:
    CCustomMenu(CParticleEngine& Particles, CTimer& Timer, CDisplay& Display, CEvents& EventHandler):
      CMenu(Display, EventHandler), Particles(Particles), Timer(Timer){}

    int Run();

private:
    CParticleEngine& Particles;
    CTimer& Timer;
};

#endif // __CUSTOM_MENU_H__