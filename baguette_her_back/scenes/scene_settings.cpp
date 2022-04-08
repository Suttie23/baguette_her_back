#include "scene_settings.h"
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
static shared_ptr<Entity> resolution;
static shared_ptr<Entity> fullscreen;

void SettingsScene::Load() {


    {
        auto background = makeEntity();
        auto bg = background->addComponent<SpriteComponent>();
        _texture = make_shared<Texture>();
        _texture->loadFromFile("res/menu/menu_bg.png");
        bg->setTexture(_texture);
        background->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.0f, 0.0f));

        auto logo = makeEntity();
        auto lo = logo->addComponent<SpriteComponent>();
        _texture = make_shared<Texture>();
        _texture->loadFromFile("res/menu/settings_title.png");
        lo->setTexture(_texture);
        logo->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.33f, 0.05f));

        auto fullscreen_text = makeEntity();
        auto t = fullscreen_text->addComponent<TextComponent>(
            "Toggle Full Screen:");
        fullscreen_text->setPosition(Vector2f(425.f, 300.f));

        auto fullscreen_ON = makeEntity();
        auto fson = fullscreen_ON->addComponent<TextComponent>(
            "ON");
        fullscreen_ON->setPosition(Vector2f(875.f, 300.f));

        auto fullscreen_OFF = makeEntity();
        auto fsoff = fullscreen_OFF->addComponent<TextComponent>(
            "OFF");
        fullscreen_OFF->setPosition(Vector2f(775.f, 300.f));

        // Menu indicator to highlight the button selected
        auto indicator = makeEntity();
        indicator->addTag("indicator");
        indicator->addComponent<BlinkComponent>(0.5f);
        auto s = indicator->addComponent<ShapeComponent>();
        indicator->setPosition(fullscreen_OFF->getPosition() - Vector2f(-4.15f, (-2.f)));
        s->setShape<sf::RectangleShape>(Vector2f(60.f, 40.f));
        s->getShape().setFillColor(Color(220, 140, 44, 128));
        s->getShape().setOrigin(Vector2f(5.f, 5.f));
        menuIndex.clear();
        menuIndex.push_back(fullscreen_OFF->getPosition());
        menuIndex.push_back(fullscreen_ON->getPosition());
        for (int i = 0; i < menuIndex.size(); i++) {
            cout << menuIndex[i] << endl;
        }

    }

    setLoaded(true);
    cout << "Settings Load \n";
}

void SettingsScene::Update(const double& dt) {
    
    static int curIndex = 0;
    // Countdown should stop the pointer from warping to ludicrous speed when navigating
    static float countdown = 0.0f;
    countdown -= dt;

    // Keyboard controls for the menu (Left and Right)
    if (sf::Keyboard::isKeyPressed(Keyboard::Right)) {
        if (curIndex < (menuIndex.size() - 1) && countdown <= 0) {
            countdown = 0.25f;
            curIndex++;
            cout << curIndex << endl;
            cout << menuIndex[curIndex] << endl;
            ents.find("indicator")[0]->setPosition(menuIndex[curIndex] - Vector2f(-4.15f, (-7.f)));
        }
    }
    if (sf::Keyboard::isKeyPressed(Keyboard::Left)) {
        if (curIndex > 0 && countdown <= 0) {
            countdown = 0.2f;
            curIndex--;
            cout << curIndex << endl;
            cout << menuIndex[curIndex] << endl;
            ents.find("indicator")[0]->setPosition(menuIndex[curIndex] - Vector2f(-4.15f, (-7.f)));
        }
    }

    // Keyboard controls for the menu (Enter)
    if (sf::Keyboard::isKeyPressed(Keyboard::Enter)) {
        if (ents.find("indicator")[0]->getPosition() == menuIndex[0] - Vector2f(-4.15f, (-7.f))) { // Fullscreen OFF
            cout << "FullScreen OFF" << endl;
        }
        else if (ents.find("indicator")[0]->getPosition() == menuIndex[1] - Vector2f(-4.15f, (-7.f))) { // Fullscreen ON
            cout << "FullScreen ON" << endl;
        }
    }


    if (sf::Keyboard::isKeyPressed(Keyboard::Escape)) {
        Engine::ChangeScene(&menu);
    }

    Scene::Update(dt);
}