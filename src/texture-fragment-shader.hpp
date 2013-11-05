#ifndef TEXTURE_FRAGMENT_SHADER_HPP
#define TEXTURE_FRAGMENT_SHADER_HPP

class TextureFragmentShader
{
    friend class TextureProgram;
    unsigned int shader;
public:
    TextureFragmentShader();
    ~TextureFragmentShader();
};

#endif //TEXTURE_FRAGMENT_SHADER_HPP
