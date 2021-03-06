#ifndef GAME_HPP
#define GAME_HPP

#include <entt/entt.hpp>
#include <space/region.hpp>
#include "../camera.hpp"
#include "../renderer/texture.hpp"

struct Event;
struct KeyPressEvent;
struct KeyReleaseEvent;
struct WindowCloseEvent;
struct WindowResizeEvent;
struct MouseDownEvent;
struct MouseMoveEvent;
struct MouseUpEvent;
struct MouseScrollEvent;

[[nodiscard]] static float rand_between(const float a, const float b) {
    return a + (b - a) * static_cast<float>(rand()) / RAND_MAX;
}

class Game {
   public:
    Game();

    [[nodiscard]] auto camera() const noexcept {
        return m_camera;
    }

    void update(const float dt);

    void render() const noexcept;

    void load_system(const space::System &system);

    void on_event(Event &e);

    void on_key_press(KeyPressEvent &e);

    void on_key_release(KeyReleaseEvent &e);

    void on_window_close(WindowCloseEvent &e);

    void on_window_resize(WindowResizeEvent &e);

    void on_mouse_down(MouseDownEvent &e);

    void on_mouse_up(MouseUpEvent &e);

    void on_mouse_move(MouseMoveEvent &e);

    void on_mouse_scroll(MouseScrollEvent &e);

    space::Region m_region;

   private:
    Texture m_star_field_texture;
    Camera m_camera;
    entt::registry m_registry;
    entt::entity m_us;
    int m_system_selected;
    int m_window_width;
    int m_window_height;
    bool m_map_view;
};

#endif
