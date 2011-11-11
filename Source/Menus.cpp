#include "Menus.h"
#include "Menus.h"

using std::string;

CMenu::CMenu(CDisplay& m_display, CEvents& m_eventHandler): display(m_display), eventHandler(m_eventHandler)
{
    this->Initialize();
}

CMenu::~CMenu()
{
    for(unsigned int i=0; i < this->buttons.size(); i++)
    {
        delete this->buttons[i];
        this->buttons[i] = NULL;
    }

    this->buttons.clear();
}

bool CMenu::Initialize()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == -1)
        return false;
    if(TTF_Init() == -1)
        return false;
    if(Mix_Init(MIX_INIT_MP3) == 0)
        return false;
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        return false;

    /* Set some default values for menus */
#if (defined _WIN32 || defined WIN32)
    this->font      = TTF_OpenFont("C:\\Windows\\Fonts\\Arial.ttf", 32);
#else
    this->font      = NULL;
#endif // _WIN32 || WIN32

    this->bg        = create_surface(this->display.GetWidth(), this->display.GetWidth(), create_color(BLACK));
    this->music     = NULL;
    this->txtColor  = create_color(WHITE);
    this->txtColor  = create_color(GREEN);
    this->offset    = get_text_height(this->font, "TEST");
    this->quit      = false;
    this->SetCenterText(true);
    this->SetStartCoordinates(this->display.GetWidth() / 2, this->display.GetWidth() / 2);

    return true;
}

void CMenu::SetBackground(SDL_Surface* bg)
{
    this->bg = bg;
}

void CMenu::SetBackground(const char* filename)
{
    this->bg = LoadImage(filename);
}

void CMenu::SetBackground(const string& filename)
{
    this->SetBackground(filename.c_str());
}

void CMenu::SetFont(TTF_Font* font)
{
    this->font = font;
}

void CMenu::SetFont(const char* font_name)
{
    this->font = TTF_OpenFont(font_name, 32);
}

void CMenu::SetFont(const string& font_name)
{
    this->SetFont(font_name.c_str());
}

void CMenu::SetTextColor(const int r, const int g, const int b)
{
    this->txtColor = create_color(r, g, b);
}

void CMenu::SetTextColor(const SDL_Color& color)
{
    this->txtColor = color;
}

void CMenu::SetHighLightColor(const int r, const int g, const int b)
{
    this->hlColor = create_color(r, g, b);
}

void CMenu::SetHighLightColor(const SDL_Color& color)
{
    this->hlColor = color;
}

void CMenu::SetMusic(const char* filename)
{
    this->music = Mix_LoadMUS(filename);
}

void CMenu::SetMusic(const string& filename)
{
    this->SetMusic(filename.c_str());
}

void CMenu::SetMusic(Mix_Music* music)
{
    this->music = music;
}

void CMenu::SetStartCoordinates(const int x, const int y)
{
    this->x = x;
    this->y = y;
}

void CMenu::SetStartCoordinatesInScreenCenter()
{
    this->x = this->display.GetWidth() / 2;
    this->y = this->display.GetHeight() / 2;
}

void CMenu::SetCenterText(bool toggle)
{
    this->center = toggle;
}

 int CMenu::AddMenuOption(
            const char* text, 
            BUTTON_TYPE btn,
            ACTION_TYPE act,
            CMenu*      SubMenu,
            bool        onByDefault)
{
    switch(btn)
    {
        case BTN_GENERIC:
            return this->AddGeneric(text);

        case BTN_SUBMENU:
            if(SubMenu == NULL)
                return -1;
            else
                return this->AddSubMenu(text, SubMenu);

        case BTN_ACTION:
            if(act == ACT_NONE)
                return -1;
            else
                return this->AddAction(text, act);

        case BTN_TOGGLE:
            return this->AddToggle(text, onByDefault);

        case BTN_TEXT:
            return this->AddText(text);

        default:
            return -1;
    }
}

 void CMenu::RemoveMenuOption(const int id)
 {
     this->buttons[id] = NULL;
 }

 int CMenu::FindMenuOptionByText(const char* text)
 {
     for(unsigned int i=0; i < this->buttons.size(); i++)
     {
         if(strncmp(this->buttons[i]->text, text, 
             min(strlen(text), strlen(this->buttons[i]->text))) == 0)
         {
             return this->buttons[i]->id;
         }
     }

     return -1;
 }

