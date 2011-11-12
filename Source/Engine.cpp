#include "Engine.h"

Engine::Engine(): MainMenu(Screen, EventHandler),
    Particles(Screen, Fps),
    Player(Screen, Fps),
    Levels(Settings.GetLevelFile())
{
    if(TTF_Init() != 0)
        handleError(TTF_GetError());

    srand((unsigned int)time(NULL));

    this->Fps.SetFrameRate(60);
    this->LoadFiles();

#ifdef _DEBUG
    this->debug         = true;
#else
    this->debug         = false;
#endif // _DEBUG

    this->quit          = false;
    this->play_again    = true;
    this->score         = 0;
    this->high_score    = 0;

    this->LoadHighScore();
}

Engine::~Engine()
{
    this->Enemies.clear();
    this->Shots.clear();

    /* Write the highscore to a file */
    std::ofstream hs(this->Settings.GetHighscoreFile(), std::ios::in | std::ios::trunc);

    if(hs.bad() || !hs.is_open())
        handleError("Unable to record score!");

    int hash = 10 + rand() % 10;
    hs << this->high_score * hash << hash << "\n";

    hs.close();

    Mix_CloseAudio();
}

void Engine::LoadHighScore()
{
    std::string tmp;
    std::ifstream hs(this->Settings.GetHighscoreFile());

    if(!hs.is_open())
        this->high_score = 0;
    else
    {
        std::getline(hs, tmp);

        int hash = atoi(tmp.substr(tmp.length() - 2, -1).c_str());
        this->high_score = atoi(tmp.substr(0, tmp.length() - 2).c_str()) / hash;
        tmp.clear();
    }

    hs.close();
}

void Engine::LoadFiles()
{
    /* Set up our main menu */
    this->MainMenu.SetFont(this->Settings.GetTitleFont());
    this->MainMenu.SetStartCoordinates(this->Screen.GetWidth() / 4, 200);
    this->MainMenu.SetCenterText(false);
    this->MainMenu.SetTextColor(UI_COLOR);
    this->MainMenu.SetHighLightColor(WHITE);
    this->MainMenu.SetMusic(this->Settings.GetTitleMusic());
    this->MainMenu.SetMusicVolume(MIX_MAX_VOLUME / 2);
    this->MainMenu.AddMenuOption("GEOMETRY WARS!\nCreated by George Kudrayvtsev\n", BTN_TEXT, ACT_NONE);
    this->MainMenu.AddMenuOption("Play Geometry Wars!", BTN_ACTION, ACT_PLAY);
    this->MainMenu.AddMenuOption("Quit", BTN_ACTION, ACT_QUIT);
}

void Engine::Menu()
{
    if(this->MainMenu.Run() == CMenu::QUIT_ID)
        return;

    else
    {
        SDL_Delay(100);
        this->Play();
    }
}

