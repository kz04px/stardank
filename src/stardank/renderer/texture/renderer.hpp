#ifndef TEXTURE_RENDERER_HPP
#define TEXTURE_RENDERER_HPP

#include <glm/glm.hpp>
#include "../program.hpp"

class Quad;

class TextureRenderer {
   public:
    struct VertexData {
        glm::vec3 position;
        glm::vec2 uv;
    };

    TextureRenderer();

    ~TextureRenderer();

    void draw(const Quad &quad, const float layer = 0.0f);

    void flush();

    glm::mat4x4 m_view;

   private:
    constexpr static int m_max_buffer_size = 1024;
    Program m_program;
    GLuint m_vao;
    GLuint m_vbo;
    int m_buffer_index;
    VertexData m_data[m_max_buffer_size];
};

#endif
