#include "Debugger.h"

CDebugger::CDebugger(CDisplay& Display, CSettingsManager& Settings): Display(Display)
{
    /* Load the main font to be used throughout
     * the debugger information.
     * TODO: ADD ERROR CHECKING!
     */
    this->debug_font = TTF_OpenFont(Settings.GetValueAt("DebugFont").c_str(), atoi(Settings.GetValueAt("DebugFontSize").c_str()));
    
    if(!this->debug_font)
        handleError(TTF_GetError());

#ifdef _DEBUG
    this->ToggleDebugMode(true);
#else
    this->ToggleDebugMode(false);
#endif // _DEBUG
}

void CDebugger::Update(const CPlayer& Player, CBullets& Bullets, CEnemies& Enemies)
{
    static std::stringstream ss;
    static int mouse_x, mouse_y;
    static SDL_Surface* debug   = NULL;
    static SDL_Color white      = create_color(WHITE);
        
    if(this->IsDebug())
    {
        GetMousePosition(mouse_x, mouse_y);

        ss << "(" << (int)Player.GetX() << ", " << (int)Player.GetY() << ")";
        debug = render_text(this->debug_font, ss.str(), NULL, white, CREATE_SURFACE | TRANSPARENT_BG);
        this->Display.Blit(debug, (int)Player.GetX(), (int)Player.GetY() - get_text_height(this->debug_font, ss.str()));

        SDL_FreeSurface(debug);
        ss.str(std::string());

        ss << "(" << mouse_x << ", " << mouse_y << ")";
        debug = render_text(this->debug_font, ss.str(), NULL, white, CREATE_SURFACE | TRANSPARENT_BG);
        this->Display.Blit(debug, mouse_x, mouse_y - get_text_height(this->debug_font, ss.str()));

        SDL_FreeSurface(debug);
        ss.str(std::string());

        for(CBullets::iterator i = Bullets.begin(); i != Bullets.end(); i++)
        {
            ss << "(" << (int)(*i)->GetX() << ", " << (int)(*i)->GetY() << ")";
            debug = render_text(this->debug_font, ss.str(), NULL, white, CREATE_SURFACE | TRANSPARENT_BG);
            this->Display.Blit(debug, (int)(*i)->GetX(), (int)(*i)->GetY() - get_text_height(this->debug_font, ss.str()));

            SDL_FreeSurface(debug);
            ss.str(std::string());
        }

        ss.str(std::string());

        for(CEnemies::iterator i = Enemies.begin(); i != Enemies.end(); i++)
        {
            ss << "(" << (int)(*i)->GetX() << ", " << (int)(*i)->GetY() << ")";
            debug = render_text(this->debug_font, ss.str(), NULL, white, CREATE_SURFACE | TRANSPARENT_BG);
            this->Display.Blit(debug, (int)(*i)->GetX(), (int)(*i)->GetY() - get_text_height(this->debug_font, ss.str()));

            SDL_FreeSurface(debug);
            ss.str(std::string());
        }
    }
}

