#include "Display.h"

using namespace std;

SDL_Surface* LoadImage(const std::string& filename)
{
    /* Simply calls the overloaded function
     * that accepts const char*.
     */
    return LoadImage(filename.c_str());
}

SDL_Surface* LoadImage(const char* filename)
{
    /* An obvious check if there even is
     * a filename.
     */
    if(filename == NULL)
        handleError("No file to load!");

    /* We need to set up our surfaces for
     * the original image and the version
     * optimized for our display.
     */
    SDL_Surface* image     = NULL;
    SDL_Surface* optimized = NULL;

    /* Use the SDL_image library to load
     * the image. This allows us to load many
     * types of image files, like PNG and JPG,
     * rather than be limited to BMP's by
     * the SDL defaults.
     */
    image = IMG_Load(filename);

    /* If the image failed to load, show
     * the user an error message.
     */
    if(!image)
    {
        handleError(SDL_GetError());
    }

    /* Optimize the image */
    optimized = SDL_DisplayFormat(image);

    /* If we could not optimize it, simply
     * return the unoptimized version, otherwise
     * we can return the optimized image.
     */
    if(!optimized)
    {
        return image;
    }
    else
    {
        SDL_FreeSurface(image);
        return optimized;
    }
}

SDL_Surface* LoadImage_Alpha(const std::string& filename)
{
    return LoadImage_Alpha(filename.c_str());
}

SDL_Surface* LoadImage_Alpha(const char* filename)
{
    /* An obvious check if there even is
     * a filename.
     */
    if(filename == NULL)
        handleError("No file to load!");

    /* This function loads images in the same way
     * that LoadImage() does, except that it will
     * maintain alpha-transparency for images such 
     * as transparent PNG's.
     */
    SDL_Surface* image     = NULL;
    SDL_Surface* optimized = NULL;

    image = IMG_Load(filename);

    if(!image)
    {
        handleError(SDL_GetError());
    }

    optimized = SDL_DisplayFormatAlpha(image);

    if(!optimized)
        return image;

    else
    {
        SDL_FreeSurface(image);
        return optimized;
    }
}

CDisplay::CDisplay()
{
    /* We didn't get any arguments,
     * so we use the default options.
     */
    this->width     = DEFAULT_SCREEN_WIDTH;
    this->height    = DEFAULT_SCREEN_HEIGHT;
    this->bpp       = DEFAULT_SCREEN_BPP;
    this->flags     = DEFAULT_SCREEN_FLAGS;

    /* Initialize the library, set up the display,
     * background layer, etc.
     */
    this->Initialize();
}

CDisplay::CDisplay(const int width, const int height)
{
    /* We only got height and width, so set those
     * accordingly, using defaults for the rest.
     */
    this->width     = width;
    this->height    = height;
    this->bpp       = DEFAULT_SCREEN_BPP;
    this->flags     = DEFAULT_SCREEN_FLAGS;

    this->Initialize();
}

CDisplay::CDisplay(const int width, const int height,
            const int bpp, const int flags)
{
    /* All custom parameters for the screen,
     * so set them accordingly.
     */
    this->width     = width;
    this->height    = height;
    this->bpp       = bpp;
    this->flags     = flags;

    this->Initialize();
}

CDisplay::~CDisplay()
{
    SDL_FreeSurface(this->background);
    IMG_Quit();
    SDL_Quit();
}

void CDisplay::Initialize()
{
    /* Set the caption to the title bar
     * of the window.
     */
    SDL_WM_SetCaption(WINDOW_CAPTION, NULL);

    /* Load the icon to show up in the top-right
     * corner of the title bar.
     */
    if(APP_ICON != NULL)
    {
        this->icon = LoadImage(APP_ICON);
        SDL_WM_SetIcon(this->icon, NULL);
        SDL_FreeSurface(this->icon);
    }

    /* We will now create a display surface using
     * the parameters provided.
     */
    this->screen = SDL_SetVideoMode(this->width,
            this->height, this->bpp, this->flags);

    /* If we couldn't create one, show an error. */
    if(!this->screen)
        handleError(SDL_GetError());

    /* This is the background image, an all black surface
     * the same size as the screen.
     */
    this->background = SDL_CreateRGBSurface(SDL_SWSURFACE, 
            this->width, this->height, this->bpp,
            rmask, gmask, bmask, 0);

    SDL_FillRect(this->background, NULL, 0x000000);}

void CDisplay::RenewDisplay()
{
    this->RenewDisplay(DEFAULT_SCREEN_WIDTH,
        DEFAULT_SCREEN_HEIGHT,
        DEFAULT_SCREEN_BPP,
        DEFAULT_SCREEN_FLAGS);
}

void CDisplay::RenewDisplay(const int width, const int height)
{
    this->RenewDisplay(width,
        height,
        DEFAULT_SCREEN_BPP,
        DEFAULT_SCREEN_FLAGS);
}

void CDisplay::RenewDisplay(const int width, const int height,
            const int bpp, const int flags)
{
    /* We create a new display with new parameters.
     * We do not need to call SDL_FreeSurface() on 
     * the main display because SDL_SetVideoMode()
     * will do that for us.
     */
    this->width     = width;
    this->height    = height;
    this->bpp       = bpp;
    this->flags     = flags;

    this->screen = SDL_SetVideoMode(width, height,
            bpp, flags);

    if(!this->screen)
        handleError(SDL_GetError());
}

void CDisplay::ClearScreen()
{
    /* This will place a surface on the main
     * display, the all-black background we
     * loaded in our constructor.
     */
    this->Blit(this->background, 0, 0);
}

void CDisplay::ClearScreen(SDL_Surface* cleaner)
{
    /* Sometimes we don't want the background
     * to just be an all-black surface, so we
     * can load a custom one.
     */
    this->Blit(cleaner, 0, 0);
}

void CDisplay::ToggleFullscreen()
{
    /* Pretty self-explanatory, we toggle the display
     * between windowed and fullscreen mode.
     */
    this->isFullscreen = !this->isFullscreen;

    if(this->isFullscreen)
        /* Here we add the fullscreen flag */
        this->flags |= SDL_FULLSCREEN;
    else
        /* And here we remove it */
        this->flags &= ~SDL_FULLSCREEN;

    this->RenewDisplay(this->width, this->height, this->bpp, this->flags);
}

void CDisplay::Blit(SDL_Surface* destination, SDL_Surface* source,
            const int x, const int y) const
{
    /* Blitting is basically placing a surface on another
     * surface at a certain location. 
     */
    SDL_Rect offset = {x, y, 0, 0};

    SDL_BlitSurface(source, NULL, destination, &offset);
}

void CDisplay::Blit(SDL_Surface* source, const int x,
            const int y) const
{
    /* This is a special case of blitting, where we
     * blit the surface directly to the display.
     */
    this->Blit(this->screen, source, x, y);
}

void CDisplay::Update() const
{
    /* Update everything that we have blitted 
     * onto the main display surface.
     */
    SDL_Flip(this->screen);
}

SDL_Surface* CDisplay::GetScreen() const
{
    /* Get our display. */
    return this->screen;
}

SDL_Surface* CDisplay::GetBackground() const
{
    /* Get the background */
    return this->background;
}

CDisplay* CDisplay::GetDisplay()
{
    /* Get ourselves */
    return this;
}
