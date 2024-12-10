#include "gtest/gtest.h"

#include "Slice3TextureRenderer.h"
#include "SomeTexture.h"

using namespace std;
using namespace PJ;

namespace Slice3TextureRendererTests {
    class MockTextureX : public SomeTexture {
    public:
        MockTextureX() : SomeTexture({.renderId = 1, .size = Vector2Int(50, 10)}) {
        }
    };
    
    class MockTextureY : public SomeTexture {
    public:
        MockTextureY() : SomeTexture({.renderId = 1, .size = Vector2Int(10, 50)}) {
        }
    };
}

using namespace Slice3TextureRendererTests;

TEST(Slice3TextureRenderer, TestBuildModelXAxis) {
    auto texture = MAKE<MockTextureX>();
    Slice3TextureRenderer renderer({.axis = Axis2D::X, . texture = texture, .axisLength = 100, .startInset = 10, .endInset = 10});
    auto sut = renderer.MakeBuildModel({100, 10, 0});
    
    EXPECT_EQ(Vector2(10, 10), sut.start.worldSize);
    EXPECT_EQ(Vector2(10, 10), sut.start.tileSize);
    EXPECT_EQ(Vector2(0, 0), sut.start.uvRect.origin);
    EXPECT_EQ(Vector2(10.0f/50.0f, 1), sut.start.uvRect.size);
    
    EXPECT_EQ(Vector2(80, 10), sut.center.worldSize);
    EXPECT_EQ(Vector2(30, 10), sut.center.tileSize);
    EXPECT_EQ(Vector2(10.0f/50.0f, 0), sut.center.uvRect.origin);
    EXPECT_EQ(Vector2(30.0f/50.0f, 1), sut.center.uvRect.size);
    
    EXPECT_EQ(Vector2(10, 10), sut.end.worldSize);
    EXPECT_EQ(Vector2(10, 10), sut.end.tileSize);
    EXPECT_EQ(Vector2(40.0f/50.0f, 0), sut.end.uvRect.origin);
    EXPECT_EQ(Vector2(10.0f/50.0f, 1), sut.end.uvRect.size);
}

TEST(Slice3TextureRenderer, TestBuildModelYAxis) {
    auto texture = MAKE<MockTextureY>();
    Slice3TextureRenderer renderer({.axis = Axis2D::Y, . texture = texture, .axisLength = 100, .startInset = 10, .endInset = 10});
    auto sut = renderer.MakeBuildModel({10, 100, 0});
    
    EXPECT_EQ(Vector2(10, 10), sut.start.worldSize);
    EXPECT_EQ(Vector2(10, 10), sut.start.tileSize);
    EXPECT_EQ(Vector2(0, 40.0f/50.0f), sut.start.uvRect.origin);
    EXPECT_EQ(Vector2(1, 10.0f/50.0f), sut.start.uvRect.size);
    
    EXPECT_EQ(Vector2(10, 80), sut.center.worldSize);
    EXPECT_EQ(Vector2(10, 30), sut.center.tileSize);
    EXPECT_EQ(Vector2(0, 10.0f/50.0f), sut.center.uvRect.origin);
    EXPECT_EQ(Vector2(1, 30.0f/50.0f), sut.center.uvRect.size);
    
    EXPECT_EQ(Vector2(10, 10), sut.end.worldSize);
    EXPECT_EQ(Vector2(10, 10), sut.end.tileSize);
    EXPECT_EQ(Vector2(0, 0), sut.end.uvRect.origin);
    EXPECT_EQ(Vector2(1, 10.0f/50.0f), sut.end.uvRect.size);
}
