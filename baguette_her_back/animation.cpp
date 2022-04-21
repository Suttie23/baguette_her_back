#include "animation.h"

Animation::Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime) 
{
	// image count = the amount of specific images in spritesheet
	this->imageCount = imageCount;
	this->switchTime = switchTime;
	totalTime = 0;
	currentFrame = 0;

	uvRect.width = texture->getSize().x / float(imageCount.x);
	uvRect.height = texture->getSize().y / float(imageCount.y);

}

Animation::~Animation() 
{

}

// setting frames
void Animation::SetFrames(std::vector<sf::Vector2u>& newFrames)
{
	Frames = newFrames;
	if (currentFrame >= Frames.size()) {
		currentFrame = 0;
	}
}

void Animation::Update(float deltaTime, bool faceRight)
{
	totalTime += deltaTime;

	if (totalTime >= switchTime)
	{
		totalTime -= switchTime;
		currentFrame++;

		if (currentFrame >= Frames.size())
		{
			currentFrame = 0;
		}
	}

	// stop out of bound frames breaking things spectacularly 
	if (Frames.empty())
	{
		return;
	}

	// setting the current image to the current frame
	sf::Vector2u currentImage = Frames[currentFrame];

	uvRect.top = currentImage.y * uvRect.height;

	// determine direction of the sprite 
	if (faceRight)
	{
		uvRect.left = currentImage.x * uvRect.width;
		uvRect.width = abs(uvRect.width);
	}
	else 
	{
		uvRect.left = (currentImage.x + 1) * abs(uvRect.width);
		uvRect.width = -abs(uvRect.width);
	}
}

