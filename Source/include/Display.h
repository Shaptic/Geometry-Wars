/****************************************
 *             DISPLAY.H                *
 *                                      *
 * An essential base class for          *
 * simplifying all screen-related       *
 * operations such as drawing,          *
 * blitting, clearing the screen, etc.  *
 * Almost every other game-related      *
 * class will need a CDisplay& member   *
 * in the constructor.                  *
 ***************************************/
 
#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "Errors.h"

/* Values for color masking, do not modify! */
#if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
#define rmask            0xff000000
#define gmask            0x00ff0000
#define bmask            0x0000ff00
#define amask            0x000000ff
#else
#define rmask            0x000000ff
#define gmask            0x0000ff00
#define bmask            0x00ff0000
#define amask            0xff000000
#endif  // SDL_BYTEORDER

/* To make managing filenames a bit simpler,
 * we define the file separator.
 */
#ifdef WIN32
#define FN_SLASH "\\"
#else
#define FN_SLASH "/"
#endif // WIN32

/* This is the icon that will show up in
 * the application window. Set it to NULL
 * for no icon.
 */
#define APP_ICON                NULL

/* This is the caption that will be shown in
 * the title bar of your window.
 */
#define WINDOW_CAPTION          "Shape Wars"

/* The Windows API also has a function called 
 * LoadImage, so we ignore it completely in
 * order for ours to work.
 */
#ifdef WIN32
#undef LoadImage
#endif // WIN32

/* Easily load an image file, uses the SDL_image library. */
SDL_Surface* LoadImage(const std::string& filename);
SDL_Surface* LoadImage(const char* filename);
SDL_Surface* LoadImage_Alpha(const std::string& filename);
SDL_Surface* LoadImage_Alpha(const char* filename);

class CDisplay
{
public:
    /* Several layers of screen parameter
     * customization in the constructors.
     */
    CDisplay();
    CDisplay(const int width, const int height);
    CDisplay(const int width, const int height,
            const int bpp, const int flags);

    ~CDisplay();

    /* We call this to re-create a display with
     * new parameters. Useful when switching to fullscreen,
     * for example.
     */
    void RenewDisplay();
    void RenewDisplay(const int width, const int height);
    void RenewDisplay(const int width, const int height,
            const int bpp, const int flags);

    /* Basically blit a background layer, by default it 
     * is simply an all-black surface filling the whole
     * screen.
     */
    void ClearScreen();
    void ClearScreen(SDL_Surface* cleaner);

    /* Self-explanatory, returns whether or not
     * the new screen is in fullscreen mode, or not.
     */
    void ToggleFullscreen();

    /* Either put a surface on the screen, or on another
     * surface. Blitting uses DirectDraw, so it's very slow,
     * but but OpenGL is the only other alternative.
     */
    void Blit(SDL_Surface* destination, SDL_Surface* source,
            const int x, const int y) const;
    void Blit(SDL_Surface* source, const int x,
            const int y) const;

    /* Flip the display buffers, showing everything on screen. */
    void Update() const;

    /* Get screen values */
    int GetWidth(){return this->width;}
    int GetHeight(){return this->height;}
    SDL_Rect GetSize(){
        SDL_Rect size = {0, 0, this->width, this->height};
        return size;
    }

    /* Get the screen, the background, and itself. */
    SDL_Surface* GetScreen() const;
    SDL_Surface* GetBackground() const;
    CDisplay*    GetDisplay();

    /* Screen parameters */
    int width;
    int height;
    int bpp;
    int flags;

private:
    /* Create the screen, initialize SDL
     * and SDL_image, etc.
     */
    void Initialize();

    /* Default flags for initialization of SDL,
     * as well as initializing the screen.
     */
    static const int DEFAULT_SCREEN_WIDTH   = 800;
    static const int DEFAULT_SCREEN_HEIGHT  = 600;
    static const int DEFAULT_SCREEN_BPP     = 32;
    static const int DEFAULT_SCREEN_FLAGS   = SDL_SWSURFACE;

    SDL_Surface* background;
    SDL_Surface* screen;
    SDL_Surface* icon;

    bool isFullscreen;
};

#endif // DISPLAY_H
