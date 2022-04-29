#pragma once

#include "engine.h"

class Level1Scene : public Scene {
private:
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
