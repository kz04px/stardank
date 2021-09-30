#include "renderer.hpp"
#include "../shader.hpp"

TextureRenderer::TextureRenderer() : m_program{}, m_vao{}, m_vbo{}, m_buffer_index{}, m_data{} {
    // Program
    auto vs = Shader::from_string(Shader::Type::Vertex,
                                  "#version 430                                        \n"
                                  "                                                    \n"
                                  "layout(location = 0) uniform mat4 view;             \n"
                                  "                                                    \n"
                                  "layout(location = 0) in vec3 vertices;              \n"
                                  "layout(location = 1) in vec2 texcoord;              \n"
                                  "                                                    \n"
                                  "layout(location = 0) out vec2 texpos;               \n"
                                  "                                                    \n"
                                  "void main() {                                       \n"
                                  "    gl_Position = view * vec4(vertices, 1.0);       \n"
                                  "    texpos = texcoord;                              \n"
                                  "}                                                   \n");

    auto fs = Shader::from_string(Shader::Type::Fragment,
                                  "#version 430                                                      \n"
                                  "                                                                  \n"
                                  "layout(location = 1) uniform sampler2D tex;                       \n"
                                  "                                                                  \n"
                                  "layout(location = 0) in vec2 texpos;                              \n"
                                  "                                                                  \n"
                                  "layout(location = 0) out vec4 FragColor;                          \n"
                                  "                                                                  \n"
                                  "void main() {                                                     \n"
                                  "    FragColor = texture(tex, texpos);                             \n"
                                  "}                                                                 \n");

    m_program.attach(vs);
    m_program.attach(fs);
    m_program.link();
    m_program.detach(vs);
    m_program.detach(fs);

    m_program.use();

    // VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // VBO
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // Vertices
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<GLvoid *>(offsetof(VertexData, position)));

    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<GLvoid *>(offsetof(VertexData, uv)));

    if (!m_program.valid()) {
        throw "Text program invalid";
    }
}

TextureRenderer::~TextureRenderer() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}
