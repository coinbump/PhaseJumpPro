#include "gtest/gtest.h"

#include "Mesh.h"

using namespace std;
using namespace PJ;

TEST(Mesh, TestCalculateBounds) {
    Mesh sut;

    VectorList<Vector3> vertices;
    Add(vertices, Vector3(-1, -2, -3));
    Add(vertices, Vector3(1, 2, 3));

    sut.vertices = vertices;

    sut.CalculateBounds();

    EXPECT_EQ(Vector3(0, 0, 0), sut.bounds.center);
    EXPECT_EQ(Vector3(1, 2, 3), sut.bounds.extents);
    EXPECT_EQ(Vector3(-1, -2, -3), sut.bounds.Min());
    EXPECT_EQ(Vector3(1, 2, 3), sut.bounds.Max());
    EXPECT_EQ(Vector3(2, 4, 6), sut.bounds.Size());
}

TEST(Mesh, TestOffsetBy) {
    Mesh sut;

    VectorList<Vector3> vertices;
    Add(vertices, Vector3(-1, -2, -3));
    Add(vertices, Vector3(1, 2, 3));

    sut.vertices = vertices;

    sut.OffsetBy(Vector2(1, 1));

    EXPECT_EQ(Vector3(0, -1, -3), sut.vertices[0]);
    EXPECT_EQ(Vector3(2, 3, 3), sut.vertices[1]);

    sut.OffsetBy(Vector3(10, 10, 10));
    EXPECT_EQ(Vector3(12, 13, 13), sut.vertices[1]);
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

    sut1.vertices = vertices1;
    sut1.triangles = triangles;
    sut2.vertices = vertices2;
    sut2.triangles = triangles;

    auto sut3 = sut1 + sut2;

    EXPECT_EQ(4, sut3.vertices.size());
    EXPECT_EQ(Vector3(1, 2, 3), sut3.vertices[0]);
    EXPECT_EQ(Vector3(4, 5, 6), sut3.vertices[1]);
    EXPECT_EQ(Vector3(7, 8, 9), sut3.vertices[2]);
    EXPECT_EQ(Vector3(10, 11, 12), sut3.vertices[3]);

    EXPECT_EQ(6, sut3.triangles.size());
    EXPECT_EQ(0, sut3.triangles[0]);
    EXPECT_EQ(1, sut3.triangles[1]);
    EXPECT_EQ(2, sut3.triangles[2]);
    EXPECT_EQ(2, sut3.triangles[3]);
    EXPECT_EQ(3, sut3.triangles[4]);
    EXPECT_EQ(4, sut3.triangles[5]);
}
