#ifndef DRAWING_HPP
#define DRAWING_HPP

class Drawing
{
protected:
    ~Drawing() {}
public:
    virtual void draw() = 0;
};

#endif //DRAWING_HPP
