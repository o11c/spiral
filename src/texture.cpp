#include "texture.hpp"

#include "gl_wrap.hpp"

sampler2D::sampler2D(unsigned i, color *rgba, unsigned w, unsigned h)
{
    which = i;
    glGenTextures(1, &object);
    glBindTexture(GL_TEXTURE_2D, object);
    glTexImage2D(GL_TEXTURE_2D, /*level*/0, /*internal format*/GL_RGBA,
            w, h, /*border*/0, /*format*/GL_RGBA, /*type*/GL_UNSIGNED_BYTE,
            rgba);
}
sampler2D::~sampler2D()
{
    glDeleteTextures(1, &object);
}
