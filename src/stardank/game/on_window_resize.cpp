#include "../events/window_event.hpp"
#include "../window/window.hpp"
#include "game.hpp"

void Game::on_window_resize(WindowResizeEvent &e) {
    m_camera.resize(e.width(), e.height());
    m_window_width = e.width();
    m_window_height = e.height();
}
