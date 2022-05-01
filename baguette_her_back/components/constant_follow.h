#pragma once


#include "cmp_actor_movement.h"
#include <ecm.h>
#include <array>
#include <unordered_map>
#include <queue>
#include <vector>



using namespace std;
using namespace sf;


struct Coord {

    int x, y;
};

template <> struct hash<Coord> {
    size_t operator () (const Coord& id) const noexcept {

        return hash<int>() (id.x ^ (id.y << 16));
    }
};

class FollowComponent : public ActorMovementComponent {
protected:
    float _delay;
    sf::Vector2f _direction;
    std::weak_ptr<Entity> _player;
    std::vector<Coord> route;
    bool can_pathfind = true;
public:
    void update(double dt) override;

    explicit FollowComponent(Entity* p);

    FollowComponent() = delete;

    std::vector<Coord> neighbors(Coord id);

    int cost(Coord from_node, Coord to_node);

    inline double heuristic(Coord a, Coord b);

    std::vector<Coord> pathFinding();

    double dist(Coord a, Coord b) {
        int i = a.x;
        int j = a.y;
        int k = b.x;
        int l = b.y;

        return sqrt((k - i) ^ 2 + (l - j) ^ 2);
    };
};
