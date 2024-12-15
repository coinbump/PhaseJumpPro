#include "gtest/gtest.h"
#include "CenterPolyFrameMeshBuilder.h"
#include "FileProcessor.Utility.h"

using namespace PJ;
using namespace std;

namespace CenterPolyFrameMeshBuilderTests {
}

using namespace CenterPolyFrameMeshBuilderTests;

TEST(CenterPolyFrameMeshBuilder, BuildClosed) {
    CenterPolyFrameMeshBuilder sut{{.worldSize = {10, 10}, .strokeWidth = 1, .model = CenterPolyModel::quad}};
    auto mesh = sut.BuildMesh();
    
    ASSERT_EQ(8, mesh.Vertices().size());
    ASSERT_EQ(24, mesh.Triangles().size());
    
    EXPECT_EQ(Vector3(0, 5, 0), mesh.Vertices()[0]);
    EXPECT_EQ(Vector3(5, 0, 0), mesh.Vertices()[1]);
    EXPECT_EQ(Vector3(0, -5, 0), mesh.Vertices()[2]);
    EXPECT_EQ(Vector3(-5, 0, 0), mesh.Vertices()[3]);
    
    EXPECT_EQ(Vector3(0, 4, 0), mesh.Vertices()[4]);
    EXPECT_EQ(Vector3(4, 0, 0), mesh.Vertices()[5]);
    EXPECT_EQ(Vector3(0, -4, 0), mesh.Vertices()[6]);
    EXPECT_EQ(Vector3(-4, 0, 0), mesh.Vertices()[7]);
    
    // First segment
    
    EXPECT_EQ(0, mesh.Triangles()[0]);
    EXPECT_EQ(1, mesh.Triangles()[1]);
    EXPECT_EQ(4, mesh.Triangles()[2]);
    
    EXPECT_EQ(1, mesh.Triangles()[3]);
    EXPECT_EQ(5, mesh.Triangles()[4]);
    EXPECT_EQ(4, mesh.Triangles()[5]);
    
    // Final segment
    
    EXPECT_EQ(3, mesh.Triangles()[18]);
    EXPECT_EQ(0, mesh.Triangles()[19]);
    EXPECT_EQ(7, mesh.Triangles()[20]);
    
    EXPECT_EQ(0, mesh.Triangles()[21]);
    EXPECT_EQ(4, mesh.Triangles()[22]);
    EXPECT_EQ(7, mesh.Triangles()[23]);
}

TEST(CenterPolyFrameMeshBuilder, BuildOpen) {
    auto model = CenterPolyModel::quad;
    model.angleDelta = Angle::WithDegrees(180);
    CenterPolyFrameMeshBuilder sut{{.worldSize = {10, 10}, .strokeWidth = 1, .model = model}};
    auto mesh = sut.BuildMesh();
    
    ASSERT_EQ(6, mesh.Vertices().size());
    ASSERT_EQ(12, mesh.Triangles().size());
    
    EXPECT_EQ(Vector3(0, 5, 0), mesh.Vertices()[0]);
    EXPECT_EQ(Vector3(5, 0, 0), mesh.Vertices()[1]);
    EXPECT_EQ(Vector3(0, -5, 0), mesh.Vertices()[2]);
    
    EXPECT_EQ(Vector3(0, 4, 0), mesh.Vertices()[3]);
    EXPECT_EQ(Vector3(4, 0, 0), mesh.Vertices()[4]);
    EXPECT_EQ(Vector3(0, -4, 0), mesh.Vertices()[5]);
    
    // First segment
    
    EXPECT_EQ(0, mesh.Triangles()[0]);
    EXPECT_EQ(1, mesh.Triangles()[1]);
    EXPECT_EQ(3, mesh.Triangles()[2]);
    
    EXPECT_EQ(1, mesh.Triangles()[3]);
    EXPECT_EQ(4, mesh.Triangles()[4]);
    EXPECT_EQ(3, mesh.Triangles()[5]);
    
    // Final segment
    
    EXPECT_EQ(1, mesh.Triangles()[6]);
    EXPECT_EQ(2, mesh.Triangles()[7]);
    EXPECT_EQ(4, mesh.Triangles()[8]);
    
    EXPECT_EQ(2, mesh.Triangles()[9]);
    EXPECT_EQ(5, mesh.Triangles()[10]);
    EXPECT_EQ(4, mesh.Triangles()[11]);
}
