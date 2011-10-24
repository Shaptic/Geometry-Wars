#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "Errors.h"

/* Link neccessary libraries, assuming
 * the Microsoft compiler.
 */
#ifdef WIN32
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDL_image.lib")
#endif // WIN32

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

/* Default flags for initialization of SDL,
 * as well as initializing the screen.
 */
#define DEFAULT_SCREEN_WIDTH    800
#define DEFAULT_SCREEN_HEIGHT   600
#define DEFAULT_SCREEN_BPP      32
#define DEFAULT_SCREEN_FLAGS    SDL_SWSURFACE
#define DEFAULT_INIT_FLAGS      SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER

/* This is the icon that will show up in
 * the application window. Set it to NULL
 * for no icon.
 */
#define APP_ICON                NULL

/* This is the caption that will be shown in
 * the title bar of your window.
 */
#define WINDOW_CAPTION          "Geometry Wars"

/* Quick macro to blit a surface onto the screen.
 * Though there is a function, it takes too long to
 * type display.Blit(surface, x, y).
 */
 #define BLIT(surface, x, y)    display->Blit(surface, x, y)

/* Another few macros to easily get the screen size, provided
 * for backward-compatibility with older Display class, and 
 * for ease of use.
 */
#define SCREEN_HEIGHT           display->height
#define SCREEN_WIDTH            display->width

/* Some retarded Windows stuff messes up function names
 * with ASCII / Unicode locales.
 */
#ifdef WIN32
#undef LoadImage
#endif // WIN32

/* Easily load an image file */
SDL_Surface* LoadImage(const std::string& filename);
SDL_Surface* LoadImage(const char* filename);
SDL_Surface* LoadImage_Alpha(const std::string& filename);
SDL_Surface* LoadImage_Alpha(const char* filename);

class Display
{
public:
    Display();
    Display(const int width, const int height);
    Display(const int width, const int height,
            const int bpp, const int flags);

    ~Display();

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

    /* Self-explanatory */
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

    /* Get the screen, the background, and itself. */
    SDL_Surface* GetScreen() const;
    SDL_Surface* GetBackground() const;
    Display*     GetDisplay();

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

    SDL_Surface* background;
    SDL_Surface* screen;
    SDL_Surface* icon;

    bool isFullscreen;
};

#endif // DISPLAY_H
