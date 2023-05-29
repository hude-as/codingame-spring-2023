#ifndef ENGINE_HPP
# define ENGINE_HPP

# include "common.hpp"

typedef void (*ActionApply)(State&, const Action&);

struct Engine {
    Engine() {};
    ~Engine() {};

};

#endif
