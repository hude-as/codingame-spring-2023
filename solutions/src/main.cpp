#include "common.hpp"

// ~> stitch <~
GameValues State::game;
int main()
{
    State       state;
    GameUtils::read_initial_inputs(state);
    GameUtils::compute_all_distances(state);
    GameUtils::compute_all_paths(state);
    // GameUtils::display_all_distances(state);
    while(1) {
        GameUtils::read_inputs(state);
        state.game.display_path(0, 5);
        state.game.display_path(1, 5);
        state.game.display_path(4, 5);
        state.game.display_path(5, 4);
        state.game.display_path(5, 5);
        cerr << state.game << endl;
        cerr << state << endl;
        cout << Action() << endl;
    }
}