void CMenu::CheckMouseOver(int mouse_x, int mouse_y)
{
    for(unsigned int i=0; i < this->buttons.size(); i++)
    {
        if(detect_collision(this->buttons[i]->mouseOver, create_rect(mouse_x, mouse_y, 0, 0)) &&
            (this->buttons[i]->highlight != NULL))
        {
            this->buttons[i]->main = this->buttons[i]->highlight;
        }
        else
        {
            this->buttons[i]->main = this->buttons[i]->normal;
        }
    }
}

int CMenu::CheckClick(const int x, const int y)
{
    for(unsigned int i=0; i < this->buttons.size(); i++)
    {
        if(detect_collision(this->buttons[i]->mouseOver, create_rect(x, y, 0, 0)))
            return i;
    }
    return -1;
}

int CMenu::AddAction(const char* text, ACTION_TYPE act)
{
    this->buttons.push_back(new Button);

    if(this->center)
    {
        this->buttons[LAST]->normal = render_text(
            this->font, (string)text, NULL, this->txtColor,
            ALIGN_CENTER | TRANSPARENT_BG | CREATE_SURFACE);

        this->buttons[LAST]->highlight = render_text(
            this->font, (string)text, NULL, this->hlColor,
            ALIGN_CENTER | TRANSPARENT_BG | CREATE_SURFACE);

        this->buttons[LAST]->x = ((this->display.GetWidth() / 2) - 
            (this->buttons[LAST]->normal->clip_rect.w / 2));

        this->offset = this->buttons[LAST]->normal->clip_rect.h;
        this->buttons[LAST]->y = this->y;
        this->y += this->offset;
    }
    else
    {
        this->buttons[LAST]->normal = render_text(
            this->font, (string)text, NULL, this->txtColor,
            TRANSPARENT_BG | CREATE_SURFACE);

        this->buttons[LAST]->highlight = render_text(
            this->font, (string)text, NULL, this->hlColor,
            TRANSPARENT_BG | CREATE_SURFACE);

        this->offset = this->buttons[LAST]->normal->clip_rect.h;
        this->buttons[LAST]->x = this->x;
        this->buttons[LAST]->y = this->y;
        this->y += this->offset;
    }

    this->buttons[LAST]->mouseOver = create_rect(this->buttons[LAST]->x,
        this->buttons[LAST]->y,
        this->buttons[LAST]->normal->clip_rect.w,
        this->buttons[LAST]->normal->clip_rect.h);

    this->buttons[LAST]->btn_type   = BTN_ACTION;
    this->buttons[LAST]->act_type   = act;
    this->buttons[LAST]->text       = new char[strlen(text) + 1];
    strncpy(this->buttons[LAST]->text, text, strlen(text) + 1);

    if(act == ACT_RETURN)
    {
        this->buttons[LAST]->id = -1;
    }
    else if(act == ACT_QUIT)
    {   
        this->buttons[LAST]->id = -2;
    }
    else
    {
        this->buttons[LAST]->id = this->buttons.size() + 69;
    }

    return this->buttons[LAST]->id;
}

