#include "RendererModel.h"

using namespace std;
using namespace PJ;

RendererModel::RendererModel(Vector3 worldSize) :
    worldSize(worldSize) {
    buildVertexColorsFunc = [](RendererModel const& model, VectorList<RenderColor>& colors) {
        colors.clear();

        if (IsEmpty(model.colors)) {
            colors.resize(model.mesh.Vertices().size(), Color::white);
        } else {
            // Default behavior: Pick the first color
            // Renders can provide their own colors builder func
            colors.resize(model.mesh.Vertices().size(), model.colors[0]);
        }
    };
}

void RendererModel::SetBuildMeshFunc(BuildMeshFunc value) {
    buildMeshFunc = value;
    SetMeshNeedsBuild();
}

void RendererModel::SetWorldSize(Vector3 value) {
    GUARD(worldSize != value)
    worldSize = value;
    SetMeshNeedsBuild();
}

void RendererModel::SetBuildVertexColorsFunc(BuildColorsFunc value) {
    buildVertexColorsFunc = value;
    SetVertexColorsNeedsBuild();
}
