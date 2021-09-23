#include "../camera.hpp"
#include "../renderer/quad.hpp"
#include "../renderer/renderer.hpp"
#include "../renderer/triangle.hpp"
#include "game.hpp"

static const Colour team_colours[] = {
    {0.8f, 0.8f, 0.8f},
    {1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
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

        // Title
        RenderAPI::draw_text("Map", 0.0f, 0.0f, 1.0f, 10.0f);

        RenderAPI::end();
    } else {
        auto view = m_registry.view<const Body, const Render>();

        RenderAPI::begin(m_camera);
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
        RenderAPI::end();
    }
}