int CMenu::AddToggle(const char* text, bool on)
{
    this->buttons.push_back(new Button);

    if(this->center)
    {
        this->buttons[LAST]->normal = render_text(
            this->font, (string)text, NULL, this->txtColor,
            ALIGN_CENTER | TRANSPARENT_BG | CREATE_SURFACE);

        this->buttons[LAST]->highlight = render_text(
            this->font, (string)text, NULL, this->hlColor,
            ALIGN_CENTER | TRANSPARENT_BG | CREATE_SURFACE);

        this->buttons[LAST]->x = ((this->display.GetWidth() / 2) - 
            (this->buttons[LAST]->normal->clip_rect.w / 2));

        this->offset = this->buttons[LAST]->normal->clip_rect.h;
        this->buttons[LAST]->y = this->y;
        this->y += this->offset;
    }
    else
    {
        this->buttons[LAST]->normal = render_text(
            this->font, (string)text, NULL, this->txtColor,
            TRANSPARENT_BG | CREATE_SURFACE);

        this->buttons[LAST]->highlight = render_text(
            this->font, (string)text, NULL, this->hlColor,
            TRANSPARENT_BG | CREATE_SURFACE);

        this->offset = this->buttons[LAST]->normal->clip_rect.h;
        this->buttons[LAST]->x = this->x;
        this->buttons[LAST]->y = this->y;
        this->y += this->offset;
    }

    this->buttons[LAST]->mouseOver = create_rect(this->buttons[LAST]->x,
        this->buttons[LAST]->y,
        this->buttons[LAST]->normal->clip_rect.w,
        this->buttons[LAST]->normal->clip_rect.h);

    this->buttons[LAST]->btn_type       = BTN_TOGGLE;
    this->buttons[LAST]->text           = new char[strlen(text) + 1];
    strncpy(this->buttons[LAST]->text, text, strlen(text) + 1);
    this->buttons[LAST]->act_type       = ACT_NONE;
    this->buttons[LAST]->switch_status  = on;

    this->buttons[LAST]->id             = this->buttons.size() + 69;
    return this->buttons[LAST]->id;
}

int CMenu::AddSubMenu(const char* text, CMenu* nextMenu)
{
    if(nextMenu == NULL)
        return -1;

    this->buttons.push_back(new Button);

    if(this->center)
    {
        this->buttons[LAST]->normal = render_text(
            this->font, (string)text, NULL, this->txtColor,
            ALIGN_CENTER | TRANSPARENT_BG | CREATE_SURFACE);

        this->buttons[LAST]->highlight = render_text(
            this->font, (string)text, NULL, this->hlColor,
            ALIGN_CENTER | TRANSPARENT_BG | CREATE_SURFACE);

        this->buttons[LAST]->x = ((this->display.GetWidth() / 2) - 
            (this->buttons[LAST]->normal->clip_rect.w / 2));

        this->offset = this->buttons[LAST]->normal->clip_rect.h;
        this->buttons[LAST]->y = this->y;
        this->y += this->offset;
    }
    else
    {
        this->buttons[LAST]->normal = render_text(
            this->font, (string)text, NULL, this->txtColor,
            TRANSPARENT_BG | CREATE_SURFACE);

        this->buttons[LAST]->highlight = render_text(
            this->font, (string)text, NULL, this->hlColor,
            TRANSPARENT_BG | CREATE_SURFACE);

        this->offset = this->buttons[LAST]->normal->clip_rect.h;
        this->buttons[LAST]->x = this->x;
        this->buttons[LAST]->y = this->y;
        this->y += this->offset;
    }

    this->buttons[LAST]->mouseOver = create_rect(this->buttons[LAST]->x,
        this->buttons[LAST]->y,
        this->buttons[LAST]->normal->clip_rect.w,
        this->buttons[LAST]->normal->clip_rect.h);

    this->buttons[LAST]->btn_type   = BTN_SUBMENU;
    this->buttons[LAST]->subMenu    = nextMenu;
    this->buttons[LAST]->act_type   = ACT_NONE;
    this->buttons[LAST]->text       = new char[strlen(text) + 1];
    strncpy(this->buttons[LAST]->text, text, strlen(text) + 1);

    this->buttons[LAST]->id         = this->buttons.size() + 69;
    return this->buttons[LAST]->id;
}

