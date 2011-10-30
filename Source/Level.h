#ifndef LEVEL_H
#define LEVEL_H

#include <fstream>
#include <string>

#include "Helpers.h"

#include "Enemy.h"
#include "Timer.h"

static int LEVEL_COUNT = 1;

struct Level
{
    unsigned int level_id;
    std::string enemy;
    unsigned int enemy_count;
    unsigned int spawn_delay;

    Level* NextLevel;
};

class LevelManager
{
public:
    LevelManager();
    LevelManager(const std::string& filename);
    ~LevelManager();

    bool CanSpawn(const int frame);
    void UpdateCurrentLevel(const int enemy_count);
    Level* GetCurrentLevel() const;

private:
    void LoadNextLevel();

    unsigned int enemies_remaining;

    Level* Levels;
    Level* CurrentLevel;
};

#endif // LEVEL_H