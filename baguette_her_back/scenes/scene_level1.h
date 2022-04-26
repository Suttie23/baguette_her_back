#pragma once

#include "engine.h"

class Level1Scene : public Scene {
private:
	bool _isPaused = false;
public:
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;
};
