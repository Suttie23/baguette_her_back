#include "move_vertical.h"
#include "cmp_hurt_player.h"

using namespace std;
using namespace sf;

void VerticalComponent::update(double dt) {
    auto mov = _direction * (float)(dt * _speed);
    mov.y += _direction.y * 16.f;
    if (!validMove(_parent->getPosition() + mov)) {
        _direction *= -1.f;
    }

    move(_direction * (float)(dt * _speed));
    ActorMovementComponent::update(dt);
}

VerticalComponent::VerticalComponent(Entity* p) : ActorMovementComponent(p) {
    _direction = Vector2f(0, 1.0f);
    _speed = 100.0f;
}