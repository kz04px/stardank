#include "../game/game.hpp"
#include "../renderer/renderer.hpp"
#include "../window/window.hpp"
#include "application.hpp"

void Application::run() {
    while (!m_quit) {
        // Events
        m_window->poll_events();

        RenderAPI::clear();

        switch (m_state) {
            case State::Menu: {
                auto camera = Camera();
                camera.position = {5.0f, 5.0f};
                camera.size = {10.0f, 10.0f};
                RenderAPI::begin(camera);
                RenderAPI::draw_text("Menu", 0.0f, 0.0f, 1.0f, 10.0f);
                RenderAPI::end();
                break;
            }
            case State::Game: {
                if (!m_paused) {
                    m_game->update(1.0f / 60);
                }

                m_game->render();

                if (m_paused) {
                    RenderAPI::begin(m_game->camera());
                    RenderAPI::draw_text("Paused", 0.0f, 0.0f, 1.0f, 10.0f);
                    RenderAPI::end();
                }

                break;
            }
            default:
                break;
        }

        // Swap buffers
        m_window->swap();
    }
}
