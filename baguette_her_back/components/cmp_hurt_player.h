#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class HurtComponent : public Component {
protected:
  void fire() const;
  std::weak_ptr<Entity> _player;

public:
  void update(double dt) override;
  void render() override {}
  explicit HurtComponent(Entity* p);
  HurtComponent() = delete;
};

class HurtComponentHazard : public Component {
protected:
	void fire() const;
	std::weak_ptr<Entity> _player;

public:
	void update(double dt) override;
	void render() override {}
	explicit HurtComponentHazard(Entity* p);
	HurtComponentHazard() = delete;
};

class HurtComponentBarrier : public Component {
protected:
	void fire() const;
	std::weak_ptr<Entity> _player;

public:
	void update(double dt) override;
	void render() override {}
	explicit HurtComponentBarrier(Entity* p);
	HurtComponentBarrier() = delete;
};

class HurtComponentRat : public Component {
protected:
	void fire() const;
	std::weak_ptr<Entity> _player;

public:
	void update(double dt) override;
	void render() override {}
	explicit HurtComponentRat(Entity* p);
	HurtComponentRat() = delete;
};
