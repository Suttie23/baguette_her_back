#pragma once

#include "ecm.h"
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include"../animation.h">
#include <SFML/Graphics/Sprite.hpp>

// STATIC SPRITE
class SpriteComponent : public Component {
protected:
  std::shared_ptr<sf::Sprite> _sprite;
  std::shared_ptr<sf::Texture> _texture;
public:
  SpriteComponent() = delete;
  explicit SpriteComponent(Entity* p);
  void update(double dt) override;
  void render() override;

  sf::Sprite& getSprite() const;


  void setTexture(std::shared_ptr<sf::Texture> tex);

};

// REPEATED SPRITE

class RepeatedSpriteComponent : public Component {
protected:
	std::shared_ptr<std::vector<sf::Sprite>> _sprite;
	std::shared_ptr<sf::Texture> _texture;
	int _repetition; std::vector<sf::Sprite>* a = new std::vector<sf::Sprite>();

public:
	explicit RepeatedSpriteComponent(Entity* p, int repetition);
	void update(double dt) override;
	void render() override;
	void decreaseRep();
	void setRep(int rep);

	sf::Texture* setTexture(sf::Texture& texture);
	template <typename... Targs> void setSprite(Targs... params) {
		Sprite s(params...);
		_sprite.reset(new std::vector<sf::Sprite>());
		for (int i = 0; i < _repetition; i++) {
			_sprite->push_back(s);
		}
	}
};

// ANIMATED SPRITE
class AnimatedSpriteComponent : public Component {
protected:
    std::shared_ptr<sf::Sprite> _sprite;
    std::shared_ptr<sf::Texture> _texture;
    Animation _animation;
public:
    AnimatedSpriteComponent() = delete;

    explicit AnimatedSpriteComponent(Entity* p, sf::Texture* texture, sf::Vector2u imageCount, float switchTime);
    void SetFrames(std::vector<sf::Vector2u>& newFrames);
    void update(double dt) override;
    void render() override;
    bool faceRight;
    sf::Sprite& getSprite() const;


    void setTextureRect(std::shared_ptr<sf::Texture> tex);
};

// SHAPE
class ShapeComponent : public Component {
protected:
  std::shared_ptr<sf::Shape> _shape;
  // sf::Shape _shape;

public:
  ShapeComponent() = delete;

  explicit ShapeComponent(Entity* p);

  void update(double dt) override;
  void render() override;
  sf::Shape& getShape() const;
  template <typename T, typename... Targs> void setShape(Targs... params) {
    _shape.reset(new T(params...));
  }
};