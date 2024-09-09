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

    EXPECT_EQ(16, sut.Vertices().size());

    EXPECT_EQ(Vector2(-5, 5), (Vector2)sut.Vertices()[0]);
    EXPECT_EQ(Vector2(0, 5), (Vector2)sut.Vertices()[1]);
    EXPECT_EQ(Vector2(-5, 0), (Vector2)sut.Vertices()[4]);
    EXPECT_EQ(Vector2(0, 0), (Vector2)sut.Vertices()[5]);

    EXPECT_EQ(Vector2(0, 5), (Vector2)sut.Vertices()[2]);
    EXPECT_EQ(Vector2(5, 5), (Vector2)sut.Vertices()[3]);
    EXPECT_EQ(Vector2(0, 0), (Vector2)sut.Vertices()[6]);
    EXPECT_EQ(Vector2(5, 0), (Vector2)sut.Vertices()[7]);

    EXPECT_EQ(Vector2(-5, 0), (Vector2)sut.Vertices()[8]);
    EXPECT_EQ(Vector2(0, 0), (Vector2)sut.Vertices()[9]);
    EXPECT_EQ(Vector2(-5, -5), (Vector2)sut.Vertices()[12]);
    EXPECT_EQ(Vector2(0, -5), (Vector2)sut.Vertices()[13]);

    EXPECT_EQ(Vector2(0, 0), (Vector2)sut.Vertices()[10]);
    EXPECT_EQ(Vector2(5, 0), (Vector2)sut.Vertices()[11]);
    EXPECT_EQ(Vector2(0, -5), (Vector2)sut.Vertices()[14]);
    EXPECT_EQ(Vector2(5, -5), (Vector2)sut.Vertices()[15]);

    auto sutUVs = sut.UVs();

    EXPECT_EQ(16, sutUVs.size());

    EXPECT_EQ(Vector2(0, 1), sutUVs[0]);
    EXPECT_EQ(Vector2(1, 1), sutUVs[1]);
    EXPECT_EQ(Vector2(0, 0), sutUVs[4]);
    EXPECT_EQ(Vector2(1, 0), sutUVs[5]);

    EXPECT_EQ(Vector2(0, 1), sutUVs[2]);
    EXPECT_EQ(Vector2(1, 1), sutUVs[3]);
    EXPECT_EQ(Vector2(0, 0), sutUVs[6]);
    EXPECT_EQ(Vector2(1, 0), sutUVs[7]);

    EXPECT_EQ(Vector2(0, 1), sutUVs[8]);
    EXPECT_EQ(Vector2(1, 1), sutUVs[9]);
    EXPECT_EQ(Vector2(0, 0), sutUVs[12]);
    EXPECT_EQ(Vector2(1, 0), sutUVs[13]);

    EXPECT_EQ(Vector2(0, 1), sutUVs[10]);
    EXPECT_EQ(Vector2(1, 1), sutUVs[11]);
    EXPECT_EQ(Vector2(0, 0), sutUVs[14]);
    EXPECT_EQ(Vector2(1, 0), sutUVs[15]);

    auto sutTris = sut.Triangles();

    EXPECT_EQ(24, sutTris.size());
    EXPECT_EQ(0, sutTris[0]);
    EXPECT_EQ(1, sutTris[1]);
    EXPECT_EQ(4, sutTris[2]);
    EXPECT_EQ(1, sutTris[3]);
    EXPECT_EQ(5, sutTris[4]);
    EXPECT_EQ(4, sutTris[5]);

    EXPECT_EQ(2, sutTris[6]);
    EXPECT_EQ(3, sutTris[7]);
    EXPECT_EQ(6, sutTris[8]);
    EXPECT_EQ(3, sutTris[9]);
    EXPECT_EQ(7, sutTris[10]);
    EXPECT_EQ(6, sutTris[11]);

    EXPECT_EQ(8, sutTris[12]);
    EXPECT_EQ(9, sutTris[13]);
    EXPECT_EQ(12, sutTris[14]);
    EXPECT_EQ(9, sutTris[15]);
    EXPECT_EQ(13, sutTris[16]);
    EXPECT_EQ(12, sutTris[17]);

    EXPECT_EQ(10, sutTris[18]);
    EXPECT_EQ(11, sutTris[19]);
    EXPECT_EQ(14, sutTris[20]);
    EXPECT_EQ(11, sutTris[21]);
    EXPECT_EQ(15, sutTris[22]);
    EXPECT_EQ(14, sutTris[23]);
}

