#include "game.hpp"
#include <space/components/acceleration.hpp>
#include <space/components/beacon.hpp>
#include <space/components/body.hpp>
#include <space/components/engine.hpp>
#include <space/components/health.hpp>
#include <space/components/laser.hpp>
#include <space/components/render.hpp>
#include <space/components/targetable.hpp>
#include <space/components/velocity.hpp>

using namespace space::components;

Game::Game()
    : m_camera{},
      m_region{},
      m_registry{},
      m_us{},
      m_system_selected{-1},
      m_entity_selected{},
      m_window_width{},
      m_window_height{},
      m_map_view{true} {
    m_region.age = 0.0f;

    {
        std::vector<std::pair<int, int>> grids_available;

        // Create grid list
        for (int i = 1; i < 9; ++i) {
            for (int j = 1; j < 9; ++j) {
                grids_available.push_back({i, j});
            }
        }

        // Create systems
        for (int i = 0; i < 30 + rand() % 20 && !grids_available.empty(); ++i) {
            const auto idx = rand() % grids_available.size();
            const auto [grid_x, grid_y] = grids_available.at(idx);

            const float dx = 0.1f + 0.8f * static_cast<float>(rand()) / RAND_MAX;
            const float dy = 0.1f + 0.8f * static_cast<float>(rand()) / RAND_MAX;

            const float x = grid_x + dx;
            const float y = grid_y + dy;

            m_region.systems.push_back({space::System::Status::Available, x, y, 0, 0.5f});

            // Remove the grid
            grids_available[idx] = grids_available.back();
            grids_available.pop_back();
        }
    }

    // Create connections

    // Trim disconnected

    // Assign system ownership
    int i = 0;
    for (auto &system : m_region.systems) {
        if (i < 8) {
            system.owner = 0;
        } else {
            system.owner = i % 2 + 1;
        }
        i++;
    }
}

void Game::load_system(const space::System &system) {
    // Add ship
    {
        m_us = m_registry.create();
        m_registry.emplace<Body>(m_us, 0.0f, 0.0f, 0.0f);
        m_registry.emplace<Velocity>(m_us, 0.0f, 0.0f);
        m_registry.emplace<Acceleration>(m_us, 0.0f, 0.0f);
        m_registry.emplace<Engine>(m_us);
        m_registry.emplace<Render>(m_us, Render::Type::Ship);
        m_registry.emplace<Laser>(m_us);
    }

    // Add asteroids
    for (auto i = 0; i < 10; ++i) {
        const auto entity = m_registry.create();
        m_registry.emplace<Body>(
            entity, rand_between(-10.0f, 10.0f), rand_between(-10.0f, 10.0f), rand_between(0.0f, 360.0f));
        m_registry.emplace<Velocity>(
            entity, rand_between(-1.0f, 1.0f), rand_between(-1.0f, 1.0f), rand_between(-1.0f, 1.0f));
        m_registry.emplace<Render>(entity, Render::Type::Asteroid);
        m_registry.emplace<Beacon>(entity, 10.0f);
        m_registry.emplace<Targetable>(entity);
        m_registry.emplace<Health>(entity, 5.0f);
    }
}
