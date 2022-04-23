#include "system_renderer.h"
#include "engine.h"
#include <LevelSystem.h>
#include <queue>

using namespace std;
using namespace sf;

static queue<const Drawable*> sprites;
static RenderWindow* rw;

void Renderer::initialise(sf::RenderWindow& r) {
	rw = &r;
	view = rw->getDefaultView();
}

void Renderer::shutdown() {
  while (!sprites.empty())
    sprites.pop();
}

void Renderer::update(const double&) {}

void Renderer::render() {
  if (rw == nullptr) {
    throw("No render window set! ");
  }
  //set view size in case window size has been changed
  if (Engine::user_preferences.changed_resolution || Engine::user_preferences.changed_fullscreen) {
	  if (Engine::user_preferences.fullscreen == 8) {
		  Engine::GetWindow().create(VideoMode(Engine::user_preferences.video_resolution.x, Engine::user_preferences.video_resolution.y), "Baguette Her Back!", Engine::user_preferences.fullscreen);
		  Engine::user_preferences.changed_fullscreen = false;
		  Engine::user_preferences.changed_resolution = false;
	  }
	  else if (Engine::user_preferences.fullscreen == 7) {
		  if (Engine::user_preferences.changed_resolution) {
			  Engine::user_preferences.changed_resolution = false;
		  }
		  else {
			  Engine::GetWindow().create(VideoMode(Engine::user_preferences.video_resolution_default.x, Engine::user_preferences.video_resolution_default.y), "Baguette Her Back!", Engine::user_preferences.fullscreen);
			  Engine::user_preferences.changed_fullscreen = false;
		  }

	  }
  }

  //view.setSize(Vector2f(Engine::getWindowSize()) * Vector2f(0.5f, 0.5f));

  Vector2f view_temp_center = view.getCenter();
  view.setCenter(Vector2f(Engine::getWindowSize().x / 2, Engine::getWindowSize().y / 2));
  rw->setView(view);

  view.setCenter(view_temp_center);

  
  float x = view.getCenter().x;
  float y = view.getCenter().y;


  view.setCenter(Vector2f(x, y));
  rw->setView(view);

  while (!sprites.empty()) {
	  rw->draw(*sprites.front());
	  sprites.pop();
  }
  view_temp_center = Vector2f(0.0f, 0.0f);

}


void Renderer::queue(const sf::Drawable* s) { sprites.push(s); }