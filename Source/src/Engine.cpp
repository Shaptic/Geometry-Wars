#include "Engine.h"

Engine::Engine(): Settings("Settings.ini"),
    Player(Display, Timer, Settings),
    Score(Display),
    Particles(Display, Timer),
    Debugger(Display, Settings),
    Levels(Settings),
    MainMenu(Display),
    Menu_Files(Settings.GetValueAt("MenuDataRoot"))
{
    srand((unsigned int)time(NULL));

    this->Timer.SetFrameRate(atoi(this->Settings.GetValueAt("FrameRate").c_str()));

    this->Player.LoadEntity(this->Settings.GetValueAt("PlayerSprite").c_str());
    this->Player.Move_Force(this->Display.GetWidth() / 2, this->Display.GetHeight() / 2);

    this->Player.SetMovementBoundaries_Max(
        this->Display.GetWidth()  - this->Player.GetCollisionBoundaries()->w,
        this->Display.GetHeight() - this->Player.GetCollisionBoundaries()->h);

    this->Player.SetMovementBoundaries_Min(0, 0);

    this->quit      = false;
    this->Game_BG   = LoadImage_Alpha(combine(this->Menu_Files, this->Settings.GetValueAt("GameBackground").c_str()));
    this->Game_Music= Mix_LoadMUS(this->Settings.GetValueAt("GameMusic").c_str());
    this->Menu_Font = TTF_OpenFont(this->Settings.GetValueAt("MenuFont").c_str(),
        atoi(this->Settings.GetValueAt("MenuFontSize").c_str()));
    this->UI_Font   = TTF_OpenFont(this->Settings.GetValueAt("UIFont").c_str(),
        atoi(this->Settings.GetValueAt("UIFontSize").c_str()));

    /* For aligning the text properly on-screen */
    int y_offset = 10;

    this->Score.SetFont(this->UI_Font);
    this->Score.SetTextColor(WHITE);
    this->Score.AddItem(10, y_offset, "Score: ", 0);

    y_offset = this->Display.GetHeight() - get_text_height(this->UI_Font, "Ammo Remaining: ");
    this->Score.AddItem(10,  y_offset, "Ammo Remaining: ", 50);

    y_offset -= get_text_height(this->UI_Font, "Lives: ");
    this->Score.AddItem(10, y_offset, "Lives: ", 3);

    if(!this->Game_Music)
        handleError(Mix_GetError());

    std::string hs_filename(this->Settings.GetValueAt("HighscoreFile"));
    if(hs_filename.empty())
        this->Settings.CreateValue("HighscoreFile", "Highscore.dat");

    hs_filename = this->Settings.GetValueAt("HighscoreFile");

    std::string scoreline;

    this->HighscoreFile.open(hs_filename, std::ios::in);
    std::getline(this->HighscoreFile, scoreline);
    this->HighscoreFile.flush();
    this->HighscoreFile.close();

    if(scoreline.empty())
        this->high_score = 0;
    else
    {
        int hash = atoi(scoreline.substr(scoreline.length() - 2, -1).c_str());
        this->high_score = atoi(scoreline.substr(0, scoreline.length() - 2).c_str()) / hash;
        scoreline.clear();
    }

    y_offset = 10;
    this->Score.AddItem(this->Display.GetWidth() - get_text_width(this->UI_Font, "Highscore: 99999"), 
        y_offset, "Highscore: ", this->high_score);
}

Engine::~Engine()
{
    this->Garbage.EmptyTrash(this->Enemies, this->Bullets);
    this->Garbage.TrashBullets(this->Bullets);
    this->Garbage.TrashEnemies(this->Enemies);
    this->Garbage.EmptyTrash(this->Enemies, this->Bullets);

    this->HighscoreFile.open(this->Settings.GetValueAt("HighscoreFile"), std::ios::out, std::ios::trunc);

    if(this->HighscoreFile.bad() || !this->HighscoreFile.is_open())
        handleError("Unable to record high score!", false);

    int hash = 10 + rand() % 10;
    this->HighscoreFile << this->high_score * hash << hash << "\n";

    this->HighscoreFile.flush();
    this->HighscoreFile.close();
}

