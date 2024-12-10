#include "gtest/gtest.h"
#include "ValueSampler.h"
#include "Funcs.h"

using namespace PJ;
using namespace std;

namespace ValueSamplerTests {
}

using namespace ValueSamplerTests;

TEST(ValueSampler, Init)
{
    int value = 0;
    ValueSampler<int> sut(10, [&]() { value++; return value; });

    auto& samples = sut.Samples();

    sut.OnUpdate({3});
    ASSERT_EQ(1, samples.size());
    EXPECT_EQ(1, samples[0]);

    sut.OnUpdate({3});
    ASSERT_EQ(2, samples.size());
    EXPECT_EQ(2, samples[1]);

    sut.OnUpdate({3});
    ASSERT_EQ(3, samples.size());
    EXPECT_EQ(3, samples[2]);

    sut.OnUpdate({3});
    ASSERT_EQ(3, samples.size());
    EXPECT_EQ(2, samples[0]);
    EXPECT_EQ(3, samples[1]);
    EXPECT_EQ(4, samples[2]);
}
