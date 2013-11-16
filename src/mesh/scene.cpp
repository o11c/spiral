#include "scene.hpp"

void PositionedModel::draw(Context& ctx)
{
    SavingMatrix sv(ctx.ModelView);
    ctx.ModelView.translate(offset);
    ctx.ModelView *= orientation;
    ctx.ModelView.scale({scale, scale, scale});
    model->draw(ctx);
}

void Scene::draw(Context& ctx)
{
    ctx.light.position = ctx.ModelView * light.position;
    ctx.light.color = light.color;
    for (PositionedModel& model : models)
        model.draw(ctx);
}
