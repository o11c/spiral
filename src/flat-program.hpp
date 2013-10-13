#ifndef FLAT_PROGRAM_HPP
#define FLAT_PROGRAM_HPP

#include "flat-vertex-shader.hpp"
#include "simple-fragment-shader.hpp"

class FlatProgram
{
    unsigned int program;
    FlatVertexShader *vs;
    SimpleFragmentShader *fs;

#define ATTRIBUTE(name, type)   public: int name##Attribute;
#define UNIFORM(name, type)     private: int name##Uniform;
#include "vertex-flat.glsl.def"
#undef ATTRIBUTE
#undef UNIFORM
public:
    FlatProgram(FlatVertexShader *v, SimpleFragmentShader *f);
    ~FlatProgram();

    void load();
};

#endif //FLAT_PROGRAM_HPP
