#pragma once
#include "ecm.h"

class BlinkComponent : public Component {
private:
	float blinktimer;
public:
	BlinkComponent() = delete;
	
	explicit BlinkComponent(Entity* p, float blinktimer);
	void update(double dt) override;
	void render() override;
};