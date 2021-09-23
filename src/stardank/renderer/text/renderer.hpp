#ifndef TEXT_RENDERER_HPP
#define TEXT_RENDERER_HPP

#include <glm/glm.hpp>
#include <string>
#include "../program.hpp"
#include "atlas.hpp"
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

class TextRenderer {
   public:
    struct VertexData {
        glm::vec3 position;
        glm::vec2 uv;
    };

    TextRenderer();

    ~TextRenderer();

    void draw(const std::string &text, float x, float y, const float height, const float layer = 0.0f);

    [[nodiscard]] std::pair<int, int> predict_size(const std::string &text, const float height);

    glm::mat4x4 m_view;

   private:
    Program m_program;
    FT_Library m_ft;
    Atlas m_atlas;
    GLuint m_vao;
    GLuint m_vbo;
};

#endif
