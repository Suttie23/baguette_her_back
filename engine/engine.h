#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "user_preferences.h"
#include <ecm.h>
#include <future>
#include <maths.h>
#include <mutex>
#include <string>

class Scene {
public:
  Scene() = default;
  virtual ~Scene();
  virtual void Load() = 0;
  virtual void LoadAsync();
  virtual void UnLoad();
  virtual void Update(const double& dt);
  virtual void Render();
  sf::View* getView();
  std::shared_ptr<sf::Sprite> getBackground();
  void setBackground(sf::Sprite& background);
  std::shared_ptr<sf::Sprite> getForeground();
  void setForeground(sf::Sprite& foreground);
  bool isLoaded() const;
  std::shared_ptr<Entity> makeEntity();

  EntityManager ents;
  sf::Vector2f mouse_pos;

protected:
  std::shared_ptr<sf::Sprite> _background;
  std::shared_ptr<sf::Sprite> _foreground;
  void setLoaded(bool);
private:
  mutable bool _loaded;
  mutable std::future<void> _loaded_future;
  mutable std::mutex _loaded_mtx;
};

class Engine {
public:
  Engine() = delete;
  static void Start(unsigned int width, unsigned int height,
                    const std::string& gameName, Scene* scn);
  static void ChangeScene(Scene*);
  static sf::RenderWindow& GetWindow();
  static sf::Vector2u getWindowSize();
  static void setVsync(bool b);
  static UserPreferences user_preferences;

private:
  static Scene* _activeScene;
  static std::string _gameName;
  static void Update();
  static void Render(sf::RenderWindow& window);
};

namespace timing {
// Return time since Epoc
long long now();
// Return time since last() was last called.
long long last();
} // namespace timing