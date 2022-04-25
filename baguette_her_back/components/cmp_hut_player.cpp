#include "cmp_hurt_player.h"
#include"cmp_life.h"
#include <engine.h>

using namespace std;
using namespace sf;

// BULLET DAMAGE

void HurtComponent::update(double dt) {
  if (auto pl = _player.lock()) {
    if (length(pl->getPosition() - _parent->getPosition()) < 30.0) {
        pl->get_components<LifeComponent>()[0]->reduceLives();
       _parent->setForDelete();
    }
  }
}

HurtComponent::HurtComponent(Entity* p)
    : Component(p), _player(_parent->scene->ents.find("player")[0]) {}

// HAZARD DAMAGE

void HurtComponentHazard::update(double dt) {
    if (auto pl = _player.lock()) {
        if (length(pl->getPosition() - _parent->getPosition()) < 40.0) {
            pl->get_components<LifeComponent>()[0]->reduceLives();
            _parent->setForDelete();
        }
    }
}

HurtComponentHazard::HurtComponentHazard(Entity* p)
    : Component(p), _player(_parent->scene->ents.find("player")[0]) {}