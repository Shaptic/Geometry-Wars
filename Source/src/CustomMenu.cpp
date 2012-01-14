#include "CustomMenu.h"

int CCustomMenu::Run()
{
    CEntity* Particle = new CEntity(this->display, this->Timer);
    Particle->SetEntity(create_surface(1, 1, create_color(rand() % 255, rand() % 255, rand() % 255)));
    Particle->Move_Force(rand() % this->display.GetWidth(), rand() % this->display.GetHeight());

    int mouse_x, mouse_y;
    int status = 0;
    int frame  = 0;

    bool clicked;

    if(this->music != NULL)
    {
        if(Mix_PlayMusic(this->music, -1) == -1)
        {
            char error[256];
            sprintf(error, "In-game music disabled!\nError: %s", Mix_GetError());
            handleError(error, false);
        }
    }

    while(!this->quit)
    {
        this->Timer.Start();
        frame++;

        /* Generate an explosion every 10th of a second */
        if(frame % 10 == 0)
        {
            delete Particle;
            Particle = new CEntity(this->display, this->Timer);
            Particle->SetEntity(create_surface(1, 1, create_color(rand() % 255, rand() % 255, rand() % 255)));
            Particle->Move_Force(rand() % this->display.GetWidth(), rand() % this->display.GetHeight());
            this->Particles.GenerateExplosion(Particle);
        }

        this->eventHandler.HandleMenuEvents(this->quit,
                mouse_x, mouse_y, clicked);

        this->CheckMouseOver(mouse_x, mouse_y);

        /* Check where the user clicked */
        if(clicked)
            status = this->CheckClick(mouse_x, mouse_y);
        else
            status = INVALID;

        if(status != INVALID)
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
        this->Timer.DelayFPS();
        this->Particles.Update();
        this->display.Update();
    }

    /* If we broke out of the while loop because quit was
     * true, we return -1, the ID to quit.
     */
    return QUIT_ID;
}