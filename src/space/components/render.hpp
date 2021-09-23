#ifndef SPACE_COMPONENTS_RENDER_HPP
#define SPACE_COMPONENTS_RENDER_HPP

namespace space::components {

struct Render {
    enum class Type
    {
        None = 0,
        Ship,
        Asteroid,
    };

    Type type = Type::None;
};

}  // namespace space::components

#endif
