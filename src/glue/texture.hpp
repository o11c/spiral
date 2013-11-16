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

struct image
{
    color *data;
    int width, height;
};

class sampler2D
{
public:
    unsigned int which, object;
public:
    sampler2D(unsigned which, const image& img);
    ~sampler2D();
};

#endif //TEXTURE_HPP
