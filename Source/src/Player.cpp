#include "Player.h"

CPlayer::CPlayer(CDisplay& m_Display, CTimer& m_Timer, CSettingsManager& Settings):
CEntity(m_Display, m_Timer)
{
    std::string tmp = Settings.GetValueAt("PlayerSpeed");
    if(tmp.empty())
        this->speed = DEF_PLAYER_SPEED;
    else
        this->speed = atoi(tmp.c_str());

    this->SetCollisionBoundaries(0, 0, 16, 16);

    this->damage     = 50;
    this->bullets    = 50;
    this->shot_delay = 0;
    this->lives      = 3;
    this->NormalShot = Mix_LoadWAV("Audio"FN_SLASH"NormalWeaponFire.wav");
    
    if(!this->NormalShot)
        handleError(Mix_GetError());

    Mix_VolumeChunk(this->NormalShot, 50);
}

CPlayer::~CPlayer()
{
    this->PlayerPowerUps.clear();
}

void CPlayer::IncreaseAmmo(const int amount)
{
    this->bullets += amount;
}

void CPlayer::AddPowerUp(const PowerUp& powerup)
{
    this->PlayerPowerUps.push_back(powerup);
}

void CPlayer::Shoot(CBullets& Bullets)
{
    if(this->CanShoot())
    {
        Mix_PlayChannel(-1, this->NormalShot, 0);

        if(this->CheckPowerUp(PowerUp::BURST_FIRE))
            this->shot_delay = ACCL_SHOT_DELAY;
        else
            this->shot_delay = DEF_SHOT_DELAY;

        CBullet* Bullet = new CBullet(
            this->Display, this->Timer,
            (int)this->GetX() + this->GetCollisionBoundaries()->w / 2,
            (int)this->GetY() + this->GetCollisionBoundaries()->h / 2);

        Bullet->LoadEntity("Images"FN_SLASH"Player_Shot.png");

        Bullets.push_back(Bullet);
        this->bullets--;
    }
}

bool CPlayer::CanShoot()
{
    if(this->shot_delay <= 0 && this->bullets > 0)
        return true;
    else
        return false;
}

bool CPlayer::CheckPowerUp(const PowerUp::ABILITY ability)
{
    for(std::list<PowerUp>::iterator i = this->PlayerPowerUps.begin();
        i != this->PlayerPowerUps.end(); i++)
    {
        if((*i).ability == ability)
            return true;
    }

    return false;
}

void CPlayer::Update()
{
    for(std::list<PowerUp>::iterator i = this->PlayerPowerUps.begin();
        i != this->PlayerPowerUps.end(); i++)
    {
        if((*i).duration == 0)
            this->PlayerPowerUps.erase(i);
        else
            (*i).duration--;
    }

    this->shot_delay--;

    if(this->lives > 0)
        this->Blit();
}

void CPlayer::Reset()
{
    this->PlayerPowerUps.clear();
    this->bullets = 50;
    this->shot_delay = 0;
    this->lives = 1;
    this->speed = DEF_PLAYER_SPEED;

    this->Move_Force(this->Display.GetWidth() / 2, this->Display.GetHeight() / 2);
}