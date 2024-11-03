#include "gtest/gtest.h"
#include "SequenceCounter.h"

using namespace PJ;
using namespace std;

namespace SequenceCounterTests {
}

using namespace SequenceCounterTests;

TEST(SequenceCounter, Test)
{
    int sequenceValue = 0;

    SequenceCounter sut(2);
    sut.onFinishSequenceFunc = [&](auto& counter) {
        sequenceValue++;
    };

    EXPECT_EQ(0, sut.Count());
    EXPECT_EQ(0, sut.SequenceCount());

    sut.Fire();
    EXPECT_EQ(1, sut.Count());
    EXPECT_EQ(0, sut.SequenceCount());

    sut.Fire();
    EXPECT_EQ(0, sut.Count());
    EXPECT_EQ(1, sut.SequenceCount());
    EXPECT_EQ(1, sequenceValue);
}
