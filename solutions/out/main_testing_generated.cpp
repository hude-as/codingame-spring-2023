// #pragma GCC optimize "O3,omit-frame-pointer,inline"

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <numeric>
#include <random>
#include <queue>

using namespace std;
// Rules
#define TIME_LIMIT_MS 100000
#define MAX_CELLS 100
#define MAX_BASE_PER_PLAYER 2
#define MAX_PLAYER 2
#define ME 0
#define OP 1
#define EGGS 1
#define CRYS 2
#define EMPTY 0

typedef chrono::time_point<chrono::high_resolution_clock> tphr_t;
typedef unsigned short ushort;
typedef unsigned char uchar;

#ifndef ACTION_HPP
# define ACTION_HPP

enum ActionType {WAIT, BEACON, LINE};
const char* ACTIONTYPE_MAP[4] = { "WAIT", "BEACON", "LINE"};

ostream& operator<<(ostream& os, enum ActionType type) {
    os << ACTIONTYPE_MAP[type];
    return os;
}

struct Action {
    ActionType  type;
    char        target_idx;
    char        strenght;
    bool        player;

    Action(): type(ActionType::WAIT), target_idx(-1), strenght(-1), player(ME) {};
    Action(char idx, char strenght, bool player) : target_idx(idx), strenght(strenght), player(player) {};
    ~Action() {};
};

ostream& operator<<(ostream& os, const Action& action) {
    os << ACTIONTYPE_MAP[action.type];
    if (action.type != ActionType::WAIT)
        os << " " << action.target_idx << " " << action.strenght;
    return os;
}

#endif

#ifndef STATE_HPP
# define STATE_HPP

struct Tile {
    Tile() : idx(-1), neb{-1} {};
    Tile(char idx) : idx(idx), neb{-1} {};
    Tile(char idx, const char* neb) : idx(idx), neb{neb[0], neb[1], neb[2], neb[3], neb[4], neb[5]} {};
    ~Tile() {};

    char   idx = -1;
    char   neb[6] = {-1};
};

struct GameValues {
    GameValues(){};
    ~GameValues(){};

    char            cells_count;
    char            bases_count;
    char            bases[2][MAX_BASE_PER_PLAYER] = {{-1}};
    ushort          required_crys_to_win;
    Tile            board[MAX_CELLS];
    short           distances[MAX_CELLS][MAX_CELLS] = {{-1}};
    vector<short>   paths[MAX_CELLS][MAX_CELLS];

    friend std::ostream& operator<<(std::ostream& os, const GameValues& game) {
        for (int i = 0; i < MAX_CELLS; i++) {
            os << "Cell[" << i << "]: " << (int)game.board[i].idx << ": {";
                for (int j = 0; j < 6; ++j) {
                    os << (int)game.board[i].neb[j] << ", ";
                }
                os << "}" << endl;
        }
        return os;
    }

    short get_distance(short cell1, short cell2);
    vector<short>& get_path(short cell1, short cell2);
    void display_path(short cell1, short cell2);
};

struct State {
    State() {};
    ~State() {};

    static GameValues  game;
    short              eggs_map[MAX_CELLS] = {-1};
    short              crys_map[MAX_CELLS] = {-1};
    short              player_ants[MAX_PLAYER][MAX_CELLS] = {{-1}};
    short              scores[MAX_PLAYER] = {0};

    State& operator=(const State& other) {
        // TODO-PERF try to memcpy the whole stuff.
        memcpy(eggs_map, other.eggs_map, sizeof(eggs_map));
        memcpy(crys_map, other.crys_map, sizeof(crys_map));
        memcpy(player_ants[ME], other.player_ants[ME], sizeof(player_ants[ME]));
        memcpy(player_ants[OP], other.player_ants[OP], sizeof(player_ants[OP]));
        memcpy(scores, other.scores, sizeof(scores));
        return *this;
    }

    bool operator==(const State& other) const {
        return (
            0 == memcmp(eggs_map, other.eggs_map, sizeof(eggs_map))
            && 0 == memcmp(crys_map, other.crys_map, sizeof(crys_map))
            && 0 == memcmp(player_ants[ME], other.player_ants[ME], sizeof(player_ants[ME]))
            && 0 == memcmp(player_ants[OP], other.player_ants[OP], sizeof(player_ants[OP]))
            && 0 == memcmp(scores, other.scores, sizeof(scores))
        );
    }

    friend std::ostream& operator<<(std::ostream& os, const State& state) {
        os << "Score Player 1:" << state.scores[0] << std::endl;
        os << "Score Player 2:" << state.scores[1] << std::endl;
        for (int i = 0; i < MAX_CELLS; i++) {
            os << "\tIdx: " << i << ", Ants: ";
            os << "[P1 - " << state.player_ants[0][i] << "\tants; ";
            os << "P2 - " << state.player_ants[1][i] << "\tants] - ";
            os << "{Crys: " << state.crys_map[i] << "; ";
            os << "Eggs: " << state.eggs_map[i] << "}" << std::endl;
        }
        return os;
    }
};

#endif

#ifndef ENGINE_HPP
# define ENGINE_HPP

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
    static void read_initial_inputs(State& initial_state);
    static void read_inputs(State& state);
    static void display_all_distances(const State& state);
    static void compute_all_distances(State& state);
    static void compute_all_paths(State& state);
};

#endif

#define DISP_ERROR std::cout << "\033[1;31m" << __FUNCTION__ << " failed on line " << __LINE__ << "\033[0m" << std::endl
#define DISP_SUCCESS std::cout << "\033[1;32m" << __FUNCTION__ << " Success !" << "\033[0m" << std::endl
#define IS_TRUE(x) { if (!(x)) DISP_ERROR; else DISP_SUCCESS; }

