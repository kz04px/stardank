#include "application.hpp"
#include <iostream>
#include "../events/window_event.hpp"
#include "../game/game.hpp"
#include "../renderer/renderer.hpp"
#include "../window/window.hpp"

Application::Application(int argc, char** argv)
    : m_window{new Window()}, m_game{new Game()}, m_state{State::Game}, m_paused{false}, m_quit{false} {
    srand(time(0));
    m_window->set_callback(std::bind(&Application::on_event, this, std::placeholders::_1));
    auto event = WindowResizeEvent(m_window->width(), m_window->height());
    on_event(event);
    RenderAPI::init();
    RenderAPI::clear_colour(0.01f, 0.008f, 0.04f);

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    std::cout << "\n";
}

Application::~Application() {
    RenderAPI::destroy();
}
