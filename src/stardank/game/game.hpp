#ifndef GAME_HPP
#define GAME_HPP

#include <entt/entt.hpp>
#include <space/components/acceleration.hpp>
#include <space/components/body.hpp>
#include <space/components/engine.hpp>
#include <space/components/render.hpp>
#include <space/components/velocity.hpp>
#include <space/region.hpp>

using namespace space::components;

struct Camera;

[[nodiscard]] static float rand_between(const float a, const float b) {
    return a + (b - a) * static_cast<float>(rand()) / RAND_MAX;
}

class Game {
   public:
    Game();

    [[nodiscard]] std::pair<float, float> camera_position() const noexcept {
        if (!m_registry.valid(m_us)) {
            return {0.0f, 0.0f};
        }

        const auto pos = m_registry.get<Body>(m_us);
        return {pos.x, pos.y};
    }

    void update(const float dt);

    void render(const Camera &camera) const noexcept;

    void load_system();

    space::Region m_region;

   private:
    entt::registry m_registry;
    entt::entity m_us;
    bool m_map_view;
};

#endif
