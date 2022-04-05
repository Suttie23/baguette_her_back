#pragma once

#include <SFML\Audio\Music.hpp>
#include "engine.h"

class MenuScene : public Scene {
private:
	std::vector<sf::Vector2f> menuIndex;
public:
  sf::Music menuTheme;
  MenuScene() = default;
  ~MenuScene() override = default;

  void Load() override;

  void Update(const double& dt) override;

};
