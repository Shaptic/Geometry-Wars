#include "Settings.h"

using namespace std;

CSettings::CSettings()
{
    /* Load a configuration file and parse
     * it for various settings and filenames
     * to be used throughout the game.
     */
    ifstream config("Settings.ini");
    string line;
    string title_music, gameplay_music;
    string title_font, ui_font;

    if(!config.is_open())
        handleError("Unable to locate Settings.ini!");

    while(config.good())
    {
        getline(config, line);

        if(line.empty() || line[0] == ';')  // Blank or comment
            continue;

        else if(line == "[SpriteFiles]") // Image data
        {
            /* Nothing so far */
        }

        else if(line == "[Music]")  // Music data
        {
            do
            {
                getline(config, line);

                if(line.find("Title") != string::npos)
                {
                    title_music = split(line, '=')[1];
                    this->title_music = Mix_LoadMUS(title_music.c_str());
                    
                    if(!this->title_music)
                        handleError(Mix_GetError());
                }

                else if(line.find("Gameplay") != string::npos)
                {
                    gameplay_music = split(line, '=')[1];

                    if(gameplay_music.find('[') != string::npos &&
                        gameplay_music.find(']') != string::npos &&
                        gameplay_music.find('-') != string::npos)
                    {
                        stringstream ss;

                        /* Filename: GameplayMusic[1-5].mp3
                         * So we parse the maximum (5)
                         * and load each file.
                         */
                        int min = atoi(gameplay_music.substr(gameplay_music.find('[') + 1, gameplay_music.find('-')).c_str());
                        int max = atoi(gameplay_music.substr(gameplay_music.find('-') + 1, gameplay_music.find(']')).c_str());
                        
                        for(short i = min; i <= max; i++)
                        {
                            ss << gameplay_music.substr(0, gameplay_music.find('[')) << i;  // filename#
                            ss << gameplay_music.substr(gameplay_music.find('.'), -1);  // .extension
                            this->gameplay_music.push_back(Mix_LoadMUS(ss.str().c_str()));
                            
                            if(!this->gameplay_music[this->gameplay_music.size() - 1])
                                handleError(Mix_GetError());

                            ss.str(string());
                        }
                    }
                    else
                    {
                        this->gameplay_music.push_back(Mix_LoadMUS(gameplay_music.c_str()));

                        if(!this->gameplay_music[this->gameplay_music.size() - 1])
                            handleError(Mix_GetError());
                    }
                }
            }
            while(!line.empty() && line[0] != '[');   // Until we hit the next header
        }

        else if(line == "[Fonts]")
        {
            do
            {
                getline(config, line);

                if(line.find("Title") != string::npos)
                {
                    string name = split(line, '=')[1];
                    int size = atoi(split(name, ':')[1].c_str());
                    name = name.substr(0, name.find(':'));

                    this->title_font = TTF_OpenFont(name.c_str(), size);
                    if(!this->title_font)
                        handleError(TTF_GetError());
                }
                if(line.find("UI") != string::npos)
                {
                    string name = split(line, '=')[1];
                    int size = atoi(split(name, ':')[1].c_str());
                    name = name.substr(0, name.find(':'));

                    this->ui_font = TTF_OpenFont(name.c_str(), size);

                    if(!this->ui_font)
                        handleError(TTF_GetError());
                }
            }
            while(!line.empty() && line[0] != '[');
        }

        else if(line == "[Data]")
        {
            do
            {
                getline(config, line);

                if(line.find("Levels") != string::npos)
                    this->level_filename = split(line, '=')[1];

                else if(line.find("Highscore") != string::npos)
                    this->highscore_filename = split(line, '=')[1];
            }
            while(!line.empty() && line[0] != '[');
        }
    }

    config.close();
}

CSettings::~CSettings()
{
    TTF_CloseFont(this->title_font);
    TTF_CloseFont(this->ui_font);

    Mix_FreeMusic(this->title_music);
    
    for(size_t i = 0; i < this->gameplay_music.size(); i++)
        Mix_FreeMusic(this->gameplay_music[i]);
}

TTF_Font* CSettings::GetTitleFont() const
{
    return this->title_font;
}

TTF_Font* CSettings::GetUIFont() const
{
    return this->ui_font;
}

Mix_Music* CSettings::GetTitleMusic() const
{
    return this->title_music;
}

Mix_Music* CSettings::GetGameplayMusic() const
{
    static bool first_run   = true;
    static int current      = 0;

    /* We keep track of the current song playing,
     * and when it ends we play the next song.
     */
    if(first_run)
    {
        first_run = false;
        return this->gameplay_music[current];
    }

    else
    {
        if(Mix_PlayingMusic() == 0 && (current < (this->gameplay_music.size() - 1)))
            current++;

        else if(Mix_PlayingMusic() == 0 && current == this->gameplay_music.size() - 1)
            current = 0;
        
        return this->gameplay_music[current];
    }
}

string CSettings::GetLevelFile() const
{
    return this->level_filename;
}

string CSettings::GetHighscoreFile() const
{
    return this->highscore_filename;
}