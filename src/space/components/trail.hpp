#ifndef SPACE_COMPONENTS_TRAIL_HPP
#define SPACE_COMPONENTS_TRAIL_HPP

namespace space::components {

struct Trail {
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;
    float frequency = 1.0f;
    float cooldown = 0.0f;
};

}  // namespace space::components

#endif
