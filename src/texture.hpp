#ifndef TEXTURE_HPP
#define TEXTURE_HPP

struct color
{
    unsigned char data[4];

    color() = default;
    color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    : data{r, g, b, a}
    {}
};

struct sampler2D
{
    unsigned int which, object;
    sampler2D(unsigned which, color *rgba, unsigned w, unsigned h);
    ~sampler2D();
};

#endif //TEXTURE_HPP
