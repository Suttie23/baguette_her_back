#include "scene_level1.h"
#include "../game.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/constant_follow.h"
#include "../components/cmp_enemy_turret.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/move_horizontal.h"
#include "../components/move_vertical.h"
#include "../components/cmp_life.h"
#include "../components/cmp_hurt_player.h"
#include <SFML\Graphics\View.hpp>
#include <SFML\Audio\Music.hpp>
#include <LevelSystem.h>
#include "system_renderer.h"
#include "system_physics.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>

using namespace std;
using namespace sf;

sf::SoundBuffer level1buffer;

// Textures
static shared_ptr<Texture> _texture;
static shared_ptr<Texture> background_text;
static shared_ptr<Texture> _buttonDefault;
static shared_ptr<Texture> _buttonSelected;

// Player, Enemies & Level
static shared_ptr<Entity> player;
static shared_ptr<Entity> enemy;
static shared_ptr<Entity> floorTile;

// UI
static shared_ptr<Entity> life;
static shared_ptr<Entity> menuBackground;
static shared_ptr<Entity> menuContinueButton;
static shared_ptr<Entity> menuLoadButton;
static shared_ptr<Entity> menuSaveButton;
static shared_ptr<Entity> menuMenuButton;
static shared_ptr<Entity> indicator;

// Sauce Frames
std::vector<sf::Vector2u>& GetSauceFrames() {
    static bool isSetup = false;
    static std::vector<sf::Vector2u> sauceFrames;

    if (!isSetup)
    {
        isSetup = true;
        sauceFrames.push_back(Vector2u(0, 0));
        sauceFrames.push_back(Vector2u(0, 1));
        sauceFrames.push_back(Vector2u(0, 2));
    }

    return sauceFrames;
}

