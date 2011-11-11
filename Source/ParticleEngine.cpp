#include "ParticleEngine.h"

CBit::CBit(CDisplay& m_Screen, CTimer& m_timer, const int x, const int y,
    const int m_dx, const int m_dy,
    const int m_lifetime): CBaseObject(m_Screen, m_timer, x, y)
{
    this->dx = m_dx;
    this->dy = m_dy;

    /* The time this particle will last (ms)*/
    this->lifetime = m_lifetime;

    this->SetEntity(create_surface(5, 5, create_color(YELLOW)));
}

CBit::CBit(CDisplay& Screen, CTimer& timer, const int x, const int y, const int m_dx, const int m_dy, const SDL_Color& color):
    CBaseObject(Screen, timer, x, y)
{
    this->dx = m_dx;
    this->dy = m_dy;

    this->lifetime = 25 + rand() % 25;

    this->SetEntity(create_surface(5, 5, color));
}

CBit::CBit(CDisplay& Screen, CTimer& timer, const int x, const int y,
    const int m_dx, const int m_dy,
    const int m_lifetime, const SDL_Color& color): CBaseObject(Screen, timer, x, y)
{
    this->dx = m_dx;
    this->dy = m_dy;

    /* The time this particle will last (ms)*/
    this->lifetime = m_lifetime;

    this->SetEntity(create_surface(5, 5, color));
}

CBit::~CBit()
{
}

void CBit::Update()
{
    if(this->lifetime > 0)
    {
        this->Move_Rate(this->dx, this->dy);
        this->lifetime--;
        this->Blit();
    }
}

bool CBit::IsAlive()
{
    return (this->lifetime > 0);
}

CParticleEngine::CParticleEngine(CDisplay& m_Screen, CTimer& m_Timer):
    Screen(m_Screen), Timer(m_Timer)
{
}

CParticleEngine::~CParticleEngine()
{
    this->particles.clear();
    this->trail.clear();
}

/* Not used EMP explosion 
void CParticleEngine::GenerateEMP(const int m_x, const int m_y)
{
    this->show_emp = true;
    this->show_emp_time = EMP_TIME;
}
*/

void CParticleEngine::ExplodeObject(CBaseObject*obj)
{
    /* We will spawn anywhere from 10-30 "bits" that
     * are left-over from the destroyed enemy.
     */
    const int bits = 10 + (rand() % 20);
    int dx, dy;

    /* Obtain pixel color for explosions */
    Uint8 r, g, b;
    SDL_GetRGB(get_pixel(obj->GetEntity(), 0, 0), 
        obj->GetEntity()->format, &r, &g, &b);

    SDL_Color color = {r, g, b};

    for(short i = 0; i < bits; i++)
    {
        /* We create a particle with a random velocity. */
        if(rand() % 2 == 0) // Make veloctiy negative
            dx = -(5 + (rand() % 5));
        else
            dx = (5 + (rand() % 5));

        if(rand() % 2 == 0) // Again for y direction
            dy = -(5 + (rand() % 5));
        else
            dy = (5 + (rand() % 5));

        this->particles.push_back(new CBit(this->Screen, this->Timer,
            (int)obj->GetX(), (int)obj->GetY(), dx, dy,
            25 + rand() % 25, color));
    }
}

void CParticleEngine::ExplodePlayer(const int x, const int y)
{
    /* We will spawn anywhere from 10-30 "bits" that
     * are left-over from the destroyed player.
     */
    const int bits = 50 + (rand() % 50);
    int dx, dy;

    SDL_Color green = create_color(GREEN);

    for(short i = 0; i < bits; i++)
    {
        /* We create a particle with a random velocity. */
        if(rand() % 2 == 0) // Make veloctiy negative
            dx = -(5 + (rand() % 5));
        else
            dx = (5 + (rand() % 5));

        if(rand() % 2 == 0) // Again for y direction
            dy = -(5 + (rand() % 5));
        else
            dy = (5 + (rand() % 5));

        this->particles.push_back(new CBit(
            this->Screen, this->Timer,
            x, y, dx, dy,
            50 + rand() % 50,
            green));
    }
}

