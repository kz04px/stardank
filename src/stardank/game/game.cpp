#include "game.hpp"

Game::Game()
    : m_camera{},
      m_region{},
      m_registry{},
      m_us{},
      m_region_selected{-1},
      m_window_width{},
      m_window_height{},
      m_map_view{true} {
    m_region.age = 0.0f;

    {
        std::vector<std::pair<int, int>> grids_available;

        // Create grid list
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                grids_available.push_back({i, j});
            }
        }

        // Create systems
        for (int i = 0; i < 10 + rand() % 5 && !grids_available.empty(); ++i) {
            const auto [grid_x, grid_y] = grids_available[rand() % grids_available.size()];

            const float dx = static_cast<float>(rand()) / RAND_MAX;
            const float dy = static_cast<float>(rand()) / RAND_MAX;

            const float x = grid_x + dx;
            const float y = grid_y + dy;

            m_region.systems.push_back({space::System::Status::Available, x, y, 0, 0.5f});
        }
    }

    // Create connections

    // Trim disconnected

    // Assign system ownership
    int i = 0;
    for (auto &system : m_region.systems) {
        system.owner = i % 4 + 1;
        i++;
    }
}

void Game::load_system() {
    // Add ship
    {
        m_us = m_registry.create();
        m_registry.emplace<Body>(m_us, 0.0f, 0.0f, 0.0f);
        m_registry.emplace<Velocity>(m_us, 0.0f, 0.0f);
        m_registry.emplace<Acceleration>(m_us, 0.0f, 0.0f);
        m_registry.emplace<Engine>(m_us);
        m_registry.emplace<Render>(m_us, Render::Type::Ship);
    }

    // Add asteroids
    for (auto i = 0; i < 10; ++i) {
        const auto entity = m_registry.create();
        m_registry.emplace<Body>(
            entity, rand_between(-10.0f, 10.0f), rand_between(-10.0f, 10.0f), rand_between(0.0f, 360.0f));
        m_registry.emplace<Velocity>(
            entity, rand_between(-1.0f, 1.0f), rand_between(-1.0f, 1.0f), rand_between(-1.0f, 1.0f));
        m_registry.emplace<Render>(entity, Render::Type::Asteroid);
    }
}
