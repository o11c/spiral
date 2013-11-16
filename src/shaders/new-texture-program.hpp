#ifndef NEW_TEXTURE_PROGRAM_HPP
#define NEW_TEXTURE_PROGRAM_HPP

#include "texture-vertex-shader.hpp"
#include "texture-fragment-shader.hpp"

#include "../mesh/scene.hpp"
#include "../math/matrix.hpp"

// identical to the other, but with a different load interface
class NewTextureProgram
{
    unsigned int program;
    TextureVertexShader *vs;
    TextureFragmentShader *fs;

#define ATTRIBUTE(name, type)   public: int name##Attribute;
#define UNIFORM(name, type)     private: int name##Uniform;
#include "vertex-texture.glsl.def"
#include "fragment-texture.glsl.def"
#undef ATTRIBUTE
#undef UNIFORM
public:
    NewTextureProgram(TextureVertexShader *v, TextureFragmentShader *f);
    ~NewTextureProgram();

    void load(Context&, Material&);
};

#endif //NEW_TEXTURE_PROGRAM_HPP
