#include "gtest/gtest.h"

#include "RenderMesh.h"

using namespace std;
using namespace PJ;

TEST(RenderMesh, TestCalculateBounds) {
    RenderMesh sut;

    VectorList<Vector3> vertices;
    vertices.Add(Vector3(-1, -2, -3));
    vertices.Add(Vector3(1, 2, 3));

    sut.vertices = vertices;

    sut.CalculateBounds();

    EXPECT_EQ(Vector3(0, 0, 0), sut.bounds.center);
    EXPECT_EQ(Vector3(1, 2, 3), sut.bounds.extents);
    EXPECT_EQ(Vector3(-1, -2, -3), sut.bounds.Min());
    EXPECT_EQ(Vector3(1, 2, 3), sut.bounds.Max());
    EXPECT_EQ(Vector3(2, 4, 6), sut.bounds.Size());
}
