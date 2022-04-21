#pragma once

#include "ecm.h"
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include"../animation.h">

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

// ANIMATED SPRITE
class AnimatedSpriteComponent : public Component {
protected:
    std::shared_ptr<sf::Sprite> _sprite;
    std::shared_ptr<sf::Texture> _texture;
    Animation _animation;
    bool _faceRight;
public:
    AnimatedSpriteComponent() = delete;

    explicit AnimatedSpriteComponent(Entity* p, sf::Texture* texture, sf::Vector2u imageCount, float switchTime);
    void update(double dt) override;
    void render() override;

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