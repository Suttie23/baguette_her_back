#include "scene_controls.h"
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

void ControlsScene::Load() {

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
        _texture->loadFromFile("res/menu/controls_title.png");
        lo->setTexture(_texture);
        logo->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.34f, 0.05f));

    }

    setLoaded(true);
    cout << "Controls Load \n";
}

void ControlsScene::Update(const double& dt) {
    
    if (sf::Keyboard::isKeyPressed(Keyboard::Escape)) {
        Engine::ChangeScene((Scene*)&menu);
    }

    Scene::Update(dt);
}

void ControlsScene::UnLoad() {
    cout << "Controls Unload" << endl;
    Scene::UnLoad();
}