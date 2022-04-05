#include "cmp_blinking.h"

BlinkComponent::BlinkComponent(Entity * p, float blinktimer)
	: Component(p), blinktimer(blinktimer) {}

void BlinkComponent::update(double dt)
{
	static float countdown = 0.0f;
	countdown -= dt;
	if (countdown <= 0) {
		countdown = blinktimer;
		bool cond = _parent->isVisible() ? false : true;
		_parent->setVisible(cond);
	}
	
}

void BlinkComponent::render() {}