void Engine::Menu()
{
    Mix_Music*   music          = Mix_LoadMUS(this->Settings.GetValueAt("MenuMusic").c_str());

    SDL_Surface* background     = LoadImage(combine(this->Menu_Files, this->Settings.GetValueAt("MenuBackground").c_str()));
    SDL_Surface* menu_overlay   = LoadImage_Alpha(combine(this->Menu_Files, "Menu_Options_Overlay.png"));
    SDL_Surface* text           = create_surface_alpha(this->Display.GetWidth(), this->Display.GetHeight());

    /* To generate explosions */
    CEntity         dot(this->Display, this->Timer);
    SDL_Color       dot_color = {rand() % 255, rand() % 255, rand() % 255};
    SDL_Surface*    dot_surface = create_surface(1, 1, dot_color);

    dot.SetEntity(dot_surface);

    int play_id     = this->MainMenu.AddMenuItem(50, 75,  combine(this->Menu_Files, "Menu_PlayGame.png"),   combine(this->Menu_Files, "Menu_PlayGame_High.png"));
    int options_id  = this->MainMenu.AddMenuItem(50, 175, combine(this->Menu_Files, "Menu_Options.png"),    combine(this->Menu_Files, "Menu_Options_High.png"));
    int credits_id  = this->MainMenu.AddMenuItem(50, 275, combine(this->Menu_Files, "Menu_Credits.png"),    combine(this->Menu_Files, "Menu_Credits_High.png"));
    int quit_id     = this->MainMenu.AddMenuItem(50, 375, combine(this->Menu_Files, "Menu_Quit.png"),       combine(this->Menu_Files, "Menu_Quit_High.png"));

    SDL_Surface* tmp = render_text(this->Menu_Font, "Shape Wars", NULL, create_color(0, 190, 225), CREATE_SURFACE | TRANSPARENT_BG);
    SDL_SetAlpha(tmp, 0, SDL_ALPHA_TRANSPARENT);
    this->Display.Blit(text, tmp, 375, 100);

    TTF_CloseFont(this->Menu_Font);
    SDL_FreeSurface(tmp);

    this->Menu_Font = TTF_OpenFont(this->Settings.GetValueAt("MenuFont").c_str(), 32);
    tmp = render_text(this->Menu_Font, "by George\nKudrayvtsev", NULL, create_color(0, 190, 225), CREATE_SURFACE | TRANSPARENT_BG);
    SDL_Surface* credits = render_text(this->Menu_Font, CREDITS_STR, NULL, create_color(0, 190, 225), CREATE_SURFACE | TRANSPARENT_BG);
    SDL_SetAlpha(tmp, 0, SDL_ALPHA_TRANSPARENT);
    this->Display.Blit(text, tmp, 375, 200);
    SDL_FreeSurface(tmp);

    if(Mix_PlayMusic(music, -1) == -1)
    {
        char error[256];
        sprintf(error, "In-game music disabled!\nError: %s", Mix_GetError());
        handleError(error, false);
    }

    Mix_VolumeMusic(50);

    int status  = 0;

    while(!this->quit)
    {
        this->Timer.Start();

        /* Generate an explosion every 10th of a second */
        if(this->Timer.GetFrame() % (this->Timer.GetFrameRate() / 6) == 0)
        {
            dot.FreeEntity();
            
            dot_color.r = rand() % 255;
            dot_color.g = rand() % 255;
            dot_color.b = rand() % 255;

            dot_surface = create_surface(1, 1, dot_color);
            dot.SetEntity(dot_surface);
            dot.Move(rand() % this->Display.GetWidth(), rand() % this->Display.GetHeight(), this->Timer.GetTicks());

            this->Particles.GenerateExplosion(&dot);
        }

        if(CheckQuit_Event())
            this->quit = true;

        this->Display.ClearScreen(background);

        this->Particles.Update();

        this->Display.Blit(menu_overlay, 25, 25);
        this->Display.Blit(text, 0, 0);

        status = this->MainMenu.Update();

        if(status == play_id)
        {
            Mix_PauseMusic();
            this->SinglePlayerGame();
            break;
        }
        else if(status == credits_id)
        {
            SDL_Surface* credits_tmp = background;
            this->Display.Blit(credits_tmp, credits, 0, 0);
            while(!this->quit)
            {
                if(CheckQuit_Event())
                    this->quit = true;
                this->Display.Blit(credits_tmp, 0, 0);
                this->Display.Update();
            }
        }
        else if(status == quit_id)
        {
            this->quit = true;
        }        

        this->Display.Update();
        this->Timer.DelayFPS();
    }
}

