#ifndef SPACE_COMPONENTS_COMMANDS_HPP
#define SPACE_COMPONENTS_COMMANDS_HPP

namespace space::components {

struct Commands {
    bool forwards = false;
    bool backwards = false;
    bool left = false;
    bool right = false;
    bool shoot = false;
};

}  // namespace space::components

#endif
