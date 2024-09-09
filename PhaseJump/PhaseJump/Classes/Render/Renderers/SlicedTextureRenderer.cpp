#include "SlicedTextureRenderer.h"
#include "QuadMeshBuilder.h"
#include "RenderContextModel.h"
#include "RenderFeature.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"
#include "SomeShaderProgram.h"
#include "TiledMeshBuilder.h"

using namespace std;
using namespace PJ;

SlicedTextureRenderer::SlicedTextureRenderer(
    SP<SomeTexture> texture, Vector2 worldSize, SlicePoints slicePoints
) :
    Base(worldSize),
    texture(texture),
    slicePoints(slicePoints) {

    model.material = MAKE<RenderMaterial>();

    auto program = SomeShaderProgram::registry.find("texture.vary");
    GUARD(program != SomeShaderProgram::registry.end())
    model.material->SetShaderProgram(program->second);
    model.material->EnableFeature(RenderFeature::Blend, true);

    if (texture) {
        model.material->Add(texture);
    }

    model.SetMeshBuilderFunc([this](RendererModel const& model) {
        return BuildMesh(model.WorldSize());
    });
}

PJ::SlicedTextureRenderer::BuildModel SlicedTextureRenderer::MakeBuildModel() const {
    BuildModel result;

    auto material = model.material;
    GUARDR(material, result)
    GUARDR(texture, result)

    result.sliceLeft = (float)min(texture->size.x - 1, max(0, slicePoints.topLeft.x));
    result.sliceTop = (float)min(texture->size.y - 1, max(0, slicePoints.topLeft.y));
    result.sliceRightOffset = (float)min(texture->size.x - 1, max(0, slicePoints.bottomRight.x));
    result.sliceBottomOffset = (float)min(texture->size.y - 1, max(0, slicePoints.bottomRight.y));

    result.sliceRight = texture->size.x - result.sliceRightOffset;
    result.sliceBottom = texture->size.y - result.sliceBottomOffset;

    auto size = model.WorldSize();
    result.left = -size.x / 2.0f + result.sliceLeft;
    result.top = (size.y / 2.0f) * vecUp + result.sliceTop * vecDown;
    result.right = size.x / 2.0f - result.sliceRightOffset;
    result.bottom = (size.y / 2.0f) * vecDown + result.sliceBottomOffset * vecUp;

    result.textureSize = texture->size;
    result.topAndBottomWorldWidth = size.x - result.sliceLeft - result.sliceRightOffset;
    result.leftAndRightWorldHeight = size.y - result.sliceTop - result.sliceBottomOffset;
    result.topAndBottomSliceWidth =
        result.textureSize.x - result.sliceLeft - result.sliceRightOffset;
    result.leftAndRightSliceHeight =
        result.textureSize.y - result.sliceTop - result.sliceBottomOffset;

    result.uvLeft = (float)result.sliceLeft / (float)result.textureSize.x;
    result.uvLeftWidth = result.uvLeft;
    result.uvTopOffset = (float)result.sliceTop / (float)result.textureSize.y;
    result.uvTopHeight = result.uvTopOffset;
    result.uvTop = 1.0f - result.uvTopOffset;
    result.uvBottom = (float)result.sliceBottomOffset / (float)result.textureSize.y;
    result.uvBottomHeight = result.uvBottom;
    result.uvRight = 1.0f - ((float)result.sliceRightOffset / (float)result.textureSize.x);
    result.uvRightWidth = (float)result.sliceRightOffset / (float)result.textureSize.x;
    result.uvTopAndBottomWidth =
        (float)(result.sliceRight - result.sliceLeft) / (float)result.textureSize.x;
    result.uvLeftAndRightHeight =
        (float)(result.sliceBottom - result.sliceTop) / (float)result.textureSize.y;

    result.topLeftUVRect =
        UVRect(Vector2(0, result.uvTop), Vector2(result.uvLeftWidth, result.uvTopHeight));
    result.topUVRect = UVRect(
        Vector2(result.uvLeft, result.uvTop),
        Vector2(result.uvTopAndBottomWidth, result.uvTopHeight)
    );
    result.topRightUVRect = UVRect(
        Vector2(result.uvRight, result.uvTop), Vector2(result.uvRightWidth, result.uvTopHeight)
    );
    result.leftUVRect = UVRect(
        Vector2(0, result.uvBottom), Vector2(result.uvLeftWidth, result.uvLeftAndRightHeight)
    );
    result.centerUVRect = UVRect(
        Vector2(result.uvLeft, result.uvBottom),
        Vector2(
            1.0f - result.uvLeftWidth - result.uvRightWidth,
            1.0f - result.uvTopHeight - result.uvBottomHeight
        )
    );
    result.rightUVRect = UVRect(
        Vector2(result.uvRight, result.uvBottom),
        Vector2(result.uvRightWidth, result.uvLeftAndRightHeight)
    );
    result.bottomLeftUVRect =
        UVRect(Vector2(0, 0), Vector2(result.uvLeftWidth, result.uvBottomHeight));
    result.bottomUVRect = UVRect(
        Vector2(result.uvLeft, 0), Vector2(result.uvTopAndBottomWidth, result.uvBottomHeight)
    );
    result.bottomRightUVRect =
        UVRect(Vector2(result.uvRight, 0), Vector2(result.uvRightWidth, result.uvBottomHeight));

    return result;
}

