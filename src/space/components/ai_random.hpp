#ifndef SPACE_COMPONENTS_AI_RANDOM_HPP
#define SPACE_COMPONENTS_AI_RANDOM_HPP

namespace space::components {

struct AIRandom {
    enum class Stage
    {
        Wait = 0,
        Turn,
        Travel,
        Target,
        Engage,
    };

    Stage stage = Stage::Wait;
    bool aggressive = true;
    float duration = 0.0f;
    float memory[1] = {};
};

}  // namespace space::components

#endif
