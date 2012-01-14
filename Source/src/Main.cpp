#include "Engine.h"

/* Link neccessary libraries, assuming
 * the Microsoft compiler.
 */
#ifdef WIN32
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDL_ttf.lib")
#pragma comment(lib, "SDL_mixer.lib")
#endif // WIN32

bool init();

int main(int argc, char* argv[])
{
    if(!init())
        handleError(SDL_GetError(), true);

    Engine ShapeWars;
    ShapeWars.Menu();

    return 0;
}

bool init()
{
    /* Initialize all neccessary libraries,
     * showing error messages if necessary.
     */
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
        return false;
    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
        return false;
    if(TTF_Init() == -1)
        return false;
    if(Mix_Init(MIX_INIT_MP3) == 0)
        return false;
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        return false;

    return true;
}