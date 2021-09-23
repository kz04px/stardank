#include "../camera.hpp"
#include "../renderer/quad.hpp"
#include "../renderer/renderer.hpp"
#include "../renderer/triangle.hpp"
#include "game.hpp"

void Game::render(const Camera &camera) const noexcept {
    auto view = m_registry.view<const Body, const Render>();

    RenderAPI::begin(camera);
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
