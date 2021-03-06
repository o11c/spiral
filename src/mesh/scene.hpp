#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <vector>

#include "../../config.h"

#include "../glue/texture.hpp"
#include "../math/quat.hpp"
#include "../math/vector.hpp"
#include "../math/matrix.hpp"
#include "../yaml/dumb.hpp"

class NewTextureProgram;

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
    // You would think that these were not necessary,
    // but gcc 4.7 and clang 3.2 and 3.3 disagree.
    // There is at least one DR about it.
    Model() = default;
    Model(Model&&) = default;
    Model& operator = (Model&&) = default;

    virtual ~Model() {}
    virtual void draw(Context&) = 0;
};

struct PositionedModel : public Model
{
    vec3 offset;
    float scale;
    quat orientation;
    std::unique_ptr<Model> model;
public:
    void draw(Context&) override;
};

struct Multi : public Model
{
    std::vector<PositionedModel> models;
public:
    Multi(NewTextureProgram *tp, YamlMulti multi);

    void draw(Context&) override;
};

struct Scene
{
    Light light;
    std::unique_ptr<Model> model;
public:
    void draw_scene(Context&);
};

#endif //SCENE_HPP
