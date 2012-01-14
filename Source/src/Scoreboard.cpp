#include "Scoreboard.h"

CScoreBoard::CScoreBoard(CDisplay& Display): Display(Display)
{
    this->Items.clear();
    this->Overlay   = create_surface_alpha(Display.GetWidth(), Display.GetHeight());
    this->MainFont  = NULL;
}

CScoreBoard::~CScoreBoard()
{
    for(size_t i = 0; i < this->Items.size(); i++)
    {
        SDL_FreeSurface(this->Items[i]->to_show);

        delete[] this->Items[i]->main_line;
        
        delete this->Items[i];
    }

    this->Items.clear();
}

void CScoreBoard::SetOverlay(SDL_Surface* Overlay)
{
    this->Overlay = Overlay;
}

void CScoreBoard::SetFont(TTF_Font* Font)
{
    this->MainFont = Font;
}

int CScoreBoard::AddItem(const int x, const int y, const char* primary, int var)
{
    static int id = rand() % 1234;

    ScoreItem* Item = new ScoreItem;

    Item->x         = x;
    Item->y         = y;
    Item->id        = id;
    Item->var       = var;

    /* C-string voodoo .__. */
    Item->main_line = new char[strlen(primary) + 1];
    strcpy(Item->main_line, primary);

    ss << primary << var;
    Item->to_show = render_text(this->MainFont, ss.str(), NULL, this->TextColor, CREATE_SURFACE|TRANSPARENT_BG);
    ss.str(std::string());

    this->Items.push_back(Item);

    id++;
    return Item->id;
}

void CScoreBoard::UpdateItem(const char* primary, const int new_value)
{
    ScoreItem* tmp = NULL;

    if((tmp = this->FindItem(primary)) == NULL)
        return;

    tmp->var = new_value;
    
    ss << primary << new_value;

    SDL_FreeSurface(tmp->to_show);

    tmp->to_show = render_text(this->MainFont, ss.str(),
        NULL, this->TextColor, CREATE_SURFACE | TRANSPARENT_BG);

    ss.str(std::string());
}

void CScoreBoard::UpdateItem(const int id, const int new_value)
{
    ScoreItem* tmp = NULL;

    if((tmp = this->FindItem(id)) == NULL)
        return;

    tmp->var = new_value;
    
    ss << tmp->main_line << new_value;

    SDL_FreeSurface(tmp->to_show);

    tmp->to_show = render_text(this->MainFont, ss.str(),
        NULL, this->TextColor, CREATE_SURFACE | TRANSPARENT_BG);

    ss.str(std::string());
}

void CScoreBoard::UpdateItem_Increment(const char* primary, const int incr)
{
    ScoreItem* tmp = NULL;

    if((tmp = this->FindItem(primary)) == NULL)
        return;

    tmp->var += incr;

    ss << primary << tmp->var;

    SDL_FreeSurface(tmp->to_show);

    tmp->to_show = render_text(this->MainFont, ss.str(),
        NULL, this->TextColor, CREATE_SURFACE | TRANSPARENT_BG);

    ss.str(std::string());
}

void CScoreBoard::UpdateItem_Increment(const int id, const int incr)
{
    ScoreItem* tmp = NULL;

    if((tmp = this->FindItem(id)) == NULL)
        return;

    tmp->var += incr;

    ss << tmp->main_line << tmp->var;

    SDL_FreeSurface(tmp->to_show);

    tmp->to_show = render_text(this->MainFont, ss.str(),
        NULL, this->TextColor, CREATE_SURFACE | TRANSPARENT_BG);

    ss.str(std::string());
}

void CScoreBoard::Update()
{
    static SDL_Surface* final = NULL;

    final = create_surface_alpha(this->Display.GetWidth(), this->Display.GetHeight());
    
    for(size_t i = 0; i < this->Items.size(); i++)
    {
        SDL_SetAlpha(this->Items[i]->to_show, 0, SDL_ALPHA_TRANSPARENT);
        this->Display.Blit(final, this->Items[i]->to_show, this->Items[i]->x, this->Items[i]->y);
    }

    this->Display.Blit(final, 0, 0);

    SDL_FreeSurface(final);
}

ScoreItem* CScoreBoard::FindItem(const char* primary)
{
    for(size_t i = 0; i < this->Items.size(); i++)
    {
        int max_size = min(strlen(this->Items[i]->main_line), strlen(primary));

        if(strncmp(primary, this->Items[i]->main_line, max_size) == 0)
        {
            return this->Items[i];
        }
    }

    return NULL;
}

ScoreItem* CScoreBoard::FindItem(const int id)
{
    for(size_t i = 0; i < this->Items.size(); i++)
    {
        if(this->Items[i]->id == id)
        {
            return this->Items[i];
        }
    }

    return NULL;
}

