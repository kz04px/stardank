#ifndef SPACE_COMPONENTS_TARGETER_HPP
#define SPACE_COMPONENTS_TARGETER_HPP

#include <entt/entt.hpp>

namespace space::components {

struct Targeter {
    entt::entity target = entt::null;
};

}  // namespace space::components

#endif
