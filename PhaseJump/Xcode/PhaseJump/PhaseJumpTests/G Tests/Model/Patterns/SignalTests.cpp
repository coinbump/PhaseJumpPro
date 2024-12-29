#include "gtest/gtest.h"
#include "ReceptorSignal.h"
#include "WorldNode.h"
#include "Utils.h"
#include "UnorderedMap.h"

using namespace PJ;
using namespace std;

namespace SignalTests {
    class TestSignal : public ReceptorSignal<String> {
    public:
        TestSignal() {
        }

        void Configure() {
            AddKey("test");
        }
    };
}

using namespace SignalTests;

TEST(Signal, Tests)
{
    TestSignal sut;
    sut.Configure();

    EXPECT_TRUE(sut.ContainsKey("test"));
    EXPECT_FALSE(sut.ContainsKey("wrong"));
}
