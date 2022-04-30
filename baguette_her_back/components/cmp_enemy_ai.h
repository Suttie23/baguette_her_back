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

template <> struct hash<Location> {
    size_t operator () (const Location& id) const noexcept {

        return hash<int>() (id.x ^ (id.y << 16));
    }
};

class EnemyAIComponent : public ActorMovementComponent {
protected:
    sf::Vector2f _direction;
    std::weak_ptr<Entity> _player;
    std::vector<Location> route;
public:
    void update(double dt) override;

    explicit EnemyAIComponent(Entity* p);

    EnemyAIComponent() = delete;

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

template<typename T, typename priority_t>
struct PriorityQueue {
    typedef pair<priority_t, T> PQElement;
    priority_queue<PQElement, vector<PQElement>, greater<PQElement>> elements;

    inline bool empty() const {
        return elements.empty();
    }

    inline void put(T item, priority_t priority) {
        elements.emplace(priority, item);
    }

    T get() {
        T best_item = elements.top().second;
        elements.pop();
        return best_item;
    }
};