void Engine::SinglePlayerGame()
{
    /* So that the menu click doesn't register 
     * as a shot.
     */
    SDL_Delay(100);

    /* Play game music! */
    if(Mix_PlayMusic(this->Game_Music, -1) == -1)
    {
        char error[256];
        sprintf(error, "In-game music disabled!\nError: %s", Mix_GetError());
        handleError(error, false);
    }

    Mix_VolumeMusic(50);

    /* Begin main game loop */
    while(!this->quit)
    {
        this->Timer.Start();

        this->Events();

        /* Temporary fix to a weird bug of player
         * moving offscreen if the window is moved.
         * TODO: Find cause.
         */
        if( this->Player.GetX() > this->Display.GetWidth() || 
            this->Player.GetY() > this->Display.GetHeight() ||
            this->Player.GetX() < 0 ||
            this->Player.GetY() < 0)
        {
            this->Player.Move_Force(
                this->Display.GetWidth() / 2,
                this->Display.GetHeight() / 2);
        }

        /* We empty the trash twice, once after
         * detecting collisions, and again after testing
         * for off-screen bullets. This is because, in rare
         * cases, a bullet will collide with an enemy offscreen,
         * causing it to be deleted twice.
         */
        if(this->CheckCollisions())
            break;
        this->Garbage.EmptyTrash(this->Enemies, this->Bullets);

        /* Update everything on-screen */
        this->Update();

        /* Update the display */
        this->Display.Update();

        /* Control framerate */
        this->Timer.DelayFPS();
    }

    if(!this->quit) // We broke out of the loop, and aren't ending the game
    {
        Mix_PauseMusic();

        static SDL_Surface* restart = render_text(this->Menu_Font,
            "GAME OVER!\nPress SPACE to start a new game!\nPress Q or ESCAPE to quit!",
            NULL, create_color(WHITE), CREATE_SURFACE | TRANSPARENT_BG | ALIGN_CENTER);

        while(!this->quit)
        {
            if(IsDown(SDLK_SPACE))
                break;
            else if(IsDown(SDLK_ESCAPE) || IsDown(SDLK_q) || CheckQuit_Event())
                this->quit = true;

            this->Timer.Start();
            this->Update();
            this->Display.Blit(restart,
                this->Display.GetWidth() / 2 - restart->clip_rect.w / 2,
                this->Display.GetHeight() / 2 - restart->clip_rect.h / 2);
            this->Display.Update();
            this->Timer.DelayFPS();
        }
    }

    /* After waiting for user-input, if quit is true user wants to
     * quit, otherwise start a new game!
     */
    if(!this->quit)
    {
        this->Reset();
        this->SinglePlayerGame();
    }
}

