/* A basic class to handle the display
 * of debugger information on the screen.
 * More or less the "console".
 * Activated by the '~' key.
 */
#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

#include <sstream>

#include "SDL_Helper.h"
#include "Display.h"
#include "Settings.h"

#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"

class CDebugger
{
public:
    CDebugger(CDisplay& Display, CSettingsManager& Settings);
    ~CDebugger(){}

    void Update(const CPlayer& Player, CBullets& Bullets, CEnemies& Enemies);
    
    /* Change debug mode to on/off */
    void ToggleDebugMode(bool toggle){this->is_debug = toggle;}

    bool IsDebug(){return this->is_debug;}

private:
    CDisplay& Display;
    TTF_Font* debug_font;

    bool is_debug;
};

#endif // __DEBUGGER_H__