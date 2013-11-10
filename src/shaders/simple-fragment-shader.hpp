#ifndef SIMPLE_FRAGMENT_SHADER_HPP
#define SIMPLE_FRAGMENT_SHADER_HPP

class SimpleFragmentShader
{
    friend class FlatProgram;
    unsigned int shader;
public:
    SimpleFragmentShader();
    ~SimpleFragmentShader();
};

#endif //SIMPLE_FRAGMENT_SHADER_HPP
