#ifndef ACTION_HPP
# define ACTION_HPP

# include "common.hpp"

enum ActionType {WAIT, MOVE, SPAWN, BUILD};
const char* ACTIONTYPE_MAP[4] = { "WAIT", "MOVE", "SPAWN", "BUILD"};

ostream& operator<<(ostream& os, enum ActionType action_type) {
    os << ACTIONTYPE_MAP[action_type];
    return os;
}

struct Action {
    ActionType  action_type;

    Action() {};
    ~Action() {};
};

ostream& operator<<(ostream& os, const Action& action) {
    os << ACTIONTYPE_MAP[action.action_type];
    return os;
}

#endif
