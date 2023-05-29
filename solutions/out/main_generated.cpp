// #pragma GCC optimize "O3,omit-frame-pointer,inline"

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <numeric>
#include <random>

using namespace std;
#define TIME_LIMIT_MS 100000

typedef chrono::time_point<chrono::high_resolution_clock> tphr_t;

#ifndef ACTION_HPP
# define ACTION_HPP

enum ActionType {WAIT, MOVE, SPAWN, BUILD};
const char* ACTIONTYPE_MAP[4] = { "WAIT", "MOVE", "SPAWN", "BUILD"};

ostream& operator<<(ostream& os, enum ActionType action_type) {
    os << ACTIONTYPE_MAP[action_type];
    return os;
}

struct Action {
    ActionType  action_type;

    Action() {};
    ~Action() {};
};

ostream& operator<<(ostream& os, const Action& action) {
    os << ACTIONTYPE_MAP[action.action_type];
    return os;
}

#endif

#ifndef STATE_HPP
# define STATE_HPP

struct State {
    State() {};
    ~State() {};
};

#endif

#ifndef ENGINE_HPP
# define ENGINE_HPP

typedef void (*ActionApply)(State&, const Action&);

struct Engine {
    Engine() {};
    ~Engine() {};

};

#endif

#ifndef AI_HPP
# define AI_HPP

struct AI {
    int             node_count;
    const State&    initial_state;
    tphr_t          start_time;

    AI(const State& initial_state, tphr_t start_time) : node_count(0), initial_state(initial_state), start_time(start_time) {};
    ~AI() {};
};

#endif

#ifndef GAMEUTILS_HPP
# define GAMEUTILS_HPP

struct GameUtils {

};

#endif

int main()
{
    cerr << "Hello game ! :)" << endl;
}
