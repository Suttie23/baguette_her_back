#include "cmp_player_physics.h"
#include "system_physics.h"
#include "cmp_sprite.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>

using namespace std;
using namespace sf;
using namespace Physics;


// Run Frames
std::vector<sf::Vector2u>& GetRunFrames() {
    static bool isSetup = false;
    static std::vector<sf::Vector2u> runFrames;

    if (!isSetup) 
    {
        isSetup = true;
        runFrames.push_back(Vector2u(0, 1));
        runFrames.push_back(Vector2u(1, 1));
        runFrames.push_back(Vector2u(2, 1));
        runFrames.push_back(Vector2u(0, 2));
        runFrames.push_back(Vector2u(1, 2));
        runFrames.push_back(Vector2u(2, 2));
    }

    return runFrames;
}

// Idle Frame
std::vector<sf::Vector2u>& GetIdleFrames() {
    static bool isSetup = false;
    static std::vector<sf::Vector2u> idleFrames;

    if (!isSetup)
    {
        isSetup = true;
        idleFrames.push_back(Vector2u(0, 0));
    }

    return idleFrames;
}

// Jump Frame
std::vector<sf::Vector2u>& GetJumpFrames() {
    static bool isSetup = false;
    static std::vector<sf::Vector2u> jumpFrames;

    if (!isSetup)
    {
        isSetup = true;
        jumpFrames.push_back(Vector2u(0, 1));
    }

    return jumpFrames;
}

// Falling Frame
std::vector<sf::Vector2u>& GetFallingFrames() {
    static bool isSetup = false;
    static std::vector<sf::Vector2u> fallingFrames;

    if (!isSetup)
    {
        isSetup = true;
        fallingFrames.push_back(Vector2u(2, 1));
    }

    return fallingFrames;
}

bool PlayerPhysicsComponent::isGrounded() const {
  auto touch = getTouching();
  const auto& pos = _body->GetPosition();
  const float halfPlrHeigt = _size.y * .5f;
  const float halfPlrWidth = _size.x * .52f;
  b2WorldManifold manifold;
  for (const auto& contact : touch) {
    contact->GetWorldManifold(&manifold);
    const int numPoints = contact->GetManifold()->pointCount;
    bool onTop = numPoints > 0;
    // If all contacts are below the player.
    for (int j = 0; j < numPoints; j++) {
      onTop &= (manifold.points[j].y < pos.y - halfPlrHeigt);
    }
    if (onTop) {
      return true;
    }
  }

  return false;
}

void PlayerPhysicsComponent::update(double dt) {

  // Get parent Component (AnimatedSpriteComponent)  
  shared_ptr<AnimatedSpriteComponent> dude { _parent->GetCompatibleComponent<AnimatedSpriteComponent>() [0]};
  const auto pos = _parent->getPosition();

  //Teleport to start if we fall off map.
  if (pos.y > ls::getHeight() * ls::getTileSize()) {
    teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
  }

  if (Keyboard::isKeyPressed(Keyboard::Left) ||
      Keyboard::isKeyPressed(Keyboard::Right) || 
      Joystick::getAxisPosition(0, Joystick::X) < -10.0f || Joystick::getAxisPosition(0, Joystick::X) > 10.0f)
  {
    // Moving Either Left or Right
    if (Keyboard::isKeyPressed(Keyboard::Right) ||
        Joystick::getAxisPosition(0, Joystick::X) > 10.0f) {
      if (getVelocity().x < _maxVelocity.x)
        impulse({(float)(dt * _groundspeed), 0});
      // Setting run frames and direction
      dude->SetFrames(GetRunFrames());
      dude->faceRight = true;
    } else {
      if (getVelocity().x > -_maxVelocity.x)
        impulse({-(float)(dt * _groundspeed), 0});
      // Setting run frames and direction
      dude->SetFrames(GetRunFrames());
      dude->faceRight = false;
    }
  } else {
    // Dampen X axis movement
    dampen({0.9f, 1.0f});
    dude->SetFrames(GetIdleFrames());
  }

  // Handle Jump
  if (Keyboard::isKeyPressed(Keyboard::Up) ||
      Joystick::isButtonPressed(0, Joystick::isButtonPressed(0,1))) {
    _grounded = isGrounded();
    if (_grounded) {
      setVelocity(Vector2f(getVelocity().x, 0.f));
      teleport(Vector2f(pos.x, pos.y - 4.0f));
      impulse(Vector2f(0, -7.f));
    }
  }


  //Are we in air?
  if (!_grounded) {
      // setting falling frames
      if (getVelocity().y > 0) {
          dude->SetFrames(GetFallingFrames());
      }
      // setting jumping frames
      else {
          dude->SetFrames(GetJumpFrames());
      }
    _grounded = isGrounded();
    // disable friction while jumping
    setFriction(0.f);
  } else {
    setFriction(0.1f);
  }

  // Clamp velocity.
  auto v = getVelocity();
  v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
  v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
  setVelocity(v);

  PhysicsComponent::update(dt);
}

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p,
                                               const Vector2f& size)
    : PhysicsComponent(p, true, size) {
  _size = sv2_to_bv2(size, true);
  _maxVelocity = Vector2f(200.f, 400.f);
  _groundspeed = 30.f;
  _grounded = false;
  _body->SetSleepingAllowed(false);
  _body->SetFixedRotation(true);
  //Bullet items have higher-res collision detection
  _body->SetBullet(true);
}
