#include "BatchByMaterialRenderProcessor.h"
#include "RenderSystem.h"

using namespace std;
using namespace PJ;

// TODO: needs unit tests
void BatchByMaterialRenderProcessor::Process(RenderSystemModel& systemModel) {
    // Make sure everything is in world coordinates before we batch
    for (auto& model : systemModel.models) {
        model.mesh *= model.matrix;
        model.matrix.LoadIdentity();
    }

    {
#ifdef PROFILE
        DevProfiler devProfiler("Camera- Combine RenderModels", [](String value) {
            std::cout << value;
        });
#endif

        // Group render models by material
        UnorderedMap<String, List<RenderModel*>> modelsByMaterial;
        for (auto& renderModel : systemModel.models) {
            GUARD_CONTINUE(renderModel.material)
            modelsByMaterial[renderModel.material->PropertyId()].push_back(&renderModel);
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
                trianglesCount += model.mesh.triangles.size();
                uvsCount += model.mesh.uvs.size();
                colorsCount += model.colors.size();
            }

            RenderModel renderModel = **materialModels.second.begin();
            renderModel.mesh.vertices.resize(vertexCount);
            renderModel.mesh.triangles.resize(trianglesCount);
            renderModel.mesh.uvs.resize(uvsCount);
            renderModel.colors.resize(colorsCount);

            size_t vertexIndex = 0;
            size_t triangleIndex = 0;
            size_t uvIndex = 0;
            size_t colorIndex = 0;

            for (auto& modelP : modelsInMaterial) {
                auto& model = *modelP;
                auto modelVertexCount = model.Vertices().size();
                auto modelTriangleCount = model.mesh.triangles.size();
                auto modelUVCount = model.UVs().size();
                auto modelColorCount = model.colors.size();

                std::copy_n(
                            model.Vertices().begin(), modelVertexCount,
                            renderModel.Vertices().begin() + vertexIndex
                            );
                std::copy_n(model.UVs().begin(), modelUVCount, renderModel.UVs().begin() + uvIndex);
                std::copy_n(
                            model.mesh.triangles.begin(), modelTriangleCount,
                            renderModel.mesh.triangles.begin() + triangleIndex
                            );
                std::copy_n(
                            model.colors.begin(), modelColorCount, renderModel.colors.begin() + colorIndex
                            );

                for (int i = 0; i < modelTriangleCount; i++) {
                    renderModel.mesh.triangles[triangleIndex + i] += vertexIndex;
                }

                vertexIndex += modelVertexCount;
                triangleIndex += modelTriangleCount;
                uvIndex += modelUVCount;
                colorIndex += modelColorCount;
            }

            models.push_back(renderModel);
        }

        systemModel.models = models;
    }
}
