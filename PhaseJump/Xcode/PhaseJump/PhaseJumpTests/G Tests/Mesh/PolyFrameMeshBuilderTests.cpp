#include "gtest/gtest.h"
#include "PolyFrameMeshBuilder.h"
#include "FileProcessor.Utility.h"

using namespace PJ;
using namespace std;

namespace PolyFrameMeshBuilderTests {
}

using namespace PolyFrameMeshBuilderTests;

TEST(PolyFrameMeshBuilder, BuildClosed) {
    Polygon poly;
    poly.Add({0, 4.5f});
    poly.Add({4.5f, 0});
    poly.Add({0, -4.5f});
    poly.Add({-4.5f, 0});
    
    PolyFrameMeshBuilder sut{{.poly = poly, .strokeWidth = 1, .polyClose = PolyClose::Closed}};
    auto mesh = sut.BuildMesh();
    
    ASSERT_EQ(16, mesh.Vertices().size());
    ASSERT_EQ(24, mesh.Triangles().size());
    
    EXPECT_NEAR(mesh.Vertices()[0].x, 0.3535f, .001f);
    EXPECT_NEAR(mesh.Vertices()[0].y, 4.85349989f, .001f);
    EXPECT_NEAR(mesh.Vertices()[2].x, -0.3535f, .001f);
    EXPECT_NEAR(mesh.Vertices()[2].y, 4.14650011f, .001f);
    
    // First segment
    
    EXPECT_EQ(0, mesh.Triangles()[0]);
    EXPECT_EQ(1, mesh.Triangles()[1]);
    EXPECT_EQ(2, mesh.Triangles()[2]);
    
    EXPECT_EQ(1, mesh.Triangles()[3]);
    EXPECT_EQ(3, mesh.Triangles()[4]);
    EXPECT_EQ(2, mesh.Triangles()[5]);

    // Final segment
    
    EXPECT_EQ(12, mesh.Triangles()[18]);
    EXPECT_EQ(13, mesh.Triangles()[19]);
    EXPECT_EQ(14, mesh.Triangles()[20]);
    
    EXPECT_EQ(13, mesh.Triangles()[21]);
    EXPECT_EQ(15, mesh.Triangles()[22]);
    EXPECT_EQ(14, mesh.Triangles()[23]);
}

TEST(PolyFrameMeshBuilder, BuildOpen) {
    Polygon poly;
    poly.Add({0, 4.5f});
    poly.Add({4.5f, 0});
    poly.Add({0, -4.5f});
    poly.Add({-4.5f, 0});
    
    PolyFrameMeshBuilder sut{{.poly = poly, .strokeWidth = 1, .polyClose = PolyClose::Open}};
    auto mesh = sut.BuildMesh();
    
    ASSERT_EQ(12, mesh.Vertices().size());
    ASSERT_EQ(18, mesh.Triangles().size());
    
    EXPECT_NEAR(mesh.Vertices()[0].x, 0.3535f, .001f);
    EXPECT_NEAR(mesh.Vertices()[0].y, 4.85349989f, .001f);
    EXPECT_NEAR(mesh.Vertices()[2].x, -0.3535f, .001f);
    EXPECT_NEAR(mesh.Vertices()[2].y, 4.14650011f, .001f);
    
    // First segment
    
    EXPECT_EQ(0, mesh.Triangles()[0]);
    EXPECT_EQ(1, mesh.Triangles()[1]);
    EXPECT_EQ(2, mesh.Triangles()[2]);
    
    EXPECT_EQ(1, mesh.Triangles()[3]);
    EXPECT_EQ(3, mesh.Triangles()[4]);
    EXPECT_EQ(2, mesh.Triangles()[5]);
    
    // Final segment
    
    EXPECT_EQ(8, mesh.Triangles()[12]);
    EXPECT_EQ(9, mesh.Triangles()[13]);
    EXPECT_EQ(10, mesh.Triangles()[14]);
    
    EXPECT_EQ(9, mesh.Triangles()[15]);
    EXPECT_EQ(11, mesh.Triangles()[16]);
    EXPECT_EQ(10, mesh.Triangles()[17]);
}
