#include "texture.hpp"
#include <stb/stb_image.h>

Texture::Texture() : id{}, width{}, height{}, channels{} {
    glCreateTextures(GL_TEXTURE_2D, 1, &id);
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

[[nodiscard]] Texture Texture::from_file(const std::string &path) {
    Texture t;
    t.load_file(path);
    return t;
}

void Texture::load_file(const std::string &path) {
    int w = 0;
    int h = 0;

    stbi_set_flip_vertically_on_load(1);
    stbi_uc *data = stbi_load(path.c_str(), &w, &h, &channels, 4);

    if (!data) {
        throw "no data";
    }

    width = w;
    height = h;

    glTextureStorage2D(id, 1, GL_RGBA8, width, height);

    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

void Texture::bind() const noexcept {
    glBindTexture(GL_TEXTURE_2D, id);
}
