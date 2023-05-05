#include "gtest/gtest.h"

#include "SlicedTextureRenderer.h"
#include "SomeTexture.h"

using namespace std;
using namespace PJ;

namespace SlicedTextureRendererTests {
    class MockTexture : public SomeTexture {
    public:
        MockTexture() : SomeTexture("", 1, Vector2Int(50, 50), TextureAlphaMode::PremultiplyAlpha) {
        }
    };
}

using namespace SlicedTextureRendererTests;

TEST(SlicedTextureRenderer, TestBuildModel) {
    auto texture = MAKE<MockTexture>();
    SlicedTextureRenderer renderer(texture, Vector2(100, 100), {Vector2Int(10, 10), Vector2Int(10, 10)});
    auto sut = renderer.MakeBuildModel();

    EXPECT_EQ(10, sut.sliceLeft);
    EXPECT_EQ(10, sut.sliceTop);
    EXPECT_EQ(10, sut.sliceRightOffset);
    EXPECT_EQ(10, sut.sliceBottomOffset);
    EXPECT_EQ(40, sut.sliceRight);
    EXPECT_EQ(40, sut.sliceBottom);

    EXPECT_EQ(-40, sut.left);
    EXPECT_EQ(40, sut.top);
    EXPECT_EQ(40, sut.right);
    EXPECT_EQ(-40, sut.bottom);

    EXPECT_EQ(80, sut.topAndBottomWorldWidth);
    EXPECT_EQ(80, sut.leftAndRightWorldHeight);
    EXPECT_EQ(30, sut.topAndBottomSliceWidth);
    EXPECT_EQ(30, sut.leftAndRightSliceHeight);

    float ten50 = 10.0f/50.0f;
    float thirty50 = 30.0f/50.0f;

    EXPECT_EQ(ten50, sut.uvLeft);
    EXPECT_EQ(ten50, sut.uvLeftWidth);
    EXPECT_EQ(ten50, sut.uvTopOffset);
    EXPECT_EQ(ten50, sut.uvTopHeight);
    EXPECT_EQ(1.0f - ten50, sut.uvTop);
    EXPECT_EQ(ten50, sut.uvBottom);
    EXPECT_EQ(ten50, sut.uvBottomHeight);
    EXPECT_EQ(1.0f - ten50, sut.uvRight);
    EXPECT_EQ(ten50, sut.uvRightWidth);
    EXPECT_EQ(thirty50, sut.uvTopAndBottomWidth);
    EXPECT_EQ(thirty50, sut.uvLeftAndRightHeight);

    EXPECT_EQ(UVRect(Vector2(0, 1.0f - ten50), Vector2(ten50, ten50)), sut.topLeftUVRect);
    EXPECT_EQ(UVRect(Vector2(ten50, 1.0f - ten50), Vector2(thirty50, ten50)), sut.topUVRect);
    EXPECT_EQ(UVRect(Vector2(1.0f - ten50, 1.0f - ten50), Vector2(ten50, ten50)), sut.topRightUVRect);
    EXPECT_EQ(UVRect(Vector2(0, ten50), Vector2(ten50, thirty50)), sut.leftUVRect);
    EXPECT_EQ(UVRect(Vector2(ten50, ten50), Vector2(thirty50, thirty50)), sut.centerUVRect);
    EXPECT_EQ(UVRect(Vector2(1.0f - ten50, ten50), Vector2(ten50, thirty50)), sut.rightUVRect);
    EXPECT_EQ(UVRect(Vector2(0, 0), Vector2(ten50, ten50)), sut.bottomLeftUVRect);
    EXPECT_EQ(UVRect(Vector2(ten50, 0), Vector2(thirty50, ten50)), sut.bottomUVRect);
    EXPECT_EQ(UVRect(Vector2(1.0f - ten50, 0), Vector2(ten50, ten50)), sut.bottomRightUVRect);
}
