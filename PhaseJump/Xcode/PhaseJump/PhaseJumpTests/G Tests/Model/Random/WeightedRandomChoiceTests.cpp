#include "gtest/gtest.h"
#include "WeightedRandomChoice.h"
#include "MockRandom.h"

#include <memory>

using namespace PJ;
using namespace std;

namespace WeightedRandomChoiceTests {
}

using namespace WeightedRandomChoiceTests;

TEST(WeightedRandomChoice, Weights) {
    WeightedRandomChoice<string> sut;
    UP<SomeWeight<string>> w1 = NEW<Weight<string>>(1.0f, "1");
    UP<SomeWeight<string>> w2 = NEW<Weight<string>>(0.5f, "2");
    UP<SomeWeight<string>> w3 = NEW<Weight<string>>(0.25f, "3");
    auto totalWeight = w1->Value() + w2->Value() + w3->Value();

    sut.Add(w1);
    sut.Add(w2);
    sut.Add(w3);

    EXPECT_EQ("1", sut.ChooseAt(.5f / totalWeight));
    EXPECT_EQ("1", sut.ChooseAt(.9f / totalWeight));
    EXPECT_EQ("2", sut.ChooseAt(1.1f / totalWeight));
    EXPECT_EQ("3", sut.ChooseAt(1.6f / totalWeight));
}

TEST(WeightedRandomChoice, DynamicWeights) {
    WeightedRandomChoice<string> sut;
    UP<SomeWeight<string>> w1 = NEW<DynamicWeight<string>>([](auto& weight) { return 1.0f; }, "1");
    UP<SomeWeight<string>> w2 = NEW<DynamicWeight<string>>([](auto& weight) { return 0.5f; }, "2");
    UP<SomeWeight<string>> w3 = NEW<DynamicWeight<string>>([](auto& weight) { return 0.25f; }, "3");
    auto totalWeight = w1->Value() + w2->Value() + w3->Value();

    sut.Add(w1);
    sut.Add(w2);
    sut.Add(w3);

    EXPECT_EQ("1", sut.ChooseAt(.5f / totalWeight));
    EXPECT_EQ("1", sut.ChooseAt(.9f / totalWeight));
    EXPECT_EQ("2", sut.ChooseAt(1.1f / totalWeight));
    EXPECT_EQ("3", sut.ChooseAt(1.6f / totalWeight));
}
