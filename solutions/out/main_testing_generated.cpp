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

#define DISP_ERROR std::cout << "\033[1;31m" << __FUNCTION__ << " failed on line " << __LINE__ << "\033[0m" << std::endl
#define DISP_SUCCESS std::cout << "\033[1;32m" << __FUNCTION__ << " Success !" << "\033[0m" << std::endl
#define IS_TRUE(x) { if (!(x)) DISP_ERROR; else DISP_SUCCESS; }

bool _is_full_of_true(bool *array, int size) {
    // Use the all_of algorithm to check if all elements in the array are true
    return all_of(array, array + size, [](bool x){ return x; });
}

int main() {
   cout << "Hello testing ! :) Gl boooy" << endl;
}
