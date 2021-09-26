#ifndef SPACE_COMPONENTS_BEAM_HPP
#define SPACE_COMPONENTS_BEAM_HPP

#include <entt/entt.hpp>

namespace space::components {

struct Beam {
    enum class Type
    {
        Damage = 0,
    };

    entt::entity parent = entt::null;
    entt::entity target = entt::null;
    Type type = Type::Damage;
    float duration = 1.0f;
    float power = 1.0f;
    float max_dist = 10.0f;
};

}  // namespace space::components

#endif
