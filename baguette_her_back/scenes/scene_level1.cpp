#include "scene_level1.h"
#include "../game.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_enemy_turret.h"
#include "../components/cmp_life.h"
#include "../components/cmp_hurt_player.h"
#include <SFML\Graphics\View.hpp>
#include <LevelSystem.h>
#include "system_renderer.h"
#include <iostream>
#include <thread>
#include <vector>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> life;
static shared_ptr<Entity> enemy;
static shared_ptr<Texture> _texture;
static shared_ptr<Texture> background_text;

void Level1Scene::Load() {
  cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/level_1.txt", 38.0f);

  auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
  ls::setOffset(Vector2f(0, 0));

  // Background
  background_text = make_shared<Texture>();
  background_text->loadFromFile("res/sprites/game_bg.png");
  Sprite bg(*background_text);
  bg.setPosition(Vector2f(0, 2));
  setBackground(bg);

  // Create player
  {
      player = makeEntity();
      player->addTag("player");
      player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
      _texture = make_shared<Texture>();
      _texture->loadFromFile("res/sprites/jaque_right.png");
      auto s = player->addComponent<AnimatedSpriteComponent>(_texture.get(), Vector2u(3, 3), 0.1f);
      s->setTextureRect(_texture);
      s->getSprite().setOrigin(Vector2f(32.f, 32.f));


      player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
      player->addComponent<LifeComponent>(3);


  }

  // Create enemy
  {
      enemy = makeEntity();
      enemy->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]));
      auto e = enemy->addComponent<ShapeComponent>();
      e->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
      e->getShape().setFillColor(Color::Magenta);
      e->getShape().setOrigin(Vector2f(10.f, 15.f));

      enemy->addComponent<EnemyTurretComponent>();

  }

  // Player health
  {
      Texture h;
      h.loadFromFile("res/sprites/life.png");
      life = makeEntity();
      life->addTag("life");
      auto li = life->addComponent<RepeatedSpriteComponent>(player->get_components<LifeComponent>()[0]->getLives());
      li->setSprite(Sprite(*(li->setTexture(h))));
      life->setPosition(Vector2f(player->getPosition().x, player->getPosition().y));
  }

  // Pause Menu
  {

  }

  // Add physics colliders to level tiles.
  {
      // WALL COLLIDERS
      {
          auto walls = ls::findTiles(ls::WALL);
          for (auto w : walls) {
              auto pos = ls::getTilePosition(w);
              pos += Vector2f(20.f, 20.f); //offset to center
              auto e = makeEntity();
              e->setPosition(pos);
              e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
          }
      }

      // HAZARD COLLIDERS
      {
          auto hazards = ls::findTiles(ls::HAZZARD);
          for (auto h : hazards) {
              auto pos = ls::getTilePosition(h);
              pos += Vector2f(20.f, 20.f); //offset to center
              auto e = makeEntity();
              e->setPosition(pos);
              e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
              e->addComponent<HurtComponentHazard>();
          }
      }

  }

  //Simulate long loading times
  //std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  cout << " Scene 1 Load Done" << endl;

  setLoaded(true);
}

void Level1Scene::UnLoad() {
  cout << "Scene 1 Unload" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {

    Renderer::view.setSize(Vector2f(Engine::getWindowSize()) * Vector2f(0.5f, 0.55f));
    Renderer::view.setCenter(Vector2f(player->getPosition().x, player->getPosition().y - 50));


  // If the player is at the end of the level, change to next scene
  if (ls::getTileAt(player->getPosition()) == ls::END) {
    Engine::ChangeScene((Scene*)&menu);
  }
  else {
      // Cheaty way to keep GUI on screen with player for testing. TODO: Create seperate view for UI
      life->setPosition(Vector2f(player->getPosition().x - 300.f, player->getPosition().y - 250.f));
  }

  if (sf::Keyboard::isKeyPressed(Keyboard::Escape)) {
      _isPaused = true;
      Engine::pausePhysics(_isPaused);
  }

  if (sf::Keyboard::isKeyPressed(Keyboard::Tab)) {
      _isPaused = false;
      Engine::pausePhysics(_isPaused);
  }

  // If the player is dead, game over.
  if (!player->isAlive()) {     
      Engine::ChangeScene((Scene*)&menu);
      return;
  }

  if (!_isPaused) {
      Scene::Update(dt);
  }
}

void Level1Scene::Render() {

    auto& window = Engine::GetWindow();

    if (_background != nullptr) {
       window.draw(*_background);
   }

  ls::render(Engine::GetWindow());

  Scene::Render();
}
