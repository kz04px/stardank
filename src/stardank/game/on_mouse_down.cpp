#include <cmath>
#include <iostream>
#include <space/components/body.hpp>
#include <space/components/targetable.hpp>
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
                auto view = m_registry.view<Body, Targetable>();
                auto [world_x, world_y] =
                    m_camera.project(static_cast<float>(e.x()) / m_window_width,
                                     static_cast<float>(m_window_height - e.y() - 1) / m_window_height);

                {
                    const auto x = e.x();
                    const auto y = e.y();
                    const auto dx = static_cast<float>(e.x()) / m_window_width;
                    const auto dy = 1.0f - static_cast<float>(e.y()) / m_window_height;
                    const auto [wx, wy] = m_camera.project(dx, dy);

                    std::cout << "x, y:   " << x << ", " << y << "\n";
                    std::cout << "dx, dy: " << dx << ", " << dy << "\n";
                    std::cout << "wx, wy: " << wx << ", " << wy << "\n";
                    std::cout << "\n";
                }

                view.each([&target, &closest_dist, world_x, world_y](auto entity, auto &body) {
                    const auto dx = world_x - body.x;
                    const auto dy = world_y - body.y;
                    const auto dist = sqrt(dx * dx + dy * dy);

                    if (dist < closest_dist) {
                        target = entity;
                        closest_dist = dist;
                    }
                });

                if (closest_dist <= 2.5f) {
                    m_entity_selected = target;
                } else {
                    m_entity_selected = entt::entity();
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
