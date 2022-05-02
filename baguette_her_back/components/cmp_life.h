#pragma once
#include <ecm.h>
#include <SFML\Audio\Music.hpp>
#include <SFML/Audio.hpp>

class LifeComponent : public Component {
private:
	int _lives;
	std::string _state;
	bool _damaged;
	float _invincibleTime;
public:
	sf::Sound damage;
	void update(double dt) override;
	void render() override {};
	int getLives();
	void setLives(int lives);
	void reduceLives();
	void increaseLives();
	explicit LifeComponent(Entity* p, int lives);
	LifeComponent() = delete;
};