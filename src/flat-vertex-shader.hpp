#ifndef FLAT_VERTEX_SHADER_HPP
#define FLAT_VERTEX_SHADER_HPP

class FlatVertexShader
{
    friend class FlatProgram;
    unsigned int shader;
public:
    FlatVertexShader();
    ~FlatVertexShader();
};

#endif //FLAT_VERTEX_SHADER_HPP
