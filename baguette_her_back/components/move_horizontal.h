#pragma once

#include "cmp_actor_movement.h"

class HorizontalComponent : public ActorMovementComponent {
protected:
	sf::Vector2f _direction;

public:
	void update(double dt) override;

	explicit HorizontalComponent(Entity* p);

	HorizontalComponent() = delete;
};
