#ifndef ENGINE_HPP
# define ENGINE_HPP

# include "common.hpp"

typedef void (*ActionApply)(State&, const Action&);

// IGNORE - Les actions LINE sont calculées.
// IMPLEM - Les actions BEACON sont calculées.
// IMPLEM - Les fourmis se déplacent.
// IMPLEM - Les oeufs sont récoltés et les nouvelles fourmis apparaissent.
// IMPLEM - Les cristaux sont récoltés et les points sont marqués.

struct AntAllocation {
    int ant_index;
    int beacon_index;
    int amount;

    AntAllocation(int ant_index, int beacon_index, int amount) : ant_index(ant_index), beacon_index(beacon_index), amount(amount) {};
    ~AntAllocation() {};
};

struct Engine {
    Engine() {};
    ~Engine() {};

    // public
    void compute_next_state(State& state, const vector<Action>& actions_to_apply);
    
    // internal
    void do_move(State& state,  short beacons_map[MAX_PLAYER][MAX_CELLS]);
    void load_beacons_map_from_actions(const vector<Action>& actions_to_apply, short beacons_map[MAX_PLAYER][MAX_CELLS]);
    vector<AntAllocation> allocate_ants(
            State& state,
            short ants_map[MAX_PLAYER][MAX_CELLS],
            short beacons_map[MAX_PLAYER][MAX_CELLS]);
};

#endif
