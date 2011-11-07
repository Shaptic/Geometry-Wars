#ifndef POWER_UPS_H
#define POWER_UPS_H

static const int POWERUP_DURATION   = 600;    // 10 seconds-ish
static const int TOTAL_POWERUPS     = 5;

enum POWERUPS
{
    SHIELD,
    MORE_SHOTS,
    LOW_SHOT_DELAY,
    EXTRA_LIFE,
    EMP,
    NO_POWERUP
};

struct PowerUp
{
    POWERUPS ability;
    short duration;
};

#endif // POWER_UPS_H