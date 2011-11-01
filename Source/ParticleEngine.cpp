#include "ParticleEngine.h"

Bit::Bit(Display* Screen, Timer* timer, const int x, const int y, const int m_dx, const int m_dy):
    BaseObject(Screen, timer, x, y)
{
    this->dx = m_dx;
    this->dy = m_dy;

    /* The time this particle will last (ms)*/
    this->lifetime = 1 + rand() % 50;

    this->SetEntity(create_surface(2, 2, create_color(YELLOW)));
}

Bit::~Bit()
{
}

void Bit::Update()
{
    if(this->lifetime > 0)
    {
        this->Move_Rate(this->dx, this->dy);
        this->lifetime--;
        this->Blit();
    }
}

bool Bit::IsAlive()
{
    return (this->lifetime > 0);
}


ParticleEngine::ParticleEngine(Display* Screen, Timer* timer)
{
    this->Screen    = Screen;
    this->timer     = timer;
}

ParticleEngine::~ParticleEngine()
{
    this->particles.clear();
}

void ParticleEngine::ExplodeObject(const int x, const int y)
{
    /* We will spawn anywhere from 10-20 "bits" that
     * are left-over from the destroyed enemy.
     */
    const int bits = 10 + (rand() % 10);
    int dx, dy;

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

        this->particles.push_back(new Bit(this->Screen, this->timer, x, y, dx, dy));
    }
}

void ParticleEngine::UpdateParticles()
{
    for(unsigned int i = 0; i < this->particles.size(); i++)
    {
        if(this->particles[i] == NULL)
            continue;
            
        if(this->particles[i]->IsAlive())
        {
            this->particles[i]->Update();
        }
        else
        {
            delete this->particles[i];
            this->particles[i] = NULL;
        }
    }
}