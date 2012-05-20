#ifndef __MENU_H__
#define __MENU_H__

#include "SDL_Helper.h"
#include "Display.h"
#include "Events.h"

#include <vector>

struct MenuItem
{
    int x, y;
    int id;

    SDL_Surface* norm;
    SDL_Surface* high;
    SDL_Surface* active;
};

class CMenu
{
public:
    CMenu(CDisplay& Display);
    ~CMenu();

    int AddMenuItem(const int x, const int y, SDL_Surface* normal, SDL_Surface* highlighted);
    int AddMenuItem(const int x, const int y, const char* normal, const char* highlighted);
    int AddMenuItem(const int x, const int y, const std::string& normal, const std::string& highlighted);

    int CheckMouseOver();

    int Update();

private:
    CDisplay& Display;
    std::vector<MenuItem*> Buttons;
};

#endif // __MENU_H__