#ifndef __LEVEL_MANAGER_H__
#define __LEVEL_MANAGER_H__

#include <vector>
#include <string>

#include "Settings.h"

struct Level
{
    char*   sprite_filename;
    int     to_spawn;
    int     spawn_delay;
    int     enemy_health;
};

class CLevelManager
{
public:
    CLevelManager(CSettingsManager& Settings);
    //CLevelManager(CSettingsManager& Settings, const std::string& filename);
    ~CLevelManager();

    bool    CanSpawn();
    char*   GetSpriteName();
    int     GetEnemyHealth();
    void    IncreaseKillCount(int kills);
    void    Reset();

private:
    void    LoadNextLevel();

    CSettingsManager& Settings;
    Level* CurrentLevel;
    int spawned, killed;
};

#endif // __LEVEL_MANAGER__H