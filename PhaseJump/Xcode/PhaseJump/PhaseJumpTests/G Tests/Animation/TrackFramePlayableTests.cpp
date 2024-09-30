#include "gtest/gtest.h"
#include "TrackFramePlayable.h"

using namespace PJ;
using namespace std;

namespace TrackFramePlayableTests {
}

using namespace TrackFramePlayableTests;

TEST(TrackFramePlayable, Tests)
{
    TrackFramePlayable sut(10);

    sut.AddFrameAt(1, 2);
    sut.OnUpdate({2});
    EXPECT_EQ(1, sut.Frame());
}
