#ifndef STATE_HPP
# define STATE_HPP

# include "common.hpp"

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
