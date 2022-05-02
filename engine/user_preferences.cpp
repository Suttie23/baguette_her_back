#include "user_preferences.h"
#include <iostream>
#include <fstream>

using namespace sf;
using namespace std;

// RETURN LEFT KEY
int UserPreferences::LoadLeft()
{
    const std::string savepath = "config/user preferences/keybinds.save";
    std::ifstream savefile(savepath);

    // If file is not opened, throw error
    if (!savefile.is_open())
    {
        std::cerr << "Keybinds not loaded! " << savepath << std::endl;
    }

    // Else, load data
    else {
        std::cerr << "Keys Loaded" << std::endl;

        int left;
        int right;
        int up;

        // Load values from the files
        savefile >> left >> right >> up;

        return left;

    }

}

// REUTRN RIGHT KEY
int UserPreferences::LoadRight()
{
    const std::string savepath = "config/user preferences/keybinds.save";
    std::ifstream savefile(savepath);

    // If file is not opened, throw error
    if (!savefile.is_open())
    {
        std::cerr << "Keybinds not loaded! " << savepath << std::endl;
    }

    // Else, load data
    else {
        std::cerr << "Keys Loaded" << std::endl;

        int left;
        int right;
        int up;

        // Load values from the files
        savefile >> left >> right >> up;

        return right;

    }

}

// RETURN UP KEY
int UserPreferences::LoadUp()
{
    const std::string savepath = "config/user preferences/keybinds.save";
    std::ifstream savefile(savepath);

    // If file is not opened, throw error
    if (!savefile.is_open())
    {
        std::cerr << "Keybinds not loaded! " << savepath << std::endl;
    }

    // Else, load data
    else {
        std::cerr << "Keys Loaded" << std::endl;

        int left;
        int right;
        int up;

        // Load values from the files
        savefile >> left >> right >> up;

        return up;

    }

}

UserPreferences::UserPreferences() :
    // Fullscreen default = 7 (OFF), Fullscreen is activated when fullscreen(8)
    fullscreen(7), video_resolution(Vector2f(1920, 1080)), changed_fullscreen(false), changed_resolution(false), moveLeft(LoadLeft()), moveRight(LoadRight()), jump(LoadUp()) {
}