int CMenu::AddText(const char* text)
{
    this->buttons.push_back(new Button);

    this->buttons[LAST]->btn_type   = BTN_TEXT;
    this->buttons[LAST]->text       = new char[strlen(text) + 1];
    strncpy(this->buttons[LAST]->text, text, strlen(text) + 1);

    if(this->center)
    {
        this->buttons[LAST]->normal = render_text(
            this->font, (string)text, NULL, this->txtColor,
            ALIGN_CENTER | TRANSPARENT_BG | CREATE_SURFACE);

        this->buttons[LAST]->highlight = NULL;

        this->buttons[LAST]->x = ((this->display.GetWidth() / 2) - 
            (this->buttons[LAST]->normal->clip_rect.w / 2));

        this->offset = this->buttons[LAST]->normal->clip_rect.h;
        this->buttons[LAST]->y = this->y;
        this->y += this->offset;
    }
    else
    {
        this->buttons[LAST]->normal = render_text(
            this->font, (string)text, NULL, this->txtColor,
            TRANSPARENT_BG | CREATE_SURFACE);

        this->buttons[LAST]->highlight = NULL;

        this->offset = this->buttons[LAST]->normal->clip_rect.h;
        this->buttons[LAST]->x = this->x;
        this->buttons[LAST]->y = this->y;
        this->y += this->offset;
    }

    this->buttons[LAST]->mouseOver = create_rect(this->buttons[LAST]->x,
        this->buttons[LAST]->y,
        this->buttons[LAST]->normal->clip_rect.w,
        this->buttons[LAST]->normal->clip_rect.h);

    this->buttons[LAST]->btn_type   = BTN_TEXT;
    this->buttons[LAST]->act_type   = ACT_NONE;
    this->buttons[LAST]->text       = new char[strlen(text) + 1];
    strncpy(this->buttons[LAST]->text, text, strlen(text) + 1);

    this->buttons[LAST]->id         = this->buttons.size() + 69;
    return this->buttons[LAST]->id;
}

int CMenu::AddGeneric(const char* text)
{
    this->buttons.push_back(new Button);

    this->buttons[LAST]->btn_type   = BTN_TEXT;
    this->buttons[LAST]->text       = new char[strlen(text) + 1];
    strncpy(this->buttons[LAST]->text, text, strlen(text) + 1);

    if(this->center)
    {
        this->buttons[LAST]->normal = render_text(
            this->font, (string)text, NULL, this->txtColor,
            ALIGN_CENTER | TRANSPARENT_BG | CREATE_SURFACE);

        this->buttons[LAST]->highlight = render_text(
            this->font, (string)text, NULL, this->hlColor,
            ALIGN_CENTER | TRANSPARENT_BG | CREATE_SURFACE);

        this->buttons[LAST]->x = ((this->display.GetWidth() / 2) - 
            (this->buttons[LAST]->normal->clip_rect.w / 2));

        this->offset = this->buttons[LAST]->normal->clip_rect.h;
        this->buttons[LAST]->y = this->y;
        this->y += this->offset;
    }
    else
    {
        this->buttons[LAST]->normal = render_text(
            this->font, (string)text, NULL, this->txtColor,
            TRANSPARENT_BG | CREATE_SURFACE);

        this->buttons[LAST]->highlight = render_text(
            this->font, (string)text, NULL, this->hlColor,
            TRANSPARENT_BG | CREATE_SURFACE);

        this->offset = this->buttons[LAST]->normal->clip_rect.h;
        this->buttons[LAST]->x = this->x;
        this->buttons[LAST]->y = this->y;
        this->y += this->offset;
    }

    this->buttons[LAST]->mouseOver = create_rect(this->buttons[LAST]->x,
        this->buttons[LAST]->y,
        this->buttons[LAST]->normal->clip_rect.w,
        this->buttons[LAST]->normal->clip_rect.h);

    this->buttons[LAST]->btn_type   = BTN_GENERIC;
    this->buttons[LAST]->act_type   = ACT_GENERIC;
    this->buttons[LAST]->text       = new char[strlen(text) + 1];
    strncpy(this->buttons[LAST]->text, text, strlen(text) + 1);

    this->buttons[LAST]->id         = this->buttons.size() + 69;
    return this->buttons[LAST]->id;
}

