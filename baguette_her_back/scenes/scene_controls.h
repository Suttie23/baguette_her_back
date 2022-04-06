#pragma once

#include <SFML\Audio\Music.hpp>
#include "engine.h"

class ControlsScene : public Scene {
public:
	ControlsScene() = default;
  ~ControlsScene() override = default;

  void Load() override;

  void Update(const double& dt) override;

  void UnLoad() override;

};
