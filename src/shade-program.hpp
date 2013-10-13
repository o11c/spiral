#ifndef SHADE_PROGRAM_HPP
#define SHADE_PROGRAM_HPP

#include "shade-vertex-shader.hpp"
#include "simple-fragment-shader.hpp"

#include "matrix.hpp"

class ShadeProgram
{
    unsigned int program;
    ShadeVertexShader *vs;
    SimpleFragmentShader *fs;

#define ATTRIBUTE(name, type)   public: int name##Attribute;
#define UNIFORM(name, type)     private: int name##Uniform;
#include "vertex-shaded.glsl.def"
#undef ATTRIBUTE
#undef UNIFORM
public:
    ShadeProgram(ShadeVertexShader *v, SimpleFragmentShader *f);
    ~ShadeProgram();

    void load();
};

#endif //SHADE_PROGRAM_HPP
