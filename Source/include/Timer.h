#ifndef TIMER_H
#define TIMER_H

#include "SDL/SDL.h"

class CTimer
{
public:
    CTimer();
    ~CTimer();

    void Start();
    float GetTicks();
    int GetFrame();
    void SetFrameRate(const unsigned int newFrameRate){ this->FRAME_RATE = newFrameRate; }
    int  GetFrameRate(){ return this->FRAME_RATE; }
    void DelayFPS();

private:
    unsigned int FRAME_RATE;
    int ticks;
    double frame;
};

#endif // TIMER_H