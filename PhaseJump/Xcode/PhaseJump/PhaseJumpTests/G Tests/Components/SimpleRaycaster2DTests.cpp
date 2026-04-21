#include "gtest/gtest.h"

#include "Colliders2D.h"
#include "MockRenderContext.h"
#include "OrthoCamera.h"
#include "SimpleRaycaster2D.h"
#include "Viewport.h"
#include "World.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;
using namespace PJTest;

namespace SimpleRaycaster2DTests {
    struct SUT {
        SP<World> world;
        OrthoCamera* camera{};
        SimpleRaycaster2D* raycaster{};

        SUT() {
            world = MAKE<World>();
            auto renderContext = NEW<MockRenderContext>(MockRenderContext::Config{
                .size = { 400, 400 }, .pixelSize = { 400, 400 } });
            world->renderContext = std::move(renderContext);

            auto& cameraNode = world->AddNode();
            camera = &cameraNode.AddComponent<OrthoCamera>();
            raycaster = &cameraNode.AddComponent<SimpleRaycaster2D>();
        }
    };
} // namespace SimpleRaycaster2DTests

using namespace SimpleRaycaster2DTests;

TEST(SimpleRaycaster2D, HitsNodeInUnscaledViewport) {
    SUT sut;

    auto& viewportNode = sut.world->AddNode();
    auto& viewport = viewportNode.AddComponent<Viewport>();
    viewport.worldSize = Vector2(100, 100);

    auto& targetNode = viewportNode.AddNode();
    targetNode.transform.SetLocalPosition(Vector3(30, 0, 0));
    targetNode.AddComponent<RectCollider2D>(Vector2(10, 10));

    // Outer-world click at (30, 0) lands on the target at viewport-local (30, 0).
    auto hits = sut.raycaster->Raycast(Vector2(30, 0), {});
    ASSERT_EQ(1u, hits.size());
    EXPECT_EQ(&targetNode, &hits[0].node);
}

TEST(SimpleRaycaster2D, HitsNodeInScaledViewport) {
    SUT sut;

    auto& viewportNode = sut.world->AddNode();
    auto& viewport = viewportNode.AddComponent<Viewport>();
    viewport.worldSize = Vector2(100, 100);
    // Simulate ContentResizeType::Scale: the viewport-bearing node is stretched 2x, so its
    // displayed content covers [-100, +100] in the outer world while buffer coords stay
    // [-50, +50].
    viewportNode.transform.SetScale(Vector3(2, 2, 1));

    auto& targetNode = viewportNode.AddNode();
    targetNode.transform.SetLocalPosition(Vector3(30, 0, 0));
    targetNode.AddComponent<RectCollider2D>(Vector2(10, 10));

    // The target sits at buffer-local (30, 0), rendered at outer world (60, 0) after scale.
    // A hit test at that outer-world click must land on the target.
    auto hits = sut.raycaster->Raycast(Vector2(60, 0), {});
    ASSERT_EQ(1u, hits.size());
    EXPECT_EQ(&targetNode, &hits[0].node);

    // A click at the pre-scale position should now miss — the displayed content moved.
    auto nearMiss = sut.raycaster->Raycast(Vector2(30, 0), {});
    EXPECT_EQ(0u, nearMiss.size());
}

TEST(SimpleRaycaster2D, ScaledViewportRespectsOwnerPosition) {
    SUT sut;

    auto& viewportNode = sut.world->AddNode();
    auto& viewport = viewportNode.AddComponent<Viewport>();
    viewport.worldSize = Vector2(100, 100);
    viewportNode.transform.SetWorldPosition(Vector3(10, 5, 0));
    viewportNode.transform.SetScale(Vector3(2, 2, 1));

    auto& targetNode = viewportNode.AddNode();
    targetNode.transform.SetLocalPosition(Vector3(30, 0, 0));
    targetNode.AddComponent<RectCollider2D>(Vector2(4, 4));

    // Buffer-local (30, 0) with viewport at outer (10, 5) scaled 2x renders at (10 + 60, 5) = (70, 5).
    auto hits = sut.raycaster->Raycast(Vector2(70, 5), {});
    ASSERT_EQ(1u, hits.size());
    EXPECT_EQ(&targetNode, &hits[0].node);
}

