
#include "cmp_enemy_ai.h"
#include <unordered_set>


size_t operator==(Location const& a, Location const& b) {
    return (a.x == b.x && a.y == b.y);
}

size_t operator<(Location const& a, Location const& b) {
    return (a.y < b.y);
}

array<Location, 4> DIRS = {
    /* East, West, North, South */
    Location{30, 0}, Location{-30, 0},
    Location{0, -30}, Location{0, 30}
};

void EnemyAIComponent::update(double dt) {
    if (route.empty()) {
        route = pathFinding();
    }
    // Vector2f disp;
     //disp.x = (route.front().x * 1.f);
     //disp.y = (route.front().y * 1.f);
     //move((_parent->getPosition()+(disp - _parent->getPosition())));
     //route.front() = std::move(route.back());
     //route.pop_back();

     //gets current direction and multiplies by speed and dt
     //adds a buffer of 16f before checking if it'll end up in a wall
    auto mov = _direction * (float)(dt * _speed);
    mov.x += _direction.x * 16.f;
    if (!validMove(_parent->getPosition() + mov)) {
        _direction *= -1.f;
    }

    //move(_direction * (float)(dt * _speed));
    ActorMovementComponent::update(dt);
}

EnemyAIComponent::EnemyAIComponent(Entity* p) : ActorMovementComponent(p) {
    _direction = Vector2f(1.0f, 0);
    _speed = 100.0f;
}

std::vector<Location> EnemyAIComponent::pathFinding() {
    Vector2f begin = (_parent->getPosition());
    Location start;

    start.x = (int)floor(begin.x);
    start.y = (int)floor(begin.y);

    Location goal;
    auto player = _player.lock();
    Vector2f end = (player->getPosition());
    goal.x = (int)floor(end.x);
    goal.y = (int)floor(end.y);

    unordered_map<Location, Location> came_from;
    unordered_map<Location, int> cost_so_far;
    PriorityQueue<Location, double> frontier;
    frontier.put(start, 0);

    came_from[start] = start;
    cost_so_far[start] = 0;

    while (!frontier.empty()) {
        Location current = frontier.get();

        if ((current.x == goal.x && current.y == goal.y) || (dist(current, goal) < 20)) {
            break;
        }

        for (Location next : neighbors(current)) {
            int new_cost = cost_so_far[current] + cost(current, next);
            if ((cost_so_far.find(next) == cost_so_far.end()) || (new_cost < cost_so_far[next])) {
                cost_so_far[next] = new_cost;
                double priority = new_cost + heuristic(next, goal);
                frontier.put(next, priority);
                came_from[next] = current;
            }
        }
    }

    vector<Location> path;
    Location cur = goal;
    while ((!(cur.x == start.x && cur.y == start.y)) || dist(cur, goal) >= 20) {  // note: this will fail if no path found
        path.push_back(cur);
        cur = came_from[cur];
    }
    //path.push_back(begin); // optional
    std::reverse(path.begin(), path.end());
    return path;

}


vector<Location> EnemyAIComponent::neighbors(Location id) {
    vector<Location> results;

    for (Location dir : DIRS) {
        Location next{ id.x + dir.x, id.y + dir.y };
        Vector2f temp;
        temp.x = (float)next.x;
        temp.y = (float)next.y;
        if (validMove(_parent->getPosition() + temp)) {
            results.push_back(next);
        }
    }

    return results;

};


int EnemyAIComponent::cost(Location from_node, Location to_node) {
    if (from_node.y < to_node.y) {
        return 2;
    }
    else {
        return 1;
    }
};



inline double EnemyAIComponent::heuristic(Location a, Location b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}