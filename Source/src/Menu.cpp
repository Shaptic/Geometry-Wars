#include "Menu.h"

CMenu::CMenu(CDisplay& Display): Display(Display)
{
    this->Buttons.clear();
}

CMenu::~CMenu()
{
    for(size_t i = 0; i < this->Buttons.size(); i++)
    {
        delete this->Buttons[i];
    }

    this->Buttons.clear();
}

int CMenu::AddMenuItem(const int x, const int y, SDL_Surface* normal, SDL_Surface* highlighted)
{
    MenuItem* Item = new MenuItem;
    Item->norm = normal;
    Item->high = highlighted;
    Item->active = Item->norm;
    Item->x    = x;
    Item->y    = y;
    Item->id   = this->Buttons.size();

    this->Buttons.push_back(Item);

    return Item->id;
}

int CMenu::AddMenuItem(const int x, const int y, const char* normal, const char* highlighted)
{
    MenuItem* Item = new MenuItem;

    Item->norm = LoadImage_Alpha(normal);
    Item->high = LoadImage_Alpha(highlighted);
    Item->active = Item->norm;
    Item->id   = this->Buttons.size();
    Item->x    = x;
    Item->y    = y;

    this->Buttons.push_back(Item);

    return Item->id;
}

int CMenu::AddMenuItem(const int x, const int y, const std::string& normal, const std::string& highlighted)
{
    return this->AddMenuItem(x, y, normal.c_str(), highlighted.c_str());
}

int CMenu::CheckMouseOver()
{
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    SDL_Rect mouse = {mouse_x, mouse_y, 0, 0};

    for(size_t i = 0; i < this->Buttons.size(); i++)
    {
        SDL_Rect button = this->Buttons[i]->norm->clip_rect;
        button.x = this->Buttons[i]->x;
        button.y = this->Buttons[i]->y;

        if(detect_collision(mouse, button))
        {
            this->Buttons[i]->active = this->Buttons[i]->high;
            if(IsPressed(SDL_BUTTON_LEFT))
                return this->Buttons[i]->id;
        }
        else
        {
            this->Buttons[i]->active = this->Buttons[i]->norm;
        }
    }

    return -1;
}

int CMenu::Update()
{
    for(size_t i = 0; i < this->Buttons.size(); i++)
    {
        this->Display.Blit(this->Buttons[i]->active, this->Buttons[i]->x, this->Buttons[i]->y);
    }

    return this->CheckMouseOver();
}


