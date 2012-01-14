/* A class that handles the Settings.ini file,
 * which contains all of the critical game values
 * such as frame rate, speeds, file locations, etc.
 */

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

#include "Errors.h"
#include "Display.h"
#include "Helpers.h"
#include "SDL_Helper.h"

class CSettingsManager
{
public:
    CSettingsManager(const std::string& filename);
    ~CSettingsManager();

    std::string GetValueAt(const std::string&);
    std::string ChooseValueAt(const std::string&);
    void        CreateValue(const std::string& id, const std::string& value);    

private:
    std::fstream   settings;
};

#endif // __SETTINGS_H__