TEST(SimpleRaycaster2D, ClipRejectsClicksOutsideScaledViewport) {
    SUT sut;

    auto& viewportNode = sut.world->AddNode();
    auto& viewport = viewportNode.AddComponent<Viewport>();
    viewport.worldSize = Vector2(100, 100);
    viewportNode.transform.SetScale(Vector3(2, 2, 1));

    auto& targetNode = viewportNode.AddNode();
    targetNode.transform.SetLocalPosition(Vector3(30, 0, 0));
    targetNode.AddComponent<RectCollider2D>(Vector2(1000, 1000));

    // Way outside the scaled viewport's outer bounds [-100, +100]: clip must prune the hit
    // even though the (enormous) collider otherwise covers the point.
    auto hits = sut.raycaster->Raycast(Vector2(500, 0), {});
    EXPECT_EQ(0u, hits.size());
}

TEST(SimpleRaycaster2D, DragFollowsCursorInScaledViewport) {
    // Drag runs in the node's viewport-virtual frame: WorldPosition and SetWorldPositionXY
    // both use ParentWorldModelMatrix, which treats a viewport parent as translate-only.
    // That keeps the transform frame identical for direct children (this test) and deeper
    // descendants, so drag input fed through HostCamera().ScreenToLocal stays consistent.
    SUT sut;

    auto& viewportNode = sut.world->AddNode();
    auto& viewport = viewportNode.AddComponent<Viewport>();
    viewport.worldSize = Vector2(100, 100);
    viewportNode.transform.SetScale(Vector3(2, 2, 1));

    auto& object = viewportNode.AddNode();
    object.transform.SetLocalPosition(Vector3(30, 0, 0));

    // Viewport-virtual (parent treated as translate-only): the stored local pos IS the
    // reported world position.
    EXPECT_EQ(Vector3(30, 0, 0), object.transform.WorldPosition());

    // Feeding a viewport-virtual target back through SetWorldPositionXY updates the local
    // position one-to-one. If drag input arrives via HostCamera().ScreenToLocal, the click's
    // screen position is already converted to this frame.
    object.transform.SetWorldPositionXY(Vector3(35, 0, 0));
    EXPECT_EQ(Vector3(35, 0, 0), object.transform.LocalPosition());
    EXPECT_EQ(Vector3(35, 0, 0), object.transform.WorldPosition());
}

TEST(SimpleRaycaster2D, DragFollowsCursorForGrandchildInScaledViewport) {
    // The original bug: a grandchild of a scaled viewport drifts during drag because its
    // WorldPosition is viewport-virtual but drag input used to come through the main
    // camera (outer world). With ParentWorldModelMatrix the frame is unified; feeding a
    // viewport-virtual target lands the node exactly under the cursor.
    SUT sut;

    auto& viewportNode = sut.world->AddNode();
    auto& viewport = viewportNode.AddComponent<Viewport>();
    viewport.worldSize = Vector2(100, 100);
    viewportNode.transform.SetScale(Vector3(2, 2, 1));

    auto& intermediate = viewportNode.AddNode();

    auto& grandchild = intermediate.AddNode();
    grandchild.transform.SetLocalPosition(Vector3(30, 0, 0));

    EXPECT_EQ(Vector3(30, 0, 0), grandchild.transform.WorldPosition());

    grandchild.transform.SetWorldPositionXY(Vector3(35, 0, 0));
    EXPECT_EQ(Vector3(35, 0, 0), grandchild.transform.LocalPosition());
    EXPECT_EQ(Vector3(35, 0, 0), grandchild.transform.WorldPosition());
}

TEST(SimpleRaycaster2D, HitsNodeOutsideAnyViewport) {
    SUT sut;

    auto& targetNode = sut.world->AddNode();
    targetNode.transform.SetLocalPosition(Vector3(50, -20, 0));
    targetNode.AddComponent<RectCollider2D>(Vector2(10, 10));

    auto hits = sut.raycaster->Raycast(Vector2(50, -20), {});
    ASSERT_EQ(1u, hits.size());
    EXPECT_EQ(&targetNode, &hits[0].node);
}
