#ifndef RENDERER_QUAD_HPP
#define RENDERER_QUAD_HPP

#include <glm/glm.hpp>
#include "colour.hpp"

struct Quad {
    glm::vec2 vertices[4] = {};
    glm::vec2 translation = {};
    glm::vec2 uv[4] = {};
    Colour colour = {};
    float rotation = 0.0f;
};

#endif
