#include "Timer.h"


CTimer::CTimer()
{
    this->FRAME_RATE    = 45;
    this->ticks         = 0;
}

CTimer::~CTimer()
{
}

void CTimer::Start()
{
    this->ticks = SDL_GetTicks();
}

float CTimer::GetTicks()
{
    Uint32 oldTicks = this->ticks;
    this->ticks = SDL_GetTicks();
    return 1.0f * (this->ticks - oldTicks);
}

void CTimer::DelayFPS()
{
    if(((unsigned)this->GetTicks() < 1000 / this->FRAME_RATE))
    {
        SDL_Delay((1000 / this->FRAME_RATE) - (int)this->GetTicks());
    }
}