void CParticleEngine::AddPlayerTrail(CPlayer& player, const int dx, const int dy)
{
    SDL_Color green = create_color(GREEN);

    if(dx == 0 && dy > 0)   // Going north
    {
        this->trail.push_back(new CBit(this->Screen, this->Timer,
            (int)player.GetX() + rand() % player.GetCollisionBoundaries()->w,
            (int)player.GetY() - player.GetCollisionBoundaries()->h - rand() % 100,
            dx, dy, 20, green));
    }
    else if(dx > 0 && dy == 0)   // Player is going east
    {
        this->trail.push_back(new CBit(this->Screen, this->Timer,
            (int)player.GetX() - rand() % 100,
            (int)player.GetY() + rand() % player.GetCollisionBoundaries()->h,
            dx, dy, 20, green));
    }
    else if(dx == 0 && dy < 0)  // Going south
    {
        this->trail.push_back(new CBit(this->Screen, this->Timer,
            (int)player.GetX() + rand() % player.GetCollisionBoundaries()->w,
            (int)player.GetY() + rand() % 100,
            dx, dy, 20, green));
    }
    else if(dx < 0 && dy == 0)   // Player is going west
    {
        this->trail.push_back(new CBit(this->Screen, this->Timer,
            (int)player.GetX() + rand() % 100,
            (int)player.GetY() + rand() % player.GetCollisionBoundaries()->h,
            dx, dy, 20, green));
    }
    else if(dx > 0 && dy < 0)   // Player is going north-east
    {
        /* Equation of the line is:
         * y - player.y = -1(x - player.x)
         * y = -x + (player.x + player.y)
         */
        int x = (int)player.GetX() - (rand() % 75);
        int y = -x + ((int)player.GetX() + (int)player.GetY());

        if(rand() % 2 == 1)
            y += rand() % 20;
        else
            y -= rand() % 20;

        this->trail.push_back(new CBit(this->Screen, this->Timer,
            x, y, dx, dy, 20, green));
    }
    else if(dx > 0 && dy > 0)   // Player is going south-east
    {
        /* Equation of the line is:
         * y - player.y = -1(x - player.x)
         * y = -x + (player.x + player.y)
         * But voodoo magic is neccessary to do it right,
         * we get the distance from the ship, double it,
         * and get the right vector.
         */
        int x = (int)player.GetX() - (rand() % 75);
        int y = -x + ((int)player.GetX() + (int)player.GetY());
        y -= (y - (int)player.GetY()) * 2;

        if(rand() % 2 == 1)
            y += rand() % 20;
        else
            y -= rand() % 20;

        this->trail.push_back(new CBit(this->Screen, this->Timer,
            x, y, dx, dy, 20, green));
    }
    else if(dx < 0 && dy > 0)  // Going south-west
    {
        int x = (int)player.GetX() + (rand() % 75);
        int y = -x + ((int)player.GetX() + (int)player.GetY());

        if(rand() % 2 == 1)
            y += rand() % 20;
        else
            y -= rand() % 20;

        this->trail.push_back(new CBit(this->Screen, this->Timer,
            x, y, dx, dy, 20, green));
    }
    else if(dx < 0 && dy < 0)   // Going north-west
    {
        int x = ((int)player.GetX() + player.GetCollisionBoundaries()->w) + (rand() % 75);
        int y = -x + ((int)player.GetX() + (int)player.GetY()) - player.GetCollisionBoundaries()->h;
        y -= (y - (int)player.GetY()) * 2;

        if(rand() % 2 == 1)
            y += rand() % 20;
        else
            y -= rand() % 20;

        this->trail.push_back(new CBit(this->Screen, this->Timer,
            x, y, dx, dy, 20, green));
    }
}

void CParticleEngine::UpdateParticles()
{
    std::vector<std::list<CBit*>::iterator> remover1;
    std::vector<std::list<CBit*>::iterator> remover2;

    /* Part of EMP explosion
     * this->Screen.Blit(emp, 0, 0);
     */

    for(std::list<CBit*>::iterator i = this->particles.begin();
        i != this->particles.end(); i++)
    {
        if((*i)->IsAlive())
            (*i)->Update();
        else
            remover1.push_back(i);
    }

    for(std::list<CBit*>::iterator i = this->trail.begin();
        i != this->trail.end(); i++)
    {
        if((*i)->IsAlive())
            (*i)->Update();
        else
            remover2.push_back(i);
    }

    for(unsigned int i = 0; i < remover1.size(); i++)
    {
        delete *remover1[i];
        this->particles.erase(remover1[i]);
    }
    for(unsigned int i = 0; i < remover2.size(); i++)
    {
        delete *remover2[i];
        this->trail.erase(remover2[i]);
    }

    /* NON-FUNCTIONAL EMP "EXPLOSION"
    if(this->show_emp)
    {
        /* This is not used!
         * Circle formula is:
         * (x-h)^2+(y-k)^2=r^2
         * Solving for y we get:
         * (y-k)^2 = r^2 - (x-h)^2
         * y = +/- sqrt(r^2 - (x-h)^2) + k
         * Then we generate a blue pixel at each point
         * along the half-circle equations.
         *
        static float radius = 0.0f;

        if(this->show_emp_time > 0)
        {
            for(float x = this->Screen.GetWidth() / 2.0f; (int)x < this->Screen.GetWidth(); x += 5.0f)
            {
                float y = sqrt(pow(radius, 2) - pow(x - this->Screen.GetWidth() / 2, 2)) + (this->Screen.GetHeight / 2.0f);

                this->Screen.Blit(emp, create_surface(10, 10, create_color(BLUE)), x,                               (int)y);
                this->Screen.Blit(emp, create_surface(10, 10, create_color(BLUE)), x,                               this->Screen.GetHeight - (int)y);
                this->Screen.Blit(emp, create_surface(10, 10, create_color(BLUE)), this->Screen.GetWidth() - (int)x,    (int)y);
                this->Screen.Blit(emp, create_surface(10, 10, create_color(BLUE)), this->Screen.GetWidth() - (int)x,    this->Screen.GetHeight - (int)y); 
            }
        }
        else
        {
            radius += 5.0f;
            this->show_emp_time = EMP_TIME;
        }

        if((int)radius >= this->Screen.GetWidth())
        {
            radius = 0.0f;
            this->show_emp = false;
        }

        this->show_emp_time--;
    }
    */
    remover1.clear();
    remover2.clear();
}