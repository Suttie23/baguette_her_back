#include "cmp_hurt_player.h"
#include "engine.h"
#include "cmp_enemy_ai.h"
#include <unordered_set>
//#include "LevelSystem.h"

size_t operator==(Location const& a, Location const& b) {
    return (a.x == b.x && a.y == b.y);
}

size_t operator<(Location const& a, Location const& b) {
    return (a.y < b.y);
}


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

array<Location, 4> DIRS = {
    /* East, West, North, South */
    Location{1, 0}, Location{-1, 0},
    Location{0, -1}, Location{0, 1}
};

void EnemyAIComponent::update(double dt) {
    Vector2f disp;
    //if (auto pl = _player.lock()) {
        //std::cout << pl->getPosition() << std::endl;
        //std::cout << "~~~~~~" << std::endl;
    //}
    if (route.empty()) {
    if (can_pathfind) {
        
            route = pathFinding();
            !can_pathfind;
            //for (int i = 0; i < route.size(); i++) {
                //std::cout << "~~~~~~" << std::endl;
                //std::cout << route[i].x << std::endl;
                //std::cout << route[i].y << std::endl;
            //}
        }
    }
        
        _delay -= dt;
        if  (_delay<=0.f && !route.empty()) {
            disp.x = (route.front().x * 1.f);
            disp.y = (route.front().y * 1.f);
            //std::cout << "moving enemy" << std::endl;
            _parent->setPosition(disp);
            //std::cout << _parent->getPosition() << std::endl;
            route.front() = std::move(route.back());
            route.pop_back();
            //std::cout << route.size() << std::endl;
            _delay = 0.05f;

        }
        
   // }
/*
     Vector2f disp;
     disp.x = (route.front().x * 1.f);
     disp.y = (route.front().y * 1.f);
     move((_parent->getPosition()+(disp - _parent->getPosition())));
     route.front() = std::move(route.back());
     route.pop_back();
     */
     //gets current direction and multiplies by speed and dt
     //adds a buffer of 16f before checking if it'll end up in a wall
    /*
    auto mov = _direction * (float)(dt * _speed);
    mov.x += _direction.x * 16.f;
    if (!validMove(_parent->getPosition() + mov)) {
        _direction *= -1.f;
    }

    move(_direction * (float)(dt * _speed));
    */
    ActorMovementComponent::update(dt);
}

EnemyAIComponent::EnemyAIComponent(Entity* p) : ActorMovementComponent(p) {
    _direction = Vector2f(1.0f, 0);
    _speed = 100.0f;
    _player =(_parent->scene->ents.find("player")[0]);
}

std::vector<Location> EnemyAIComponent::pathFinding() {
    vector<Location> path;
    
    Vector2f begin = (_parent->getPosition());
    Location start;

    start.x = (int)floor(begin.x);
    start.y = (int)floor(begin.y);
    
    Location goal;
    Vector2f end;
    if (auto pl = _player.lock()) {
        end = (pl->getPosition());
        //std::cout << "player lock worked" << std::endl;
    }
    else {
        end = { 800.f, 1000.f };
    }
    goal.x = (int)floor(end.x);
    goal.y = (int)floor(end.y);
    //std::cout << begin << std::endl;
    //std::cout << start.x << std::endl;
    //std::cout << end << std::endl;
    //std::cout << goal.x << std::endl;
    
    unordered_map<Location, Location> came_from;
    unordered_map<Location, int> cost_so_far;
    PriorityQueue<Location, double> frontier;
    //queue<Location> frontier;
    //frontier.put(start, 0);
    if (begin != end) {
        frontier.put(start, 0);
        came_from[start] = start;
        cost_so_far[start] = 0;
        //std::cout << frontier.empty() << std::endl;
        while (!frontier.empty()) {
            Location current = frontier.get();

            if (current.x == goal.x && current.y == goal.y) {
                //std::cout << dist(current, goal) << std::endl;
                break;
            }
            
            for (Location next : neighbors(current)) {
                
                //std::cout << "~~~~~~" << std::endl;
                //std::cout << next.x << std::endl;
                //std::cout << next.y << std::endl;
                int new_cost = cost_so_far[current] + cost(current, next);

                if ((cost_so_far.find(next) == cost_so_far.end()) || (new_cost < cost_so_far[next])) {
                    cost_so_far[next] = new_cost;
                    double priority = new_cost + heuristic(next, goal);
                    frontier.put(next, priority);
                    came_from[next] = current;
                }
            }
        }

        Location cur = goal;
        //std::cout << dist(cur, start) << std::endl;
        int counter = 0;
        while (cur.x != start.x && cur.y != start.y) {  // note: this will fail if no path found
            
                path.push_back(cur);
            
            cur = came_from[cur];

        }

        return path;
        
    
    }
}


vector<Location> EnemyAIComponent::neighbors(Location id) {
    vector<Location> results;
    for (Location dir : DIRS) {
        
        Location next{ id.x + dir.x, id.y + dir.y };
        Vector2f temp;
        temp.x = (float)next.x;
        temp.y = (float)next.y;
 
        if (validMove(temp)) {
            results.push_back(next);
            
        }
        //std::cout << "got here5" << std::endl;
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