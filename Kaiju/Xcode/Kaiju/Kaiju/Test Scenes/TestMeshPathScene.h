#ifndef TestMeshPathScene_h
#define TestMeshPathScene_h

#if DEVELOPMENT
#include <PhaseJump-Dev/PhaseJump-Dev.h>
#else
#include <PhaseJump/PhaseJump.h>
#endif

using namespace PJ;

class TestMeshPathScene : public Scene {
public:
    TestMeshPathScene() {}

    void LoadInto(WorldNode& root) {
        root.name = "TestMeshPathScene";

        QB(root).And("Camera").With<OrthoCamera>().AndPrefab("archer");

        foo1(root);
        foo2(root);
    }

    void foo1(WorldNode& root) {
        QB qb(root.And("Circle Path"));
        qb.With<CirclePathLayout2D>(500);

        int count = 20;
        Repeat(count, [&]() { qb.And("Mesh").With<MeshRenderer>(Vector3(40, 20, 0)).Pop(); });

        qb.ModifyAll<MeshRenderer>([=](MeshRenderer& c, auto index) {
              float progress = (float)index / (float)count;

              c.SetColor(Color(progress, 1.0f - progress, 0, 1));
              c.owner->SetScale(Vector3(1.0f + progress, 1.0f + progress, 1));

              c.model.SetBuildMeshFunc([](RendererModel const& model) {
                  EllipseMeshBuilder builder(model.WorldSize(), Angle::DegreesAngle(5.0f));
                  return builder.BuildMesh();
              });
          }).ModifyAll<CirclePathLayout2D>([](CirclePathLayout2D& c, auto index) {
            c.orientToPath = true;
            c.ApplyLayout();
        });
    }

    void foo2(WorldNode& root) {
        QB qb(root.And("Circle Path"));
        qb.With<CirclePathLayout2D>(500);

        int count = 20;
        Repeat(count, [&]() { qb.And("Mesh").With<MeshRenderer>(Vector3(40, 20, 0)).Pop(); });

        qb.ModifyAll<MeshRenderer>([=](MeshRenderer& c, auto index) {
              c.model.SetBuildMeshFunc([](RendererModel const& model) {
                  EllipseMeshBuilder builder(model.WorldSize(), Angle::DegreesAngle(5.0f));
                  return builder.BuildMesh();
              });

              c.SetColor(Color(1.0f, 0, (float)count / (float)index, 1));
          }).ModifyAll<CirclePathLayout2D>([](CirclePathLayout2D& c, auto index) {
            c.orientToPath = false;
            c.ApplyLayout();
        });
    }
};

#endif