int CMenu::Run()
{
    int mouse_x, mouse_y;
    int status = -1;

    bool clicked;

    if(this->music != NULL)
    {
        if(Mix_PlayMusic(this->music, -1) == -1)
        {
            char error[256];
            sprintf(error, "In-game music disabled!\nError: %s", Mix_GetError());
            handleError(error);
        }
    }

    while(!this->quit)
    {
        this->eventHandler.HandleMenuEvents(this->quit,
                mouse_x, mouse_y, clicked);

        this->CheckMouseOver(mouse_x, mouse_y);

        /* Check where the user clicked */
        if(clicked)
            status = this->CheckClick(mouse_x, mouse_y);
        else
            status = -1;

        if(status != -1)
        {
            if(this->buttons[status]->btn_type == BTN_SUBMENU)
            {
                int retval = this->buttons[status]->subMenu->Run();
                if(this->buttons[status]->subMenu->FindMenuOptionByText("Return") != retval)
                    return retval;
            }

            else if(this->buttons[status]->btn_type == BTN_TOGGLE)
            {
                string tmp(this->buttons[status]->text);

                if(this->buttons[status]->switch_status)
                {
                    this->buttons[status]->switch_status = false;
                    tmp = tmp.substr(0, tmp.find("ON"));
                    tmp += "OFF";
                }
                else
                {
                    this->buttons[status]->switch_status = true;
                    tmp = tmp.substr(0, tmp.find("OFF"));
                    tmp += "ON";
                }

                delete[] this->buttons[status]->text;
                this->buttons[status]->text = new char[tmp.length() + 1];
                strncpy(this->buttons[status]->text, tmp.c_str(), tmp.length() + 1);

                if(this->center)
                {
                    this->buttons[status]->normal = render_text(
                        this->font, tmp, NULL, this->txtColor,
                        ALIGN_CENTER | TRANSPARENT_BG | CREATE_SURFACE);

                    this->buttons[status]->highlight = render_text(
                        this->font, tmp, NULL, this->hlColor,
                        ALIGN_CENTER | TRANSPARENT_BG | CREATE_SURFACE);

                    this->buttons[status]->x = ((this->display.GetWidth() / 2) - 
                        (this->buttons[status]->normal->clip_rect.w / 2));
                }

                else
                {
                    this->buttons[status]->normal = render_text(
                        this->font, tmp, NULL, this->txtColor,
                        TRANSPARENT_BG | CREATE_SURFACE);

                    this->buttons[status]->highlight = render_text(
                        this->font, tmp, NULL, this->hlColor,
                        TRANSPARENT_BG | CREATE_SURFACE);
                }
            }

            else if(this->buttons[status]->btn_type == BTN_ACTION)
            {
                if(this->buttons[status]->act_type == ACT_QUIT)
                    return QUIT_ID;
                else if(this->buttons[status]->act_type == ACT_RETURN)
                    return RETURN_ID;
                else if(this->buttons[status]->act_type == ACT_PLAY)
                {
                    if(this->music != NULL)
                        Mix_HaltMusic();

                    return this->buttons[status]->id;
                }
                else if(this->buttons[status]->act_type == ACT_GENERIC)
                {
                    return this->buttons[status]->id;
                }
            }

            else if(this->buttons[status]->btn_type == BTN_GENERIC)
            {
                return this->buttons[status]->id;
            }

            else if(this->buttons[status]->btn_type == BTN_TEXT)
            {
                /* Do nothing */
            }
        }

        this->display.Blit(this->bg, 0, 0);

        /* CDisplay each menu option */
        for(unsigned int i=0; i < this->buttons.size(); i++)
        {
            this->display.Blit(this->buttons[i]->main,
                this->buttons[i]->x, 
                this->buttons[i]->y);
        }

        /* Update the screen */
        this->display.Update();
    }

    /* If we broke out of the while loop because quit was
     * true, we return -1, the ID to quit.
     */
    return -2;
}

