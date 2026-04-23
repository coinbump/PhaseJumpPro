#include "gtest/gtest.h"
#include "BatchByMaterialRenderProcessor.h"
#include "MaterialRenderModel.h"
#include "RenderMaterial.h"
#include "RenderPassModel.h"
#include "StencilPopRenderModel.h"
#include "StencilPushRenderModel.h"

using namespace PJ;
using namespace std;

namespace BatchByMaterialRenderProcessorTests {
    /// Material that returns a fixed propertyId — lets tests pin "same material" groupings
    /// without dragging in the real RenderMaterial setup.
    class TestMaterial : public RenderMaterial {
    public:
        String pid;

        TestMaterial(String id) :
            pid(id) {}

        // The real RenderMaterial::PropertyId hashes configured features/textures/shader.
        // For batching, the processor keys on propertyId — same pid means same batch.
    };

    /// Build a MaterialRenderModel node with a given material and a trivial mesh (so
    /// Combine's vertex work doesn't dereference empty data).
    RenderPassModel::NodeSharedPtr MakeMaterialNode(
        RenderPassModel& pass, RenderPassModel::NodeSharedPtr parent,
        SP<RenderMaterial> const& material
    ) {
        auto m = MAKE<MaterialRenderModel>();
        m->SetMaterial(material.get());

        Mesh mesh;
        VectorList<Vector3> verts{ Vector3{ 0, 0, 0 }, Vector3{ 1, 0, 0 }, Vector3{ 0, 1, 0 } };
        mesh.SetVertices(verts);
        VectorList<uint32_t> tris{ 0, 1, 2 };
        mesh.Triangles() = tris;
        m->SetMesh(mesh);
        VectorList<RenderColor> colors(verts.size(), RenderColor(255, 255, 255, 255));
        m->SetVertexColors(colors);

        return pass.Add(m, parent);
    }

    size_t ChildCount(RenderPassModel::NodeSharedPtr const& node) {
        size_t n = 0;
        for (auto& edge : node->Edges()) {
            if (edge && edge->toNode && edge->toNode->Value()) {
                ++n;
            }
        }
        return n;
    }
} // namespace BatchByMaterialRenderProcessorTests

using namespace BatchByMaterialRenderProcessorTests;

TEST(BatchByMaterialRenderProcessor, ContiguousSameMaterialSiblings_CollapseIntoOne)
{
    auto pass = MAKE<RenderPassModel>();
    auto material = MAKE<TestMaterial>("m1");

    auto parent = pass->rootNode;
    MakeMaterialNode(*pass, parent, material);
    MakeMaterialNode(*pass, parent, material);
    MakeMaterialNode(*pass, parent, material);

    ASSERT_EQ(3u, ChildCount(parent));

    BatchByMaterialRenderProcessor processor;
    processor.Process(*parent);

    // All three same-material siblings collapse into a single combined node.
    EXPECT_EQ(1u, ChildCount(parent));
}

TEST(BatchByMaterialRenderProcessor, StencilPushBreaksBatching)
{
    auto pass = MAKE<RenderPassModel>();
    auto material = MAKE<TestMaterial>("m1");

    auto parent = pass->rootNode;
    MakeMaterialNode(*pass, parent, material);
    MakeMaterialNode(*pass, parent, material);
    MakeMaterialNode(*pass, parent, material);

    // StencilPush breaks the run; materials after it start a new batch.
    pass->Add(MAKE<StencilPushRenderModel>(), parent);

    MakeMaterialNode(*pass, parent, material);
    MakeMaterialNode(*pass, parent, material);
    MakeMaterialNode(*pass, parent, material);

    pass->Add(MAKE<StencilPopRenderModel>(), parent);

    ASSERT_EQ(8u, ChildCount(parent));

    BatchByMaterialRenderProcessor processor;
    processor.Process(*parent);

    // Expected: [batched-m1, stencilPush, batched-m1, stencilPop] → 4 children.
    EXPECT_EQ(4u, ChildCount(parent));
}

TEST(BatchByMaterialRenderProcessor, DifferentMaterialsRemainSeparate)
{
    auto pass = MAKE<RenderPassModel>();
    auto m1 = MAKE<TestMaterial>("m1");
    auto m2 = MAKE<TestMaterial>("m2");

    auto parent = pass->rootNode;
    MakeMaterialNode(*pass, parent, m1);
    MakeMaterialNode(*pass, parent, m1);
    MakeMaterialNode(*pass, parent, m2);
    MakeMaterialNode(*pass, parent, m2);
    MakeMaterialNode(*pass, parent, m1);

    ASSERT_EQ(5u, ChildCount(parent));

    BatchByMaterialRenderProcessor processor;
    processor.Process(*parent);

    // Expected: [batched-m1(×2), batched-m2(×2), lone-m1] → 3 children.
    EXPECT_EQ(3u, ChildCount(parent));
}

TEST(BatchByMaterialRenderProcessor, BatchingRecursesIntoNonMaterialSubtrees)
{
    auto pass = MAKE<RenderPassModel>();
    auto material = MAKE<TestMaterial>("m1");

    auto parent = pass->rootNode;
    auto push = pass->Add(MAKE<StencilPushRenderModel>(), parent);

    // Materials inside the push subtree should batch together.
    MakeMaterialNode(*pass, push, material);
    MakeMaterialNode(*pass, push, material);
    MakeMaterialNode(*pass, push, material);

    pass->Add(MAKE<StencilPopRenderModel>(), parent);

    ASSERT_EQ(3u, ChildCount(push));

    BatchByMaterialRenderProcessor processor;
    processor.Process(*parent);

    // Root's direct children unchanged (push + pop), but push's subtree batched to 1.
    EXPECT_EQ(2u, ChildCount(parent));
    EXPECT_EQ(1u, ChildCount(push));
}

TEST(BatchByMaterialRenderProcessor, CameraScopedProcess_BatchesUnderCameraNode)
{
    // Simulates the camera-scoped invocation: processor is given a camera's DAG node as
    // its scope root, not the pass root. Batching applies inside that camera's subtree.
    auto pass = MAKE<RenderPassModel>();
    auto material = MAKE<TestMaterial>("m1");

    // Fake camera node — a bare SomeRenderModel stand-in (RenderCameraModel would require
    // Camera/World setup that tests don't have).
    auto cameraMarker = MAKE<SomeRenderModel>();
    auto cameraNode = pass->Add(cameraMarker, pass->rootNode);

    MakeMaterialNode(*pass, cameraNode, material);
    MakeMaterialNode(*pass, cameraNode, material);
    MakeMaterialNode(*pass, cameraNode, material);

    ASSERT_EQ(3u, ChildCount(cameraNode));

    BatchByMaterialRenderProcessor processor;
    processor.Process(*cameraNode);

    EXPECT_EQ(1u, ChildCount(cameraNode));
}
