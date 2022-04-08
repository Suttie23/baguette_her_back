#pragma once

#include <SFML\Audio\Music.hpp>
#include "engine.h"

class SettingsScene : public Scene {
private:
	std::vector<sf::Vector2f> menuIndex;
public:
  SettingsScene() = default;
  ~SettingsScene() override = default;

  void Load() override;

  void Update(const double& dt) override;


};
