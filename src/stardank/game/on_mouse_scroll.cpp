#include "../events/mouse_event.hpp"
#include "game.hpp"

void Game::on_mouse_scroll(MouseScrollEvent &e) {
    // Forwards
    if (e.amount() > 0.1) {
        m_camera.size /= 1.1f;
    }
    // backwards
    else if (e.amount() < -0.1) {
        m_camera.size *= 1.1f;
    }
}
