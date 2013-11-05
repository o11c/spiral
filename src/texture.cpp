#include "texture.hpp"

#include "gl_wrap.hpp"

sampler2D::sampler2D(unsigned i, const image& img)
{
    which = i;
    glGenTextures(1, &object);
    glBindTexture(GL_TEXTURE_2D, object);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, /*level*/0, /*internal format*/GL_RGBA,
            img.width, img.height, /*border*/0, /*format*/GL_RGBA, /*type*/GL_UNSIGNED_BYTE,
            img.data);
}
sampler2D::~sampler2D()
{
    glDeleteTextures(1, &object);
}
