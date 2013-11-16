#ifndef TEXTURE_FRAGMENT_SHADER_HPP
#define TEXTURE_FRAGMENT_SHADER_HPP

class TextureFragmentShader
{
    friend class TextureProgram;
    friend class NewTextureProgram;
    unsigned int shader;
public:
    TextureFragmentShader();
    ~TextureFragmentShader();
};

#endif //TEXTURE_FRAGMENT_SHADER_HPP
