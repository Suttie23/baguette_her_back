#include "cmp_life.h"
#include "cmp_blinking.h"
#include "engine.h"
#include "cmp_sprite.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <SFML/Audio.hpp>

sf::SoundBuffer damagebuffer;

sf::SoundBuffer damagebuffer;

void LifeComponent::update(double dt)
{
	//If lives are below 1, kill the player
	static sf::Clock clock;
	if (_lives < 1)
		_parent->setForDelete();
	if (_damaged) {
		clock.restart();
		_damaged = false;
		Engine::getActiveScene()->ents.find("life")[0]->get_components<RepeatedSpriteComponent>()[0]->decreaseRep();
	}
	// ensure that player has a small damage buffer after taking damage
	if (_state == "invincible")
	{
		float dt = clock.restart().asSeconds();
		static float timer = 0.0f;
		timer += dt;
		
		if (timer > _invincibleTime){
			_state = "vulnerable";
			timer = 0.0f;
			_parent->get_components<BlinkComponent>()[0]->_fordeletion = true;
			_parent->setVisible(true);
			
		}
	}
}

int LifeComponent::getLives()
{
	return _lives;
}

void LifeComponent::setLives( int lives )
{
	_lives = lives;
	Engine::getActiveScene()->ents.find( "life" )[0]->get_components<RepeatedSpriteComponent>()[0]->setRep( _lives );
}

void LifeComponent::reduceLives() { 
	if(_state != "invincible"){
		_lives --;
		_state = "invincible";
		_damaged = true;
		_parent->addComponent<BlinkComponent>(0.3f);
		std::cout << _lives << std::endl;
		if (!damagebuffer.loadFromFile("res/sfx/damage_sound.ogg")) {
			std::cout << "ERROR" << std::endl;
		}
		this->damage.setBuffer(damagebuffer);
		this->damage.setVolume(15);
		this->damage.play();
	} 	
}

void LifeComponent::increaseLives() { _lives++; }

LifeComponent::LifeComponent(Entity * p, int lives) 
	: Component(p), _lives(lives), _state("vulnerable"), _invincibleTime(3.0f), _damaged(false)
{
}
