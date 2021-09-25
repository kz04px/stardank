#include <cmath>
#include <iostream>
#include <space/components/body.hpp>
#include <space/components/targetable.hpp>
#include <space/components/targeter.hpp>
#include "../events/mouse_event.hpp"
#include "game.hpp"

using namespace space::components;

void Game::on_mouse_down(MouseDownEvent &e) {
    switch (e.button()) {
        case 0:  // Left
            if (m_map_view) {
                const float map_x = static_cast<float>(e.x()) / m_window_width * 10.0f;
                const float map_y = static_cast<float>((m_window_height - e.y())) / m_window_height * 10.0f;

                float closest_dist = std::numeric_limits<float>::max();
                std::size_t closest = 0;

                for (std::size_t i = 0; i < m_region.systems.size(); ++i) {
                    const auto dx = m_region.systems[i].x - map_x;
                    const auto dy = m_region.systems[i].y - map_y;
                    const auto dist = sqrt(dx * dx + dy * dy);

                    if (dist < closest_dist) {
                        closest = i;
                        closest_dist = dist;
                    }
                }

                if (closest_dist <= 0.5f) {
                    m_system_selected = closest;
                } else {
                    m_system_selected = -1;
                }

                std::cout << "Selected system: " << m_system_selected << "\n";
            } else {
                // Overlay

                // Game
                auto target = entt::entity();
                float closest_dist = std::numeric_limits<float>::max();
                const auto [world_x, world_y] =
                    m_camera.project(static_cast<float>(e.x()) / m_window_width,
                                     static_cast<float>(m_window_height - e.y() - 1) / m_window_height);

                // Find closest targetable entity
                auto view = m_registry.view<Body, Targetable>();
                view.each([&target, &closest_dist, world_x, world_y](auto entity, auto &body) {
                    const auto dx = world_x - body.x;
                    const auto dy = world_y - body.y;
                    const auto dist = sqrt(dx * dx + dy * dy);

                    if (dist < closest_dist) {
                        target = entity;
                        closest_dist = dist;
                    }
                });

                auto &targeter = m_registry.get<Targeter>(m_us);

                if (closest_dist <= 2.5f) {
                    targeter.target = target;
                } else {
                    targeter.target = entt::entity();
                }
            }
            break;
        case 1:  // Right
            break;
        case 2:  // Middle
            break;
        default:
            break;
    }
}
