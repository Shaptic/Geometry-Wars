#include "Entity.h"

CEntity::CEntity(CDisplay& display, CTimer& timer):
    Display(display), Timer(timer), x(0.0f), y(0.0f), max_x(INT_MAX),
        max_y(INT_MAX), min_x(INT_MIN), min_y(INT_MIN)
{
    /* Set member values, including location,
     * boundaries, and the display class.
     */
}

CEntity::CEntity(CDisplay& display, CTimer& timer, const float m_x, const float m_y):
    Display(display), Timer(timer), x(m_x), y(m_y), max_x(INT_MAX),
        max_y(INT_MAX), min_x(INT_MIN), min_y(INT_MIN)
{
    /* Set member values, including location,
     * boundaries, and the display class.
     */
}

CEntity::~CEntity()
{
    /* Class destructor (virtual) will
     * free the entity if it isn't free
     * already.
     */
    if(this->main_entity != NULL)
        SDL_FreeSurface(this->main_entity);
}

void CEntity::SetEntity(SDL_Surface* entity)
{
    /* Here we can set the entity to a surface,
     * be it an image, or custom shape, we previously
     * created.
     */
    this->main_entity = entity;

    if(this->main_entity == NULL)
        handleError(SDL_GetError());

    /* We set the collision boundaries of our 
     * entity to those of the entity given 
     * to us.
     */
    this->SetCollisionBoundaries(
        create_rect((int)this->x, (int)this->y,
        this->main_entity->clip_rect.w,
        this->main_entity->clip_rect.h));
}

void CEntity::LoadEntity(const char* filename)
{
    /* Here we can load an image using the LoadImage()
     * function found in CDisplay.h.
     */
    this->main_entity = LoadImage_Alpha(filename);

    if(this->main_entity == NULL)
        handleError(SDL_GetError());

    /* We set the collision boundaries of our 
     * entity to those of the entity given 
     * to us.
     */
    this->SetCollisionBoundaries(
        create_rect((int)this->x, (int)this->y,
        this->main_entity->clip_rect.w,
        this->main_entity->clip_rect.h));
}

void CEntity::FreeEntity()
{
    /* Once we are done using the SDL_Surface*,
     * we need to free memory. Remember, when you
     * new, please delete.
     */
    if(this->main_entity != NULL)
    {
        SDL_FreeSurface(this->main_entity);
        this->main_entity = NULL;
    }
}

void CEntity::FillRect(const int x, const int y, const int w, const int h,
            const SDL_Color& fill_color)
{
    /* This will fill a portion of the main entity
     * with a color.
     */
    if(this->main_entity != NULL)
        fill_rect(this->main_entity, create_rect(x, y, w, h), fill_color);
}

void CEntity::FillRect(const SDL_Rect& rect, const SDL_Color& fill_color)
{
    /* Same as other CEntity::FillRect() */
    fill_rect(this->main_entity, (SDL_Rect)rect, fill_color);
}

void CEntity::Move(const int x, const int y, float delta)
{
    /* Move the entity to a certain location
     * in the game world.
     */
    this->Move(x * 1.0f, y * 1.0f, delta);
}

void CEntity::Move(const float x, const float y, float delta)
{
    /* Move the entity to a certain location
     * in the game world.
     */

    /* Are we in our boundaries?
     * If not, do nothing.
     */
    if((int)x >= this->max_x || (int)y >= this->max_y)
        return;

    if((int)x <= this->min_x || (int)y <= this->min_y)
        return;

    /* If we are, set the values accordingly.
     * First we calculate how much to move based
     * on the difference in time between the frames
     * as well as the preset framerate.
     * Then we update the position accordingly.
     * We update the collision box as well.
     */
    if(delta > 1000 / this->Timer.GetFrameRate())
        delta /= 1.0f * this->Timer.GetFrameRate();
    else
        delta = 1.0f;

    this->x = x * delta;
    this->y = y * delta;

    this->collision_box.x = (int)x;
    this->collision_box.y = (int)y;
}

void CEntity::Move_Rate(const int dx, const int dy)
{
    /* Rather than moving directly to an (x, y) coordinate,
     * we can move it in a certain direction with a certain
     * speed.
     */
    this->Move(this->GetX() + dx, this->GetY() + dy, this->Timer.GetTicks());
}

void CEntity::Move_Rate(const float dx, const float dy)
{
    /* Rather than moving directly to an (x, y) coordinate,
     * we can move it in a certain direction with a certain
     * speed.
     */
    this->Move(this->GetX() + dx, this->GetY() + dy, this->Timer.GetTicks());
}


void CEntity::Move_Force(const int x, const int y)
{
    /* Move the entity to a certain location
     * in the game world.
     */
    this->Move_Force(x * 1.0f, y * 1.0f);
}

void CEntity::Move_Force(const float x, const float y)
{
    /* Force the entity to a certain location
     * in the game world.
     */
    this->x = x;
    this->y = y;

    this->collision_box.x = (int)x;
    this->collision_box.y = (int)y;
}


void CEntity::Blit()
{
    /* Update the entity on the screen. */
    if(this->main_entity != NULL)
        this->Display.Blit(this->main_entity, (int)this->x, (int)this->y);
}

void CEntity::SetCollisionBoundaries(const int x, const int y,
        const int w, const int h)
{
    /* Set up collision boundaries. This really
     * shouldn't be messed with unless you really know
     * what you are doing.
     */
    this->collision_box.x = x;
    this->collision_box.y = y;
    this->collision_box.w = w;
    this->collision_box.h = h;
}

void CEntity::SetCollisionBoundaries(const SDL_Rect& boundaries)
{
    /* Overloaded method. */
    this->collision_box = boundaries;
}

void CEntity::SetCollisionBoundaries(const SDL_Rect* boundaries)
{
    /* Overloaded again. */
    this->collision_box.x = boundaries->x;
    this->collision_box.y = boundaries->y;
    this->collision_box.w = boundaries->w;
    this->collision_box.h = boundaries->h;
}

bool CEntity::DetectCollision(CEntity* obj) const
{
    /* Detect whether or not our entity has collided
     * with another entity.
     */
    return detect_collision(this->GetCollisionBoundaries(), obj->GetCollisionBoundaries());
}

bool CEntity::DetectCollision(const SDL_Rect& rect) const
{
    /* Detect whether or not our entity has collided
     * with a rectangle representing a place in the game
     * world.
     */
    return detect_collision(this->GetCollisionBoundaries(), &rect);
}

bool CEntity::DetectCollision(const SDL_Rect* rect) const
{
    /* Overloaded for an SDL_Rect*. */
    return detect_collision(this->GetCollisionBoundaries(), rect);
}

void CEntity::SetMovementBoundaries_Max(const int x, const int y)
{
    /* Set maximum movement boundaries */
    this->max_x = x;
    this->max_y = y;
}

void CEntity::SetMovementBoundaries_Min(const int x, const int y)
{
    /* Set minimum movement boundaries */
    this->min_x = x;
    this->min_y = y;
}

/* Just a bunch of get() methods to 
 * retrieve information about our entity.
 */

float CEntity::GetX() const
{
    return this->x;
}

float CEntity::GetY() const
{
    return this->y;
}

SDL_Surface* CEntity::GetEntity() const
{
    return this->main_entity;
}

const SDL_Rect* CEntity::GetCollisionBoundaries() const
{
    return &this->collision_box;
}
