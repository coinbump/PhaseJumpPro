#include "gtest/gtest.h"

#include "TiledMeshBuilder.h"
#include "StringUtils.h"

using namespace std;
using namespace PJ;

TEST(TiledMeshBuilder, TestMeshSizeEven) {
    TiledMeshBuilder sut(Vector2(100, 100), Vector2(20, 10));
    auto meshSize = sut.MeshSize();

    EXPECT_EQ(Vec2I(5, 10), meshSize);
}

TEST(TiledMeshBuilder, TestMeshSizeUneven) {
    TiledMeshBuilder sut(Vector2(100, 100), Vector2(22, 12));
    auto meshSize = sut.MeshSize();

    EXPECT_EQ(Vec2I(5, 9), meshSize);
}

TEST(TiledMeshBuilder, TestBuildMeshEven) {
    TiledMeshBuilder builder(Vector2(10, 10), Vector2(5, 5));
    auto sut = builder.BuildMesh();

    EXPECT_EQ(16, sut.vertices.size());

    EXPECT_EQ(Vector2(-5, 5), (Vector2)sut.vertices[0]);
    EXPECT_EQ(Vector2(0, 5), (Vector2)sut.vertices[1]);
    EXPECT_EQ(Vector2(-5, 0), (Vector2)sut.vertices[4]);
    EXPECT_EQ(Vector2(0, 0), (Vector2)sut.vertices[5]);

    EXPECT_EQ(Vector2(0, 5), (Vector2)sut.vertices[2]);
    EXPECT_EQ(Vector2(5, 5), (Vector2)sut.vertices[3]);
    EXPECT_EQ(Vector2(0, 0), (Vector2)sut.vertices[6]);
    EXPECT_EQ(Vector2(5, 0), (Vector2)sut.vertices[7]);

    EXPECT_EQ(Vector2(-5, 0), (Vector2)sut.vertices[8]);
    EXPECT_EQ(Vector2(0, 0), (Vector2)sut.vertices[9]);
    EXPECT_EQ(Vector2(-5, -5), (Vector2)sut.vertices[12]);
    EXPECT_EQ(Vector2(0, -5), (Vector2)sut.vertices[13]);

    EXPECT_EQ(Vector2(0, 0), (Vector2)sut.vertices[10]);
    EXPECT_EQ(Vector2(5, 0), (Vector2)sut.vertices[11]);
    EXPECT_EQ(Vector2(0, -5), (Vector2)sut.vertices[14]);
    EXPECT_EQ(Vector2(5, -5), (Vector2)sut.vertices[15]);

    EXPECT_EQ(16, sut.uvs.size());

    EXPECT_EQ(Vector2(0, 1), sut.uvs[0]);
    EXPECT_EQ(Vector2(1, 1), sut.uvs[1]);
    EXPECT_EQ(Vector2(0, 0), sut.uvs[4]);
    EXPECT_EQ(Vector2(1, 0), sut.uvs[5]);

    EXPECT_EQ(Vector2(0, 1), sut.uvs[2]);
    EXPECT_EQ(Vector2(1, 1), sut.uvs[3]);
    EXPECT_EQ(Vector2(0, 0), sut.uvs[6]);
    EXPECT_EQ(Vector2(1, 0), sut.uvs[7]);

    EXPECT_EQ(Vector2(0, 1), sut.uvs[8]);
    EXPECT_EQ(Vector2(1, 1), sut.uvs[9]);
    EXPECT_EQ(Vector2(0, 0), sut.uvs[12]);
    EXPECT_EQ(Vector2(1, 0), sut.uvs[13]);

    EXPECT_EQ(Vector2(0, 1), sut.uvs[10]);
    EXPECT_EQ(Vector2(1, 1), sut.uvs[11]);
    EXPECT_EQ(Vector2(0, 0), sut.uvs[14]);
    EXPECT_EQ(Vector2(1, 0), sut.uvs[15]);

    EXPECT_EQ(24, sut.triangles.size());
    EXPECT_EQ(0, sut.triangles[0]);
    EXPECT_EQ(1, sut.triangles[1]);
    EXPECT_EQ(4, sut.triangles[2]);
    EXPECT_EQ(1, sut.triangles[3]);
    EXPECT_EQ(5, sut.triangles[4]);
    EXPECT_EQ(4, sut.triangles[5]);

    EXPECT_EQ(2, sut.triangles[6]);
    EXPECT_EQ(3, sut.triangles[7]);
    EXPECT_EQ(6, sut.triangles[8]);
    EXPECT_EQ(3, sut.triangles[9]);
    EXPECT_EQ(7, sut.triangles[10]);
    EXPECT_EQ(6, sut.triangles[11]);

    EXPECT_EQ(8, sut.triangles[12]);
    EXPECT_EQ(9, sut.triangles[13]);
    EXPECT_EQ(12, sut.triangles[14]);
    EXPECT_EQ(9, sut.triangles[15]);
    EXPECT_EQ(13, sut.triangles[16]);
    EXPECT_EQ(12, sut.triangles[17]);

    EXPECT_EQ(10, sut.triangles[18]);
    EXPECT_EQ(11, sut.triangles[19]);
    EXPECT_EQ(14, sut.triangles[20]);
    EXPECT_EQ(11, sut.triangles[21]);
    EXPECT_EQ(15, sut.triangles[22]);
    EXPECT_EQ(14, sut.triangles[23]);
}

