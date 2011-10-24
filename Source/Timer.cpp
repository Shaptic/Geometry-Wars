#include "Timer.h"


Timer::Timer()
{
    this->FRAME_RATE = 45;
    this->ticks = 0;
}

Timer::~Timer()
{
}

void Timer::Start()
{
    this->ticks = SDL_GetTicks();
}

float Timer::GetTicks()
{
    Uint32 oldTicks = this->ticks;
    this->ticks = SDL_GetTicks();
    return 1.0f * (this->ticks - oldTicks);
}

void Timer::DelayFPS()
{
    if(((unsigned)this->GetTicks() < 1000 / this->FRAME_RATE))
    {
        SDL_Delay((1000 / this->FRAME_RATE) - (int)this->GetTicks());
    }
}
