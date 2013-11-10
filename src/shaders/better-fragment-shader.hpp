#ifndef BETTER_FRAGMENT_SHADER_HPP
#define BETTER_FRAGMENT_SHADER_HPP

class BetterFragmentShader
{
    friend class ShadeProgram;
    unsigned int shader;
public:
    BetterFragmentShader();
    ~BetterFragmentShader();
};

#endif //BETTER_FRAGMENT_SHADER_HPP
