#include "scene_level1.h"
#include "../game.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_enemy_turret.h"
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
static shared_ptr<Texture> glass_text;

void Level1Scene::Load() {
  cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/level_1.txt", 38.0f);

  auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
  ls::setOffset(Vector2f(0, ho));

  // Background
  background_text = make_shared<Texture>();
  background_text->loadFromFile("res/menu/menu_bg.png");
  Sprite bg(*background_text);
  bg.setPosition(Vector2f(0, 0));
  setBackground(bg);

  // Foreground (glass)
  glass_text = make_shared<Texture>();
  glass_text->loadFromFile("res/menu/glasstest.png");
  Sprite gl(*glass_text);
  gl.setPosition(Vector2f(0, 0));
  setForeground(gl);

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


  }

  // Player health
  {
      life = makeEntity();
      auto li = life->addComponent<SpriteComponent>();
      _texture = make_shared<Texture>();
      _texture->loadFromFile("res/sprites/healthbar_full.png");
      li->setTexture(_texture);
      life->setPosition(Vector2f(player->getPosition().x, player->getPosition().y));
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
;
  }

  // Add physics colliders to level tiles.
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
    Renderer::view.setCenter(Vector2f(player->getPosition().x, player->getPosition().y - 150));


  if (ls::getTileAt(player->getPosition()) == ls::END) {
    Engine::ChangeScene((Scene*)&menu);
  }

  // Cheaty way to keep GUI on screen with player. Would be better to create another view
  life->setPosition(Vector2f(player->getPosition().x - 300.f, player->getPosition().y - 350.f));

  Scene::Update(dt);
}

void Level1Scene::Render() {

    auto& window = Engine::GetWindow();

    if (_background != nullptr) {
       window.draw(*_background);
   }

  ls::render(Engine::GetWindow());


  //if (_foreground != nullptr) {
   //   window.draw(*_foreground);
 // }

  Scene::Render();
}
