#ifndef ENGINE_H
#define ENGINE_H

#include <cstdlib>
#include <ctime>

#include <sstream>
#include <list>

#include "SDL_Helper.h"
#include "Display.h"
#include "Events.h"
#include "Menus.h"
#include "Entity.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Level.h"
#include "Timer.h"
#include "ParticleEngine.h"

static const int PLAYER_SPEED   = 5;

typedef std::list<CEnemy*>                  AllEnemies;
typedef std::list<CBullet*>                 AllBullets;
typedef std::vector<CEnemy*>                EnemyHolder;
typedef std::vector<CBullet*>               BulletHolder;

class Engine
{
public:
    Engine();
    ~Engine();

    void Menu();
    void Play();
    void NewGame();

private:
    void LoadHighScore();
    void Events();

    void AddEnemy();

    void CheckCollisions();
    void DestroyEnemy(CEnemy* Enemy);
    void RemoveEnemies();
    void RemoveShots();

    void ShowDebugInfo();
    void UpdateAll();

    CDisplay            Screen;
    CTimer              Fps;
    CEvents             EventHandler;
    CMenu               MainMenu;
    CPlayer             Player;
    CLevelManager       Levels;
    CParticleEngine     Particles;
    AllEnemies          Enemies;
    AllBullets          Shots;

    EnemyHolder         remove_enemies;
    BulletHolder        remove_shots;

    TTF_Font*           main_font;

    bool                quit;       // Quit the game
    bool                play_again; // Is playing again a possibility?
                                    // Setting to false means no "Play Again?"
                                    // prompt after quit = true
    bool                debug;

    int                 score;
    int                 high_score;
};

#endif // ENGINE_H