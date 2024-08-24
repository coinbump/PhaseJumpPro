#include "SomeCamera.h"
#include "Colliders2D.h"
#include "ColorRenderer.h"
#include "DevProfiler.h"
#include "Matrix4x4.h"
#include "QuadFrameMeshBuilder.h"
#include "RenderContextModel.h"
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
// }

#include <future>
#include <iostream>
#include <thread>

template <class Result>
struct Thread {
    std::thread thread;
    Result result;
};

VectorList<RenderModel>
MakeRenderModels(RenderContextModel const& contextModel, size_t start, size_t end) {
    VectorList<RenderModel> result;

    for (size_t i = start; i < end; i++) {
        auto& node = contextModel.nodes[i];

        List<SP<SomeRenderer>> renderers;
        node->TypeComponents<SomeRenderer>(renderers);
        for (auto& renderer : renderers) {
            /// Most renderers produce render models, for a list of draw calls to be sent to the
            /// GPU by the graphics engine
            auto thisRenderModels = renderer->MakeRenderModels(contextModel);
            AddRange(result, thisRenderModels);
        }
    }

    return result;
}

RenderProcessModel SomeCamera::MakeRenderModel(RenderContextModel const& contextModel) {
    RenderProcessModel result;

    GUARDR(owner, result)
    GUARDR(owner->World(), result)

    std::mutex renderModelsMutex;
    VectorList<RenderModel> renderModels;

    // FUTURE: track how many render models are produced for each render and use
    // that information for pre-emptive renderModel resize (Weighted average over past N cycles)
    // #define RESIZE

#ifdef RESIZE
    // Optimization: Pre-emptively resize the render models
    renderModels.resize(contextModel.nodes.size());
#endif

    {
#ifdef PROFILE
        DevProfiler devProfiler("Camera- Make RenderModels", [](String value) {
            std::cout << value;
        });

        cout << "Profile: Nodes count: " << contextModel.nodes.size() << std::endl;
#endif

        // #define PARALLEL

#ifdef PARALLEL
        constexpr size_t taskCount = 16;
        constexpr size_t minNodesPerTask = 1;

        size_t nodesSize = contextModel.nodes.size();
        size_t nodesPerTask = std::max(minNodesPerTask, nodesSize / taskCount);
        size_t start = 0;
        size_t end = start + nodesPerTask;
        std::vector<std::future<VectorList<RenderModel>>> futures;

        // TODO: use futures?
        // https://stackoverflow.com/questions/9094422/how-to-check-if-a-stdthread-is-still-running

        for (size_t i = 0; i < taskCount; i++) {
            GUARD_BREAK(
                IsValidIndex(contextModel.nodes, start) && IsValidIndex(contextModel.nodes, end - 1)
            )

            std::future<VectorList<RenderModel>> future = std::async(std::launch::async, [=] {
                VectorList<RenderModel> result;

                //                std::thread::id this_id = std::this_thread::get_id();
                //                std::cout << "thread " << this_id << std::endl;

                for (size_t i = start; i < end; i++) {
                    auto& node = contextModel.nodes[i];

                    List<SP<SomeRenderer>> renderers;
                    node->TypeComponents<SomeRenderer>(renderers);
                    for (auto& renderer : renderers) {
                        /// Most renderers produce render models, for a list of draw calls to be
                        /// sent to the GPU by the graphics engine
                        auto thisRenderModels = renderer->MakeRenderModels(contextModel);
                        AddRange(result, thisRenderModels);
                    }
                }

                return result;
            });
            futures.push_back(std::move(future));

            start = end;
            end = std::min(contextModel.nodes.size(), start + nodesPerTask);

            // Process any remaining nodes (may not be evenly divisible)
            if (i == taskCount - 2) {
                end = contextModel.nodes.size();
            }
        }

        // Retrieve results from futures
        for (auto& future : futures) {
            try {
                auto const& result = future.get();
                std::lock_guard guard(renderModelsMutex);
                AddRange(renderModels, result);
            } catch (const std::exception& e) {
                // FUTURE: handle InterruptedException if needed:
                // https://stackoverflow.com/questions/2665569/in-what-cases-does-future-get-throw-executionexception-or-interruptedexception
                std::cerr << "Exception: " << e.what() << std::endl;
            }
        }
#endif

        size_t renderModelCount = 0;

        // FUTURE: this should be done on parallel threads
        for (auto& node : contextModel.nodes) {
            List<SP<SomeRenderer>> renderers;
            node->TypeComponents<SomeRenderer>(renderers);
            for (auto& renderer : renderers) {
                /// Some renderers use direct render (Example: imGui)
                renderer->RenderInto(contextModel);

#ifndef PARALLEL
                /// Most renderers produce render models, for a list of draw calls to be sent to the
                /// GPU by the graphics engine
                auto thisRenderModels = renderer->MakeRenderModels(contextModel);
                auto thisRenderModelCount = thisRenderModels.size();

                GUARD_CONTINUE(thisRenderModelCount > 0)

#ifdef RESIZE
                renderModels.resize(
                    std::max(renderModels.size(), renderModelCount + thisRenderModelCount)
                );

                std::copy_n(
                    thisRenderModels.begin(), thisRenderModels.size(),
                    renderModels.begin() + renderModelCount
                );

                renderModelCount += thisRenderModelCount;
#else
                AddRange(renderModels, thisRenderModels);
#endif
#endif
            }
        }
    }

    // #define SHOW_COLLIDERS

#ifdef SHOW_COLLIDERS
    auto colliderColor = Color::blue;
    auto colliderMaterial = ColorRenderer::MakeMaterial(colliderColor);
    result.tempMaterials.push_back(colliderMaterial);

    for (auto& node : contextModel.nodes) {
        auto colliders = node->GetComponents<SomeCollider2D>();
        for (auto& collider : colliders) {
            auto polyCollider = As<PolygonCollider2D>(collider.get());
            if (polyCollider) {
                Vector2 polySize(polyCollider->poly.Width(), polyCollider->poly.Height());

                ColorRenderer renderer(colliderMaterial, colliderColor, polySize);
                renderer.model.SetMeshBuilderFunc([=](MeshRendererModel const& model) {
                    QuadFrameMeshBuilder builder(polySize, Vector2(3, 3));
                    Mesh mesh = builder.BuildMesh();
                    return mesh;
                });

                renderer.owner = node;
                auto debugRenderModels = renderer.MakeRenderModels(contextModel);
                AddRange(renderModels, debugRenderModels);
                continue;
            }
        }
    }
#endif

    // #define SHOW_BOUNDS

#ifdef SHOW_BOUNDS
    auto boundsColor = Color::green;
    auto boundsMaterial = ColorRenderer::MakeMaterial(boundsColor);
    result.tempMaterials.push_back(boundsMaterial);

    for (auto& node : contextModel.nodes) {
        auto& components = node->Components();
        for (auto& component : components) {
            auto ws = As<WorldSizeable>(component.get());
            GUARD_CONTINUE(ws)

            auto worldSize = ws->WorldSize();
            GUARD_CONTINUE(worldSize)

            ColorRenderer renderer(boundsMaterial, boundsColor, *worldSize);
            renderer.model.SetMeshBuilderFunc([=](MeshRendererModel const& model) {
                QuadFrameMeshBuilder builder(*worldSize, Vector2(3, 3));
                Mesh mesh = builder.BuildMesh();
                return mesh;
            });

            renderer.owner = node;
            auto debugRenderModels = renderer.MakeRenderModels(contextModel);
            AddRange(renderModels, debugRenderModels);
            break;
        }
    }
#endif

    // TODO: Move this elsewhere?
    for (auto& model : renderModels) {
        GUARD_BREAK(model.isValid);
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
        UnorderedMap<String, List<RenderModel>> modelsByMaterial;
        for (auto& renderModel : renderModels) {
            GUARD_BREAK(renderModel.isValid)
            GUARD_CONTINUE(renderModel.material)
            modelsByMaterial[renderModel.material->PropertyId()].push_back(renderModel);
        }

        result.renderModels.reserve(modelsByMaterial.size());

        // Combine render models that have a matching material (doesn't have to be shared)
        for (auto& materialModels : modelsByMaterial) {
            auto& modelsInMaterial = materialModels.second;

            size_t vertexCount = 0;
            size_t trianglesCount = 0;
            size_t uvsCount = 0;
            size_t colorsCount = 0;

            for (auto& model : modelsInMaterial) {
                vertexCount += model.Vertices().size();
                trianglesCount += model.mesh.triangles.size();
                uvsCount += model.mesh.uvs.size();
                colorsCount += model.colors.size();
            }

            RenderModel renderModel = *materialModels.second.begin();
            renderModel.mesh.vertices.resize(vertexCount);
            renderModel.mesh.triangles.resize(trianglesCount);
            renderModel.mesh.uvs.resize(uvsCount);
            renderModel.colors.resize(colorsCount);

            size_t vertexIndex = 0;
            size_t triangleIndex = 0;
            size_t uvIndex = 0;
            size_t colorIndex = 0;

            for (auto& model : modelsInMaterial) {
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

            result.renderModels.push_back(renderModel);
        }
    }

    return result;
}
