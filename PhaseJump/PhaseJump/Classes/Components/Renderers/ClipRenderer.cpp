#include "ClipRenderer.h"
#include "ColorRenderer.h"
#include "QuadMeshBuilder.h"
#include "RenderTypes.h"
#include "StencilPopRenderModel.h"
#include "StencilPushRenderModel.h"
#include "WorldNode.h"

using namespace PJ;

RenderModelList ClipRenderer::RenderModels() {
    if (!stencilMaterial) {
        // Build material, color doesn't matter for stencil
        stencilMaterial = ColorRenderer::MakeMaterial(RenderOpacityType::Opaque);
    }

    // TODO: why not use RendererModel here?
    auto pushModel = MAKE<StencilPushRenderModel>();
    pushModel->mesh = QuadMeshBuilder(worldSize).BuildMesh();
    pushModel->material = stencilMaterial.get();
    if (owner) {
        pushModel->matrix = owner->ModelMatrix();
    }
    RenderModelList result;
    result.push_back(pushModel);
    return result;
}

RenderModelList ClipRenderer::PostRenderModels() {
    auto pop = MAKE<StencilPopRenderModel>();
    RenderModelList result;
    result.push_back(pop);
    return result;
}
