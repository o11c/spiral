#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <vector>

#include "../glue/texture.hpp"
#include "../math/quat.hpp"
#include "../math/vector.hpp"
#include "../math/matrix.hpp"

struct Material
{
    std::unique_ptr<sampler2D> ambient;
    std::unique_ptr<sampler2D> diffuse;
    std::unique_ptr<sampler2D> specular;
    //std::unique_ptr<sampler2D> normal;
    float shininess;
};

struct Light
{
    vec4 position;
    vec4 color;
};

struct Context
{
    mat4 ModelView, Projection;
    Light light;
};

struct Model
{
    virtual ~Model() {}
    virtual void draw(Context&) = 0;
};

struct PositionedModel
{
    vec3 offset;
    float scale;
    quat orientation;
    std::unique_ptr<Model> model;
public:
    void draw(Context&);
};

struct Scene
{
    Light light;
    std::vector<PositionedModel> models;
public:
    void draw(Context&);
};

#endif //SCENE_HPP