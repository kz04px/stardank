#ifndef SPACE_COMPONENTS_ENGINE_HPP
#define SPACE_COMPONENTS_ENGINE_HPP

namespace space::components {

struct Engine {
    float max_thrust = 10.0f;
    float max_rotation = 2.0f;
    bool forwards = false;
    bool backwards = false;
    bool cw = false;
    bool ccw = false;
};

}  // namespace space::components

#endif