void Engine::Reset()
{
    this->Garbage.EmptyTrash(this->Enemies, this->Bullets);
    this->Garbage.TrashBullets(this->Bullets);
    this->Garbage.TrashEnemies(this->Enemies);
    this->Garbage.EmptyTrash(this->Enemies, this->Bullets);

    this->Bullets.clear();
    this->Enemies.clear();

#ifdef _DEBUG
    this->Debugger.ToggleDebugMode(true);
#else
    this->Debugger.ToggleDebugMode(false);
#endif // _DEBUG

    this->Levels.Reset();
    this->Player.Reset();

    this->Score.UpdateItem("Score: ", 0);
    this->Score.UpdateItem("Lives: ", 3);
}

void Engine::Events()
{
    static SDL_Event event;

    if(CheckQuit(SDLK_ESCAPE))
        this->quit = true;

    /* Handle everything dealing with
     * player controls.
     */

    if(IsDown(SDLK_UP) || IsDown(SDLK_w))
        this->Player.Move_Rate(0, -this->Player.GetSpeed());
    else if(IsDown(SDLK_DOWN) || IsDown(SDLK_s))
        this->Player.Move_Rate(0, this->Player.GetSpeed());

    if(IsDown(SDLK_RIGHT) || IsDown(SDLK_d))
        this->Player.Move_Rate(this->Player.GetSpeed(), 0);
    else if(IsDown(SDLK_LEFT) || IsDown(SDLK_a))
        this->Player.Move_Rate(-this->Player.GetSpeed(), 0);

    if(IsDown(SDLK_q) && this->Debugger.IsDebug())
    {
        CEnemy* Enemy = NULL;
        std::string filename(this->Levels.GetSpriteName());

        if(filename.empty())
            Enemy = new CEnemy(this->Display, this->Timer);
        else
            Enemy = new CEnemy(this->Display, this->Timer, filename);

        this->Enemies.push_back(Enemy);
    }

    if(IsPressed(SDL_BUTTON_LEFT))
    {
        this->Player.Shoot(this->Bullets);
    }

    /* Check the event poll */
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            this->quit = true;
            break;

        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_BACKQUOTE:
                this->Debugger.ToggleDebugMode(!this->Debugger.IsDebug());
                break;
            }
            break;
        }
    }
}

void Engine::GenerateEMP()
{
    this->Levels.IncreaseKillCount(this->Enemies.size());
    this->Player.IncreaseAmmo(5 * this->Enemies.size());
    if(!this->Debugger.IsDebug())
        this->Score.UpdateItem_Increment("Score: ", 5 * this->Enemies.size());

    for(CEnemies::iterator i = this->Enemies.begin();
        i != this->Enemies.end(); i++)
    {
        this->Particles.GenerateExplosion(*i);
    }

    this->Garbage.TrashEnemies(this->Enemies);
    this->Garbage.EmptyTrash(this->Enemies, this->Bullets);
}

