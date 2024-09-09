#include "BatchByMaterialRenderProcessor.h"
#include "RenderWorldSystem.h"

using namespace std;
using namespace PJ;

// TODO: needs unit tests
void BatchByMaterialRenderProcessor::Process(RenderSystemModel& systemModel) {
    // Make sure everything is in world coordinates before we batch
    for (auto& model : systemModel.models) {
        model.mesh *= model.matrix;
        model.matrix.LoadIdentity();
    }

    // Force the z order for an orthographic camera
    // FUTURE: will need a different solution for perspective cameras
    float z = 0;
    float zStep = 1.0f / systemModel.models.size();
    std::for_each(systemModel.models.begin(), systemModel.models.end(), [&](RenderModel& model) {
        model.z = z;
        z += zStep * Vector3::back.z;
    });

    {
#ifdef PROFILE
        DevProfiler devProfiler("Camera- Combine RenderModels", [](String value) {
            std::cout << value;
        });
#endif

        // Group render models by material
        UnorderedMap<String, VectorList<RenderModel*>> modelsByMaterial;
        for (auto& renderModel : systemModel.models) {
            auto modelMaterial = renderModel.Material();
            GUARD_CONTINUE(modelMaterial)
            modelsByMaterial[modelMaterial->PropertyId()].push_back(&renderModel);
        }

        VectorList<RenderModel> models;
        models.reserve(modelsByMaterial.size());

        // Combine render models that have a matching material (doesn't have to be shared)
        for (auto& materialModels : modelsByMaterial) {
            auto& modelsInMaterial = materialModels.second;

            size_t vertexCount = 0;
            size_t trianglesCount = 0;
            size_t uvsCount = 0;
            size_t colorsCount = 0;

            for (auto& modelP : modelsInMaterial) {
                auto& model = *modelP;
                vertexCount += model.Vertices().size();
                trianglesCount += model.mesh.Triangles().size();
                uvsCount += model.mesh.UVs().size();

                // 1 color per vertex
                colorsCount += model.Vertices().size();

                auto thisColorsCount = model.colors.size();
                auto thisVertexCount = model.Vertices().size();
                if (thisColorsCount != thisVertexCount) {
                    PJLog(
                        "WARNING: colorsCount %d and vertexCount %d don't match",
                        (int)thisColorsCount, (int)thisVertexCount
                    )
                }
            }

            RenderModel renderModel = **materialModels.second.begin();
            VectorList<Vector3> vertices;
            vertices.resize(vertexCount);
            renderModel.mesh.SetVertices(vertices);
            renderModel.mesh.BaseTriangles().resize(trianglesCount);

            VectorList<Vector2> uvs;
            uvs.resize(uvsCount);
            renderModel.mesh.SetUVs(uvs);
            renderModel.colors.resize(colorsCount);

            size_t vertexIndex = 0;
            size_t triangleIndex = 0;
            size_t uvIndex = 0;
            size_t colorIndex = 0;

            for (auto& modelP : modelsInMaterial) {
                auto& model = *modelP;
                auto modelVertexCount = model.Vertices().size();
                auto modelTriangleCount = model.mesh.Triangles().size();
                auto modelUVCount = model.UVs().size();
                auto modelColorCount = model.colors.size();

                // Force the z order for an orthographic camera
                // FUTURE: will need a different solution for perspective cameras
                std::for_each(
                    model.ModifiableVertices().begin(), model.ModifiableVertices().end(),
                    [=](Vector3& vertex) { vertex.z = model.z; }
                );

                std::copy_n(
                    model.Vertices().cbegin(), modelVertexCount,
                    renderModel.ModifiableVertices().begin() + vertexIndex
                );
                std::copy_n(
                    model.UVs().cbegin(), modelUVCount, renderModel.mesh.UVs().begin() + uvIndex
                );
                std::copy_n(
                    model.mesh.Triangles().cbegin(), modelTriangleCount,
                    renderModel.mesh.BaseTriangles().begin() + triangleIndex
                );

                // Max: 1 color per vertex
                std::copy_n(
                    model.colors.cbegin(), std::min(modelColorCount, modelVertexCount),
                    renderModel.colors.begin() + colorIndex
                );

                for (int i = 0; i < modelTriangleCount; i++) {
                    renderModel.mesh.BaseTriangles()[triangleIndex + i] += vertexIndex;
                }

                vertexIndex += modelVertexCount;
                triangleIndex += modelTriangleCount;
                uvIndex += modelUVCount;

                // 1 color per vertex
                colorIndex += modelVertexCount;
            }

            models.push_back(renderModel);
        }

        systemModel.models = models;
    }
}
