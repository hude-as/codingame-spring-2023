#include "State.hpp"

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
