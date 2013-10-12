#ifndef SPINE_HPP
#define SPINE_HPP

#include "drawing.hpp"
#include "flat-program.hpp"

class Spine : public Drawing
{
    FlatProgram *program;
public:
    Spine(FlatProgram *p) : program(p) {}
    void draw() override;
};

#endif
