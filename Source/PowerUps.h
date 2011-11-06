#ifndef POWER_UPS_H
#define POWER_UPS_H

typedef int POWER_UP;

static const int POWERUP_DURATION   = 600;    // 10 seconds-ish

static const POWER_UP SHIELD        = 0x02;
static const POWER_UP MORE_SHOTS    = 0x04;
static const POWER_UP LOW_SHOT_DELAY= 0x08;
static const POWER_UP EXTRA_LIFE    = 0x16;
static const POWER_UP EMP           = 0x32;
static const POWER_UP NO_POWERUP    = 0x64;
static const int TOTAL_POWERUPS     = 5;

#endif // POWER_UPS_H