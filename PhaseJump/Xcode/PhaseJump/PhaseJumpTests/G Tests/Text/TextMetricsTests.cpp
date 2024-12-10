#include "gtest/gtest.h"
#include "TextMetrics.h"
#include "Font.h"

using namespace PJ;
using namespace std;

namespace TextMetricsTests {
}

using namespace TextMetricsTests;

TEST(TextMetrics, NoRoomToWrap)
{
    TextMetrics sut;
    sut.lines.push_back({.y = 10, .size = {10, 20}});
    sut.lines.push_back({.y = 30, .size = {40, 15}});

    auto size = sut.CalculateSize();
    EXPECT_EQ(40, size.x);
    EXPECT_EQ(45, size.y);
}
