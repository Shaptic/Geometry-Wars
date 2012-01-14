#ifndef SDL_HELPER_H
#define SDL_HELPER_H

#include <string>
#include <vector>

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define rmask            0xff000000
#define gmask            0x00ff0000
#define bmask            0x0000ff00
#define amask            0x000000ff
#else
#define rmask            0x000000ff
#define gmask            0x0000ff00
#define bmask            0x00ff0000
#define amask            0xff000000
#endif

#define ALIGN_CENTER    0x01
#define CREATE_SURFACE  0x02
#define TRANSPARENT_BG  0x04

#define WHITE           0xFF, 0xFF, 0xFF
#define BLACK           0x00, 0x00, 0x00
#define RED             0xFF, 0x00, 0x00
#define GREEN           0x00, 0xFF, 0x00
#define BLUE            0x00, 0x00, 0xFF
#define YELLOW          0xFF, 0xFF, 0x00
#define PURPLE          0xFF, 0x00, 0xFF

struct SDL_Circle
{
    int x;
    int y;
    unsigned int radius;
};

SDL_Rect        create_rect(const int x, const int y, const int w, const int h);
SDL_Color       create_color(const int r, const int g, const int b);
Uint32          create_uint32_color(SDL_Surface* source, const SDL_Color& color);

SDL_Surface*    create_surface(const int w, const int h);
SDL_Surface*    create_surface(const int w, const int h, const SDL_Color& color);
SDL_Surface*    create_surface_alpha(const int w, const int h);
SDL_Surface*    create_surface_alpha(const int w, const int h, const SDL_Color& color);

SDL_Surface*    render_text(TTF_Font* font,
                            const std::string& text,
                            const SDL_Color* bg_color,      // Optional if not transparent background
                            const SDL_Color& text_color,
                            const int flags);

SDL_Surface*    tile_surface(SDL_Surface* source, const int w, const int h);
SDL_Surface*    tile_surface(SDL_Surface* source, const int x, const int y, const int end_x, const int end_y);
SDL_Surface*    tile_surface(SDL_Surface* source, const SDL_Rect& rect);

SDL_Surface*    clip_sprite_sheet(SDL_Surface* sheet, const int x, const int y, const int w, const int h);

void            remove_color(SDL_Surface* source, const SDL_Color& color);
void            remove_color(SDL_Surface* source, const int r, const int g, const int b);
void            fill_rect(SDL_Surface* source, SDL_Rect& fill, const SDL_Color& color);
void            fill_rect(SDL_Surface* source, SDL_Rect* fill, const SDL_Color& color);

int             get_text_height(TTF_Font* font, const std::string& text);
int             get_text_width(TTF_Font* font, const std::string& text);

bool            detect_collision(const SDL_Rect& obj1, const SDL_Rect& obj2);
bool            detect_collision(const SDL_Rect* obj1, const SDL_Rect* obj2);

Uint32          get_pixel(SDL_Surface* surface, int x, int y);

#endif // SDL_HELPER_H


