#include "Slice3TextureRenderer.h"
#include "RenderFeature.h"
#include "RenderMaterial.h"
#include "ShaderProgram.h"
#include "TiledMeshBuilder.h"

using namespace std;
using namespace PJ;

Slice3TextureRenderer::Slice3TextureRenderer(Config const& config) :
    Base({}),
    texture(config.texture),
    axis(config.axis),
    startInset(config.startInset),
    endInset(config.endInset) {

    model.material = MAKE<RenderMaterial>(RenderMaterial::Config{
        .texture = config.texture,
        .shaderId = ShaderId::TextureVary,
        .features = { { RenderFeature::Blend, RenderFeatureState::Enable } } });

    model.SetBuildMeshFunc([this](RendererModel const& model) {
        return BuildMesh(model.WorldSize());
    });

    if (config.texture) {
        Vector2 worldSize;
        worldSize.AxisValue(axis) = config.axisLength;
        Vector2 textureSize = texture->size;
        worldSize.AxisValueOrthogonal(axis) = textureSize.AxisValueOrthogonal(axis);
        SetWorldSize(worldSize);
    }
}

PJ::Slice3TextureRenderer::BuildModel Slice3TextureRenderer::MakeBuildModel(Vector3 worldSize
) const {
    auto material = model.material;
    GUARDR(material, {})
    GUARDR(texture, {})
    GUARDR_LOG(worldSize.x > 0 && worldSize.y > 0, {}, "ERROR. Sliced texture size is invalid")

    BuildModel result;

    Vector2 textureSize = texture->size;
    GUARDR_LOG(textureSize.x > 0 && textureSize.y > 0, {}, "ERROR. Invalid slice texture")

    result.start.worldSize.AxisValue(axis) = startInset;
    result.start.worldSize.AxisValueOrthogonal(axis) = textureSize.AxisValueOrthogonal(axis);
    result.start.tileSize = result.start.worldSize;

    result.start.uvRect.size.AxisValue(axis) = startInset / textureSize.AxisValue(axis);
    result.start.uvRect.size.AxisValueOrthogonal(axis) = 1;

    Vector2 size = worldSize;
    result.center.worldSize.AxisValue(axis) = size.AxisValue(axis) - startInset - endInset;
    result.center.worldSize.AxisValueOrthogonal(axis) = textureSize.AxisValueOrthogonal(axis);
    result.center.tileSize = result.center.worldSize;
    result.center.tileSize.AxisValue(axis) = textureSize.AxisValue(axis) - startInset - endInset;

    result.center.uvRect.origin.AxisValue(axis) = result.start.uvRect.size.AxisValue(axis);
    result.center.uvRect.size.AxisValue(axis) =
        result.center.tileSize.AxisValue(axis) / textureSize.AxisValue(axis);
    result.center.uvRect.size.AxisValueOrthogonal(axis) = 1;

    result.end.worldSize.AxisValue(axis) = endInset;
    result.end.worldSize.AxisValueOrthogonal(axis) = textureSize.AxisValueOrthogonal(axis);
    result.end.tileSize = result.end.worldSize;

    result.end.uvRect.origin.AxisValue(axis) = 1.0f - (endInset / textureSize.AxisValue(axis));
    result.end.uvRect.size.AxisValue(axis) = endInset / textureSize.AxisValue(axis);
    result.end.uvRect.size.AxisValueOrthogonal(axis) = 1;

    switch (axis) {
    case Axis2D::Y:
        std::swap(result.start, result.end);
        break;
    default:
        break;
    }

    return result;
}

Mesh Slice3TextureRenderer::BuildMesh(Vector3 worldSize) {
    auto bm = MakeBuildModel(worldSize);

    Mesh mesh;

    if (bm.start.IsValid()) {
        auto startMesh =
            TiledMeshBuilder(bm.start.worldSize, bm.start.tileSize, bm.start.uvRect).BuildMesh();

        Vector2 offset;
        offset.AxisValue(axis) = (bm.center.worldSize.AxisValue(axis) / 2.0f +
                                  bm.start.worldSize.AxisValue(axis) / 2.0f) *
                                 Vector2::upLeft.AxisValue(axis);

        startMesh.Offset(offset);
        mesh += startMesh;
    }
    if (bm.center.IsValid()) {
        auto centerMesh =
            TiledMeshBuilder(bm.center.worldSize, bm.center.tileSize, bm.center.uvRect).BuildMesh();

        mesh += centerMesh;
    }
    if (bm.end.IsValid()) {
        auto endMesh =
            TiledMeshBuilder(bm.end.worldSize, bm.end.tileSize, bm.end.uvRect).BuildMesh();

        Vector2 offset;
        offset.AxisValue(axis) =
            (bm.center.worldSize.AxisValue(axis) / 2.0f + bm.end.worldSize.AxisValue(axis) / 2.0f) *
            Vector2::downRight.AxisValue(axis);

        endMesh.Offset(offset);
        mesh += endMesh;
    }

    return mesh;
}
