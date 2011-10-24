#ifndef ENGINE_H
#define ENGINE_H

#include <sstream>
#include <list>

#include <cstdlib>
#include <ctime>

#include "SDL_Helper.h"
#include "Display.h"
#include "Events.h"
#include "Menus.h"
#include "Entity.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Timer.h"

static const int PLAYER_SPEED   = 5;
static const double SHOT_ANGLE  = 10.0;

typedef std::list<Enemy*>                   AllEnemies;
typedef std::list<Bullet*>                  AllBullets;
typedef std::vector<AllEnemies::iterator>   EnemyIterator;
typedef std::vector<AllBullets::iterator>   ShotIterator;

class Engine
{
public:
    Engine();
    ~Engine();

    void Play();

private:
    void Events();

    void AddEnemy();
    void Shoot();
    
    void CheckCollisions(std::vector<AllEnemies::iterator>& enemy_iters,
            std::vector<AllBullets::iterator>& shot_iters);
    void RemoveEnemies(std::vector<AllEnemies::iterator>& iters);
    void RemoveShots(std::vector<AllBullets::iterator>& iters);

    void UpdateAll();

    Display*        Screen;
    Menu*           MainMenu;
    CPlayer*        Player;
    AllEnemies      Enemies;
    AllBullets      Shots;
    Timer*          Fps;

    EnemyIterator   enemy_iters;
    ShotIterator    shot_iters;

    bool            quit;
    bool            debug;
};

#endif // ENGINE_H