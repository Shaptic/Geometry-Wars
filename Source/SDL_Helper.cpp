#include "SDL_Helper.h"

using namespace std;

SDL_Rect create_rect(const int x, const int y, const int w, const int h)
{
    SDL_Rect rect = {x, y, w, h};
    return rect;
}

SDL_Color create_color(const int r, const int g, const int b)
{
    SDL_Color color = {r, g, b, 0};
    return color;
}

Uint32 create_uint32_color(SDL_Surface* source, const SDL_Color& color)
{
    return SDL_MapRGB(source->format, color.r, color.g, color.b);
}

SDL_Surface* create_surface(const int w, const int h)
{
    return SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, rmask, gmask, bmask, amask);
}

SDL_Surface* create_surface(const int w, const int h, const SDL_Color& color)
{
    SDL_Surface* final = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h,32, rmask, gmask, bmask, amask);
    fill_rect(final, NULL, color);
    return final;
}

SDL_Surface* create_surface_alpha(const int w, const int h)
{
    return SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA | SDL_RLEACCEL,
                w, h, 32, rmask, gmask, bmask, amask);
}

SDL_Surface* create_surface_alpha(const int w, const int h, const SDL_Color& color)
{
    SDL_Surface* final = create_surface_alpha(w, h);
    fill_rect(final, NULL, color);
    return final;
}

SDL_Surface* render_text(TTF_Font* font, const string& text, 
        const SDL_Color* bg_color, const SDL_Color& txt_color, const int flags)
{
    if(font == NULL)
        return NULL;

    int skipSize    = TTF_FontLineSkip(font);
    int height      = TTF_FontHeight(font);
    int width       = 0;
    int index       = -1;

    SDL_Surface* textSurface = NULL;
    vector<string> lines;
    string tmp(text);

    do
    {
        int w = 0;
        index = tmp.find("\n", 0);

        lines.push_back(tmp.substr(0, index));

        if(index != -1)
        {
            tmp = tmp.substr(index + 1, -1);
        }
        TTF_SizeText(font, lines.at(lines.size() - 1).c_str(), &w, &height);
        if(width < w)
        {
            width = w;
        }
    }
    while(index != -1);

    height = (lines.size() - 1) * skipSize + height;

    if(flags & TRANSPARENT_BG)
        textSurface = create_surface_alpha(width, height);
    else
        textSurface = create_surface(width, height, *bg_color);

    SDL_Surface* lineText = NULL;

    for(unsigned int i = 0; i < lines.size(); i++)
    {
        if(lines[i] == "")
            lineText = TTF_RenderText_Blended(font, " ", txt_color);
        else
            lineText = TTF_RenderText_Blended(font, lines.at(i).c_str(), txt_color);

        if(flags & TRANSPARENT_BG)
            SDL_SetAlpha(lineText, 0, SDL_ALPHA_TRANSPARENT);

        if(flags & ALIGN_CENTER)
        {
            int w = 0;
            int h = 0;
            TTF_SizeText(font, lines.at(i).c_str(), &w, &h);
            SDL_BlitSurface(lineText, NULL, textSurface, &(SDL_Rect)create_rect(width / 2 - w / 2, i * skipSize, 0, 0));
        }
        else
        {
            SDL_BlitSurface(lineText, NULL, textSurface, &(SDL_Rect)create_rect(0, i * skipSize, 0, 0));
        }
        SDL_FreeSurface(lineText);
    }

    return textSurface;
}

SDL_Surface* clip_sprite_sheet(SDL_Surface* sheet, const int x, const int y,
    const int w, const int h)
{
    SDL_Surface* sprite = create_surface(w, h);
    SDL_Rect offset     = create_rect(x, y, w, h);

    SDL_BlitSurface(sheet, &offset, sprite, NULL);

    return sprite;
}

