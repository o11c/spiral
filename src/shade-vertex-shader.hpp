#ifndef SHADE_VERTEX_SHADER_HPP
#define SHADE_VERTEX_SHADER_HPP

class ShadeVertexShader
{
    friend class ShadeProgram;
    unsigned int shader;
public:
    ShadeVertexShader();
    ~ShadeVertexShader();
};

#endif //SHADE_VERTEX_SHADER_HPP
