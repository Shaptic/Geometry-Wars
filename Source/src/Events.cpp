#include "Events.h"

using namespace std;

CEvents::CEvents()
{
}

CEvents::~CEvents()
{
}

void CEvents::HandleMenuEvents(bool& quit, int& mouseX,
    int& mouseY, bool& clicked)
{
    while(SDL_PollEvent(&this->event))
    {
        switch(this->event.type)
        {
        case SDL_QUIT:
            quit = true;
            return;

        case SDL_MOUSEBUTTONDOWN:
            if(this->event.button.button == SDL_BUTTON_LEFT)
            {
                clicked = true;
                mouseX = this->event.button.x;
                mouseY = this->event.button.y;
                SDL_GetMouseState(&mouseX, &mouseY);
                return;
            }
            break;

        default:
            SDL_GetMouseState(&mouseX, &mouseY);
            break;
        }
    }
    clicked = false;
}

void CEvents::HandleQuit(bool* quit)
{
    while(SDL_PollEvent(&this->event))
    {
        if(event.type == SDL_QUIT)
            *quit = true;
    }
}

Uint8* GetKeyState()
{
    return SDL_GetKeyState(NULL);
}

bool IsDown(const SDLKey KEY)
{
    return GetKeyState()[KEY];
}

bool IsPressed(const int button)
{
    return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button);
}

void GetMousePosition(int& x, int& y)
{
    SDL_GetMouseState(&x, &y);
}

bool CheckQuit()
{
    return GetKeyState()[QUIT_KEY];
}

bool CheckQuit(SDLKey key)
{
    return IsDown(key);
}

bool CheckQuit_Event()
{
    SDL_Event evt;
    while(SDL_PollEvent(&evt))
    {
        if(evt.type == SDL_QUIT)
            return true;
    }

    return false;
}

bool IsDown_Event(const SDLKey key)
{
    SDL_Event evt;
    while(SDL_PollEvent(&evt))
    {
        if(evt.type == SDL_KEYDOWN)
            if(evt.key.keysym.sym == key)
                return true;
    }

    return false;
}