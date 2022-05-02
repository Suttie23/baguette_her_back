#pragma once

#include "engine.h"
#include <SFML\Audio\Music.hpp>
#include <SFML/Audio.hpp>

class Level2Scene : public Scene {
private:
	sf::Music levelTrack;
	std::vector<sf::Vector2f> menuIndex;
	bool _isPaused = false;
	void LoadGame();
	void SaveGame();
public:
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;
};