void Engine::Play()
{
    /* Some local game values, for handling 
     * mouse, keyboard, speeds, exiting, etc.
     */
    int     dx      = 0;
    int     dy      = 0;
    int     mouse_x = 0;
    int     mouse_y = 0;
    int     frame   = 0;

    GetMousePosition(mouse_x, mouse_y);

    if(Mix_PlayingMusic() == 0) // No music
    {
        Mix_PlayMusic(this->Settings.GetGameplayMusic(), 0);
        Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    }

    while(!this->quit)
    {
        if(frame == INT_MAX)
            frame = 1;

        frame++;

        if(this->Levels.CanSpawn(frame))
        {
            this->AddEnemy();
        }

        this->Fps.Start();

        /* Handle our events */
        this->Events();

        if(CheckQuit())
        {
            this->play_again = false;
            this->quit = true;
        }

        if(IsDown(SDLK_UP) || IsDown(SDLK_w))
            dy = -PLAYER_SPEED;
        else if(IsDown(SDLK_DOWN) || IsDown(SDLK_s))
            dy = PLAYER_SPEED;
        else
            dy = 0;

        if(IsDown(SDLK_LEFT) || IsDown(SDLK_a))
            dx = -PLAYER_SPEED;
        else if(IsDown(SDLK_RIGHT) || IsDown(SDLK_d))
            dx = PLAYER_SPEED;
        else
            dx = 0;

        if(IsPressed(SDL_BUTTON_LEFT))
            this->Player.Shoot(this->Shots);

        this->Particles.AddPlayerTrail(this->Player, dx, dy);

        /* Move player accordingly */
        this->Player.Move_Rate(dx, dy);

        /* Check collisions of bullets with enemies,
         * enemies with player, etc.
         */
        this->CheckCollisions();
        this->RemoveEnemies();
        this->RemoveShots();

        /* Place images on the screen and 
         * update the display.
         */
        this->UpdateAll();
    }

    frame = 0;

    if(this->play_again)
    {
        while(frame < 60 * 2)
        {
            frame++;
            this->UpdateAll();
        }

        Mix_PauseMusic();

        SDL_Surface* again = render_text(this->Settings.GetTitleFont(), "Game over!\nPress SPACE to play again!\nAnd ESCAPE to quit",
            NULL, UI_COLOR, CREATE_SURFACE | ALIGN_CENTER | TRANSPARENT_BG);

        while(this->play_again)
        {
            if(CheckQuit(SDLK_ESCAPE) || CheckQuit_Event())
                this->play_again = false;

            if(IsDown(SDLK_SPACE))
                break;
            
            this->Screen.ClearScreen();
            this->Screen.Blit(again,
                (this->Screen.GetWidth() / 2) - (again->clip_rect.w / 2),
                (this->Screen.GetHeight() / 2) - (again->clip_rect.h / 2));
            this->Screen.Update();
        }

        if(this->play_again)
            this->NewGame();
    }
}

void Engine::NewGame()
{
    this->quit = false;

#ifdef _DEBUG
    this->debug = true;
#else
    this->debug = false;
#endif // _DEBUG

    /* Restart levels */
    this->Levels.Reset();
    
    /* Renew player */
    this->Player.Reset();

    /* Remove all entities on the screen */
    for(AllEnemies::iterator i = this->Enemies.begin(); i != this->Enemies.end(); i++)
        delete (*i);

    for(AllBullets::iterator i = this->Shots.begin(); i != this->Shots.end(); i++)
        delete (*i);

    this->Enemies.clear();
    this->Shots.clear();
    this->remove_enemies.clear();
    this->remove_shots.clear();

    /* Move player to center, reset score */
    this->Player.Move_Force(this->Screen.GetWidth() / 2, this->Screen.GetHeight() / 2);
    this->score = 0;

    this->quit = false;
    this->play_again = true;

    Mix_ResumeMusic();
    this->Play();
}

void Engine::Events()
{
    static SDL_Event evt;
    while(SDL_PollEvent(&evt))
    {
        if(evt.type == SDL_QUIT)
        {
            this->quit = true;
            this->play_again = false;
            return;
        }
        else if(evt.type == SDL_KEYDOWN)
        {
            if(evt.key.keysym.sym == SDLK_BACKQUOTE)
                this->debug = !this->debug;
            else if(evt.key.keysym.sym == SDLK_e && this->debug)
            {
                /* The "e" key forces an EMP explosion */
                this->ForceEMP();
            }
        }
    }

    if(IsDown(SDLK_q) && this->debug)
        this->AddEnemy();
}

void Engine::ForceEMP()
{
    CEnemy emp(this->Screen, this->Fps, this->Player, "Images"FN_SLASH"Circle.png");
    emp.SetPowerUp(PowerUp::EMP);
    this->DestroyEnemy(&emp);
}

