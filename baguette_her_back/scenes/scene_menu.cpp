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

void MenuScene::Load() {

    if (!this->menuTheme.openFromFile("res/music/new_york_baguette.mp3"))
        cout << "Error: we not found music file";
    this->menuTheme.setLoop(true);
    this->menuTheme.setVolume(50);
    this->menuTheme.play();

    setLoaded(true);
    cout << "Menu Load \n";
    {

        // Creating menu button entities and setting position
        auto title = makeEntity();
        auto t = title->addComponent<TextComponent>(
            "Baguette Her Back!");
        title->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.1f, 0.05f));

        auto playGame = makeEntity();
        auto pg = playGame->addComponent<TextComponent>(
            "Play Now");
        playGame->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.1f, 0.4f));

        auto options = makeEntity();
        auto o = options->addComponent<TextComponent>(
            "Options");
        options->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.1f, 0.5f));

        auto exit = makeEntity();
        auto e = exit->addComponent<TextComponent>(
            "Exit");
        exit->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.1f, 0.6f));

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
        menuIndex.push_back(options->getPosition());
        menuIndex.push_back(exit->getPosition());
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