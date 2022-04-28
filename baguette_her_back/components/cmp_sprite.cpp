
#include "cmp_sprite.h"
#include "system_renderer.h"
#include <SFML/Graphics.hpp>
#include "../animation.h"

using namespace std;

// STATIC SPRITE COMPONENT

void SpriteComponent::setTexture(std::shared_ptr<sf::Texture> tex)
{
  _texture = tex;
  _sprite->setTexture(*_texture);
}

SpriteComponent::SpriteComponent(Entity* p)
    : Component(p), _sprite(make_shared<sf::Sprite>()) {}

void SpriteComponent::update(double dt) {
  _sprite->setPosition(_parent->getPosition());
  _sprite->setRotation(sf::degrees(_parent->getRotation()));
}


void SpriteComponent::render() { Renderer::queue(_sprite.get()); }

sf::Sprite& SpriteComponent::getSprite() const { return *_sprite; }

// SHAPE COMPONENT

void ShapeComponent::update(double dt) {
  _shape->setPosition(_parent->getPosition());
  _shape->setRotation(sf::degrees(_parent->getRotation()));
}

void ShapeComponent::render() { Renderer::queue(_shape.get()); }

sf::Shape& ShapeComponent::getShape() const { return *_shape; }

ShapeComponent::ShapeComponent(Entity* p)
    : Component(p), _shape(make_shared<sf::CircleShape>()) {}

// ANIMATED SPRITE COMPONENT

void AnimatedSpriteComponent::setTextureRect(std::shared_ptr<sf::Texture> tex)
{
    _texture = tex;
    _sprite->setTexture(*_texture);
    _sprite->setTextureRect(sf::IntRect(sf::Vector2i(13, 1), sf::Vector2i(23, 47)));

}

void AnimatedSpriteComponent::SetFrames(std::vector<sf::Vector2u>& newFrames)
{
    _animation.SetFrames(newFrames);
}

AnimatedSpriteComponent::AnimatedSpriteComponent(Entity* p, sf::Texture* texture, sf::Vector2u imageCount, float switchTime)
    : Component(p), _sprite(make_shared<sf::Sprite>()), _animation(texture, imageCount, switchTime) {}

void AnimatedSpriteComponent::update(double dt) {
    _sprite->setPosition(_parent->getPosition());
    _sprite->setRotation(sf::degrees(_parent->getRotation()));

    _animation.Update(dt, faceRight);
    _sprite->setTextureRect(_animation.uvRect);

}

void AnimatedSpriteComponent::render() { Renderer::queue(_sprite.get()); }

sf::Sprite& AnimatedSpriteComponent::getSprite() const { return *_sprite; }

// REPEATED SPRITE COMPONENT

RepeatedSpriteComponent::RepeatedSpriteComponent(Entity* p, int repetition)
	: Component(p), _repetition(repetition), _sprite(make_shared<std::vector<sf::Sprite>>())
	, _texture(make_shared<sf::Texture>())
{
	for (int i = 0; i < _repetition; i++) {
		_sprite->push_back(sf::Sprite());
	}

}

void RepeatedSpriteComponent::update(double dt)
{
	for (int i = 0; i < _repetition; i++) {
		_sprite->at(i).setPosition(_parent->getPosition());
	}
}

void RepeatedSpriteComponent::render()
{
	for (int i = 0; i < _repetition; i++) {
		float x = _parent->getPosition().x + _sprite->at(i).getLocalBounds().width * i + i * 5.0f;
		float y = _parent->getPosition().y;
		_sprite->at(i).setPosition(sf::Vector2f(x, y));
		Renderer::queue(&_sprite->at(i));
	}
}

void RepeatedSpriteComponent::decreaseRep()
{
	_repetition--;
}

void RepeatedSpriteComponent::setRep(int reps)
{
	_repetition = reps;
}

sf::Texture* RepeatedSpriteComponent::setTexture(sf::Texture& texture)
{
	*_texture = texture;
	return &(*_texture);
}
