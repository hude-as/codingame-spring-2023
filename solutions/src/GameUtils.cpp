#include "GameUtils.hpp"

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
