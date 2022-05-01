#pragma once

#include "cmp_physics.h"
#include <SFML\Audio\Music.hpp>
#include <SFML/Audio.hpp>
#include "user_preferences.h"

class PlayerPhysicsComponent : public PhysicsComponent {
protected:
  b2Vec2 _size;
  sf::Vector2f _maxVelocity;
  bool _grounded;
  float _groundspeed;

  bool isGrounded() const;

public:
	sf::Sound jump;
	static UserPreferences user_preferences;
  void update(double dt) override;

  explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size);

  PlayerPhysicsComponent() = delete;
};