TEST(TiledMeshBuilder, TestUVBoundsInternal) {
    TiledMeshBuilder builder(Vector2(10, 10), Vector2(5, 5), UVRect(Vector2(.1, .1), Vector2(.5, .5)));
    auto sut = builder.BuildMesh();
    auto sutUVs = sut.UVs();

    EXPECT_EQ(16, sutUVs.size());

    EXPECT_EQ(Vector2(.1, .6), sutUVs[0]);
    EXPECT_EQ(Vector2(.6, .6), sutUVs[1]);
    EXPECT_EQ(Vector2(.1, .1), sutUVs[4]);
    EXPECT_EQ(Vector2(.6, .1), sutUVs[5]);

    EXPECT_EQ(Vector2(.1, .6), sutUVs[2]);
    EXPECT_EQ(Vector2(.6, .6), sutUVs[3]);
    EXPECT_EQ(Vector2(.1, .1), sutUVs[6]);
    EXPECT_EQ(Vector2(.6, .1), sutUVs[7]);

    EXPECT_EQ(Vector2(.1, .6), sutUVs[8]);
    EXPECT_EQ(Vector2(.6, .6), sutUVs[9]);
    EXPECT_EQ(Vector2(.1, .1), sutUVs[12]);
    EXPECT_EQ(Vector2(.6, .1), sutUVs[13]);

    EXPECT_EQ(Vector2(.1, .6), sutUVs[10]);
    EXPECT_EQ(Vector2(.6, .6), sutUVs[11]);
    EXPECT_EQ(Vector2(.1, .1), sutUVs[14]);
    EXPECT_EQ(Vector2(.6, .1), sutUVs[15]);
}

TEST(TiledMeshBuilder, TestBuildMeshUneven) {
    TiledMeshBuilder builder(Vector2(10, 10), Vector2(8, 8));
    auto sut = builder.BuildMesh();

    EXPECT_EQ(16, sut.Vertices().size());

    EXPECT_EQ(Vector2(-5, 5), (Vector2)sut.Vertices()[0]);
    EXPECT_EQ(Vector2(3, 5), (Vector2)sut.Vertices()[1]);
    EXPECT_EQ(Vector2(-5, -3), (Vector2)sut.Vertices()[4]);
    EXPECT_EQ(Vector2(3, -3), (Vector2)sut.Vertices()[5]);

    EXPECT_EQ(Vector2(3, 5), (Vector2)sut.Vertices()[2]);
    EXPECT_EQ(Vector2(5, 5), (Vector2)sut.Vertices()[3]);
    EXPECT_EQ(Vector2(3, -3), (Vector2)sut.Vertices()[6]);
    EXPECT_EQ(Vector2(5, -3), (Vector2)sut.Vertices()[7]);

    EXPECT_EQ(Vector2(-5, -3), (Vector2)sut.Vertices()[8]);
    EXPECT_EQ(Vector2(3, -3), (Vector2)sut.Vertices()[9]);
    EXPECT_EQ(Vector2(-5, -5), (Vector2)sut.Vertices()[12]);
    EXPECT_EQ(Vector2(3, -5), (Vector2)sut.Vertices()[13]);

    EXPECT_EQ(Vector2(3, -3), (Vector2)sut.Vertices()[10]);
    EXPECT_EQ(Vector2(5, -3), (Vector2)sut.Vertices()[11]);
    EXPECT_EQ(Vector2(3, -5), (Vector2)sut.Vertices()[14]);
    EXPECT_EQ(Vector2(5, -5), (Vector2)sut.Vertices()[15]);

    auto sutUVs = sut.UVs();
    
    EXPECT_EQ(16, sutUVs.size());

    EXPECT_EQ(Vector2(0, 1), sutUVs[0]);
    EXPECT_EQ(Vector2(1, 1), sutUVs[1]);
    EXPECT_EQ(Vector2(0, 0), sutUVs[4]);
    EXPECT_EQ(Vector2(1, 0), sutUVs[5]);

    EXPECT_EQ(Vector2(0, 1), sutUVs[2]);
    EXPECT_EQ(Vector2(0.25, 1), sutUVs[3]);
    EXPECT_EQ(Vector2(0, 0), sutUVs[6]);
    EXPECT_EQ(Vector2(0.25, 0), sutUVs[7]);

    EXPECT_EQ(Vector2(0, 1), sutUVs[8]);
    EXPECT_EQ(Vector2(1, 1), sutUVs[9]);
    EXPECT_EQ(Vector2(0, 0.75), sutUVs[12]);
    EXPECT_EQ(Vector2(1, 0.75), sutUVs[13]);

    EXPECT_EQ(Vector2(0, 1), sutUVs[10]);
    EXPECT_EQ(Vector2(0.25, 1), sutUVs[11]);
    EXPECT_EQ(Vector2(0, 0.75), sutUVs[14]);
    EXPECT_EQ(Vector2(0.25, 0.75), sutUVs[15]);
}
