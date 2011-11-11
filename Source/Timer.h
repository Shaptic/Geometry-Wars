#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

class CTimer
{
public:
    CTimer();
    ~CTimer();

    void Start();
    float GetTicks();
    void SetFrameRate(const unsigned int newFrameRate){this->FRAME_RATE = newFrameRate;}
    void DelayFPS();

private:
    unsigned int FRAME_RATE;
    int ticks;
};

#endif // TIMER_H