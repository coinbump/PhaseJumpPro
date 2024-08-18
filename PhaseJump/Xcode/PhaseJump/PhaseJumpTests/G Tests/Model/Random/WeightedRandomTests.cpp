#include "gtest/gtest.h"
#include "Model/WeightedRandomChoice.h"
#include "FixedRandom.h"

#include <memory>

using namespace PJ;
using namespace std;

namespace WeightedRandomTests {
}

using namespace WeightedRandomTests;

TEST(WeightedRandom, Factory) {
    WeightedRandomChoice<string> sut;
    auto w1 = Weight<string>(1.0f, "1");
    auto w2 = Weight<string>(0.5f, "2");
    auto w3 = Weight<string>(0.25f, "3");
    auto totalWeight = w1.Value() + w2.Value() + w3.Value();

    sut.Add(w1);
    sut.Add(w2);
    sut.Add(w3);

    EXPECT_EQ("1", sut.ChooseWeightAt(.5f / totalWeight).value().core);
    EXPECT_EQ("1", sut.ChooseWeightAt(.9f / totalWeight).value().core);
    EXPECT_EQ("2", sut.ChooseWeightAt(1.1f / totalWeight).value().core);
    EXPECT_EQ("3", sut.ChooseWeightAt(1.6f / totalWeight).value().core);
}
