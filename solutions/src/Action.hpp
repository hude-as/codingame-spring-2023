#ifndef ACTION_HPP
# define ACTION_HPP

# include "common.hpp"

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
