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
            case State::Menu:
                RenderAPI::begin(m_camera);
                RenderAPI::draw_text("Menu", 0.0f, 0.0f, 1.0f, 10.0f);
                RenderAPI::end();
                break;
            case State::Game: {
                if (m_paused) {
                    RenderAPI::begin(m_camera);
                    RenderAPI::draw_text("Paused", 0.0f, 0.0f, 1.0f, 10.0f);
                    RenderAPI::end();
                } else {
                    m_game->update(1.0f / 60);
                }
                const auto [x, y] = m_game->camera_position();
                m_camera.position = {x, y};
                m_game->render(m_camera);
                break;
            }
            case State::Map:
                RenderAPI::begin(m_camera);
                RenderAPI::draw_text("Map", 0.0f, 0.0f, 1.0f, 10.0f);
                RenderAPI::end();
                break;
            default:
                break;
        }

        // Swap buffers
        m_window->swap();
    }
}
