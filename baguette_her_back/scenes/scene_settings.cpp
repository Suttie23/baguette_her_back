#include "scene_settings.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include "../components/cmp_text_list.h"
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

        auto resolution_text = makeEntity();
        auto tr = resolution_text->addComponent<TextComponent>(
            "Resolution:");
        resolution_text->setPosition(Vector2f(425.f, 400.f));

        resolution = makeEntity();
        vector<string> textlist1{ "800 x 600", "1024 x 800", "1280 x 1024", "1360 x 768", "1600 x 900", "1920 x 1080" };
        auto res = resolution->addComponent<TextListComponent>(textlist1);
        res->setDefault(to_string(static_cast<int>(Engine::user_preferences.video_resolution.x)) + " x " + to_string(static_cast<int>(Engine::user_preferences.video_resolution.y)));
        resolution->setPosition(Vector2f(650.0f, 400.f));

        auto fullscreen_text = makeEntity();
        auto tf = fullscreen_text->addComponent<TextComponent>(
            "Toggle Full Screen:");
        fullscreen_text->setPosition(Vector2f(425.f, 300.f));

        // fullscreen toggle
        fullscreen = makeEntity();
        vector<string> fsoptions{ "ON", "OFF" };
        auto fs = fullscreen->addComponent<TextListComponent>(fsoptions);
        if (Engine::user_preferences.fullscreen == 7)
            fs->setDefault("OFF");
        else
            fs->setDefault("ON");
        fullscreen->setPosition(Vector2f(775.0f, 300.0f));

    }

    setLoaded(true);
    cout << "Settings Load \n";
}

void SettingsScene::Update(const double& dt) {
    
    if (resolution->get_components<TextListComponent>()[0]->changed)
    {
        string text = resolution->get_components<TextListComponent>()[0]->getText().getString();
        int index = text.find('x');
        int i1;
        int i2;
        stringstream(text.substr(0, index - 1)) >> i1;
        stringstream(text.substr(index + 1)) >> i2;

        cout << i1 << "x" << i2 << endl;

        Engine::user_preferences.video_resolution = Vector2f(i1, i2);
        Engine::user_preferences.changed_resolution = true;
    }

    // Check whether the fullscreen mode has been toggled or not
    if (fullscreen->get_components<TextListComponent>()[0]->changed) {
        Engine::user_preferences.changed_fullscreen = true;
        if (fullscreen->get_components<TextListComponent>()[0]->getText().getString() == "ON") {
            Engine::user_preferences.fullscreen = 8;
            cout << "ON" << endl;
        }
        else {
            cout << "OFF" << endl;
            Engine::user_preferences.fullscreen = 7;
        }
    }

    if (sf::Keyboard::isKeyPressed(Keyboard::Escape)) {
            Engine::ChangeScene(&menu);
    }

    Scene::Update(dt);
}
