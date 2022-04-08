#include "system_renderer.h"
#include "engine.h"
#include <queue>

using namespace std;
using namespace sf;

static queue<const Drawable*> sprites;
static RenderWindow* rw;

void Renderer::initialise(sf::RenderWindow& r) { rw = &r; }

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

  while (!sprites.empty()) {
    rw->draw(*sprites.front());
    sprites.pop();
  }
}

void Renderer::queue(const sf::Drawable* s) { sprites.push(s); }