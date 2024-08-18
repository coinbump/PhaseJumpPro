#include "SomeCamera.h"
#include "Colliders2D.h"
#include "ColorRenderer.h"
#include "DevProfiler.h"
#include "Matrix4x4.h"
#include "QuadFrameMeshBuilder.h"
#include "RenderIntoModel.h"
#include "SomeCollider2D.h"
#include "SomeRenderContext.h"
#include "SomeRenderEngine.h"
#include "SomeRenderer.h"
#include "World.h"

// #define PROFILE

// TODO: temp
#include "GLShaderProgram.h"

using namespace std;
using namespace PJ;

// class RenderProcessModel {
//     VectorList<RenderModel> renderModels;
// };
//
// class RenderSetModel {
//     VectorList<RenderModel> renderModels;
// };
//
//// TODO: just make these funcs! (obviously)
// class RenderProcessor {
// public:
//     virtual ~RenderProcessor() {}
//
//     virtual RenderProcessModel Process(RenderProcessModel const& processModel) = 0;
// };
//
///// Sets z values of render model so they match their desired Z-order in 2D space
// class ZSort2DRenderProcessor : public RenderProcessor {
//     RenderProcessModel Process(RenderProcessModel const& processModel) override {
//         return processModel;
//     }
// };
//
///// Sorts render models to put opaque models first (front-to-back) and transparent models last
///// (back-to-front)
// class SortOpaqueModelsFirstRenderProcessor : public RenderProcessor {
//     RenderProcessModel Process(RenderProcessModel const& processModel) override {
//         return processModel;
//     }
// };
//
///// Combines render models that can be combined with each other
// class CombineModelsRenderProcessor : public RenderProcessor {
//     RenderProcessModel Process(RenderProcessModel const& processModel) override {
//         return processModel;
//     }
// };

CameraRenderModel SomeCamera::MakeRenderModel(
    VectorList<WorldNode*>& nodes, SomeRenderContext& renderContext,
    RenderContextModel& contextModel
) {
    CameraRenderModel result;

    GUARDR(owner, result)
    GUARDR(owner->World(), result)

    VectorList<RenderModel> renderModels;
    RenderIntoModel renderIntoModel(&renderContext);

    {
#ifdef PROFILE
        DevProfiler devProfiler("Camera- Make RenderModels", [](String value) {
            std::cout << value;
        });
#endif

        // FUTURE: this should be done on parallel threads
        for (auto& node : nodes) {
            auto renderers = node->TypeComponents<SomeRenderer>();
            for (auto& renderer : renderers) {
                /// Some renderers use direct render (Example: imGui)
                renderer->RenderInto(renderIntoModel);

                /// Most renderers produce render models, for a list of draw calls to be sent to the
                /// GPU by the graphics engine
                auto thisRenderModels = renderer->MakeRenderModels(renderIntoModel);
                AddRange(renderModels, thisRenderModels);
            }
        }
    }

    // Uncomment to show blue quads around colliders
    //    for (auto& node : nodes) {
    //        auto colliders = node->TypeComponents<SomeCollider2D>();
    //        for (auto& collider : colliders) {
    //            auto polyCollider = As<PolygonCollider2D>(collider.get());
    //            if (polyCollider) {
    //                Vector2 polySize(polyCollider->poly.Width(), polyCollider->poly.Height());
    //
    //                auto program = GLShaderProgram::registry["color.uniform"];
    //
    //                ColorRenderer renderer(Color::blue, polySize);
    //                renderer.model.SetMeshBuilderFunc([=](MeshRendererModel const& model) {
    //                    QuadFrameMeshBuilder builder(polySize, Vector2(3, 3));
    //                    Mesh mesh = builder.BuildMesh();
    //                    return mesh;
    //                });
    //
    //                renderer.material->SetShaderProgram(program);
    //                renderer.owner = node;
    //                auto debugRenderModels = renderer.MakeRenderModels(renderIntoModel);
    //                AddRange(renderModels, debugRenderModels);
    //                continue;
    //            }
    //        }
    //    }

    // TODO: Move this elsewhere?
    std::transform(
        renderModels.cbegin(), renderModels.cend(), renderModels.begin(),
        [](RenderModel model) {
            model.mesh *= model.matrix;
            model.matrix.LoadIdentity();
            return model;
        }
    );

    {
#ifdef PROFILE
        DevProfiler devProfiler("Camera- Combine RenderModels", [](String value) {
            std::cout << value;
        });
#endif

        // Group render models by material
        UnorderedMap<String, List<RenderModel>> modelsByMaterial;
        for (auto& renderModel : renderModels) {
            modelsByMaterial[renderModel.material.PropertyId()].push_back(renderModel);
        }

        result.renderModels.reserve(modelsByMaterial.size());

        // Combine render models that have a matching material (doesn't have to be shared)
        for (auto& materialModels : modelsByMaterial) {
            auto& modelsInMaterial = materialModels.second;

            size_t vertexCount = 0;
            size_t trianglesCount = 0;
            size_t uvsCount = 0;

            for (auto& model : modelsInMaterial) {
                vertexCount += model.Vertices().size();
                trianglesCount += model.mesh.triangles.size();
                uvsCount += model.mesh.uvs.size();
            }

            RenderModel renderModel = *materialModels.second.begin();
            renderModel.mesh.vertices.resize(vertexCount);
            renderModel.mesh.triangles.resize(trianglesCount);
            renderModel.mesh.uvs.resize(uvsCount);

            size_t vertexIndex = 0;
            size_t triangleIndex = 0;
            size_t uvIndex = 0;

            for (auto& model : modelsInMaterial) {
                auto modelVertexCount = model.Vertices().size();
                auto modelTriangleCount = model.mesh.triangles.size();
                auto modelUVCount = model.UVs().size();

                memcpy(
                    &renderModel.Vertices()[vertexIndex], model.Vertices().data(),
                    sizeof(Vector3) * modelVertexCount
                );
                memcpy(
                    &renderModel.UVs()[uvIndex], model.UVs().data(), sizeof(Vector2) * modelUVCount
                );
                memcpy(
                    &renderModel.mesh.triangles[triangleIndex], model.mesh.triangles.data(),
                    sizeof(uint32_t) * modelTriangleCount
                );

                for (int i = 0; i < modelTriangleCount; i++) {
                    renderModel.mesh.triangles[triangleIndex + i] += vertexIndex;
                }

                vertexIndex += modelVertexCount;
                triangleIndex += modelTriangleCount;
                uvIndex += modelUVCount;
            }

            result.renderModels.push_back(renderModel);
        }
    }

    return result;
}
