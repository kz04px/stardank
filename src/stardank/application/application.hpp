#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>
#include "../camera.hpp"
#include "state.hpp"

struct Event;
struct KeyPressEvent;
struct KeyReleaseEvent;
struct WindowCloseEvent;
struct WindowResizeEvent;
struct MouseDownEvent;
struct MouseMoveEvent;
struct MouseUpEvent;
struct MouseScrollEvent;
class Window;
class Game;

class Application {
   public:
    Application(int argc, char **argv);

    ~Application();

    void run();

    void on_event(Event &e);

    void on_key_press(KeyPressEvent &e);

    void on_key_release(KeyReleaseEvent &e);

    void on_window_close(WindowCloseEvent &e);

    void on_window_resize(WindowResizeEvent &e);

    void on_mouse_down(MouseDownEvent &e);

    void on_mouse_up(MouseUpEvent &e);

    void on_mouse_move(MouseMoveEvent &e);

    void on_mouse_scroll(MouseScrollEvent &e);

   private:
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Game> m_game;
    Camera m_camera;
    State m_state;
    bool m_paused;
    bool m_quit;
};

#endif
