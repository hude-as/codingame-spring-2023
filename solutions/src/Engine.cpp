#include "Engine.hpp"

// Define a custom comparison function or lambda expression
bool compare_pairs(const pair<int, int> ant_beacon1, const pair<int, int> ant_beacon2, const State& state) {
    // Compare based on distance
    int distance1 = state.game.get_distance(ant_beacon1.first, ant_beacon1.second);
    int distance2 = state.game.get_distance(ant_beacon2.first, ant_beacon2.second);
    if (distance1 != distance2) {
        return distance1 < distance2;
    }

    // Compare based on ant cell index
    if (ant_beacon1.first != ant_beacon2.first) {
        return ant_beacon1.first < ant_beacon2.first;
    }
    // Compare based beacon cell index
    return ant_beacon1.second < ant_beacon2.second;
}

// TODO-PERF change implem to pass pointer ? verify perf.
void Engine::load_beacons_map_from_actions(const vector<Action>& actions_to_apply, short beacons_map[MAX_PLAYER][MAX_CELLS]) {
    for (const auto& action : actions_to_apply) {
        if (action.type == ActionType::BEACON) {
            beacons_map[(int)action.player][(int)action.target_idx] = action.strenght;
        }
    }
}

vector<AntAllocation> Engine::allocate_ants(
            State& state,
            short ants_map[MAX_PLAYER][MAX_CELLS],
            short beacons_map[MAX_PLAYER][MAX_CELLS]) {
        vector<AntAllocation> ant_allocations;

        for (int i = 0; i < MAX_PLAYER; ++i) {
            int ant_sum = 0;
            int beacon_sum = 0;
            double scaling_factor = 0;
            long beacons[MAX_CELLS] = {0};
            long wiggle_room[MAX_CELLS] = {0};

            // sum ants & beacons to compute scaling factor
            for (int j = 0; j < MAX_CELLS; ++j) {                
                ant_sum += ants_map[i][j];
                beacon_sum += beacons_map[i][j];
            }
            scaling_factor = (double) ant_sum / beacon_sum;
            
            // compute beacons array & wiggleRoom
            for (int j = 0; j < MAX_CELLS; ++j) {
                long high_beacon_value = static_cast<long>(ceil(scaling_factor));
                long low_beacon_value = static_cast<long>(scaling_factor);
                beacons[j] = max(1L, low_beacon_value);
                wiggle_room[j] = high_beacon_value - beacons[j];
            }

            vector<pair<int, int>> ant_id_beacon_id;
            for (int j = 0; j < MAX_CELLS; ++j) {
                for (int h = 0; h < MAX_CELLS; ++h) {
                    if (beacons_map[i][h] != 0 && ants_map[i][j] != -1 && state.game.distances[j][h] != -1) {
                        ant_id_beacon_id.push_back({j, h});
                    }
                }
            }

            // Sort the allPairs vector using the custom comparison function
            sort(ant_id_beacon_id.begin(), ant_id_beacon_id.end(), bind(compare_pairs, std::placeholders::_1, std::placeholders::_2, state));

            bool stragglers = false;
            while (!ant_id_beacon_id.empty()) {
                for (const auto& [ant_id, beacon_id] : ant_id_beacon_id) {
                    int ant_count = ants_map[i][ant_id];
                    int beacon_count = beacons[beacon_id];
                    int wiggle_ro = wiggle_room[beacon_id]; // NOT SURE HERE

                    int max_alloc = (int)(stragglers ? min(ant_count, beacon_count + wiggle_ro) : min(ant_count, beacon_count));
                    if (max_alloc > 0) {
                        ant_allocations.emplace_back(ant_id, beacon_id, max_alloc);
                        ants_map[i][ant_id] -= max_alloc;
                        if (!stragglers) {
                            beacons[beacon_id] -= max_alloc;
                        } else {
                            beacons[beacon_id] -= (max_alloc - wiggle_ro);
                            wiggle_room[beacon_id] = 0;
                        }
                    }
                }
                ant_id_beacon_id.erase(std::remove_if(ant_id_beacon_id.begin(), ant_id_beacon_id.end(), [&ants_map, i](pair<int, int> element) {
                    int ant_id = element.first;
                    return (ants_map[i][ant_id] <= 0);
                }), ant_id_beacon_id.end());
            }
        }
        return ant_allocations;
    }

// TODO-PERF change implem to pass pointer ? verify perf.

void Engine::do_move(State& state, short beacons_map[MAX_PLAYER][MAX_CELLS]) {
    (void) state;
    (void) beacons_map;

    vector<AntAllocation> ant_allocation = allocate_ants(state, state.player_ants, beacons_map);

    map<pair<int, int>, int> moves_to_apply;

    for (auto it = ant_allocation.begin(); it != ant_allocation.end(); ++it) {
        vector<short> path = state.game.paths[it->ant_index][it->beacon_index];
        if (path.size() > 1) {
            int neighbor = path[0];

            pair<int, int> ant_move = {it->ant_index, neighbor};
            moves_to_apply[ant_move] += it->amount;
        }
    }

    // mettre la boucle for du player ici.
    // retirer le amount du player_ants correspondant :D
    // ajouter le amount du move sur la case cible.

}

void Engine::compute_next_state(State& state, const vector<Action>& actions_to_apply) {
    short beacons_map[MAX_PLAYER][MAX_CELLS];

    // 1 Create beacons map
    load_beacons_map_from_actions(actions_to_apply, beacons_map);
    (void) state;
    // 2
    do_move(state, beacons_map);
}
