#include "Engine.h"

Engine::Engine()
{
    srand((unsigned int)time(NULL));

    this->Screen        = new Display(800, 600);
    this->MainMenu      = new Menu(this->Screen);
    this->Fps           = new Timer();

    /* Set up the player image and limitations. */
    this->Player        = new CPlayer(this->Screen, this->Fps);

    /* Set up our main menu */
    this->MainMenu->AddMenuOption("Play Geometry Wars!", BTN_ACTION, ACT_PLAY);
    this->MainMenu->AddMenuOption("Quit", BTN_ACTION, ACT_QUIT);

    this->debug         = true;
    this->quit          = false;
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

    this->AddEnemy();

    while(!this->quit)
    {
        if(frame % 60 == 0)
        {
            this->AddEnemy();
            frame = 1;
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

        if(IsPressed(SDL_BUTTON_LEFT) && this->Player->CanShoot())
            this->Shoot();

        /* Move player accordingly */
        this->Player->Move_Rate(dx, dy);
        
        /* Check collisions of bullets with enemies,
         * enemies with player, etc.
         */
        this->CheckCollisions(this->enemy_iters, this->shot_iters);
        this->RemoveEnemies(this->enemy_iters);
        this->RemoveShots(this->shot_iters);
        this->enemy_iters.clear();
        this->shot_iters.clear();

        /* Place images on the screen and 
         * update the display.
         */
        this->UpdateAll();
        this->Screen->Update();
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
        }
    }

    if(IsDown(SDLK_q))
        this->AddEnemy();
}

void Engine::Shoot()
{
    /* We need to calculate the slope that the bullet needs
     * to have in order to be going in the direction of the 
     * mouse pointer.
     */
    
    double angle_iter       = SHOT_ANGLE;
    
    for(short i = 0; i <= 3; i++)
    {
        if(i % 2 == 0)
            angle_iter = -(i/2) * SHOT_ANGLE;
        else
            angle_iter = (i/2) * SHOT_ANGLE;

        Bullet* shot = new Bullet(
            this->Screen, this->Fps,
            (int)this->Player->GetX() + this->Player->GetCollisionBoundaries()->w / 2,
            (int)this->Player->GetY() + this->Player->GetCollisionBoundaries()->h / 2,
            angle_iter);

        shot->SetEntity(create_surface(5, 5, create_color(BLUE)));
        this->Shots.push_back(shot);
    }
}

void Engine::CheckCollisions(std::vector<AllEnemies::iterator>& enemy_iters,
    std::vector<AllBullets::iterator>& shot_iters)
{
    for(AllEnemies::iterator i = this->Enemies.begin();
        i != this->Enemies.end(); i++)
    {
        for(AllBullets::iterator j = this->Shots.begin();
            j != this->Shots.end(); j++)
        {
            if((*i)->DetectCollision((*j)))
            {
                enemy_iters.push_back(i);
                shot_iters.push_back(j);
                return;
            }
        }

        if(this->Player->DetectCollision((*i)) && (!this->debug))
        {
            this->quit = true;
            return;
        }
    }
}

void Engine::RemoveEnemies(std::vector<AllEnemies::iterator>& iters)
{
    for(unsigned int i = 0; i < iters.size(); i++)
    {
        delete (*iters[i]);
        this->Enemies.erase(iters[i]);
    }
}

void Engine::RemoveShots(std::vector<AllBullets::iterator>& iters)
{
    for(unsigned int i = 0; i < iters.size(); i++)
    {
        delete (*iters[i]);
        this->Shots.erase(iters[i]);
    }
}

void Engine::AddEnemy()
{
    Enemy* enemy = new Enemy(this->Screen, this->Fps, this->Player);
    enemy->SetEntity(create_surface(25, 25, create_color(YELLOW)));
    this->Enemies.push_back(enemy);
}

void Engine::UpdateAll()
{
    this->Screen->ClearScreen();
    this->Player->Blit();

    if(debug)
    {
        static TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\Arial.ttf", 16);
        static SDL_Surface* mouse = NULL;
        static std::stringstream ss;
        static int x, y;

        GetMousePosition(x, y);
        ss << "(" << x << ", " << y << ")";

        mouse = render_text(font, ss.str(), NULL,
            create_color(WHITE), CREATE_SURFACE | TRANSPARENT_BG);

        this->Screen->Blit(mouse, x, y);

        ss.str(string());
    }

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

    for(AllEnemies::iterator i = this->Enemies.begin();
        i != this->Enemies.end(); i++)
    {
        (*i)->Update();
    }

    for(unsigned int i = 0; i < shots_to_remove.size(); i++)
    {
        delete (*shots_to_remove[i]);
        this->Shots.erase(shots_to_remove[i]);
    }

    shots_to_remove.resize(0);
}
