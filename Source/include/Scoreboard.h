#ifndef __SCOREBOARD_H__
#define __SCOREBOARD_H__

#include <vector>
#include <sstream>

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#include "SDL_Helper.h"
#include "Display.h"

struct ScoreItem
{
    int             id;         // Unique item ID

    int             x;          // Position on the screen
    int             y;
    
    char*           main_line;  // The unchanging text such as "Lives: " or "Score: "
    int             var;        // The changing number value that complements the above string.

    SDL_Surface*    to_show;    // The surface to show up on the scoreboard
};

class CScoreBoard
{
public:
    CScoreBoard(CDisplay& Display);
    ~CScoreBoard();

    /* Font to be used */
    void SetFont(TTF_Font* Font);

    /* Text color */
    void SetTextColor(const SDL_Color& Color);
    void SetTextColor(const int r, const int g, const int b)
    {
        this->TextColor = create_color(r,g,b);
    }

    /* An overlay can be something like an image that matches
     * the layout of the scoreboard to add borders or something.
     */
    void SetOverlay(SDL_Surface* overlay);

    /* AddItem() will return a unique ID for the item. This does NOT
     * have to be used to access it, but the only other way to 
     * access it is by knowing the primary text line.
     */
    int  AddItem(const int x, const int y, const char* primary, int var);

    /* Updating an item on the scoreboard can be done either by using
     * the unique ID returned by AddItem(), or by searching all of the
     * items for the primary text line.
     */
    void UpdateItem(const char* primary, const int new_value);
    void UpdateItem(const int id, const int new_value);
    void UpdateItem_Increment(const char* primary, const int incr);
    void UpdateItem_Increment(const int id, const int incr);

    /* Updating can be done on either the entire scoreboard, or on
     * only a certain section of it.
     */
    void Update();
    void Update(const char* primary);
    void Update(const int id);

private:
    /* Just because... */
    std::stringstream ss;

    /* Many of the functions all have to search through the vector
     * in the exact same way so this'll simplify things.
     */
    ScoreItem* FindItem(const char* primary);
    ScoreItem* FindItem(const int id);

    CDisplay& Display;

    SDL_Color TextColor;
    SDL_Surface* Overlay;
    TTF_Font* MainFont;
    
    std::vector<ScoreItem*> Items;
};

#endif // __SCOREBOARD_H__