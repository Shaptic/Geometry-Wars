#ifndef SETTINGS_H
#define SETTINGS_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Errors.h"
#include "Helpers.h"

/* This class relies on you initializing the TTF
 * and MIX libraries beforehand.
 */

typedef std::vector<Mix_Music*> GameMusic;

class CSettings
{
public:
    CSettings();
    CSettings(const char* filename);
    CSettings(const std::string& filename);

    ~CSettings();

    TTF_Font*   GetTitleFont() const;
    TTF_Font*   GetUIFont() const;

    Mix_Music*  GetTitleMusic() const;
    Mix_Music*  GetGameplayMusic() const;

    std::string GetLevelFile() const;
    std::string GetHighscoreFile() const;

private:
    TTF_Font*   title_font;
    TTF_Font*   ui_font;

    Mix_Music*  title_music;
    GameMusic   gameplay_music;

    std::string level_filename;
    std::string highscore_filename;
};

#endif // SETTINGS_H