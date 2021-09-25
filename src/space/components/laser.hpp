#ifndef SPACE_COMPONENTS_LASER_HPP
#define SPACE_COMPONENTS_LASER_HPP

namespace space::components {

struct Laser {
    float range = 10.0f;
    float strength = 1.0f;
    float cooldown = 0.0f;
};

}  // namespace space::components

#endif
