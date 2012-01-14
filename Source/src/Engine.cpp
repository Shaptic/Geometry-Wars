#include "Engine.h"

Engine::Engine(): Settings("Settings.ini"),
    Player(Display, Timer, Settings),
    Score(Display),
    Particles(Display, Timer),
    Debugger(Display, Settings),
    Levels(Settings),
    MainMenu(Display)
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
    this->Game_BG   = LoadImage_Alpha(this->Settings.GetValueAt("GameBackground"));
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
}

Engine::~Engine()
{
    this->Garbage.EmptyTrash(this->Enemies, this->Bullets);
    this->Garbage.TrashBullets(this->Bullets);
    this->Garbage.TrashEnemies(this->Enemies);
    this->Garbage.EmptyTrash(this->Enemies, this->Bullets);
}

void Engine::Menu()
{
    Mix_Music*   music          = Mix_LoadMUS(this->Settings.GetValueAt("MenuMusic").c_str());

    SDL_Surface* background     = LoadImage("Images/Menu_BG.png");
    SDL_Surface* menu_overlay   = LoadImage_Alpha("Images/Menu_Options_Overlay.png");
    SDL_Surface* text           = create_surface_alpha(this->Display.GetWidth(), this->Display.GetHeight());
    
    /* To generate explosions */
    CEntity* Particle = new CEntity(this->Display, this->Timer);
    Particle->SetEntity(create_surface(1, 1, create_color(rand() % 255, rand() % 255, rand() % 255)));
    Particle->Move_Force(rand() % this->Display.GetWidth(), rand() % this->Display.GetHeight());

    int play_id     = this->MainMenu.AddMenuItem(50, 75, "Images/Menu_PlayGame.png", "Images/Menu_PlayGame_High.png");
    int options_id  = this->MainMenu.AddMenuItem(50, 175, "Images/Menu_Options.png", "Images/Menu_Options_High.png");
    int credits_id  = this->MainMenu.AddMenuItem(50, 275, "Images/Menu_Credits.png", "Images/Menu_Credits_High.png");
    int quit_id     = this->MainMenu.AddMenuItem(50, 375, "Images/Menu_Quit.png", "Images/Menu_Quit_High.png");

    SDL_Surface* tmp = render_text(this->Menu_Font, "Shape Wars", NULL, create_color(0, 190, 225), CREATE_SURFACE | TRANSPARENT_BG);
    SDL_SetAlpha(tmp, 0, SDL_ALPHA_TRANSPARENT);
    this->Display.Blit(text, tmp, 375, 100);
    
    TTF_CloseFont(this->Menu_Font);
    SDL_FreeSurface(tmp);

    this->Menu_Font = TTF_OpenFont(this->Settings.GetValueAt("MenuFont").c_str(), 32);
    tmp = render_text(this->Menu_Font, "by George\nKudrayvtsev", NULL, create_color(0, 190, 225), CREATE_SURFACE | TRANSPARENT_BG);
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

    int frame   = 0;
    int status  = 0;

    while(!this->quit)
    {
        this->Timer.Start();
        frame++;

        /* Generate an explosion every 10th of a second */
        if(frame % 10 == 0)
        {
            delete Particle;
            Particle = new CEntity(this->Display, this->Timer);
            Particle->SetEntity(create_surface(1, 1, create_color(rand() % 255, rand() % 255, rand() % 255)));
            Particle->Move_Force(rand() % this->Display.GetWidth(), rand() % this->Display.GetHeight());
            this->Particles.GenerateExplosion(Particle);
        }

        if(CheckQuit_Event())
            break;

        this->Display.ClearScreen(background);
        
        this->Particles.Update();

        this->Display.Blit(menu_overlay, 25, 25);
        this->Display.Blit(text, 0, 0);
        
        status = this->MainMenu.Update();
        
        if(status == play_id)
        {
            /*int time = 0;
            while(time <= 100)
            {
                this->Timer.Start();
                time++;
                this->Display.ClearScreen();
                SDL_Surface* encouragement = render_text(this->Menu_Font, "They are pussies;\nYou are a viking;\nBeat the shit\nout of them.", NULL, create_color(0, 190, 225), CREATE_SURFACE | ALIGN_CENTER | TRANSPARENT_BG);
                this->Display.Blit(encouragement, this->Display.GetWidth() / 2, this->Display.GetHeight() / 2);
                this->Display.Update();
                this->Timer.DelayFPS();
                SDL_FreeSurface(encouragement);
            }*/

            this->SinglePlayerGame();
            break;
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

    Mix_ResumeMusic();

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

bool Engine::CheckCollisions()
{
    /* For every enemy that is alive, check
     * to see if any bullets have hit it.
     *
     * Because std::list is stupid, you must
     * break out of the loop everytime you
     * want to delete something, meaning sometimes
     * bullet and enemy collisions won't register.
     * TODO: Maybe solve this?
     */
    for(CEnemies::iterator i = this->Enemies.begin();
        i != this->Enemies.end(); i++)
    {
        for(CBullets::iterator j = this->Bullets.begin();
            j != this->Bullets.end(); j++)
        {
            if((*j)->DetectCollision(*i))
            {
                if((*i)->Die(this->Player.GetDamage()))
                {
                    this->Levels.IncreaseKillCount(1);
                    this->Player.IncreaseAmmo(5);

                    this->Particles.GenerateExplosion(*i);

                    if(!this->Debugger.IsDebug())
                    {
                        this->Score.UpdateItem_Increment("Score: ", 5);
                    }

                    this->Garbage.TrashEnemy(*i);
                }
                this->Garbage.TrashBullet(*j);
                return false;
            }
        }
        if(this->Player.DetectCollision(*i))
        {
            if((*i)->Die(this->Player.GetDamage()))
            {
                this->Player.IncreaseAmmo(5);
                this->Levels.IncreaseKillCount(1);
                this->Particles.GenerateExplosion(*i);
                this->Garbage.TrashEnemy(*i);
            }

            if(!this->Debugger.IsDebug())
            {
                /* Reduce life supply, if we have no more, die
                 * for realsies. 
                 * TODO: Fix the fact that hitting an enemy takes >1 life based
                 *       on enemy health.
                 */
                this->Player.Die();
                this->Score.UpdateItem("Lives: ", this->Player.GetLives());

                if(this->Player.GetLives() <= 0)
                {
                    this->Particles.GenerateExplosion(&this->Player);
                    return true;
                }
                else
                {
                    this->Score.UpdateItem_Increment("Score: ", 5);
                    return false;
                }
            }
        }
    }

    return false;
}

void Engine::Update()
{
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