#ifndef SPACE_COMPONENTS_RENDER_HPP
#define SPACE_COMPONENTS_RENDER_HPP

namespace space::components {

struct Render {
    enum class Type
    {
        None = 0,
        Ship,
        Asteroid,
        Particle,
    };

    Type type = Type::None;
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 1.0f;
};

}  // namespace space::components

#endif
