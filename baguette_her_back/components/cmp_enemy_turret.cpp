#include "cmp_enemy_turret.h"
#include "cmp_bullet.h"
#include "cmp_hurt_player.h"
#include "engine.h"
#include <SFML/Graphics/CircleShape.hpp>
using namespace std;
using namespace sf;

void EnemyTurretComponent::update(double dt) {
	if (auto pl = _player.lock()) {
		// Range of engagement
		if (length(pl->getPosition() - _parent->getPosition()) < 500.0)
			_firetime -= dt;

	}

	if (_firetime <= 0.f) {
		fire();
		_firetime = 3.f;
	}
}

void EnemyTurretComponent::fire() const {
  auto bullet = _parent->scene->makeEntity();
  bullet->setPosition(_parent->getPosition());
  bullet->addComponent<HurtComponent>();
  bullet->addComponent<BulletComponent>();
  auto s = bullet->addComponent<ShapeComponent>();

  s->setShape<sf::CircleShape>(8.f);
  s->getShape().setFillColor(Color::Red);
  s->getShape().setOrigin(Vector2f(4.f, 4.f));
  auto p = bullet->addComponent<PhysicsComponent>(true, Vector2f(10.f, 10.f));
  p->setRestitution(0.0f);
  p->setFriction(.001f);
  p->setMass(0.005);

  if (auto pl = _player.lock()) {

	  Vector2f direction(pl->getPosition().x - _parent->getPosition().x, pl->getPosition().y - _parent->getPosition().y);
	  //cout << direction << endl;
	  float length_vector = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
	  p->impulse(direction / length_vector * _speed);

  }
}

void EnemyTurretComponent::setSpeed(float& speed) { _speed = speed; }

EnemyTurretComponent::EnemyTurretComponent(Entity* p)
    : Component(p), _firetime(3.f), _player(_parent->scene->ents.find("player")[0]), _speed(15.0f) {

}