void remove_color(SDL_Surface* source, const SDL_Color& color)
{
    Uint32 colorkey  = create_uint32_color(source, color);
    SDL_SetColorKey(source, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
}

void fill_rect(SDL_Surface* source, SDL_Rect& box, const SDL_Color& color)
{
    SDL_FillRect(source, &box, create_uint32_color(source, color));
}

void fill_rect(SDL_Surface* source, SDL_Rect* box, const SDL_Color& color)
{
    SDL_FillRect(source, box, create_uint32_color(source, color));
}

SDL_Surface* tile_surface(SDL_Surface* source, const int w, const int h)
{
    SDL_Surface* tiledSurf  = create_surface(w, h, create_color(BLACK));
    const int surfWidth     = source->clip_rect.w;
    const int surfHeight    = source->clip_rect.h;

    int surfsMade           = 0;
    int widthTaken          = 0;
    int heightTaken         = 0;

    int current_x           = 0;
    int current_y           = 0;

    SDL_Rect whereToBlit    = create_rect(current_x, current_y, 0, 0);

    while(true)
    {
        SDL_BlitSurface(source, NULL, tiledSurf, &whereToBlit);

        if(widthTaken >= w && !(heightTaken >= h))
            // Move to the next line
        {
            widthTaken  = 0;
            heightTaken += surfHeight;
            current_y   += surfHeight;
            current_x   = 0;
        }
        else
        {
            // Move one tile over
            current_x   += surfWidth;
        }

        if(widthTaken >= w && heightTaken >= h)
            // Meaning the whole screen is tiled
            break;

        widthTaken    += surfWidth;

        whereToBlit.x = current_x;
        whereToBlit.y = current_y;

        surfsMade++;
    }

    return tiledSurf;
}

SDL_Surface* tile_surface(SDL_Surface* source, const int m_x, const int m_y, const int end_x, const int end_y)
{
    SDL_Surface* tiledSurf    = create_surface(end_x - m_x, end_y - m_y, create_color(BLACK));

    int x = m_x;
    int y = m_y;

    int max_w = end_x - m_x;
    int max_h = end_y - m_y;

    const int w = source->clip_rect.w;
    const int h = source->clip_rect.h;

    int w_taken = 0;
    int h_taken = 0;

    SDL_Rect blitRect = create_rect(x, y, 0, 0);

    while(true)
    {
        SDL_BlitSurface(source, NULL, tiledSurf, &blitRect);

        if(w_taken >= max_w && h_taken >= max_h)
            break;

        if(x >= end_x && !(y >= end_y))
        {
            x = 0;
            w_taken = 0;
            y += h;
            h_taken += h;
        }
        else
        {
            x += w;
            w_taken += w;
        }

        blitRect.x = x;
        blitRect.y = y;
    }
    return tiledSurf;
}

int get_text_width(TTF_Font* font, const string& text)
{
    int width = 0;
    TTF_SizeText(font, text.c_str(), &width, NULL);
    return width;
}

int get_text_height(TTF_Font* font, const string& text)
{
    int height = 0;
    TTF_SizeText(font, text.c_str(), NULL, &height);
    return height;
}

bool detect_collision(const SDL_Rect& obj1, const SDL_Rect& obj2)
{
    int top1, bottom1, right1, left1;
    int top2, bottom2, right2, left2;

    left1   = obj1.x;
    right1  = obj1.x + obj1.w;
    top1    = obj1.y;
    bottom1 = obj1.y + obj1.h;

    left2   = obj2.x;
    right2  = obj2.x + obj2.w;
    top2    = obj2.y;
    bottom2 = obj2.y + obj2.h;

    if(bottom1 <= top2)
        return false;
    if(top1 >= bottom2)
        return false;
    if(right1 <= left2)
        return false;
    if(left1 >= right2)
        return false;

    return true;
}

bool detect_collision(const SDL_Rect* obj1, const SDL_Rect* obj2)
{
    int top1, bottom1, right1, left1;
    int top2, bottom2, right2, left2;

    left1   = obj1->x;
    right1  = obj1->x + obj1->w;
    top1    = obj1->y;
    bottom1 = obj1->y + obj1->h;

    left2   = obj2->x;
    right2  = obj2->x + obj2->w;
    top2    = obj2->y;
    bottom2 = obj2->y + obj2->h;

    if(bottom1 <= top2)
        return false;
    if(top1 >= bottom2)
        return false;
    if(right1 <= left2)
        return false;
    if(left1 >= right2)
        return false;

    return true;
}
