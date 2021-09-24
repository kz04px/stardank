#include <space/components/beacon.hpp>
#include <space/components/body.hpp>
#include <space/components/render.hpp>
#include "../camera.hpp"
#include "../renderer/quad.hpp"
#include "../renderer/renderer.hpp"
#include "../renderer/triangle.hpp"
#include "game.hpp"

using namespace space::components;

static const Colour team_colours[] = {
    {0.8f, 0.8f, 0.8f},
    {1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {1.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 1.0f},
};

void Game::render() const noexcept {
    if (m_map_view) {
        auto map_camera = Camera();
        map_camera.position = {5.0f, 5.0f};
        map_camera.size = {10.0f, 10.0f};

        RenderAPI::begin(map_camera);

        // Draw systems
        for (const auto &system : m_region.systems) {
            auto quad = Quad();
            quad.vertices[0] = glm::vec2{-0.1f, -0.1f};
            quad.vertices[1] = glm::vec2{-0.1f, 0.1f};
            quad.vertices[2] = glm::vec2{0.1f, 0.1f};
            quad.vertices[3] = glm::vec2{0.1f, -0.1f};
            quad.colour = team_colours[system.owner];
            quad.rotation = 0.0f;
            quad.translation = {system.x, system.y};
            RenderAPI::draw(quad, 0);
        }

        // Draw connections

        // Draw selected region highlight
        if (m_system_selected >= 0) {
            const auto &system = m_region.systems.at(m_system_selected);

            auto quad = Quad();
            quad.vertices[0] = glm::vec2{-0.01f, -0.15f};
            quad.vertices[1] = glm::vec2{-0.01f, 0.15f};
            quad.vertices[2] = glm::vec2{0.01f, 0.15f};
            quad.vertices[3] = glm::vec2{0.01f, -0.15f};
            quad.colour = {0.0f, 1.0f, 0.0f};
            quad.rotation = 0;

            quad.translation = {system.x - 0.15f, system.y};
            RenderAPI::draw(quad, 0);

            quad.translation = {system.x + 0.15f, system.y};
            RenderAPI::draw(quad, 0);
        }

        // Title
        RenderAPI::draw_text("Map", 0.0f, 0.0f, 1.0f, 10.0f);

        RenderAPI::end();
    } else {
        RenderAPI::begin(m_camera);

        {
            // Render entities
            auto view = m_registry.view<const Body, const Render>();
            view.each([](const auto &body, const auto &render) {
                switch (render.type) {
                    case Render::Type::None:
                        break;
                    case Render::Type::Ship: {
                        auto tri = Triangle();
                        tri.vertices[0] = glm::vec2{-0.5f, -0.5f};
                        tri.vertices[1] = glm::vec2{0.0f, 1.0f};
                        tri.vertices[2] = glm::vec2{0.5f, -0.5f};
                        tri.colour = colour::red;
                        tri.rotation = body.r;
                        tri.translation = {body.x, body.y};
                        RenderAPI::draw(tri, 0);
                        break;
                    }
                    case Render::Type::Asteroid: {
                        auto quad = Quad();
                        quad.vertices[0] = glm::vec2{-0.5f, -0.5f};
                        quad.vertices[1] = glm::vec2{-0.5f, 0.5f};
                        quad.vertices[2] = glm::vec2{0.5f, 0.5f};
                        quad.vertices[3] = glm::vec2{0.5f, -0.5f};
                        quad.colour = {0.8f, 0.8f, 0.8f};
                        quad.rotation = body.r;
                        quad.translation = {body.x, body.y};
                        RenderAPI::draw(quad, 0);
                        break;
                    }
                    default:
                        break;
                }
            });
        }

        {
            // Render direction markers
            auto view = m_registry.view<const Body, const Beacon>();
            view.each([this](const auto &body, const auto &beacon) {
                const glm::vec2 target = {body.x, body.y};
                const auto diff = target - m_camera.position;

                // Out of range
                if (abs(diff.x) > beacon.range || abs(diff.y) > beacon.range || glm::length(diff) > beacon.range) {
                    return;
                }

                auto indicator = diff;
                if (indicator.x > m_camera.size.x / 2) {
                    indicator *= (m_camera.size.x / 2) / indicator.x;
                }
                if (indicator.x < -m_camera.size.x / 2) {
                    indicator *= (-m_camera.size.x / 2) / indicator.x;
                }
                if (indicator.y > m_camera.size.y / 2) {
                    indicator *= (m_camera.size.y / 2) / indicator.y;
                }
                if (indicator.y < -m_camera.size.y / 2) {
                    indicator *= (-m_camera.size.y / 2) / indicator.y;
                }

                // Direction indicator
                if (indicator != diff) {
                    auto quad = Quad();
                    quad.vertices[0] = glm::vec2{-0.1f, -0.1f};
                    quad.vertices[1] = glm::vec2{-0.1f, 0.1f};
                    quad.vertices[2] = glm::vec2{0.1f, 0.1f};
                    quad.vertices[3] = glm::vec2{0.1f, -0.1f};
                    quad.colour = {0.0f, 1.0f, 1.0f};
                    quad.translation = m_camera.position + indicator;
                    RenderAPI::draw(quad, 1.0f);
                }
            });
        }

        RenderAPI::end();
    }
}