int CMenu::RunNoBlock(const int framesToRunFor)
{
    int count = 0;
    int mouse_x, mouse_y;
    int status;

    bool clicked;

    if(this->music != NULL)
    {
        if(Mix_PlayMusic(this->music, -1) == -1)
        {
            char error[256];
            sprintf(error, "In-game music disabled!\nError: %s", Mix_GetError());
            handleError(error);
        }
    }

    while(!this->quit && count < framesToRunFor)
    {
        count++;
        this->eventHandler.HandleMenuEvents(this->quit,
                mouse_x, mouse_y, clicked);

        this->CheckMouseOver(mouse_x, mouse_y);

        /* Check where the user clicked */
        if(clicked)
            status = this->CheckClick(mouse_x, mouse_y);
        else
            status = -1;

        if(status != -1)
        {
            if(this->buttons[status]->btn_type == BTN_SUBMENU)
            {
                int retval = this->buttons[status]->subMenu->Run();
                if(this->buttons[status]->subMenu->FindMenuOptionByText("Return") != retval)
                    return retval;
            }

            else if(this->buttons[status]->btn_type == BTN_TOGGLE)
            {
                string tmp(this->buttons[status]->text);

                if(this->buttons[status]->switch_status)
                {
                    this->buttons[status]->switch_status = false;
                    tmp = tmp.substr(0, tmp.find("ON"));
                    tmp += "OFF";
                }
                else
                {
                    this->buttons[status]->switch_status = true;
                    tmp = tmp.substr(0, tmp.find("OFF"));
                    tmp += "ON";
                }

                delete[] this->buttons[status]->text;
                this->buttons[status]->text = new char[tmp.length() + 1];
                strncpy(this->buttons[status]->text, tmp.c_str(), tmp.length() + 1);

                if(this->center)
                {
                    this->buttons[status]->normal = render_text(
                        this->font, tmp, NULL, this->txtColor,
                        ALIGN_CENTER | TRANSPARENT_BG | CREATE_SURFACE);

                    this->buttons[status]->highlight = render_text(
                        this->font, tmp, NULL, this->hlColor,
                        ALIGN_CENTER | TRANSPARENT_BG | CREATE_SURFACE);

                    this->buttons[status]->x = ((this->display.GetWidth() / 2) - 
                        (this->buttons[status]->normal->clip_rect.w / 2));
                }

                else
                {
                    this->buttons[status]->normal = render_text(
                        this->font, tmp, NULL, this->txtColor,
                        TRANSPARENT_BG | CREATE_SURFACE);

                    this->buttons[status]->highlight = render_text(
                        this->font, tmp, NULL, this->hlColor,
                        TRANSPARENT_BG | CREATE_SURFACE);
                }
            }

            else if(this->buttons[status]->btn_type == BTN_ACTION)
            {
                if(this->buttons[status]->act_type == ACT_QUIT)
                    return QUIT_ID;
                else if(this->buttons[status]->act_type == ACT_RETURN)
                    return RETURN_ID;
                else if(this->buttons[status]->act_type == ACT_PLAY)
                {
                    if(this->music != NULL)
                        Mix_HaltMusic();

                    return this->buttons[status]->id;
                }
                else if(this->buttons[status]->act_type == ACT_GENERIC)
                {
                    return this->buttons[status]->id;
                }
            }

            else if(this->buttons[status]->btn_type == BTN_GENERIC)
            {
                return this->buttons[status]->id;
            }

            else if(this->buttons[status]->btn_type == BTN_TEXT)
            {
                /* Do nothing */
            }
        }

        this->display.Blit(this->bg, 0, 0);

        /* CDisplay each menu option */
        for(unsigned int i=0; i < this->buttons.size(); i++)
        {
            this->display.Blit(this->buttons[i]->main,
                this->buttons[i]->x, 
                this->buttons[i]->y);
        }

        /* Update the screen */
        this->display.Update();
    }

    /* If we broke out of the while loop because quit was
     * true, we return -1, the ID to quit.
     */
    return -2;
}