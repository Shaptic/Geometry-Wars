#ifndef POWER_UPS_H
#define POWER_UPS_H

static const int POWERUP_DURATION   = 600;    // 10 seconds-ish

struct PowerUp
{
    enum POWERUPS
    {
        SHIELD = -1,
        MORE_SHOTS,
        LOW_SHOT_DELAY,
        EXTRA_LIFE,
        EMP,
        NO_POWERUP,
        TOTAL_POWERUPS
    } ability;

    short duration;
};

#endif // POWER_UPS_H