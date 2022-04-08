#include "user_preferences.h"
#include <iostream>

using namespace sf;
using namespace std;

UserPreferences::UserPreferences() :
	// Fullscreen default = 7 (OFF), Fullscreen is activated when fullscreen(8)
	fullscreen(7), video_resolution(Vector2f(800,600)), changed_fullscreen(false), changed_resolution(false){
} 
