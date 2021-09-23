cmake_minimum_required(VERSION 3.10)

# Project
project(Stardank VERSION 1.0 LANGUAGES CXX)

# Find packages
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads)
find_package(Freetype)

# Includes
include_directories(src/)
include_directories(src/entt-3.8.1/single_include/)

# Flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add engine executable
add_executable(
    stardank

    src/stardank/main.cpp

    # Application
    src/stardank/application/application.cpp
    src/stardank/application/on_event.cpp
    src/stardank/application/on_key_press.cpp
    src/stardank/application/on_key_release.cpp
    src/stardank/application/on_mouse_down.cpp
    src/stardank/application/on_mouse_move.cpp
    src/stardank/application/on_mouse_scroll.cpp
    src/stardank/application/on_mouse_up.cpp
    src/stardank/application/on_window_close.cpp
    src/stardank/application/on_window_resize.cpp
    src/stardank/application/run.cpp

    # Render API
    src/stardank/renderer/program.cpp
    src/stardank/renderer/renderer.cpp
    src/stardank/renderer/shader.cpp
    src/stardank/renderer/texture.cpp

    # Text Renderer
    src/stardank/renderer/text/atlas.cpp
    src/stardank/renderer/text/draw_text.cpp
    src/stardank/renderer/text/renderer.cpp

    # Quad Renderer
    src/stardank/renderer/quad/draw_line.cpp
    src/stardank/renderer/quad/draw_point.cpp
    src/stardank/renderer/quad/draw_quad.cpp
    src/stardank/renderer/quad/draw_triangle.cpp
    src/stardank/renderer/quad/flush.cpp
    src/stardank/renderer/quad/renderer.cpp

    # Window
    src/stardank/window/window.cpp
    src/stardank/window/callback_cursor_position.cpp
    src/stardank/window/callback_mouse_scroll.cpp
    src/stardank/window/callback_keyboard.cpp
    src/stardank/window/callback_window_close.cpp
    src/stardank/window/callback_mouse_button.cpp
    src/stardank/window/callback_window_size.cpp

    # Game
    src/stardank/game/game.cpp
    src/stardank/game/on_event.cpp
    src/stardank/game/on_key_press.cpp
    src/stardank/game/on_key_release.cpp
    src/stardank/game/on_mouse_down.cpp
    src/stardank/game/on_mouse_move.cpp
    src/stardank/game/on_mouse_scroll.cpp
    src/stardank/game/on_mouse_up.cpp
    src/stardank/game/on_window_close.cpp
    src/stardank/game/on_window_resize.cpp
    src/stardank/game/render.cpp
    src/stardank/game/update.cpp
)

target_link_libraries(stardank glfw)
target_link_libraries(stardank GLEW)
target_link_libraries(stardank OpenGL)
target_link_libraries(stardank Threads::Threads)
target_link_libraries(stardank Freetype::Freetype)
