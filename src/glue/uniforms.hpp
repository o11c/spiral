#ifndef UNIFORMS_HPP
#define UNIFORMS_HPP

#include "gl_wrap.hpp"
#include "../math/matrix.hpp"
#include "texture.hpp"

inline
void load_uniform_float(GLint location, float& value)
{
    glUniform1fv(location, 1, &value);
}
inline
void load_uniform_vec2(GLint location, vec2& value)
{
    glUniform2fv(location, 1, value.ptr());
}
inline
void load_uniform_vec3(GLint location, vec3& value)
{
    glUniform3fv(location, 1, value.ptr());
}
inline
void load_uniform_vec4(GLint location, vec4& value)
{
    glUniform4fv(location, 1, value.ptr());
}

inline
void load_uniform_int(GLint location, int& value)
{
    glUniform1iv(location, 1, &value);
}
inline
void load_uniform_ivec2(GLint location, ivec2& value)
{
    glUniform2iv(location, 1, value.ptr());
}
inline
void load_uniform_ivec3(GLint location, ivec3& value)
{
    glUniform3iv(location, 1, value.ptr());
}
inline
void load_uniform_ivec4(GLint location, ivec4& value)
{
    glUniform4iv(location, 1, value.ptr());
}

inline
void load_uniform_mat2(GLint location, mat2& value)
{
    glUniformMatrix2fv(location, 1, GL_FALSE, value.ptr());
}
inline
void load_uniform_mat3(GLint location, mat3& value)
{
    glUniformMatrix3fv(location, 1, GL_FALSE, value.ptr());
}
inline
void load_uniform_mat4(GLint location, mat4& value)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, value.ptr());
}

inline
void load_uniform_sampler2D(GLint location, sampler2D& value)
{
    glActiveTexture(GL_TEXTURE0 + value.which);
    glBindTexture(GL_TEXTURE_2D, value.object);
    glUniform1i(location, value.which);
}

#endif //UNIFORMS_HPP
