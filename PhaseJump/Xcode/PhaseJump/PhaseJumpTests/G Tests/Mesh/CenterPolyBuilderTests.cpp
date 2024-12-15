#include "gtest/gtest.h"
#include "CenterPolyBuilder.h"
#include "FileProcessor.Utility.h"

using namespace PJ;
using namespace std;

namespace CenterPolyBuilderTests {
}

using namespace CenterPolyBuilderTests;

TEST(CenterPolyModel, IsClosed) {
    {
        CenterPolyModel sut{.startAngle = Angle::WithDegrees(0), .angleDelta = Angle::WithDegrees(360), .angleStep = Angle::WithDegrees(10)};
        EXPECT_TRUE(sut.IsClosed());
    }
    
    {
        CenterPolyModel sut{.startAngle = Angle::WithDegrees(-45), .angleDelta = Angle::WithDegrees(360), .angleStep = Angle::WithDegrees(10)};
        EXPECT_TRUE(sut.IsClosed());
    }
    
    {
        CenterPolyModel sut{.startAngle = Angle::WithDegrees(-45), .angleDelta = Angle::WithDegrees(3600), .angleStep = Angle::WithDegrees(10)};
        EXPECT_TRUE(sut.IsClosed());
    }
}

TEST(CenterPolyModel, IsClosedFalse) {
    CenterPolyModel sut{.startAngle = Angle::WithDegrees(0), .angleDelta = Angle::WithDegrees(350), .angleStep = Angle::WithDegrees(10)};
    EXPECT_FALSE(sut.IsClosed());
}

TEST(CenterPolyBuilder, BuildClosed) {
    CenterPolyBuilder sut;
    Polygon poly = sut.Build({}, {10, 10}, CenterPolyModel::quad);
    
    ASSERT_EQ(4, poly.Count());
    EXPECT_EQ(Vector2(0, 5), poly[0]);
    EXPECT_EQ(Vector2(5, 0), poly[1]);
    EXPECT_EQ(Vector2(0, -5), poly[2]);
    EXPECT_EQ(Vector2(-5, 0), poly[3]);
}

TEST(CenterPolyBuilder, BuildOpen) {
    CenterPolyBuilder sut;
    auto model = CenterPolyModel::quad;
    model.angleDelta = Angle::WithDegrees(180);
    Polygon poly = sut.Build({}, {10, 10}, model);
    
    ASSERT_EQ(3, poly.Count());
    EXPECT_EQ(Vector2(0, 5), poly[0]);
    EXPECT_EQ(Vector2(5, 0), poly[1]);
    EXPECT_EQ(Vector2(0, -5), poly[2]);
}
