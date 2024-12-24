#include "Mesh.h"
#include "gtest/gtest.h"

using namespace std;
using namespace PJ;

TEST(Mesh, TestCalculateBounds) {
    Mesh sut;

    VectorList<Vector3> vertices;
    Add(vertices, Vector3(-1, -2, -3));
    Add(vertices, Vector3(1, 2, 3));

    sut.SetVertices(vertices);

    auto sutBounds = sut.GetBounds();
    EXPECT_EQ(Vector3(0, 0, 0), sutBounds.center);
    EXPECT_EQ(Vector3(1, 2, 3), sutBounds.extents);
    EXPECT_EQ(Vector3(-1, -2, -3), sutBounds.Min());
    EXPECT_EQ(Vector3(1, 2, 3), sutBounds.Max());
    EXPECT_EQ(Vector3(2, 4, 6), sutBounds.Size());
}

TEST(Mesh, TestOffset) {
    Mesh sut;

    VectorList<Vector3> vertices;
    Add(vertices, Vector3(-1, -2, -3));
    Add(vertices, Vector3(1, 2, 3));

    sut.SetVertices(vertices);

    sut.Offset(Vector2(1, 1));

    EXPECT_EQ(Vector3(0, -1, -3), sut.Vertices()[0]);
    EXPECT_EQ(Vector3(2, 3, 3), sut.Vertices()[1]);

    sut.Offset(Vector3(10, 10, 10));
    EXPECT_EQ(Vector3(12, 13, 13), sut.Vertices()[1]);
}

TEST(Mesh, TestPlusOperator) {
    Mesh sut1;
    Mesh sut2;

    VectorList<Vector3> vertices1;
    Add(vertices1, Vector3(1, 2, 3));
    Add(vertices1, Vector3(4, 5, 6));

    VectorList<Vector3> vertices2;
    Add(vertices2, Vector3(7, 8, 9));
    Add(vertices2, Vector3(10, 11, 12));

    VectorList<uint32_t> triangles;
    Add(triangles, 0);
    Add(triangles, 1);
    Add(triangles, 2);

    VectorList<Vector2> uvs{2, Vector2()};
    
    sut1.Update(vertices1, triangles, uvs);
    sut2.Update(vertices2, triangles, uvs);

    auto sut3 = sut1 + sut2;

    EXPECT_EQ(4, sut3.Vertices().size());
    EXPECT_EQ(4, sut3.UVs().size());
    EXPECT_EQ(Vector3(1, 2, 3), sut3.Vertices()[0]);
    EXPECT_EQ(Vector3(4, 5, 6), sut3.Vertices()[1]);
    EXPECT_EQ(Vector3(7, 8, 9), sut3.Vertices()[2]);
    EXPECT_EQ(Vector3(10, 11, 12), sut3.Vertices()[3]);

    EXPECT_EQ(6, sut3.Triangles().size());
    EXPECT_EQ(0, sut3.Triangles()[0]);
    EXPECT_EQ(1, sut3.Triangles()[1]);
    EXPECT_EQ(2, sut3.Triangles()[2]);
    EXPECT_EQ(2, sut3.Triangles()[3]);
    EXPECT_EQ(3, sut3.Triangles()[4]);
    EXPECT_EQ(4, sut3.Triangles()[5]);
}
