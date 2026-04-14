#include "gtest/gtest.h"
#include "Receptor.h"
#include "WorldNode.h"
#include "Utils.h"
#include "UnorderedMap.h"

using namespace PJ;
using namespace std;

namespace ReceptorTests {
    class TestReceptor : public Receptor<String> {
    public:
        UnorderedMap<String, int> matches;

        void OnSignalMatch(ReceptorSignal<String> const& signal) override {
            matches[*signal.Keys().begin()] += 1;
        }
    };

    class TestSignal  {
    public:
        ReceptorSignal<String> signal;
        
        TestSignal() {
        }

        void Configure() {
            signal.AddKey("test");
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
    badSignal.signal.AddKey("wrong");

    sut.AddLockForKey("test");

    sut.OnSignal(badSignal.signal);
    sut.OnSignal(signal.signal);
    sut.OnSignal(signal.signal);

    EXPECT_EQ(2, sut.matches["test"]);
}
