#include "../events/mouse_event.hpp"
#include "application.hpp"

void Application::on_mouse_up(MouseUpEvent &e) {
    switch (e.button()) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
}
