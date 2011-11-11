#include "Level.h"

using namespace std;

CLevelManager::CLevelManager()
{
    string tmp;
    ifstream file("LevelData.db");

    /* The first line of the file says how many
     * levels there are formatted like so:
     * LEVELCOUNT:#OfLevels
     */
    getline(file, tmp, '\n');

    /* We have the like "LEVELCOUNT:#" so we get
     * the part after the colon (the # of levels)
     */
    tmp = split(tmp, ':')[1];
    int count = atoi(tmp.c_str());

    this->Levels = new Level;
    Level* tmp1  = NULL;

    for(short i = 1; i <= count; i++)
    {
        /* Fill out the level struct based on file data.
         * Level file is organized like so:
         * NewLine
         * LEVEL:Level#
         * ENEMY_TYPE:FileName
         * ENEMY_COUNT:#OfEnemies
         * SPAWN_DELAY:DelayBetweenEnemySpawns
         */

        getline(file, tmp, '\n');   // Skip a blank line
        getline(file, tmp, '\n');   // We don't need LEVEL:Level#

        Level* tmp2 = NULL;

        if(i == 1)
            tmp2 = this->Levels;
        else
            tmp2 = new Level;

        tmp2->level_id = i;

        getline(file, tmp, '\n');
        tmp2->enemy = split(tmp, ':')[1];

        getline(file, tmp, '\n');
        tmp2->enemy_count = atoi(split(tmp, ':')[1].c_str());

        getline(file, tmp, '\n');
        tmp2->spawn_delay = atoi(split(tmp, ':')[1].c_str());

        if(i != 1)
            tmp1->NextLevel = tmp2;

        tmp1 = tmp2;    
    }

    tmp1->NextLevel = NULL;

    this->CurrentLevel = this->Levels;
    this->enemies_remaining = this->CurrentLevel->enemy_count;
    this->spawned = 0;
}

CLevelManager::CLevelManager(const std::string& filename)
{
    string tmp;
    ifstream file(filename);

    /* The first line of the file says how many
     * levels there are formatted like so:
     * LEVELCOUNT:#OfLevels
     */
    getline(file, tmp, '\n');

    /* We have the like "LEVELCOUNT:#" so we get
     * the part after the colon (the # of levels)
     */
    tmp = split(tmp, ':')[1];
    int count = atoi(tmp.c_str());

    this->Levels = new Level;
    Level* tmp1  = NULL;

    for(short i = 1; i <= count; i++)
    {
        /* Fill out the level struct based on file data.
         * Level file is organized like so:
         * NewLine
         * LEVEL:Level#
         * ENEMY_TYPE:EnemyName
         * ENEMY_COUNT:#OfEnemies
         * SPAWN_DELAY:DelayBetweenEnemySpawns
         */

        getline(file, tmp, '\n');   // Skip a blank line
        getline(file, tmp, '\n');   // We don't need LEVEL:Level#

        Level* tmp2 = NULL;

        if(i == 1)
            tmp2 = this->Levels;
        else
            tmp2 = new Level;

        tmp2->level_id = i;

        getline(file, tmp, '\n');
        tmp2->enemy = split(tmp, ':')[1];

        getline(file, tmp, '\n');
        tmp2->enemy_count = atoi(split(tmp, ':')[1].c_str());

        getline(file, tmp, '\n');
        tmp2->spawn_delay = atoi(split(tmp, ':')[1].c_str());

        if(i != 1)
            tmp1->NextLevel = tmp2;

        tmp1 = tmp2;    
    }

    tmp1->NextLevel = NULL;

    this->CurrentLevel = this->Levels;
}

CLevelManager::~CLevelManager()
{
    for(Level* tmp = this->Levels; tmp != NULL; )
    {
        Level* tmp2 = tmp;
        tmp = tmp->NextLevel;
        delete tmp2;
    }
}

bool CLevelManager::CanSpawn(const int frame)
{
    if(this->spawned >= this->CurrentLevel->enemy_count)
    {
        return false;
    }
    else if(frame % (int)this->CurrentLevel->spawn_delay == 0)
    {
        this->spawned++;
        return true;
    }
    else
    {
        return false;
    }
}

void CLevelManager::UpdateCurrentLevel(const int enemy_count)
{
    if(enemy_count == 0)
        return;

    this->enemies_remaining -= enemy_count;

    if(this->enemies_remaining == 0)
        this->LoadNextLevel();
}

void CLevelManager::LoadNextLevel()
{
    this->CurrentLevel = this->CurrentLevel->NextLevel;
    if(this->CurrentLevel == NULL)
        this->CurrentLevel = this->Levels;

    this->spawned           = 0;
    this->enemies_remaining = this->CurrentLevel->enemy_count;
}

Level* CLevelManager::GetCurrentLevel() const
{
    return this->CurrentLevel;
}