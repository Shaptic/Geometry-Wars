/* A collection of classes, data structures, 
 * and functions to easily create customizable
 * and versatile menus.
 *
 * Special return values from CMenu::Run():
 *
 * -1       - Return to last menu
 * -2       - Quit game completely
 * 0        - Success
 * other    - Unique button ID
 */

/* We have to protect against double
 * includes and linkage errors!
 */
#ifndef MENUS_H
#define MENUS_H

/* Include standard headers */
#include <vector>
#include <string>

/* All of the SDL headers neccessary */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

/* SDL wrapper classes */
#include "SDL_Helper.h"
#include "Display.h"
#include "Events.h"

/* If we are using Windows, we can
 * assume the MSVC++ compiler, and
 * can link libraries using pragmas.
 */
#if (defined _WIN32 || defined WIN32)
    #pragma comment(lib, "SDLmain.lib")
    #pragma comment(lib, "SDL.lib")
    #pragma comment(lib, "SDL_ttf.lib")
    #pragma comment(lib, "SDL_image.lib")
    #pragma comment(lib, "SDL_mixer.lib")
#endif // _WIN32 || WIN32

/* A handy little macro to access
 * the last element of the buttons
 * vector in the Menu class.
 */
#define LAST this->buttons.size() - 1

/* A space saver */
using std::vector;
using std::string;

/* Forward declaration of the Menu class. */
class CMenu;

/* Identifiers for various button types
 * that can be used within a menu. A generic
 * button will return a unique value when
 * clicked. This value is determined at the 
 * time that the button is created.
 */
enum BUTTON_TYPE
{
    BTN_GENERIC = 10,
    BTN_TOGGLE,
    BTN_SUBMENU,
    BTN_TEXT,
    BTN_ACTION
};

/* Identifiers for the different actions
 * that an action button can perform. Most
 * menus have some sort of option to quit, 
 * play, or return to the last menu, that's
 * why they have unique ID's.
 */
enum ACTION_TYPE
{
    ACT_GENERIC = 20,
    ACT_QUIT,
    ACT_RETURN,
    ACT_PLAY,
    ACT_NONE
};

struct Button
{
    SDL_Surface*    main;           // Placed on screen every frame
    SDL_Surface*    highlight;      // The highlighted text
    SDL_Surface*    normal;         // Non-highlighted text

    SDL_Rect        mouseOver;      // Detect collision with mouse

    CMenu*          subMenu;        // If button is a submenu, this is it

    BUTTON_TYPE     btn_type;       // ID for button type
    ACTION_TYPE     act_type;       // What does the button do?

    int             id;             // Unique button ID

    int             x;              // Coordinates of the button on
    int             y;              // the screen.

    char*           text;           // Toggles switch the text on the 
                                    // button, therefore we need to store
                                    // the text so we can modify it later on
                                    // when the toggle is clicked.

    bool            switch_status;  // If the button is a toggle, is it
                                    // turned on or off?
};

class CMenu
{
public:
    /* Overloaded constructor so the user can
     * use a previously created CEvents and CDisplay
     * objects.
     */
    CMenu(CDisplay& display, CEvents& eventHandler);

    ~CMenu();

    /* Set a background for the menu. Can be used
     * to load a file, or use a pre-loaded surface.
     */
    void SetBackground(SDL_Surface* bg);
    void SetBackground(const char* filename);
    void SetBackground(const string& filename);

    /* Set a font for the menu. Either use one already
     * created, load a new one, or if this is not called
     * at all, a default will be used.
     */
    void SetFont(TTF_Font* font);
    void SetFont(const char* font_name);
    void SetFont(const string& font_name);

    /* Set the text color (normal) for the menu. If not
     * provided, black is used by default.
     */
    void SetTextColor(const int r, const int g, const int b);
    void SetTextColor(const SDL_Color& color);

    /* Set the text color (highlight) for the menu. If not
     * provided, white is used by default.
     */
    void SetHighLightColor(const int r, const int g, const int b);
    void SetHighLightColor(const SDL_Color& color);

    /* Sets music to be played while the menu is running. If not
     * provided, no music is played.
     */
    void SetMusic(const char* filename);
    void SetMusic(const string& filename);
    void SetMusic(Mix_Music* music);

    /* Set the coordinates for the first button. Also can
     * set whether or not the text should be aligned in
     * the center, or aligned to the right margin (default).
     */
    void SetStartCoordinates(const int x, const int y);
    void SetStartCoordinatesInScreenCenter();
    void SetCenterText(bool);

    /* Add an option to the menu. Here the user will provide
     * info on the type of option and what it does. Returns
     * -1 on error, and a unique ID if a generic
     * button type is chosen.
     */
    int  AddMenuOption(
            const char* text, 
            BUTTON_TYPE btn,
            ACTION_TYPE act,
            CMenu*      SubMenu     = NULL,
            bool        onByDefault = true);

    /* Remove an option from the menu. This makes it easy to
     * modify menus on the fly instead of re-creating a menu
     * from scratch. Requires an ID for the button.
     */
    void RemoveMenuOption(const int optionID);

    /* Find a menu option by what the text is on it, and 
     * return its unique ID.
     */
    int FindMenuOptionByText(const char* text);

    /* Run the menu. When an action or button is clicked, the
     * function will return the unique ID for that menu option.
     * runNoBlock(int) will run for a certain amount of frames
     * rather than running infinitely until an option is clicked.
     */
    int  Run();
    int  RunNoBlock(const int framesToRunFor);

    /* Default ID's for common game menu
     * options.
     */
    static const signed short int QUIT_ID   = -2;
    static const signed short int RETURN_ID = -1;

private:
    bool Initialize();
    void CheckMouseOver(const int x, const int y);
    int  CheckClick(const int x, const int y);

    /* Private methods for adding various types
     * of buttons to the menu.
     */
    int  AddSubMenu(const char* text, CMenu* SubMenu);
    int  AddAction (const char* text, ACTION_TYPE act);
    int  AddToggle (const char* text, bool onByDefault);
    int  AddGeneric(const char* text);
    int  AddText   (const char* text);

    /* Handling screen functions and events */
    CDisplay&        display;
    CEvents&         eventHandler;

    /* Menu background, text colors, menu font,
     * menu music, customized options, positions.
     */
    SDL_Surface*    bg;
    SDL_Color       txtColor;
    SDL_Color       hlColor;
    TTF_Font*       font;
    Mix_Music*      music;

    bool            quit;
    bool            center;

    int             x;
    int             y;
    int             offset;

    /* A STL container to hold all of the buttons. */
    vector<Button*> buttons;
};

#endif // MENUS_H
