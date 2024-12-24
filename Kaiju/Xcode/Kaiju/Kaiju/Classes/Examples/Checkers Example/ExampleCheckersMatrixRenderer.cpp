#include "ExampleCheckersMatrixRenderer.h"

using namespace std;
using namespace PJ;
using namespace Example;
using namespace Checkers;

MatrixRenderer::MatrixRenderer(Vector3 worldSize) :
    Base(worldSize) {

    auto material = MAKE<RenderMaterial>(RenderMaterial::Config{ .shaderId = ShaderId::ColorVary });

    Vec2I matrixSize{ 8, 8 };
    model.material = material;

    model.SetBuildMeshFunc([=](auto& model) {
        Mesh mesh;

        Vector3 cellSize{ model.WorldSize().x / matrixSize.x, model.WorldSize().y / matrixSize.y,
                          0 };
        QuadMeshBuilder meshBuilder(cellSize);

        for (int y = 0; y < matrixSize.y; y++) {
            for (int x = 0; x < matrixSize.x; x++) {
                Vector2Int location(x, y);

                auto qm = meshBuilder.BuildMesh();
                qm.Offset(Vector2{ (model.WorldSize().x / 2.0f) * vecLeft +
                                       ((cellSize.x / 2.0f + cellSize.x * x) * vecRight),
                                   (model.WorldSize().y / 2.0f) * vecUp +
                                       ((cellSize.y / 2.0f + cellSize.y * y) * vecDown) });
                mesh += qm;
            }
        }

        return mesh;
    });

    model.SetBuildVertexColorsFunc([=](auto const& model, auto& colors) {
        colors.resize(matrixSize.x * matrixSize.y * 4);

        bool isDark = false;
        size_t i = 0;
        for (int y = 0; y < matrixSize.y; y++) {
            for (int x = 0; x < matrixSize.x; x++) {
                Vector2Int location(x, y);

                Color32 color = isDark ? Color32(67, 36, 16) : Color32(228, 211, 169);

                colors[i++] = color;
                colors[i++] = color;
                colors[i++] = color;
                colors[i++] = color;

                Toggle(isDark);
            }
            Toggle(isDark);
        }
    });
}
