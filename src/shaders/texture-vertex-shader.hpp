#ifndef TEXTURE_VERTEX_SHADER_HPP
#define TEXTURE_VERTEX_SHADER_HPP

class TextureVertexShader
{
    friend class TextureProgram;
    unsigned int shader;
public:
    TextureVertexShader();
    ~TextureVertexShader();
};

#endif //TEXTURE_VERTEX_SHADER_HPP
