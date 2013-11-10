#ifndef TEXTURE_PROGRAM_HPP
#define TEXTURE_PROGRAM_HPP

#include "texture-vertex-shader.hpp"
#include "texture-fragment-shader.hpp"

#include "../math/matrix.hpp"

class TextureProgram
{
    unsigned int program;
    TextureVertexShader *vs;
    TextureFragmentShader *fs;

#define ATTRIBUTE(name, type)   public: int name##Attribute;
#define UNIFORM(name, type)     private: int name##Uniform;
#include "vertex-texture.glsl.def"
#undef ATTRIBUTE
#undef UNIFORM
public:
    TextureProgram(TextureVertexShader *v, TextureFragmentShader *f);
    ~TextureProgram();

    void load();
};

#endif //TEXTURE_PROGRAM_HPP
