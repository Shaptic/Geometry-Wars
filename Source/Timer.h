#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

class Timer
{
public:
    Timer();
    ~Timer();

    void Start();
    float GetTicks();
    void SetFrameRate(const unsigned int newFrameRate){this->FRAME_RATE = newFrameRate;}
    void DelayFPS();

    unsigned int FRAME_RATE;

private:
    int ticks;
};

#endif // TIMER_H