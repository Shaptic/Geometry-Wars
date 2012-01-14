#ifndef EVENTS_H
#define EVENTS_H

#include "SDL/SDL.h"

/* This class is deprecated, it is only kept here 
 * for backward-compatibility with the Menu class
 * found in Menus.h. For all other event needs, 
 * use the functions rather than creating an Event
 * class for it.
 */
class CEvents
{
public:
    CEvents();
    ~CEvents();

    void    HandleMenuEvents(bool& quit, int& mouseX,
                int& mouseY, bool& clicked);
    void    HandleQuit(bool* quit);

private:
    SDL_Event                   event;
};

/*******************************/
/** BEGIN NEW EVENT SYSTEM    **/
/*******************************/

const int QUIT_KEY = SDLK_ESCAPE;

/* Get the current state of the keyboard */
Uint8*  GetKeyState();

/* Check if a button on the keyboard is down */
bool    IsDown(const SDLKey key);
bool    IsDown_Event(const SDLKey key);

/* Check if a mouse button is down */
bool    IsPressed(const int mouse_button);

/* Get the current mouse position */
void    GetMousePosition(int& x, int& y);

/* Check if the key for quitting (declared above)
 * is pressed down, or check if another key used
 * to quit is pressed down, or check for an SDL_QUIT
 * event.
 */
bool    CheckQuit();
bool    CheckQuit(const SDLKey key);
bool    CheckQuit_Event();

#endif // EVENTS_H