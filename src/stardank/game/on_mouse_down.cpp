#include "../events/mouse_event.hpp"
#include "game.hpp"

void Game::on_mouse_down(MouseDownEvent &e) {
    switch (e.button()) {
        case 0:  // Left
            break;
        case 1:  // Right
            break;
        case 2:  // Middle
            break;
        default:
            break;
    }
}
