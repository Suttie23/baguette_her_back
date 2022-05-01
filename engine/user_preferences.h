#pragma once
#include"SFML\System\Vector2.hpp"
#include "SFML\Window\WindowStyle.hpp"
#include <SFML/Window/Keyboard.hpp>
#include<map>
#include <iostream>
#include <fstream>


struct UserPreferences
{
public:

	int LoadLeft();
	int LoadRight();
	int LoadUp();

	// Fullscreen & Resolution
	bool changed_fullscreen;
	bool changed_resolution;
	sf::Vector2f video_resolution_default;
	sf::Vector2f video_resolution;
	unsigned int fullscreen;

	// Keybinds
	int moveLeft;
	int moveRight;
	int jump;

	UserPreferences();
};


