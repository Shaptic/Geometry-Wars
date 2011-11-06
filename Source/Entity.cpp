#include "Entity.h"

BaseObject::BaseObject(Display* display, Timer* timer)
{
    /* Set member values, including location,
     * boundaries, and the display class.
     */
    this->display   = display;
    this->timer     = timer;
    this->x         = 0.0f;
    this->y         = 0.0f;

    this->max_x     = INT_MAX;
    this->max_y     = INT_MAX;
    this->min_x     = INT_MIN;
    this->min_y     = INT_MIN;
}

BaseObject::BaseObject(Display* display, Timer* timer, const int x, const int y)
{
    /* Set member values, including location,
     * boundaries, and the display class.
     */
    this->display   = display;
    this->timer     = timer;
    this->x         = x * 1.0f;
    this->y         = y * 1.0f;

    this->max_x     = INT_MAX;
    this->max_y     = INT_MAX;
    this->min_x     = INT_MIN;
    this->min_y     = INT_MIN;
}

BaseObject::BaseObject(Display* display, Timer* timer, const float x, const float y)
{
    /* Set member values, including location,
     * boundaries, and the display class.
     */
    this->display   = display;
    this->timer     = timer;
    this->x         = x;
    this->y         = y;

    this->max_x     = INT_MAX;
    this->max_y     = INT_MAX;
    this->min_x     = INT_MIN;
    this->min_y     = INT_MIN;
}

BaseObject::~BaseObject()
{
    /* Class destructor (virtual) will
     * free the entity if it isn't free
     * already.
     */
    if(this->main_entity != NULL)
        SDL_FreeSurface(this->main_entity);
}

void BaseObject::SetEntity(SDL_Surface* entity)
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

void BaseObject::LoadEntity(const char* filename)
{
    /* Here we can load an image using the LoadImage()
     * function found in Display.h.
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

void BaseObject::FreeEntity()
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

void BaseObject::FillRect(const int x, const int y, const int w, const int h,
            const SDL_Color& fill_color)
{
    /* This will fill a portion of the main entity
     * with a color.
     */
    if(this->main_entity != NULL)
        fill_rect(this->main_entity, create_rect(x, y, w, h), fill_color);
}

void BaseObject::FillRect(const SDL_Rect& rect, const SDL_Color& fill_color)
{
    /* Same as other BaseObject::FillRect() */
    fill_rect(this->main_entity, (SDL_Rect)rect, fill_color);
}

void BaseObject::Move(const int x, const int y, float delta)
{
    /* Move the entity to a certain location
     * in the game world.
     */
    this->Move(x * 1.0f, y * 1.0f, delta);
}

void BaseObject::Move(const float x, const float y, float delta)
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
     * A frame every 20ms is good, so if the change
     * is more than 20ms, we multiply the change by
     * the excess.
     * We update the collision box as well.
     */
    if(delta > 20)
        delta /= 20.0f;
    else
        delta = 1.0f;

    this->x = x * delta;
    this->y = y * delta;

    this->collision_box.x = (int)x;
    this->collision_box.y = (int)y;
}

void BaseObject::Move_Rate(const int dx, const int dy)
{
    /* Rather than moving directly to an (x, y) coordinate,
     * we can move it in a certain direction with a certain
     * speed.
     */
    this->Move(this->GetX() + dx, this->GetY() + dy, this->timer->GetTicks());
}

void BaseObject::Move_Rate(const float dx, const float dy)
{
    /* Rather than moving directly to an (x, y) coordinate,
     * we can move it in a certain direction with a certain
     * speed.
     */
    this->Move(this->GetX() + dx, this->GetY() + dy, this->timer->GetTicks());
}


void BaseObject::Move_Force(const int x, const int y)
{
    /* Move the entity to a certain location
     * in the game world.
     */
    this->Move_Force(x * 1.0f, y * 1.0f);
}

void BaseObject::Move_Force(const float x, const float y)
{
    /* Force the entity to a certain location
     * in the game world.
     */
    this->x = x;
    this->y = y;

    this->collision_box.x = (int)x;
    this->collision_box.y = (int)y;
}


void BaseObject::Blit()
{
    /* Update the entity on the screen. */
    if(this->main_entity != NULL)
        this->display->Blit(this->main_entity, (int)this->x, (int)this->y);
}

void BaseObject::SetCollisionBoundaries(const int x, const int y,
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

void BaseObject::SetCollisionBoundaries(const SDL_Rect& boundaries)
{
    /* Overloaded method. */
    this->collision_box = boundaries;
}

void BaseObject::SetCollisionBoundaries(const SDL_Rect* boundaries)
{
    /* Overloaded again. */
    this->collision_box.x = boundaries->x;
    this->collision_box.y = boundaries->y;
    this->collision_box.w = boundaries->w;
    this->collision_box.h = boundaries->h;
}

bool BaseObject::DetectCollision(BaseObject* obj) const
{
    /* Detect whether or not our entity has collided
     * with another entity.
     */
    return detect_collision(this->GetCollisionBoundaries(), obj->GetCollisionBoundaries());
}

bool BaseObject::DetectCollision(const SDL_Rect& rect) const
{
    /* Detect whether or not our entity has collided
     * with a rectangle representing a place in the game
     * world.
     */
    return detect_collision(this->GetCollisionBoundaries(), &rect);
}

bool BaseObject::DetectCollision(const SDL_Rect* rect) const
{
    /* Overloaded for an SDL_Rect*. */
    return detect_collision(this->GetCollisionBoundaries(), rect);
}

void BaseObject::SetMovementBoundaries_Max(const int x, const int y)
{
    /* Set maximum movement boundaries */
    this->max_x = x;
    this->max_y = y;
}

void BaseObject::SetMovementBoundaries_Min(const int x, const int y)
{
    /* Set minimum movement boundaries */
    this->min_x = x;
    this->min_y = y;
}

/* Just a bunch of get() methods to 
 * retrieve information about our entity.
 */

float BaseObject::GetX() const
{
    return this->x;
}

float BaseObject::GetY() const
{
    return this->y;
}

const SDL_Surface* BaseObject::GetEntity() const
{
    return this->main_entity;
}

const SDL_Rect* BaseObject::GetCollisionBoundaries() const
{
    return &this->collision_box;
}