void Engine::CheckCollisions()
{
    for(AllEnemies::iterator i = this->Enemies.begin();
        i != this->Enemies.end(); i++)
    {
        for(AllBullets::iterator j = this->Shots.begin();
            j != this->Shots.end(); j++)
        {
            if((*i)->DetectCollision(*j))
            {
                this->DestroyEnemy(*i);
                this->remove_shots.push_back(*j);
                return;
            }
        }

        if(this->Player.DetectCollision(*i))
        {
            if(!this->debug)
                this->Player.Kill();

            /* We delete the enemy we collided with
             * no matter what, but only die if 
             * debug mode is off
             */
            if((*i)->GetPowerUp()->ability == PowerUp::EMP)
            {
                this->DestroyEnemy(*i);
                return;
            }
            else
            {
                this->DestroyEnemy(*i);
            }
        }
    }

    if(!this->debug && this->Player.IsDead())
    {
        this->Particles.ExplodePlayer((int)this->Player.GetX(), (int)this->Player.GetY());

        this->play_again = true;
        this->quit = true;

        return;
    }
}

void Engine::DestroyEnemy(CEnemy* Enemy)
{
    static SDL_Surface* explosion = NULL;

    if(Enemy->GetPowerUp()->ability == PowerUp::EMP)
    {
        /* EMP Destroys all enemies on the screen */

        /* First we update the level info */
        this->Levels.UpdateCurrentLevel(Enemies.size());

        /* Then the score, 5 * all the enemies destroyed, but
         * only if it was used non-cheating.
         */
        if(!this->debug)
            this->score += (this->Enemies.size() * 5);
        
        /* We update the highscore if it has been reached */
        if(this->score > this->high_score)
            this->high_score = this->score;

        /* Then we create an explosion for every enemy on screen */
        for(AllEnemies::iterator j = this->Enemies.begin();
            j != this->Enemies.end(); j++)
        {
            this->Particles.ExplodeObject((*j));
        }

        /* Then we remove the enemies */
        this->Enemies.clear();
        this->remove_enemies.clear();
    }
    else
    {
        /* Generate explosion */
        this->Particles.ExplodeObject(Enemy);

        /* Increase score and add enemy for deletion */
        if(!this->debug)
            this->score += 5;
        this->remove_enemies.push_back(Enemy);

        /* We update the highscore if it has been reached */
        if(this->score > this->high_score)
            this->high_score = this->score;
    }
}

void Engine::RemoveEnemies()
{
    this->Levels.UpdateCurrentLevel(this->remove_enemies.size());

    for(unsigned int i = 0; i < this->remove_enemies.size(); i++)
    {
        delete this->remove_enemies[i];
        this->Enemies.remove(this->remove_enemies[i]);
    }

    this->remove_enemies.clear();
}

void Engine::RemoveShots()
{
    for(unsigned int i = 0; i < this->remove_shots.size(); i++)
    {
        delete this->remove_shots[i];
        this->Shots.remove(this->remove_shots[i]);
    }

    this->remove_shots.clear();
}

void Engine::AddEnemy()
{
    CEnemy* enemy = new CEnemy(this->Screen, this->Fps, this->Player, this->Levels.GetCurrentLevel()->enemy);
    this->Enemies.push_back(enemy);
}

void Engine::ShowDebugInfo()
{
    if(!this->debug)
        return;

    static int x;
    static int y;
    static std::stringstream ss;
    static SDL_Surface* mouse   = NULL;
    static SDL_Surface* player  = NULL;
    static TTF_Font* font       = this->Settings.GetUIFont();

    GetMousePosition(x, y);

    ss << "(" << x << ", " << y << ")";

    mouse = render_text(font, ss.str(), NULL,
        create_color(WHITE), CREATE_SURFACE | TRANSPARENT_BG);

    ss.str(string());
    ss << "(" << this->Player.GetX() << ", " << this->Player.GetY() << ")";

    player = render_text(font, ss.str(), NULL,
        create_color(WHITE), CREATE_SURFACE | TRANSPARENT_BG);

    this->Screen.Blit(mouse, x, y);
    this->Screen.Blit(player,
        (int)this->Player.GetX() - this->Player.GetCollisionBoundaries()->w,
        (int)this->Player.GetY() - this->Player.GetCollisionBoundaries()->h);

    SDL_FreeSurface(mouse);
    SDL_FreeSurface(player);
    ss.str(string());

    mouse   = NULL;
    player  = NULL;
}

