#include "Debugger.h"

CDebugger::CDebugger(CDisplay& Display, CSettingsManager& Settings): Display(Display)
{
    /* Load the main font to be used throughout
     * the debugger information.
     */
    std::string font_name(Settings.GetValueAt("DebugFont"));
    if(font_name == "")
        /* Erroneous font name, hopefully we can
         * load a safe alternative.
         */
        font_name == "C:\\Windows\\Fonts\\Arial.ttf";
    
    std::string size(Settings.GetValueAt("DebugFontSize"));
    if(size == "")
        /* No font size given! Most likely the font name
         * check failed as well, so a 12pt Arial font
         * would be just fine for debug info.
         */
        size = "12";
    
    
    
    this->debug_font = TTF_OpenFont(font_name.c_str(), atoi(size.c_str()));

    if(!this->debug_font)
        handleError(TTF_GetError());

    /* For debug builds of the program, 
     * debugging is automatically on by default.
     */
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

        /* First, place (x, y) coordinates next to the player sprite. */
        ss << "(" << (int)Player.GetX() << ", " << (int)Player.GetY() << ")";
        debug = render_text(this->debug_font, ss.str(), NULL, white, CREATE_SURFACE | TRANSPARENT_BG);
        this->Display.Blit(debug, (int)Player.GetX(), (int)Player.GetY() - get_text_height(this->debug_font, ss.str()));

        SDL_FreeSurface(debug); // Free memory
        ss.str(std::string());

        /* Then, place (x, y) coordinates next to the mouse. */
        ss << "(" << mouse_x << ", " << mouse_y << ")";
        debug = render_text(this->debug_font, ss.str(), NULL, white, CREATE_SURFACE | TRANSPARENT_BG);
        this->Display.Blit(debug, mouse_x, mouse_y - get_text_height(this->debug_font, ss.str()));

        SDL_FreeSurface(debug); // Free memory
        ss.str(std::string());

        /* Every bullet also gets (x, y) info next to it. */
        for(CBullets::iterator i = Bullets.begin(); i != Bullets.end(); i++)
        {
            ss << "(" << (int)(*i)->GetX() << ", " << (int)(*i)->GetY() << ")";
            debug = render_text(this->debug_font, ss.str(), NULL, white, CREATE_SURFACE | TRANSPARENT_BG);
            this->Display.Blit(debug, (int)(*i)->GetX(), (int)(*i)->GetY() - get_text_height(this->debug_font, ss.str()));

            SDL_FreeSurface(debug); // Free memory
            ss.str(std::string());
        }

        ss.str(std::string());

        /* And finally, every enemy gets (x, y) as well. */
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