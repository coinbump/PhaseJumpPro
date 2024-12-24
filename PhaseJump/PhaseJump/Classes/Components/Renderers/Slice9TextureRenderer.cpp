#include "Slice9TextureRenderer.h"
#include "RenderFeature.h"
#include "RenderMaterial.h"
#include "SomeShaderProgram.h"
#include "TiledMeshBuilder.h"

using namespace std;
using namespace PJ;

Slice9TextureRenderer::Slice9TextureRenderer(
    SP<SomeTexture> texture, Vector2 worldSize, SliceModel sliceModel
) :
    Base(worldSize),
    texture(texture),
    sliceModel(sliceModel) {

    model.material = MAKE<RenderMaterial>(RenderMaterial::Config{
        .texture = texture,
        .shaderId = ShaderId::TextureVary,
        .features = { { RenderFeature::Blend, RenderFeatureState::Enable } } });

    if (texture) {
        model.material->Add(texture);
    }

    model.SetBuildMeshFunc([this](RendererModel const& model) {
        return BuildMesh(model.WorldSize());
    });
}

PJ::Slice9TextureRenderer::BuildModel Slice9TextureRenderer::MakeBuildModel(Vector3 worldSize
) const {
    auto material = model.material;
    GUARDR(material, {})
    GUARDR(texture, {})

    GUARDR_LOG(worldSize.x > 0 && worldSize.y > 0, {}, "ERROR. Sliced texture size is invalid")

    BuildModel result;

    result.sliceLeft = (float)min(texture->size.x - 1, max(0, sliceModel.topLeftInsets.x));
    result.sliceTop = (float)min(texture->size.y - 1, max(0, sliceModel.topLeftInsets.y));
    result.sliceRightOffset =
        (float)min(texture->size.x - 1, max(0, sliceModel.bottomRightInsets.x));
    result.sliceBottomOffset =
        (float)min(texture->size.y - 1, max(0, sliceModel.bottomRightInsets.y));

    result.sliceRight = texture->size.x - result.sliceRightOffset;
    result.sliceBottom = texture->size.y - result.sliceBottomOffset;

    auto size = worldSize;
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

Mesh Slice9TextureRenderer::BuildMesh(Vector3 worldSize) {
    std::array<Mesh, 9> meshList;

    auto bm = MakeBuildModel(worldSize);
    bool isModelValid = bm.topAndBottomWorldWidth > 0 && bm.leftAndRightWorldHeight > 0;
    GUARDR_LOG(isModelValid, {}, "ERROR. Sliced texture size is invalid")

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

    meshList[MeshIndex::TopLeft].Offset(Vector2(
        (bm.left - bm.right) / 2.0f - bm.sliceLeft / 2.0f,
        (bm.top - bm.bottom) / 2.0f + bm.sliceTop / 2.0f
    ));
    meshList[MeshIndex::Top].Offset(
        Vector2(0, bm.leftAndRightWorldHeight / 2.0f + bm.sliceTop / 2.0f)
    );
    meshList[MeshIndex::TopRight].Offset(Vector2(
        (bm.right - bm.left) / 2.0f + bm.sliceRightOffset / 2.0f,
        (bm.top - bm.bottom) / 2.0f + bm.sliceTop / 2.0f
    ));
    meshList[MeshIndex::Left].Offset(Vector2((bm.left - bm.right) / 2.0f - bm.sliceLeft / 2.0f, 0));
    // meshList[MeshIndex::Center].OffsetBy(Vector2(0, 0));
    meshList[MeshIndex::Right].Offset(
        Vector2((bm.right - bm.left) / 2.0f + bm.sliceRightOffset / 2.0f, 0)
    );
    meshList[MeshIndex::BottomLeft].Offset(Vector2(
        (bm.left - bm.right) / 2.0f - bm.sliceLeft / 2.0f,
        (bm.bottom - bm.top) / 2.0f - bm.sliceBottomOffset / 2.0f
    ));
    meshList[MeshIndex::Bottom].Offset(
        Vector2(0, -bm.leftAndRightWorldHeight / 2.0f - bm.sliceBottomOffset / 2.0f)
    );
    meshList[MeshIndex::BottomRight].Offset(Vector2(
        (bm.right - bm.left) / 2.0f + bm.sliceRightOffset / 2.0f,
        (bm.bottom - bm.top) / 2.0f - bm.sliceBottomOffset / 2.0f
    ));

    Mesh mesh;
    mesh = meshList[MeshIndex::TopLeft] + meshList[MeshIndex::Top] + meshList[MeshIndex::TopRight] +
           meshList[MeshIndex::Left] + meshList[MeshIndex::Center] + meshList[MeshIndex::Right] +
           meshList[MeshIndex::BottomLeft] + meshList[MeshIndex::Bottom] +
           meshList[MeshIndex::BottomRight];

    return mesh;
}