bool Engine::CheckCollisions()
{
    /* For every enemy that is alive, check
     * to see if any bullets have hit it.
     *
     * Because std::list is stupid, you must
     * break out of the loop everytime you
     * want to delete something, meaning sometimes
     * bullet and enemy collisions won't register
     * at the right time, especially if there is an
     * extremely large amount of them.
     * TODO: Maybe solve this?
     */
    for(CEnemies::iterator i = this->Enemies.begin();
        i != this->Enemies.end(); i++)
    {
        /* Check if any of the bullets have collided with any
         * of the enemies. If they have, and the enemy is dead,
         * set up power-ups accordingly. If not dead, just add
         * the damage.
         */
        for(CBullets::iterator j = this->Bullets.begin();
            j != this->Bullets.end(); j++)
        {
            if((*j)->DetectCollision(*i))
            {
                if((*i)->Die(this->Player.GetDamage()))
                {
                    if(this->HandlePowerups(*i))    // Power-up isn't an EMP so we destroy
                                                    // the single enemy
                    {
                        if(!this->Debugger.IsDebug())
                        {
                            this->Score.UpdateItem_Increment("Score: ", 5);
                            if(this->Score.GetItemValue("Score: ") >= this->high_score)
                            {
                                this->high_score = this->Score.GetItemValue("Score: ");
                                this->Score.UpdateItem("Highscore: ", this->high_score);
                            }
                        }

                        this->Levels.IncreaseKillCount(1);
                        this->Player.IncreaseAmmo(5);
                    }
                }

                this->Garbage.TrashBullet(*j);
                return false;
            }
        }
        /* If the player has run into the enemy, lose
         * a life, check powerups, destroy the enemy,
         * explode, and if no lives are left, end the game.
         */
        if(this->Player.DetectCollision(*i))
        {
            if((*i)->Die(this->Player.GetDamage() * 1000))  // Damage is 1000 so there's no issue with
                                                            // the enemy not dying
            {
                if(this->HandlePowerups(*i))
                {
                    this->Levels.IncreaseKillCount(1);
                    this->Player.IncreaseAmmo(5);
                }
            }

            if(!this->Debugger.IsDebug())
            {
                /* Reduce life supply, if we have no more, die
                 * for realsies. 
                 */
                this->Player.Die();

                if(this->Player.GetLives() <= 0)
                {
                    this->Particles.GenerateExplosion(&this->Player);
                    return true;
                }

                this->Score.UpdateItem_Increment("Score: ", 5);
                if(this->Score.GetItemValue("Score: ") >= this->high_score)
                {
                    this->high_score = this->Score.GetItemValue("Score: ");
                    this->Score.UpdateItem("Highscore: ", this->high_score);
                }
            }

            return false;
        }
    }

    return false;
}

bool Engine::HandlePowerups(CEnemy* Enemy)
{
    CPlayer::PowerUp Tmp = Enemy->GetPowerUp();

    if(Tmp.ability == CPlayer::PowerUp::NUKE)
    {
        this->GenerateEMP();
        return false;
    }
    else if(Tmp.ability != CPlayer::PowerUp::NO_POWERUP)
    {
        this->Player.AddPowerUp(Enemy->GetPowerUp());
        this->Particles.GenerateExplosion(Enemy);
        this->Garbage.TrashEnemy(Enemy);
    }
    else
    {
        this->Particles.GenerateExplosion(Enemy);
        this->Garbage.TrashEnemy(Enemy);
    }

    return true;
}

void Engine::Update()
{
    this->Score.UpdateItem("Lives: ", this->Player.GetLives());
    this->Score.UpdateItem("Ammo Remaining: ", this->Player.GetAmmoCount());

    if(this->Levels.CanSpawn())
    {
        CEnemy* Enemy = NULL;

        std::string filename(this->Levels.GetSpriteName());

        if(filename.empty())
            Enemy = new CEnemy(this->Display, this->Timer);
        else
            Enemy = new CEnemy(this->Display, this->Timer, filename, this->Levels.GetEnemyHealth());

        this->Enemies.push_back(Enemy);
    }

    this->Display.ClearScreen(this->Game_BG);

    this->Player.Update();
    this->Particles.Update();
    this->Score.Update();

    /* Check for off-screen bullets */
    for(CBullets::iterator j = this->Bullets.begin();
        j != this->Bullets.end(); j++)
    {
        if((*j)->GetX() > this->Display.GetWidth() || ((*j)->GetX() < 0))
            this->Garbage.TrashBullet(*j);

        else if((*j)->GetY() > this->Display.GetHeight() || ((*j)->GetY() < 0))
            this->Garbage.TrashBullet(*j);

        else
            (*j)->Update();
    }

    /* Update all enemies */
    for(CEnemies::iterator i = this->Enemies.begin();
        i != this->Enemies.end(); i++)
    {
        (*i)->Update(this->Player);
    }

    /* Show debug info if necessary */
    this->Debugger.Update(this->Player, this->Bullets, this->Enemies);

    /* Empty the trash again */
    this->Garbage.EmptyTrash(this->Enemies, this->Bullets);
}