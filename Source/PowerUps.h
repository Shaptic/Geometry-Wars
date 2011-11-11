#ifndef POWER_UPS_H
#define POWER_UPS_H

namespace PowerUps
{
    static const int POWERUP_DURATION   = 600;    // 10 seconds-ish

    enum POWERUPS
    {
        SHIELD = -1,
        MORE_SHOTS,
        LOW_SHOT_DELAY,
        EXTRA_LIFE,
        EMP,
        NO_POWERUP,
        TOTAL_POWERUPS
    };

    struct PowerUp
    {
        POWERUPS ability;
        short duration;
    };
}

#endif // POWER_UPS_H