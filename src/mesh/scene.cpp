#include "scene.hpp"

#include "../lieu/mem.hpp"
#include "mesh.hpp"

Multi::Multi(NewTextureProgram *tp, YamlMulti multi)
{
    for (auto& ymesh : multi.meshes)
    {
        PositionedModel mesh;
        mesh.offset = ymesh.position;
        mesh.scale = ymesh.scale;
        mesh.orientation = quat(Degrees(ymesh.orient_angle), ymesh.orient_axis);
        mesh.model = make_unique<Mesh>(tp, ymesh.mesh);
        models.push_back(std::move(mesh));
    }
}

void Multi::draw(Context& ctx)
{
    for (PositionedModel& model : models)
        model.draw(ctx);
}

void PositionedModel::draw(Context& ctx)
{
    SavingMatrix sv(ctx.ModelView);
    ctx.ModelView.translate(offset);
    ctx.ModelView *= orientation;
    ctx.ModelView.scale({scale, scale, scale});
    model->draw(ctx);
}

void Scene::draw_scene(Context& ctx)
{
    ctx.light.position = ctx.ModelView * light.position;
    ctx.light.color = light.color;
    model->draw(ctx);
}
