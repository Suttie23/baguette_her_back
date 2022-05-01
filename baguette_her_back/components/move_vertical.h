#pragma once

#include "cmp_actor_movement.h"

class VerticalComponent : public ActorMovementComponent {
protected:
	sf::Vector2f _direction;

public:
	void update(double dt) override;

	explicit VerticalComponent(Entity* p);

	VerticalComponent() = delete;
};