void Engine::UpdateAll()
{
    /* For some reason, if the window is moved, and
     * also at random occurences in the game, the player
     * sprite will move way way way offscreen, so we
     * must bring him back!
     */
    if(this->Player.GetX() > this->Screen.GetWidth())
        this->Player.Move_Force(this->Screen.GetWidth() / 2, this->Screen.GetHeight() / 2);

    if(this->Player.GetY() > this->Screen.GetHeight())
        this->Player.Move_Force(this->Screen.GetWidth() / 2, this->Screen.GetHeight() / 2);

    /* Play the next song if the previous one has ended */
    if(Mix_PlayingMusic() == 0)
    {
        Mix_PlayMusic(this->Settings.GetGameplayMusic(), 0);
        Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    }

    /* Now we clear the screen, update the player,
     * show some info if debugging, show score, 
     * delete off-screen sprites, update locations,
     * do some AI, etc.
     */
    this->Screen.ClearScreen();
    if(!this->Player.IsDead())
        this->Player.Blit();
    this->Particles.UpdateParticles();
    this->ShowDebugInfo();

    /* The font style for the UI */
    static TTF_Font* ui = this->Settings.GetUIFont();

    /* Show the score on the screen */
    static std::stringstream ss;
    ss << "SCORE: " << this->score;

    SDL_Surface* score_surf = render_text(ui,
        ss.str(), NULL, UI_COLOR, CREATE_SURFACE|TRANSPARENT_BG);

    this->Screen.Blit(score_surf, 0, 0);

    ss.str(string());
    ss << "HIGHSCORE: " << this->high_score;

    SDL_Surface* hs_surf = render_text(ui, ss.str(),
        NULL, UI_COLOR, CREATE_SURFACE | TRANSPARENT_BG);

    this->Screen.Blit(hs_surf, this->Screen.GetWidth() - get_text_width(ui, ss.str()), 0);

    ss.str(string());
    ss << "Lives: " << this->Player.GetLives();

    SDL_Surface* lives = render_text(ui, ss.str(),
        NULL, UI_COLOR, CREATE_SURFACE | TRANSPARENT_BG);

    this->Screen.Blit(lives,
        this->Screen.GetWidth() - get_text_width(ui, ss.str()),
        this->Screen.GetHeight() - get_text_height(ui, ss.str()));

    ss.str(string());

    /* Get rid of any bullets that have gone off-screen,
     * and any bullets not offscreen, update. */
    std::vector<AllBullets::iterator> shots_to_remove;

    for(AllBullets::iterator i = this->Shots.begin();
        i != this->Shots.end(); i++)
    {
        if((*i)->GetX() > this->Screen.GetWidth() || (*i)->GetX() < 0 ||
           (*i)->GetY() > this->Screen.GetHeight() || (*i)->GetY() < 0)
        {
            shots_to_remove.push_back(i);
        }
        else
        {
            (*i)->Update();
            (*i)->Blit();
        }
    }

    /* Update each enemy */
    for(AllEnemies::iterator i = this->Enemies.begin();
        i != this->Enemies.end(); i++)
    {
        (*i)->Update();
    }

    /* Remove each shot */
    for(unsigned int i = 0; i < shots_to_remove.size(); i++)
    {
        delete (*shots_to_remove[i]);
        this->Shots.erase(shots_to_remove[i]);
    }

    shots_to_remove.clear();

    /* Update the entire display */
    this->Screen.Update();

    SDL_FreeSurface(score_surf);
    SDL_FreeSurface(hs_surf);
    SDL_FreeSurface(lives);
}
