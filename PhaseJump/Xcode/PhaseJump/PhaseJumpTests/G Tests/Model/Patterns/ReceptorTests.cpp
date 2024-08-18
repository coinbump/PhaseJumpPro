#include "gtest/gtest.h"
#include "SomeReceptor.h"
#include "WorldNode.h"
#include "Utils.h"
#include "UnorderedMap.h"

using namespace PJ;
using namespace std;

namespace ReceptorTests {
    class TestReceptor : public SomeReceptor<String> {
    public:
        UnorderedMap<String, int> matches;

        void OnSignalMatch(Signal<String> signal) override {
            matches[*signal.Keys().begin()] += 1;
        }
    };

    class TestSignal : public Signal<String> {
    public:
        TestSignal() {
        }

        void Configure() {
            AddKey("test");
        }
    };
}

using namespace ReceptorTests;

TEST(Receptor, Tests)
{
    TestReceptor sut;
    TestSignal signal;
    signal.Configure();

    TestSignal badSignal;
    badSignal.AddKey("wrong");

    sut.AddLockForKey("test");

    sut.OnSignal(badSignal);
    sut.OnSignal(signal);
    sut.OnSignal(signal);

    EXPECT_EQ(2, sut.matches["test"]);
}
