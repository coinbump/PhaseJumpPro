#include "gtest/gtest.h"
#include "CenterPolyMeshBuilder.h"
#include "FileProcessor.Utility.h"

using namespace PJ;
using namespace std;

namespace CenterPolyMeshBuilderTests {
}

using namespace CenterPolyMeshBuilderTests;

TEST(CenterPolyMeshBuilder, BuildClosed) {
    CenterPolyMeshBuilder sut{{10, 10}, CenterPolyModel::quad};
    auto mesh = sut.BuildMesh();
    
    ASSERT_EQ(5, mesh.Vertices().size());
    ASSERT_EQ(12, mesh.Triangles().size());
    
    EXPECT_EQ(Vector3(0, 0, 0), mesh.Vertices()[0]);
    EXPECT_EQ(Vector3(0, 5, 0), mesh.Vertices()[1]);
    EXPECT_EQ(Vector3(5, 0, 0), mesh.Vertices()[2]);
    EXPECT_EQ(Vector3(0, -5, 0), mesh.Vertices()[3]);
    EXPECT_EQ(Vector3(-5, 0, 0), mesh.Vertices()[4]);
    
    // First segment
    
    EXPECT_EQ(0, mesh.Triangles()[0]);
    EXPECT_EQ(1, mesh.Triangles()[1]);
    EXPECT_EQ(2, mesh.Triangles()[2]);
    
    EXPECT_EQ(0, mesh.Triangles()[3]);
    EXPECT_EQ(2, mesh.Triangles()[4]);
    EXPECT_EQ(3, mesh.Triangles()[5]);
    
    // Final segment
    
    EXPECT_EQ(0, mesh.Triangles()[9]);
    EXPECT_EQ(4, mesh.Triangles()[10]);
    EXPECT_EQ(1, mesh.Triangles()[11]);
}

TEST(CenterPolyMeshBuilder, BuildOpen) {
    auto model = CenterPolyModel::quad;
    model.angleDelta = Angle::WithDegrees(180);
    CenterPolyMeshBuilder sut{{10, 10}, model};
    auto mesh = sut.BuildMesh();
    
    ASSERT_EQ(4, mesh.Vertices().size());
    ASSERT_EQ(6, mesh.Triangles().size());
    
    EXPECT_EQ(Vector3(0, 0, 0), mesh.Vertices()[0]);
    EXPECT_EQ(Vector3(0, 5, 0), mesh.Vertices()[1]);
    EXPECT_EQ(Vector3(5, 0, 0), mesh.Vertices()[2]);
    EXPECT_EQ(Vector3(0, -5, 0), mesh.Vertices()[3]);
    
    // First segment
    
    EXPECT_EQ(0, mesh.Triangles()[0]);
    EXPECT_EQ(1, mesh.Triangles()[1]);
    EXPECT_EQ(2, mesh.Triangles()[2]);
    
    // Final segment
    
    EXPECT_EQ(0, mesh.Triangles()[3]);
    EXPECT_EQ(2, mesh.Triangles()[4]);
    EXPECT_EQ(3, mesh.Triangles()[5]);
}
