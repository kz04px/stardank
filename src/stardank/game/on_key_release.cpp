#include "../events/keyboard_event.hpp"
#include "../inputs.hpp"
#include "game.hpp"

void Game::on_key_release(KeyReleaseEvent &e) {
    switch (e.key()) {
        case 256:  // Esc
            break;
        case 32:  // Space
            break;
        case 290:  // F1
            break;
        case 291:  // F2
            break;
        case 292:  // F3
            break;
        case 293:  // F4
            break;
        case 49:  // 1
            break;
        case 50:  // 2
            break;
        case 51:  // 3
            break;
        case 52:  // 4
            break;
        case 53:  // 5
            break;
        default:
            break;
    }
}
