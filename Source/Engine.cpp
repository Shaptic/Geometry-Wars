#include "Engine.h"

Engine::Engine()
{
    srand((unsigned int)time(NULL));

    if(TTF_Init() != 0)
        handleError(TTF_GetError());

    this->main_font     = TTF_OpenFont("C:\\Windows\\Fonts\\Arial.ttf", 24);

    this->Screen        = new Display(800, 600);
    this->MainMenu      = new Menu(this->Screen);
    this->Levels        = new LevelManager();
    this->Fps           = new Timer();
    this->Particles     = new ParticleEngine(this->Screen, this->Fps);

    /* Set up the player image and limitations. */
    this->Player        = new CPlayer(this->Screen, this->Fps);

    /* Set up our main menu */
    this->MainMenu->SetFont(this->main_font);
    this->MainMenu->SetStartCoordinates(this->Screen->width / 2, 200);
    this->MainMenu->SetHighLightColor(WHITE);
    this->MainMenu->AddMenuOption("GEOMETRY WARS!\nCreated by George Kudrayvtsev\n", BTN_TEXT, ACT_NONE);
    this->MainMenu->AddMenuOption("Play Geometry Wars!", BTN_ACTION, ACT_PLAY);
    this->MainMenu->AddMenuOption("Quit", BTN_ACTION, ACT_QUIT);

#ifdef _DEBUG
    this->debug         = true;
#else
    this->debug         = false;
#endif // _DEBUG

    this->quit          = false;
    this->score         = 0;
}

Engine::~Engine()
{
    delete this->Screen;
    delete this->MainMenu;
    delete this->Fps;
    delete this->Player;

    this->Enemies.clear();
    this->Shots.clear();
}

void Engine::Play()
{
    if(this->MainMenu->Run() == QUIT_ID)
        return;

    SDL_Delay(100);

    /* Some local game values, for handling 
     * mouse, keyboard, speeds, exiting, etc.
     */
    int     dx      = 0;
    int     dy      = 0;
    int     mouse_x = 0;
    int     mouse_y = 0;
    int     frame   = 0;

    GetMousePosition(mouse_x, mouse_y);

    while(!this->quit)
    {
        if(frame == INT_MAX)
            frame = 1;

        frame++;

        if(this->Levels->CanSpawn(frame))
        {
            this->AddEnemy();
        }

        this->Fps->Start();

        /* Handle our events */
        this->Events();

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
            this->Player->Shoot(this->Shots);

        this->Particles->AddPlayerTrail(this->Player, dx, dy);

        /* Move player accordingly */
        this->Player->Move_Rate(dx, dy);

        /* Check collisions of bullets with enemies,
         * enemies with player, etc.
         */
        this->CheckCollisions();
        this->RemoveEnemies();
        this->RemoveShots();
        this->enemy_iters.clear();
        this->shot_iters.clear();

        /* Place images on the screen and 
         * update the display.
         */
        this->UpdateAll();
    }
}

void Engine::Events()
{
    static SDL_Event evt;
    while(SDL_PollEvent(&evt))
    {
        if(evt.type == SDL_QUIT)
        {
            this->quit = true;
            return;
        }
        else if(evt.type == SDL_KEYDOWN)
        {
            if(evt.key.keysym.sym == SDLK_BACKQUOTE)
                this->debug = !this->debug;
            else if(evt.key.keysym.sym == SDLK_e && this->debug)
            {
                /* The "e" key forces an EMP explosion */
                Enemy emp(this->Screen, this->Fps, this->Player);
                emp.SetPowerUp(EMP);
                std::list<Enemy*> tmp;
                tmp.push_back(&emp);
                this->DestroyEnemy(tmp.begin());
            }
        }
    }

    if(IsDown(SDLK_q) && this->debug)
        this->AddEnemy();
}

void Engine::CheckCollisions()
{
    for(AllEnemies::iterator i = this->Enemies.begin();
        i != this->Enemies.end(); i++)
    {
        for(AllBullets::iterator j = this->Shots.begin();
            j != this->Shots.end(); j++)
        {
            if((*i)->DetectCollision((*j)))
            {
                this->DestroyEnemy(i);
                this->shot_iters.push_back(j);
                return;
            }
        }

        if(this->Player->DetectCollision((*i)))
        {
            /* We delete the enemy we collided with
             * no matter what, but only die if 
             * debug mode is off
             */
            if((*i)->GetPowerUp()->ability == EMP)
            {
                this->DestroyEnemy(i);
                return;
            }
            else
                this->DestroyEnemy(i);

            if(!this->debug)
            {
                this->quit = true;
                return;
            }
        }
    }
}