void Level1Scene::Load() {

    if (!this->levelTrack.openFromFile("res/music/swing_time.mp3"))
        std::cout << "Error: we not found music file";
    this->levelTrack.setLoop(true);
    this->levelTrack.setVolume(15);
    this->levelTrack.play();

  std::cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/level_1.txt", 38.0f);

  auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
  ls::setOffset(Vector2f(0, 0));


  // Background
  background_text = make_shared<Texture>();
  background_text->loadFromFile("res/level_assets/game_bg.png");
  Sprite bg(*background_text);
  bg.setPosition(Vector2f(0, 2));
  setBackground(bg);

  
  // Add physics colliders + textures to level tiles.
  {
      // WALL COLLIDERS
      {
          _texture = make_shared<Texture>();
          _texture->loadFromFile("res/level_assets/wall.png");
          auto walls = ls::findTiles(ls::WALL);
          for (auto w : walls) {
              auto pos = ls::getTilePosition(w);
              pos += Vector2f(20.f, 20.f); //offset to center
              auto e = makeEntity();
              e->setPosition(pos);
              auto s = e->addComponent<SpriteComponent>();
              s->setOrigin(20.f, 20.f);
              s->setTexture(_texture);
              auto phy = e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
          }
      }
     
      // FLOOR COLLIDERS
      {
          _texture = make_shared<Texture>();
          _texture->loadFromFile("res/level_assets/floor.png");
          auto floor = ls::findTiles(ls::FLOOR);
          for (auto w : floor) {
              auto pos = ls::getTilePosition(w);
              pos += Vector2f(20.f, 20.f); //offset to center
              auto e = makeEntity();
              e->setPosition(pos);
              auto s = e->addComponent<SpriteComponent>();
              s->setOrigin(20.f, 20.f);
              s->setTexture(_texture);
              auto phy = e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));

          }
      }
  }

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

      Renderer::view.setSize(Vector2f(Engine::getWindowSize()) * Vector2f(0.5f, 0.55f));
      Renderer::view.setCenter(Vector2f(player->getPosition().x, player->getPosition().y - 50));    
  }
  
  // Create Verticle Enemy
  {
      auto movingBarrier = ls::findTiles(ls::VERT_ENEMY);
      for (auto mb : movingBarrier) {
          Vector2f pos = ls::getTilePosition(mb);
          auto mb = makeEntity();
          mb->setPosition(pos);
          _texture = make_shared<Texture>();
          _texture->loadFromFile("res/level_assets/spike_barrier.png");
          auto s = mb->addComponent<SpriteComponent>();
          s->setTexture(_texture);
          s->getSprite().setOrigin(Vector2f(10.f, 15.f));
          mb->addComponent<VerticalComponent>();
          mb->addComponent<HurtComponentBarrier>();
      }

  }

  // Create Horizontal Enemy
  {
      auto rat = ls::findTiles(ls::HORIZ_ENEMY);
      for (auto r : rat) {
          Vector2f pos = ls::getTilePosition(r);
          auto r = makeEntity();
          r->setPosition(pos);
          _texture = make_shared<Texture>();
          _texture->loadFromFile("res/sprites/rat.png");
          auto s = r->addComponent<SpriteComponent>(); // SHOULD BE ANIMATED, BUT HAVE NO TIME
          s->setTexture(_texture);
          s->getSprite().setOrigin(Vector2f(10.f, -10.f));
          r->addComponent<HorizontalComponent>();
          r->addComponent<HurtComponentBarrier>();
      }

  }

  //Create a turret enemy

  {
      auto angrybread = ls::findTiles(ls::TURRET);
      for (auto ab : angrybread) {
          Vector2f pos = ls::getTilePosition(ab);
          auto ab = makeEntity();
          ab->setPosition(pos);
          _texture = make_shared<Texture>();
          _texture->loadFromFile("res/sprites/angry_bread1.png");
          auto s = ab->addComponent<SpriteComponent>(); // SHOULD BE ANIMATED, BUT HAVE NO TIME
          s->setTexture(_texture);
          s->getSprite().setOrigin(Vector2f(10.f, 2.f));
          ab->addComponent<EnemyTurretComponent>();

      }

  }

  //Create door texture
  {
      auto door = ls::findTiles(ls::END);
      for (auto d : door) {
          Vector2f pos = ls::getTilePosition(d);
          auto d = makeEntity();
          d->setPosition(pos);
          _texture = make_shared<Texture>();
          _texture->loadFromFile("res/level_assets/door.png");
          auto s = d->addComponent<SpriteComponent>();
          s->setTexture(_texture);
          s->getSprite().setOrigin(Vector2f(0.f, 1.f));
      }

  }
   */

      // HAZARD COLLIDERS
      {
          auto hazards = ls::findTiles(ls::HAZARD);
          for (auto h : hazards) {
              auto pos = ls::getTilePosition(h);
              pos += Vector2f(20.f, 20.f); //offset to center
              auto e = makeEntity();
              e->setPosition(pos);
              _texture = make_shared<Texture>();
              _texture->loadFromFile("res/level_assets/sauce.png");
              auto s = e->addComponent<AnimatedSpriteComponent>(_texture.get(), Vector2u(1, 3), 0.2f);
              e->addComponent<HurtComponentHazard>();
              s->setTextureRect(_texture);
              s->getSprite().setOrigin(Vector2f(20.f, 32.f));
              auto phy = e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
              s->SetFrames(GetSauceFrames());
          }
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

      // Pause Menu Background
      menuBackground = makeEntity();
      auto mbg = menuBackground->addComponent<SpriteComponent>();
      _texture = make_shared<Texture>();
      _texture->loadFromFile("res/pause/pause_menu_background.png");
      mbg->setTexture(_texture);
      menuBackground->setVisible(false);

      // Pause Menu Continue Button
      menuContinueButton = makeEntity();
      auto mcb = menuContinueButton->addComponent<ButtonComponent>();
      _texture = make_shared<Texture>();
      _texture->loadFromFile("res/pause/continue_button_selected.png");
      mcb->setTexture(_texture);
      menuContinueButton->setVisible(false);

      // Pause Menu Load Button
      menuLoadButton = makeEntity();
      auto mlb = menuLoadButton->addComponent<ButtonComponent>();
      _texture = make_shared<Texture>();
      _texture->loadFromFile("res/pause/load_button.png");
      mlb->setTexture(_texture);
      menuLoadButton->setVisible(false);

      // Pause Menu Save Button
      menuSaveButton = makeEntity();
      auto msb = menuSaveButton->addComponent<ButtonComponent>();
      _texture = make_shared<Texture>();
      _texture->loadFromFile("res/pause/save_button.png");
      msb->setTexture(_texture);
      menuSaveButton->setVisible(false);

      // Pause Menu Save Button
      menuMenuButton = makeEntity();
      auto mmb = menuMenuButton->addComponent<ButtonComponent>();
      _texture = make_shared<Texture>();
      _texture->loadFromFile("res/pause/menu_button.png");
      mmb->setTexture(_texture);
      menuMenuButton->setVisible(false);

      // Menu indicator to highlight the button selected
      indicator = makeEntity();
      indicator->addTag("indicator");
      auto s = indicator->addComponent<ShapeComponent>();
      s->setShape<sf::RectangleShape>(Vector2f(125.f, 40.f));
      s->getShape().setFillColor(Color(220, 140, 44, 128));
      s->getShape().setOrigin(Vector2f(5.f, 5.f));
      indicator->setVisible(false);

  }

  std::cout << " Scene 1 Load Done" << endl;

  setLoaded(true);

}