Mesh SlicedTextureRenderer::BuildMesh(Vector3 worldSize) {
    auto material = model.material;
    GUARDR(material && texture, {})

    Mesh mesh;

    std::array<Mesh, 9> meshList;

    auto bm = MakeBuildModel();

    meshList[MeshIndex::TopLeft] =
        TiledMeshBuilder(
            Vector2(bm.sliceLeft, bm.sliceTop), Vector2(bm.sliceLeft, bm.sliceTop), bm.topLeftUVRect
        )
            .BuildMesh();
    meshList[MeshIndex::Top] = TiledMeshBuilder(
                                   Vector2(bm.topAndBottomWorldWidth, bm.sliceTop),
                                   Vector2(bm.topAndBottomSliceWidth, bm.sliceTop), bm.topUVRect
    )
                                   .BuildMesh();
    meshList[MeshIndex::TopRight] = TiledMeshBuilder(
                                        Vector2(bm.sliceRightOffset, bm.sliceTop),
                                        Vector2(bm.sliceRightOffset, bm.sliceTop), bm.topRightUVRect
    )
                                        .BuildMesh();

    meshList[MeshIndex::Left] = TiledMeshBuilder(
                                    Vector2(bm.sliceLeft, bm.leftAndRightWorldHeight),
                                    Vector2(bm.sliceLeft, bm.leftAndRightSliceHeight), bm.leftUVRect
    )
                                    .BuildMesh();
    meshList[MeshIndex::Center] =
        TiledMeshBuilder(
            Vector2(bm.topAndBottomWorldWidth, bm.leftAndRightWorldHeight),
            Vector2(
                bm.textureSize.x - bm.sliceLeft - bm.sliceRightOffset,
                bm.textureSize.y - bm.sliceTop - bm.sliceBottomOffset
            ),
            bm.centerUVRect
        )
            .BuildMesh();
    meshList[MeshIndex::Right] =
        TiledMeshBuilder(
            Vector2(bm.sliceRightOffset, bm.leftAndRightWorldHeight),
            Vector2(bm.sliceRightOffset, bm.leftAndRightSliceHeight), bm.rightUVRect
        )
            .BuildMesh();

    meshList[MeshIndex::BottomLeft] =
        TiledMeshBuilder(
            Vector2(bm.sliceLeft, bm.sliceBottomOffset),
            Vector2(bm.sliceLeft, bm.sliceBottomOffset), bm.bottomLeftUVRect
        )
            .BuildMesh();
    meshList[MeshIndex::Bottom] =
        TiledMeshBuilder(
            Vector2(bm.topAndBottomWorldWidth, bm.sliceBottomOffset),
            Vector2(bm.topAndBottomSliceWidth, bm.sliceBottomOffset), bm.bottomUVRect
        )
            .BuildMesh();
    meshList[MeshIndex::BottomRight] =
        TiledMeshBuilder(
            Vector2(bm.sliceRightOffset, bm.sliceBottomOffset),
            Vector2(bm.sliceRightOffset, bm.sliceBottomOffset), bm.bottomRightUVRect
        )
            .BuildMesh();

    meshList[MeshIndex::TopLeft].OffsetBy(Vector2(
        (bm.left - bm.right) / 2.0f - bm.sliceLeft / 2.0f,
        (bm.top - bm.bottom) / 2.0f + bm.sliceTop / 2.0f
    ));
    meshList[MeshIndex::Top].OffsetBy(
        Vector2(0, bm.leftAndRightWorldHeight / 2.0f + bm.sliceTop / 2.0f)
    );
    meshList[MeshIndex::TopRight].OffsetBy(Vector2(
        (bm.right - bm.left) / 2.0f + bm.sliceRightOffset / 2.0f,
        (bm.top - bm.bottom) / 2.0f + bm.sliceTop / 2.0f
    ));
    meshList[MeshIndex::Left].OffsetBy(Vector2((bm.left - bm.right) / 2.0f - bm.sliceLeft / 2.0f, 0)
    );
    // meshList[MeshIndex::Center].OffsetBy(Vector2(0, 0));
    meshList[MeshIndex::Right].OffsetBy(
        Vector2((bm.right - bm.left) / 2.0f + bm.sliceRightOffset / 2.0f, 0)
    );
    meshList[MeshIndex::BottomLeft].OffsetBy(Vector2(
        (bm.left - bm.right) / 2.0f - bm.sliceLeft / 2.0f,
        (bm.bottom - bm.top) / 2.0f - bm.sliceBottomOffset / 2.0f
    ));
    meshList[MeshIndex::Bottom].OffsetBy(
        Vector2(0, -bm.leftAndRightWorldHeight / 2.0f - bm.sliceBottomOffset / 2.0f)
    );
    meshList[MeshIndex::BottomRight].OffsetBy(Vector2(
        (bm.right - bm.left) / 2.0f + bm.sliceRightOffset / 2.0f,
        (bm.bottom - bm.top) / 2.0f - bm.sliceBottomOffset / 2.0f
    ));

    mesh = meshList[MeshIndex::TopLeft] + meshList[MeshIndex::Top] + meshList[MeshIndex::TopRight] +
           meshList[MeshIndex::Left] + meshList[MeshIndex::Center] + meshList[MeshIndex::Right] +
           meshList[MeshIndex::BottomLeft] + meshList[MeshIndex::Bottom] +
           meshList[MeshIndex::BottomRight];

    return mesh;
}

VectorList<RenderModel> SlicedTextureRenderer::MakeRenderModels() {
    VectorList<RenderModel> result;

    auto material = model.material;
    if (nullptr == material) {
        PJLog("ERROR. Missing material.");
        return result;
    }

    auto mesh = model.Mesh();

    RenderModelBuilder builder;
    VectorList<SomeTexture*> textures{ texture.get() };
    auto renderModel = builder.Build(*this, mesh, *material, textures);

    renderModel.colors = VectorList<RenderColor>(mesh.Vertices().size(), Color::white);

    result.push_back(renderModel);
    return result;
}