void Engine::DestroyEnemy(AllEnemies::iterator i)
{
    if((*i)->GetPowerUp()->ability == EMP)
    {
        /* EMP Destroys all enemies on the screen */

        /* First we update the level info */
        this->Levels->UpdateCurrentLevel(Enemies.size());

        /* Then the score, 5 * all the enemies destroyed */
        this->score += (this->Enemies.size() * 5);
        
        /* Then we create an explosion for every enemy on screen */
        for(AllEnemies::iterator j = this->Enemies.begin();
            j != this->Enemies.end(); j++)
        {
            this->Particles->ExplodeObject((int)(*j)->GetX(), (int)(*j)->GetY());
        }

        /* Then we remove the enemies */
        this->Enemies.clear();
        this->enemy_iters.clear();
    }
    else
    {
        this->Particles->ExplodeObject((int)(*i)->GetX(), (int)(*i)->GetY());
        this->score += 5;
        this->enemy_iters.push_back(i);
    }
}

void Engine::RemoveEnemies()
{
    this->Levels->UpdateCurrentLevel(this->enemy_iters.size());

    for(unsigned int i = 0; i < this->enemy_iters.size(); i++)
    {
        delete (*this->enemy_iters[i]);
        this->Enemies.erase(this->enemy_iters[i]);
    }
}

void Engine::RemoveShots()
{
    for(unsigned int i = 0; i < this->shot_iters.size(); i++)
    {
        delete (*this->shot_iters[i]);
        this->Shots.erase(this->shot_iters[i]);
    }
}

void Engine::AddEnemy()
{
    Enemy* enemy = new Enemy(this->Screen, this->Fps, this->Player);
    this->Enemies.push_back(enemy);
}

void Engine::ShowDebugInfo()
{
    if(!this->debug)
        return;

    static int x;
    static int y;
    static std::stringstream ss;
    static SDL_Surface* mouse = NULL;
    static SDL_Surface* player = NULL;
    static TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\Arial.ttf", 16);

    GetMousePosition(x, y);

    ss << "(" << x << ", " << y << ")";

    mouse = render_text(font, ss.str(), NULL,
        create_color(WHITE), CREATE_SURFACE | TRANSPARENT_BG);

    ss.str(string());
    ss << "(" << this->Player->GetX() << ", " << this->Player->GetY() << ")";

    player = render_text(font, ss.str(), NULL,
        create_color(WHITE), CREATE_SURFACE | TRANSPARENT_BG);

    this->Screen->Blit(mouse, x, y);
    this->Screen->Blit(player, (int)this->Player->GetX(), (int)this->Player->GetY());

    SDL_FreeSurface(mouse);
    SDL_FreeSurface(player);
    mouse = NULL;
    player = NULL;
    ss.str(string());
}

void Engine::UpdateAll()
{
    /* For some reason, if the window is moved, and
     * also at random occurences in the game, the player
     * sprite will move way way way offscreen, so we
     * must bring him back!
     */
    if(this->Player->GetX() > this->Screen->width)
        this->Player->Move_Force(this->Screen->width / 2, this->Screen->height / 2);

    if(this->Player->GetY() > this->Screen->height)
        this->Player->Move_Force(this->Screen->width / 2, this->Screen->height / 2);

    /* Now we clear the screen, update the player,
     * show some info if debugging, show score, 
     * delete off-screen sprites, update locations,
     * do some AI, etc.
     */
    this->Screen->ClearScreen();
    this->Player->Blit();
    this->Particles->UpdateParticles();
    this->ShowDebugInfo();

    /* Show the score on the screen */
    static std::stringstream ss;
    ss << "SCORE: " << this->score;

    SDL_Surface* score_surf = render_text(this->main_font,
        ss.str(), NULL, create_color(WHITE), CREATE_SURFACE|TRANSPARENT_BG);

    ss.str(string());

    this->Screen->Blit(score_surf, 0, 0);

    /* Get rid of any bullets that have gone off-screen,
     * and any bullets not offscreen, update. */
    std::vector<AllBullets::iterator> shots_to_remove;

    for(AllBullets::iterator i = this->Shots.begin();
        i != this->Shots.end(); i++)
    {
        if((*i)->GetX() > 800 || (*i)->GetX() < 0 ||
           (*i)->GetY() > 600 || (*i)->GetY() < 0)
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

    shots_to_remove.resize(0);

    /* Update the entire display */
    this->Screen->Update();
    SDL_FreeSurface(score_surf);
}
