#ifndef GAMEUTILS_HPP
# define GAMEUTILS_HPP

# include "common.hpp"

struct GameUtils {
    static void read_initial_inputs(State& initial_state);
    static void read_inputs(State& state);
    static void display_all_distances(const State& state);
    static void compute_all_distances(State& state);
    static void compute_all_paths(State& state);
};

#endif