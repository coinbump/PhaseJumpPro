#include "RendererModel.h"
#include "RenderFeature.h"

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

void RendererModel::SetOnColorsChangeFunc(RendererFunc value) {
    onColorsChangeFunc = value;

    GUARD(onColorsChangeFunc)
    onColorsChangeFunc(*this);
}

void RendererModel::SetColors(VectorList<ColorType> const& value) {
    colors = value;
    SetVertexColorsNeedsBuild();

    GUARD(onColorsChangeFunc)
    onColorsChangeFunc(*this);
}

void RendererModel::SetColor(ColorType value) {
    SetColors({ value });
}

void RendererModel::SetAlpha(float value) {
    auto colors = this->colors;
    std::for_each(colors.begin(), colors.end(), [=](auto& color) { color.a = value; });
    SetColors(colors);
}
