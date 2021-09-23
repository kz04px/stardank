#include <cmath>
#include <iostream>
#include "../events/mouse_event.hpp"
#include "game.hpp"

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
                    m_region_selected = closest;
                } else {
                    m_region_selected = -1;
                }

                std::cout << "Selected region: " << m_region_selected << "\n";
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
