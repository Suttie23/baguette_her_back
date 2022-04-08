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
sf::SoundBuffer buffer;

void MenuScene::Load() {

    if (!this->menuTheme.openFromFile("res/music/new_york_baguette.mp3"))
        cout << "Error: we not found music file";
    this->menuTheme.setLoop(true);
    this->menuTheme.setVolume(50);
    this->menuTheme.play();

    if (!buffer.loadFromFile("res/sfx/menu_crunch.ogg")) {
        std::cout << "ERROR" << std::endl;
    }
        this->crunch.setBuffer(buffer);
        this->crunch.setVolume(15);

    {

        // Background Entity
        auto background = makeEntity();
        auto bg = background->addComponent<SpriteComponent>();
        _texture = make_shared<Texture>();
        _texture->loadFromFile("res/menu/menu_bg.png");
        bg->setTexture(_texture);
        background->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.0f, 0.0f));

        // Logo Entity
        auto logo = makeEntity();
        auto lo = logo->addComponent<SpriteComponent>();
        _texture = make_shared<Texture>();
        _texture->loadFromFile("res/menu/game_logo.png");
        lo->setTexture(_texture);
        logo->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.33f, 0.05f));

        // Play Button Entity
        auto playGame = makeEntity();
        playGame->addTag("playButton");
        auto pg = playGame->addComponent<SpriteComponent>();
        _texture = make_shared<Texture>();
        _texture->loadFromFile("res/menu/play_button.png");
        pg->setTexture(_texture);
        playGame->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.45f, 0.45f));

        // Settings Button Entity
        auto settings = makeEntity();
        settings->addTag("settingsButton");
        auto se = settings->addComponent<SpriteComponent>();
        _texture = make_shared<Texture>();
        _texture->loadFromFile("res/menu/settings_button.png");
        se->setTexture(_texture);
        settings->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.45f, 0.55f));

        // Controls Button Entity
        auto controls = makeEntity();
        controls->addTag("settingsButton");
        auto ct = controls->addComponent<SpriteComponent>();
        _texture = make_shared<Texture>();
        _texture->loadFromFile("res/menu/controls_button.png");
        ct->setTexture(_texture);
        controls->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.45f, 0.65f));

        // Quit Button Entity
        auto quit = makeEntity();
        quit->addTag("quitButton");
        auto qu = quit->addComponent<SpriteComponent>();
        _texture = make_shared<Texture>();
        _texture->loadFromFile("res/menu/quit_button.png");
        qu->setTexture(_texture);
        quit->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.45f, 0.75f));

        // Menu indicator to highlight the button selected
        auto indicator = makeEntity();
        indicator->addTag("indicator");
        auto s = indicator->addComponent<ShapeComponent>();
        indicator->setPosition(playGame->getPosition() - Vector2f(-4.15f, (-7.f)));
        s->setShape<sf::RectangleShape>(Vector2f(132.f, 40.f));
        s->getShape().setFillColor(Color(220, 140, 44, 128));
        s->getShape().setOrigin(Vector2f(5.f, 5.f));
        menuIndex.clear(); // Ensures that the index does not stack each time the level is loaded
        menuIndex.push_back(playGame->getPosition());
        menuIndex.push_back(settings->getPosition());
        menuIndex.push_back(controls->getPosition());
        menuIndex.push_back(quit->getPosition());
        for (int i = 0; i < menuIndex.size(); i++) {
            cout << menuIndex[i] << endl;
        }
           
    }

    // Weird bug, having this commented out stops the menu from breaking as much?
    //setLoaded(true);
    //cout << "Menu Load \n";
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
            ents.find("indicator")[0]->setPosition(menuIndex[curIndex] - Vector2f(-4.15f, (-7.f)));
            this->crunch.play();
        }
    }
    if (sf::Keyboard::isKeyPressed(Keyboard::Up)) {
        if (curIndex > 0 && countdown <= 0) {
            countdown = 0.2f;
            curIndex--;
            cout << curIndex << endl;
            cout << menuIndex[curIndex] << endl;
            ents.find("indicator")[0]->setPosition(menuIndex[curIndex] - Vector2f(-4.15f, (-7.f)));
            this->crunch.play();
        }
    }

    // Keyboard controls for the menu (Enter)
    if (sf::Keyboard::isKeyPressed(Keyboard::Enter)) {
        if (ents.find("indicator")[0]->getPosition() == menuIndex[0] - Vector2f(-4.15f, (-7.f))) { // Play Game
            Engine::ChangeScene(&level1);
            this->menuTheme.stop();
        }
        else if (ents.find("indicator")[0]->getPosition() == menuIndex[1] - Vector2f(-4.15f, (-7.f))) { // Options
            Engine::ChangeScene(&settings);
        }
        else if (ents.find("indicator")[0]->getPosition() == menuIndex[2] - Vector2f(-4.15f, (-7.f))) {// Exit
            MenuScene::UnLoad();
            Engine::ChangeScene(&controls);
        }
        else if (ents.find("indicator")[0]->getPosition() == menuIndex[3] - Vector2f(-4.15f, (-7.f))) {// Exit
            Engine::GetWindow().close();
        }
    }

    Scene::Update(dt);
}
