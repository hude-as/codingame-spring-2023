#ifndef AI_HPP
# define AI_HPP

# include "common.hpp"

struct AI {
    int             node_count;
    const State&    initial_state;
    tphr_t          start_time;

    AI(const State& initial_state, tphr_t start_time) : node_count(0), initial_state(initial_state), start_time(start_time) {};
    ~AI() {};
};

#endif
