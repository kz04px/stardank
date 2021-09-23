#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <string>

struct Texture {
    Texture();

    ~Texture();

    // static Texture from_file(const std::string &path);

    // void load_file(const std::string &path);

    void bind() const noexcept;

    GLuint id;
    unsigned int width;
    unsigned int height;
    int channels;
};

#endif
