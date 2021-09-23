#include <iostream>
#include "../events/keyboard_event.hpp"
#include "../inputs.hpp"
#include "../renderer/renderer.hpp"
#include "application.hpp"

void Application::on_key_press(KeyPressEvent &e) {
    // Update inputs singleton
    Inputs::press(e.key());

    switch (e.key()) {
        case 256:  // Esc
            m_quit = true;
            break;
        case 32:  // Space
            m_paused = !m_paused;
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
            m_state = State::Menu;
            break;
        case 50:  // 2
            m_state = State::Game;
            break;
        case 51:  // 3
            m_state = State::Map;
            break;
        case 52:  // 4
            break;
        case 53:  // 5
            break;
        default:
            std::cout << "Unused key: " << e.key() << std::endl;
            break;
    }
}