bool _is_full_of_true(bool *array, int size) {
    // Use the all_of algorithm to check if all elements in the array are true
    return all_of(array, array + size, [](bool x){ return x; });
}

// TODO-PERF (change inline)
short GameValues::get_distance(short cell1, short cell2) {
    return distances[cell1][cell2];
}

// TODO-PERF (change inline)
vector<short>& GameValues::get_path(short cell1, short cell2) {
    return paths[cell1][cell2];
}

void GameValues::display_path(short cell1, short cell2) {
    cerr << "{" << cell1 << "; " << cell2 << "}: ";
    for (auto it = paths[cell1][cell2].begin(); it != paths[cell1][cell2].end(); ++it) {
        cerr << *it << " ,";
    }
    cerr << endl;

}

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

void GameUtils::read_initial_inputs(State& initial_state) {
    int cell_count;
    cin >> cell_count; cin.ignore();
    initial_state.game.cells_count = cell_count;
    for (int i = 0; i < initial_state.game.cells_count; i++) {
        int type;
        int initial_resources;
        int neb_buffer[6];
        initial_state.game.board[i].idx = i;
        cin >> type
            >> initial_resources
            >> neb_buffer[0]
            >> neb_buffer[1]
            >> neb_buffer[2]
            >> neb_buffer[3]
            >> neb_buffer[4]
            >> neb_buffer[5]; cin.ignore();
        initial_state.game.board[i].neb[0] = neb_buffer[0];
        initial_state.game.board[i].neb[1] = neb_buffer[1];
        initial_state.game.board[i].neb[2] = neb_buffer[2];
        initial_state.game.board[i].neb[3] = neb_buffer[3];
        initial_state.game.board[i].neb[4] = neb_buffer[4];
        initial_state.game.board[i].neb[5] = neb_buffer[5];
        if (type == EGGS)
            initial_state.eggs_map[i] = initial_resources;
        else if (type == CRYS)
            initial_state.crys_map[i] = initial_resources;
    }
    int base_count;
    cin >> base_count; cin.ignore();
    initial_state.game.bases_count = base_count;
    for (int p = 0; p < 2; p++) {
        for (int i = 0; i < initial_state.game.bases_count; i++) {
            int base_idx;
            cin >> base_idx; cin.ignore();
            initial_state.game.bases[p][i] = base_idx;
        }
    }
}

void GameUtils::read_inputs(State& state) {
    int s1, s2;
    cin >> s1 >> s2; cin.ignore();
    state.scores[0] = s1; state.scores[1] = s2;
    for (int i = 0; i < state.game.cells_count; i++) {
        int resources, a1, a2; // the current amount of eggs/crystals on this cell
        cin >> resources >> a1 >> a2; cin.ignore();
        state.player_ants[ME][i] = a1; state.player_ants[OP][i] = a2;
        if (state.eggs_map[i] > 0)
            state.eggs_map[i] = (ushort)resources;
        else if (state.crys_map[i] > 0)
            state.crys_map[i] = (ushort)resources;
    }
}

void GameUtils::compute_all_distances(State& state) {    
    for (int i = 0; i < MAX_CELLS; ++i) {
        queue<pair<int, int>> q;
        state.game.distances[i][i] = 0;
        q.push({i, 0});

        while (!q.empty()) {
            pair<int, int>  current = q.front(); q.pop();
            int current_cell_id = current.first;
            int current_distance = current.second;

            Tile*   current_cell = &state.game.board[current_cell_id];
            for (int j = 0; j < 6; ++j) {
                int neighbor_id = current_cell->neb[j];

                if (neighbor_id != -1 && state.game.distances[i][neighbor_id] == -1) {
                    state.game.distances[i][neighbor_id] = current_distance + 1;
                    q.push({neighbor_id, current_distance + 1});
                }
            }
        }
    }
}

void GameUtils::display_all_distances(const State& state) {
    for (int i = 0; i < MAX_CELLS; i++) {
        for (int j = 0; j < MAX_CELLS; j++) {
            cerr << state.game.distances[i][j] << ", ";
        }
        cerr << endl;
    }
}

void GameUtils::compute_all_paths(State& state) {
    bool already_visited[MAX_CELLS][MAX_CELLS] = {{ false }};

    for (int i = 0; i < MAX_CELLS; ++i) {
        queue<pair<int, int>> q;
        q.push({i, i});

        while (!q.empty()) {
            pair<int, int>  current = q.front(); q.pop();
            int current_cell_id = current.first;
            int previous_cell_id = current.second;
            already_visited[i][current_cell_id] = true;
            if (state.game.paths[i][current_cell_id].empty()) {
                state.game.paths[i][current_cell_id] = state.game.paths[i][previous_cell_id];
                state.game.paths[i][current_cell_id].push_back(current_cell_id);
            }

            Tile*   current_cell = &state.game.board[current_cell_id];
            for (int j = 0; j < 6; ++j) {
                int neighbor_id = current_cell->neb[j];
                if (neighbor_id != -1 && !already_visited[i][neighbor_id]) {
                    q.push({neighbor_id, current_cell_id});
                }
            }
        }
    }
}

GameValues State::game;
int main() {
   cout << "Hello testing ! :) Gl boooy" << endl;
   State       state;
   GameUtils::read_initial_inputs(state);
   while (1){
      GameUtils::read_inputs(state);
      cout << "WAIT" << endl;
   }
}
