#pragma once


#include "cmp_actor_movement.h"
#include <ecm.h>
#include <array>
#include <unordered_map>
#include <queue>
#include <vector>



using namespace std;
using namespace sf;


struct Location {

    int x, y;
};


class FollowComponent : public ActorMovementComponent {
protected:
    float _delay;
    sf::Vector2f _direction;
    std::weak_ptr<Entity> _player;
    std::vector<Location> route;
    bool can_pathfind = true;
public:
    void update(double dt) override;

    explicit FollowComponent(Entity* p);

    FollowComponent() = delete;

    std::vector<Location> neighbors(Location id);

    int cost(Location from_node, Location to_node);

    inline double heuristic(Location a, Location b);

    std::vector<Location> pathFinding();

    double dist(Location a, Location b) {
        int i = a.x;
        int j = a.y;
        int k = b.x;
        int l = b.y;

        return sqrt((k - i) ^ 2 + (l - j) ^ 2);
    };
};
