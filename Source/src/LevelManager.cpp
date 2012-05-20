#include "LevelManager.h"

CLevelManager::CLevelManager(CSettingsManager& Settings): Settings(Settings)
{
    this->CurrentLevel = new Level;

    /* I hate strings -___- */
    std::string tmp(Settings.ChooseValueAt("EnemySprites"));
    this->CurrentLevel->sprite_filename = new char[tmp.length() + 1];
    strcpy(this->CurrentLevel->sprite_filename, tmp.c_str());
    tmp.clear();

    this->CurrentLevel->spawn_delay = 30;
    this->CurrentLevel->to_spawn = 2;
    this->CurrentLevel->enemy_health = 100;
    this->spawned = 0;
    this->killed  = 0;
}

CLevelManager::~CLevelManager()
{
    delete this->CurrentLevel;
}

bool CLevelManager::CanSpawn()
{
    static int ticks = 0;
    ticks++;

    if(ticks % this->CurrentLevel->spawn_delay == 0 &&
        this->spawned <= this->CurrentLevel->to_spawn && 
        this->killed < this->CurrentLevel->to_spawn)
    {
        this->spawned++;
        return true;
    }
    else
        return false;
}

char* CLevelManager::GetSpriteName()
{
    return this->CurrentLevel->sprite_filename;
}

int CLevelManager::GetEnemyHealth()
{
    return this->CurrentLevel->enemy_health;
}

void CLevelManager::IncreaseKillCount(int kills)
{
    this->killed += kills;
    if(this->killed >= this->CurrentLevel->to_spawn)
        this->LoadNextLevel();
}

void CLevelManager::LoadNextLevel()
{
    static int level = 1;

    level++;

    /* Still hate them.. */
    std::string tmp = this->Settings.ChooseValueAt("EnemySprites");
    delete[] this->CurrentLevel->sprite_filename;
    this->CurrentLevel->sprite_filename = new char[tmp.length() + 1];
    strcpy(this->CurrentLevel->sprite_filename, tmp.c_str());
    tmp.clear();

    if(level % 10 == 0)
    {
        //this->CurrentLevel->enemy_health *= 1.5f;
    }
    else if(level % 5 == 0)
    {
        this->CurrentLevel->to_spawn *= 1.5f;
    }

    this->CurrentLevel->spawn_delay *= 0.99f;
    if(this->CurrentLevel->spawn_delay == 0)
        this->CurrentLevel->spawn_delay = 1;

    this->spawned   = 0;
    this->killed    = 0;
}

void CLevelManager::Reset()
{
    this->CurrentLevel->spawn_delay = 30;
    this->CurrentLevel->to_spawn = 2;
    this->spawned = 0;
    this->killed  = 0;
}
