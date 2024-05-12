#include "gtest/gtest.h"
#include "SomeCancellable.h"

using namespace PJ;
using namespace std;

namespace CancellableTests {
}

using namespace CancellableTests;

TEST(Cancellable, Test_Cancel)
{
    auto sut = MAKE<Cancellable>();
    EXPECT_FALSE(sut->IsCancelled());

    sut->Cancel();
    EXPECT_TRUE(sut->IsCancelled());
}
