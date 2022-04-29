#pragma once
#include"SFML\System\Vector2.hpp"
#include "SFML\Window\WindowStyle.hpp"
#include <SFML/Window/Keyboard.hpp>
#include<map>


struct UserPreferences
{
public:
	bool changed_fullscreen;
	bool changed_resolution;
	sf::Vector2f video_resolution_default;
	sf::Vector2f video_resolution;
	unsigned int fullscreen;

	UserPreferences();
};


