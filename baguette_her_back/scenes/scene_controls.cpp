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
        background->setPosition(Vector2f(0.0f, 0.0f));

        auto logo = makeEntity();
        auto lo = logo->addComponent<SpriteComponent>();
        _texture = make_shared<Texture>();
        _texture->loadFromFile("res/menu/controls_title.png");
        lo->setTexture(_texture);
        logo->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.33f, 0.05f));

        auto heading = makeEntity();
        auto h = heading->addComponent<TextComponent>(
            "KEY REBINDING");
        heading->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.42f, 0.45f));

        auto instructions = makeEntity();
        auto i = instructions->addComponent<TextComponent>(
            "TO REBIND - GO TO: bin/debug/config/user preferences");
        instructions->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.10f, 0.55f));

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
