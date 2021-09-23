#include "game.hpp"

Game::Game() : m_registry{}, m_us{} {
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
