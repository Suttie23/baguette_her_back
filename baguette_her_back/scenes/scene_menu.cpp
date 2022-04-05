#include "scene_menu.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include "../components/cmp_blinking.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include "SFML/Graphics.hpp"
#include <SFML\Audio\Music.hpp>
#include <iostream>

using namespace std;
using namespace sf;

static shared_ptr<Texture> _texture;

void MenuScene::Load() {

    if (!this->menuTheme.openFromFile("res/music/new_york_baguette.mp3"))
        cout << "Error: we not found music file";
    this->menuTheme.setLoop(true);
    this->menuTheme.setVolume(50);
    this->menuTheme.play();

    setLoaded(true);
    cout << "Menu Load \n";
    {

        auto background = makeEntity();
        auto bg = background->addComponent<SpriteComponent>();
        _texture = make_shared<Texture>();
        _texture->loadFromFile("res/menu/test_bg.png");
        bg->setTexture(_texture);
        background->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.0f, 0.0f));

        auto logo = makeEntity();
        auto lo = logo->addComponent<SpriteComponent>();
        _texture = make_shared<Texture>();
        _texture->loadFromFile("res/menu/game_logo.png");
        lo->setTexture(_texture);
        logo->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.34f, 0.05f));

        auto playGame = makeEntity();
        auto pg = playGame->addComponent<SpriteComponent>();
        _texture = make_shared<Texture>();
        _texture->loadFromFile("res/menu/play_button_norm.png");
        pg->setTexture(_texture);
        playGame->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.45f, 0.45f));

        auto settings = makeEntity();
        auto se = settings->addComponent<SpriteComponent>();
        _texture = make_shared<Texture>();
        _texture->loadFromFile("res/menu/settings_button_norm.png");
        se->setTexture(_texture);
        settings->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.45f, 0.55f));

        auto quit = makeEntity();
        auto qu = quit->addComponent<SpriteComponent>();
        _texture = make_shared<Texture>();
        _texture->loadFromFile("res/menu/quit_button_norm.png");
        qu->setTexture(_texture);
        quit->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.45f, 0.65f));



        // Menu pointer to indicate which option the user is hovering over
        auto pointer = makeEntity();
        pointer->addTag("pointer");
        pointer->addComponent<BlinkComponent>(0.5f);
        auto s = pointer->addComponent<ShapeComponent>();
        pointer->setPosition(playGame->getPosition() - Vector2f(20.f, -(20.f)));
        s->setShape<sf::RectangleShape>(Vector2f(10.f, 10.f));
        s->getShape().setFillColor(Color::Cyan);
        s->getShape().setOrigin(Vector2f(5.f, 5.f));
        menuIndex.push_back(playGame->getPosition());
        menuIndex.push_back(settings->getPosition());
        menuIndex.push_back(quit->getPosition());
        for (int i = 0; i < menuIndex.size(); i++) {
            cout << menuIndex[i] << endl;
        }
        
      
    }
}

void MenuScene::Update(const double& dt) {
    
    static int curIndex = 0;
    // Countdown should stop the pointer from warping to ludicrous speed when navigating
    static float countdown = 0.0f;
    countdown -= dt;

    // Keyboard controls for the menu (Up and Down)
    if (sf::Keyboard::isKeyPressed(Keyboard::Down)) {
        if (curIndex < (menuIndex.size() - 1) && countdown <= 0) {
            countdown = 0.25f;
            curIndex++;
            cout << curIndex << endl;
            cout << menuIndex[curIndex] << endl;
            ents.find("pointer")[0]->setPosition(menuIndex[curIndex] - Vector2f(20.f, -20.f));
        }
    }
    if (sf::Keyboard::isKeyPressed(Keyboard::Up)) {
        if (curIndex > 0 && countdown <= 0) {
            countdown = 0.2f;
            curIndex--;
            ents.find("pointer")[0]->setPosition(menuIndex[curIndex] - Vector2f(20.f, -20.f));

        }
    }

    // Keyboard controls for the menu (Select)
    if (sf::Keyboard::isKeyPressed(Keyboard::Enter)) {
        if (ents.find("pointer")[0]->getPosition() == menuIndex[0] - Vector2f(20.f, -20.f)) { // Play Game
            Engine::ChangeScene(&level1);
            this->menuTheme.stop();
        }
        else if (ents.find("pointer")[0]->getPosition() == menuIndex[1] - Vector2f(20.f, -20.f)) { // Options
            //Engine::ChangeScene(&optionScene)
        }
        else if (ents.find("pointer")[0]->getPosition() == menuIndex[2] - Vector2f(20.f, -20.f)) {// Exit
            Engine::GetWindow().close();
        }
    }

    Scene::Update(dt);
}