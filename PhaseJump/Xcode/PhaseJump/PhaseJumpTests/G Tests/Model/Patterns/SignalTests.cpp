#include "gtest/gtest.h"
#include "ReceptorSignal.h"
#include "WorldNode.h"
#include "Utils.h"
#include "UnorderedMap.h"

using namespace PJ;
using namespace std;

namespace SignalTests {
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

using namespace SignalTests;

TEST(Signal, Tests)
{
    TestSignal sut;
    sut.Configure();

    EXPECT_TRUE(sut.signal.ContainsKey("test"));
    EXPECT_FALSE(sut.signal.ContainsKey("wrong"));
}
