#ifndef ENTITY_H
#define ENTITY_H

#include <cstdlib>
#include <ctime>

#include "Display.h"
#include "SDL_Helper.h"
#include "Timer.h"

class BaseObject
{
    /* A base object class with various functionality
     * such as loading an image, setting up surfaces,
     * moving the object, detecting collisions, etc.
     */

public:
    BaseObject(Display* display, Timer* timer);
    BaseObject(Display* display, Timer* timer, const int x, const int y);
    BaseObject(Display* display, Timer* timer, const float x, const float y);
    virtual ~BaseObject();

    /* Load or set a surface as the image for the object,
     * and clean it up whenever you want.
     */
    void SetEntity(SDL_Surface* entity);
    void LoadEntity(const char* filename);
    void FreeEntity();

    /* Fill up the player sprite with some color at some
     * rectangle or coordinates.
     */
    void FillRect(const int x, const int y, const int w, const int h,
            const SDL_Color& fill_color);
    void FillRect(const SDL_Rect& rect, const SDL_Color& fill_color);

    /* Move the object to a location, or move it in a
     * certain direction at a certain speed.*/
    void Move(const int x, const int y, float time_elapsed);
    void Move(const float x, const float y, float time_elapsed);
    void Move_Rate(const int dx, const int dy);
    void Move_Rate(const float dx, const float dy);

    /* Update the object on to the display */
    void Blit();

    /* Set up the collision box to calculate collision with
     * other game objects.
     */
    void SetCollisionBoundaries(const int x, const int y, const int w,
            const int h);
    void SetCollisionBoundaries(const SDL_Rect& boundaries);
    void SetCollisionBoundaries(const SDL_Rect* boundaries);

    /* Detect a collision with another object in the game world */
    bool DetectCollision(BaseObject* obj) const;
    bool DetectCollision(const SDL_Rect& rect) const;
    bool DetectCollision(const SDL_Rect* rect) const;

    /* Set up the movement boundaries, such as not being able
     * to move offscreen.
     */
    void SetMovementBoundaries_Max(const int x, const int y);
    void SetMovementBoundaries_Min(const int x, const int y);

    /* Get the coordinates of current location, current collision
     * box settings, and the current sprite.
     */
    float  GetX() const;
    float  GetY() const;

    const SDL_Surface* GetEntity() const;
    const SDL_Rect*    GetCollisionBoundaries() const;

protected:
    Display*        display;
    Timer*          timer;
    SDL_Surface*    main_entity;
    SDL_Rect        collision_box;

    float           x;
    float           y;

    int             max_x;
    int             max_y;
    int             min_x;
    int             min_y;
};

#endif // ENTITY_H



