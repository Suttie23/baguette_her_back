#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Animation
{
public:
	Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);
	~Animation();

	void Update(float deltaTime, bool faceRight);
	void SetFrames(std::vector<sf::Vector2u>& newFrames);

public:

	sf::IntRect uvRect;

private:
	sf::Vector2u imageCount;
	int currentFrame;
	std::vector<sf::Vector2u> Frames;

	float totalTime;
	float switchTime;
};