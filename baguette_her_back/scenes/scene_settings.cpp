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
        logo->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.34f, 0.05f));

 
    }

    setLoaded(true);
    cout << "Settings Load \n";
}

void SettingsScene::Update(const double& dt) {
    
    if (sf::Keyboard::isKeyPressed(Keyboard::Escape)) {
        Engine::ChangeScene(&menu);
    }

    Scene::Update(dt);
}

void SettingsScene::UnLoad() {
    cout << "Settings Unload" << endl;
    Scene::UnLoad();
}