void Level1Scene::UnLoad() {
  std::cout << "Scene 1 Unload" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {


    Renderer::view.setSize(Vector2f(Engine::getWindowSize()) * Vector2f(0.5f, 0.55f));
    Renderer::view.setCenter(Vector2f(player->getPosition().x, player->getPosition().y - 50));

    //std::std::cout << player->getPosition().x << std::endl;
    //std::std::cout << player->getPosition().y << std::endl;
  // If the player is dead, game over. 
  if (!player->isAlive()) {     
      this->levelTrack.stop();
      Engine::ChangeScene((Scene*)&menu);
      return;
  }

  // If the player is at the end of the level, change to next scene
  if (ls::getTileAt(player->getPosition()) == ls::END) {
      Engine::ChangeScene((Scene*)&menu);
      levelTrack.stop();
  }
  else {

      // Cheaty way to keep GUI on screen with player for testing. TODO: Create seperate view for UI
      {
          life->setPosition(Vector2f(player->getPosition().x - 300.f, player->getPosition().y - 250.f));

          // Scuffed and very ineffcient way of keeping the pause menu in the center of the screen. Should use a view instead, but it works
          menuBackground->setPosition(Vector2f(Renderer::view.getCenter().x - 135, Renderer::view.getCenter().y - 135));
          menuContinueButton->setPosition(Vector2f(menuBackground->getPosition().x + 60, menuBackground->getPosition().y + 110));
          menuLoadButton->setPosition(Vector2f(menuBackground->getPosition().x + 60, menuBackground->getPosition().y + 160));
          menuSaveButton->setPosition(Vector2f(menuBackground->getPosition().x + 60, menuBackground->getPosition().y + 210));
          menuMenuButton->setPosition(Vector2f(menuBackground->getPosition().x + 60, menuBackground->getPosition().y + 260));

          // Ensures that the index does not stack each time the game is paused
          menuIndex.clear();
          menuIndex.push_back(menuContinueButton->getPosition());
          menuIndex.push_back(menuLoadButton->getPosition());
          menuIndex.push_back(menuSaveButton->getPosition());
          menuIndex.push_back(menuMenuButton->getPosition());
      }
  }

  // Display Pause menu
  if (sf::Keyboard::isKeyPressed(Keyboard::Escape)) {
      _isPaused = true;
      Engine::pausePhysics(_isPaused);
      menuBackground->setVisible(true);
      menuContinueButton->setVisible(true);
      menuLoadButton->setVisible(true);
      menuSaveButton->setVisible(true);
      menuMenuButton->setVisible(true);
      indicator->setVisible(false);

  }

  if (_isPaused) {

      static int curIndex = 0;
      // Countdown should stop the pointer from warping to ludicrous speed when navigating
      static float countdown = 0.0f;
      countdown -= dt;

      ents.find("indicator")[0]->setPosition(menuIndex[curIndex] - Vector2f(-4.15f, (-7.f)));
      _buttonDefault = make_shared<Texture>();
      _buttonSelected = make_shared<Texture>();

      if (sf::Keyboard::isKeyPressed(Keyboard::Down)) {
          if (curIndex < (menuIndex.size() - 1) && countdown <= 0) {
              countdown = 0.25f;
              curIndex++;
              std::cout << curIndex << endl;
              std::cout << menuIndex[curIndex] << endl;
              ents.find("indicator")[0]->setPosition(menuIndex[curIndex] - Vector2f(-4.15f, (-7.f)));

              if (curIndex == 1) {
                  _buttonSelected->loadFromFile("res/pause/load_button_selected.png");
                  menuLoadButton->GetCompatibleComponent<ButtonComponent>()[0]->setTexture(_buttonSelected);
                  _buttonDefault->loadFromFile("res/pause/continue_button.png");
                  menuContinueButton->GetCompatibleComponent<ButtonComponent>()[0]->setTexture(_buttonDefault);
              }
              else if (curIndex == 2) {
                  _buttonSelected->loadFromFile("res/pause/save_button_selected.png");
                  menuSaveButton->GetCompatibleComponent<ButtonComponent>()[0]->setTexture(_buttonSelected);
                  _buttonDefault->loadFromFile("res/pause/load_button.png");
                  menuLoadButton->GetCompatibleComponent<ButtonComponent>()[0]->setTexture(_buttonDefault);
              }
              else if (curIndex == 3) {
                  _buttonSelected->loadFromFile("res/pause/menu_button_selected.png");
                  menuMenuButton->GetCompatibleComponent<ButtonComponent>()[0]->setTexture(_buttonSelected);
                  _buttonDefault->loadFromFile("res/pause/save_button.png");
                  menuSaveButton->GetCompatibleComponent<ButtonComponent>()[0]->setTexture(_buttonDefault);
              }
          }
      }

      if (sf::Keyboard::isKeyPressed(Keyboard::Up)) {
          if (curIndex > 0 && countdown <= 0) {
              countdown = 0.2f;
              curIndex--;
              std::cout << curIndex << endl;
              std::cout << menuIndex[curIndex] << endl;
              ents.find("indicator")[0]->setPosition(menuIndex[curIndex] - Vector2f(-4.15f, (-7.f)));

              if (curIndex == 0) {
                  _buttonSelected->loadFromFile("res/pause/continue_button_selected.png");
                  menuContinueButton->GetCompatibleComponent<ButtonComponent>()[0]->setTexture(_buttonSelected);
                  _buttonDefault->loadFromFile("res/pause/load_button.png");
                  menuLoadButton->GetCompatibleComponent<ButtonComponent>()[0]->setTexture(_buttonDefault);
              }
              else if (curIndex == 1) {
                  _buttonSelected->loadFromFile("res/pause/load_button_selected.png");
                  menuLoadButton->GetCompatibleComponent<ButtonComponent>()[0]->setTexture(_buttonSelected);
                  _buttonDefault->loadFromFile("res/pause/save_button.png");
                  menuSaveButton->GetCompatibleComponent<ButtonComponent>()[0]->setTexture(_buttonDefault);
              }
              else if (curIndex == 2) {
                  _buttonSelected->loadFromFile("res/pause/save_button_selected.png");
                  menuSaveButton->GetCompatibleComponent<ButtonComponent>()[0]->setTexture(_buttonSelected);
                  _buttonDefault->loadFromFile("res/pause/menu_button.png");
                  menuMenuButton->GetCompatibleComponent<ButtonComponent>()[0]->setTexture(_buttonDefault);
              }
          }
      }

      // Keyboard controls for the menu (Enter)
      if (sf::Keyboard::isKeyPressed(Keyboard::Enter)) {
          if (ents.find("indicator")[0]->getPosition() == menuIndex[0] - Vector2f(-4.15f, (-7.f))) { // Continue
              _isPaused = false;
              Engine::pausePhysics(_isPaused);
              menuBackground->setVisible(false);
              menuContinueButton->setVisible(false);
              menuLoadButton->setVisible(false);
              menuSaveButton->setVisible(false);
              menuMenuButton->setVisible(false);
              indicator->setVisible(false);
              menuIndex.clear(); // Ensures that the index does not stack each time the game is paused
          }
          else if (ents.find("indicator")[0]->getPosition() == menuIndex[1] - Vector2f(-4.15f, (-7.f))) { // Load
              _isPaused = false;
              Engine::pausePhysics(_isPaused);
              menuBackground->setVisible(false);
              menuContinueButton->setVisible(false);
              menuLoadButton->setVisible(false);
              menuSaveButton->setVisible(false);
              menuMenuButton->setVisible(false);
              indicator->setVisible(false);
              LoadGame();
              menuIndex.clear(); // Ensures that the index does not stack each time the game is paused
          }
          else if (ents.find("indicator")[0]->getPosition() == menuIndex[2] - Vector2f(-4.15f, (-7.f))) {// Save
              _isPaused = false;
              Engine::pausePhysics(_isPaused);
              menuBackground->setVisible(false);
              menuContinueButton->setVisible(false);
              menuLoadButton->setVisible(false);
              menuSaveButton->setVisible(false);
              menuMenuButton->setVisible(false);
              indicator->setVisible(false);
              SaveGame();
              menuIndex.clear(); // Ensures that the index does not stack each time the game is paused
          }
          else if (ents.find("indicator")[0]->getPosition() == menuIndex[3] - Vector2f(-4.15f, (-7.f))) {// Menu
              _isPaused = false;
              Engine::pausePhysics(_isPaused);
              menuBackground->setVisible(false);
              menuContinueButton->setVisible(false);
              menuLoadButton->setVisible(false);
              menuSaveButton->setVisible(false);
              menuMenuButton->setVisible(false);
              indicator->setVisible(false);
              Engine::ChangeScene(&menu);
              this->levelTrack.stop();
              menuIndex.clear(); // Ensures that the index does not stack each time the game is paused
          }
      }
  }
     if (!_isPaused) {
          Scene::Update(dt);
      }
  
}

const std::string savepath = "config/save/level1.save";

void Level1Scene::SaveGame()
{
    std::ofstream savefile(savepath);

    // If file is not opened, throw error
    if (!savefile.is_open())
    {
        std::cerr << "Error while opening file for writing: " << savepath << std::endl;
        return;
    }
    // Else, save data
    else
    {
        std::cerr << "Game Saved" << std::endl;

        // We save player position and player lives.
        Vector2f position = player->getPosition();
        auto lives = player->GetCompatibleComponent<LifeComponent>();
        assert(lives.size());

        savefile << position.x << " " << position.y << std::endl;
        savefile << lives[0]->getLives() << std::endl;

        return;
    }
}

void Level1Scene::LoadGame()
{
    std::ifstream savefile(savepath);

    // If file is not opened, throw error
    if (!savefile.is_open())
    {
        std::cerr << "Error while opening save file: " << savepath << std::endl;
        return;
    }

    // Else, load data
    else {
        std::cerr << "Game Loaded" << std::endl;

        Vector2f position;
        int lives;
        int score;

        // Load values from the files
        savefile >> position.x >> position.y >> lives;

        // Get lives
        auto lifecomp = player->GetCompatibleComponent<LifeComponent>();
        assert(lifecomp.size());
        lifecomp[0]->setLives(lives);

        // Get physics 
        auto body = player->GetCompatibleComponent<PhysicsComponent>();
        player->setPosition(position);
        body[0]->teleport(position);

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