TEST(TiledMeshBuilder, TestUVBoundsInternal) {
    TiledMeshBuilder builder(Vector2(10, 10), Vector2(5, 5), UVRect(Vector2(.1, .1), Vector2(.5, .5)));
    auto sut = builder.BuildMesh();

    EXPECT_EQ(16, sut.uvs.size());

    EXPECT_EQ(Vector2(.1, .6), sut.uvs[0]);
    EXPECT_EQ(Vector2(.6, .6), sut.uvs[1]);
    EXPECT_EQ(Vector2(.1, .1), sut.uvs[4]);
    EXPECT_EQ(Vector2(.6, .1), sut.uvs[5]);

    EXPECT_EQ(Vector2(.1, .6), sut.uvs[2]);
    EXPECT_EQ(Vector2(.6, .6), sut.uvs[3]);
    EXPECT_EQ(Vector2(.1, .1), sut.uvs[6]);
    EXPECT_EQ(Vector2(.6, .1), sut.uvs[7]);

    EXPECT_EQ(Vector2(.1, .6), sut.uvs[8]);
    EXPECT_EQ(Vector2(.6, .6), sut.uvs[9]);
    EXPECT_EQ(Vector2(.1, .1), sut.uvs[12]);
    EXPECT_EQ(Vector2(.6, .1), sut.uvs[13]);

    EXPECT_EQ(Vector2(.1, .6), sut.uvs[10]);
    EXPECT_EQ(Vector2(.6, .6), sut.uvs[11]);
    EXPECT_EQ(Vector2(.1, .1), sut.uvs[14]);
    EXPECT_EQ(Vector2(.6, .1), sut.uvs[15]);
}

TEST(TiledMeshBuilder, TestBuildMeshUneven) {
    TiledMeshBuilder builder(Vector2(10, 10), Vector2(8, 8));
    auto sut = builder.BuildMesh();

    EXPECT_EQ(16, sut.vertices.size());

    EXPECT_EQ(Vector2(-5, 5), (Vector2)sut.vertices[0]);
    EXPECT_EQ(Vector2(3, 5), (Vector2)sut.vertices[1]);
    EXPECT_EQ(Vector2(-5, -3), (Vector2)sut.vertices[4]);
    EXPECT_EQ(Vector2(3, -3), (Vector2)sut.vertices[5]);

    EXPECT_EQ(Vector2(3, 5), (Vector2)sut.vertices[2]);
    EXPECT_EQ(Vector2(5, 5), (Vector2)sut.vertices[3]);
    EXPECT_EQ(Vector2(3, -3), (Vector2)sut.vertices[6]);
    EXPECT_EQ(Vector2(5, -3), (Vector2)sut.vertices[7]);

    EXPECT_EQ(Vector2(-5, -3), (Vector2)sut.vertices[8]);
    EXPECT_EQ(Vector2(3, -3), (Vector2)sut.vertices[9]);
    EXPECT_EQ(Vector2(-5, -5), (Vector2)sut.vertices[12]);
    EXPECT_EQ(Vector2(3, -5), (Vector2)sut.vertices[13]);

    EXPECT_EQ(Vector2(3, -3), (Vector2)sut.vertices[10]);
    EXPECT_EQ(Vector2(5, -3), (Vector2)sut.vertices[11]);
    EXPECT_EQ(Vector2(3, -5), (Vector2)sut.vertices[14]);
    EXPECT_EQ(Vector2(5, -5), (Vector2)sut.vertices[15]);

    EXPECT_EQ(16, sut.uvs.size());

    EXPECT_EQ(Vector2(0, 1), sut.uvs[0]);
    EXPECT_EQ(Vector2(1, 1), sut.uvs[1]);
    EXPECT_EQ(Vector2(0, 0), sut.uvs[4]);
    EXPECT_EQ(Vector2(1, 0), sut.uvs[5]);

    EXPECT_EQ(Vector2(0, 1), sut.uvs[2]);
    EXPECT_EQ(Vector2(0.25, 1), sut.uvs[3]);
    EXPECT_EQ(Vector2(0, 0), sut.uvs[6]);
    EXPECT_EQ(Vector2(0.25, 0), sut.uvs[7]);

    EXPECT_EQ(Vector2(0, 1), sut.uvs[8]);
    EXPECT_EQ(Vector2(1, 1), sut.uvs[9]);
    EXPECT_EQ(Vector2(0, 0.75), sut.uvs[12]);
    EXPECT_EQ(Vector2(1, 0.75), sut.uvs[13]);

    EXPECT_EQ(Vector2(0, 1), sut.uvs[10]);
    EXPECT_EQ(Vector2(0.25, 1), sut.uvs[11]);
    EXPECT_EQ(Vector2(0, 0.75), sut.uvs[14]);
    EXPECT_EQ(Vector2(0.25, 0.75), sut.uvs[15]);
}
