#ifndef __ENGINE_H__
#define __ENGINE_H__

/* For storage */
#include <list>

/* SDL API and other libraries */
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

/* A collection of helper functions */
#include "Helpers.h"

/* Wrappers for SDL functions */
#include "SDL_Helper.h"
#include "Display.h"
#include "Events.h"
#include "Timer.h"

/* A simple menu class for easy menu creation */
#include "Menu.h"

/* All of the objects that will be in the game */
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

/* Updating and tracking the score, along with
 * other HUD items.
 */
#include "Scoreboard.h"

/* Creating explosions */
#include "Particles.h"

/* Level generator */
#include "LevelManager.h"

/* Console debugging interface */
#include "Debugger.h"

/* Maintaining cleanliness */
#include "GarbageDisposal.h"

/* Controlling the Settings.ini file */
#include "Settings.h"

class Engine
{
public:
    Engine();
    ~Engine();

    void Menu();

private:
    
    /* Various game modes, and starting a new game */
    void SinglePlayerGame();
    //void MultiPlayerGame();
    void Reset();

    /* Handling game events */
    void Events();

    /* Take care of all collisions */
    bool CheckCollisions();

    /* Render everything on the screen */
    void Update();

    /* The game classes */
    CDisplay            Display;
    CSettingsManager    Settings;
    CTimer              Timer;
    CEvents             EventHandler;
    
    CPlayer             Player;
    CEnemies            Enemies;
    CBullets            Bullets;

    CScoreBoard         Score;
    CParticleEngine     Particles;
    CLevelManager       Levels;
    CDebugger           Debugger;
    CGarbageDisposal    Garbage;
    
    CMenu               MainMenu;

    /* Game background */
    SDL_Surface*        Game_BG;

    /* In-Game soundtrack */
    Mix_Music*          Game_Music;

    /* Game fonts */
    TTF_Font*           Menu_Font;
    TTF_Font*           UI_Font;

    /* Game variables */
    bool                quit;

};


#endif // __ENGINE_H__