#include "gtest/gtest.h"
#include "WeakReference.h"
#include "StrongReference.h"

#include <memory>

using namespace PJ;
using namespace std;

namespace SomeReferenceTests {
    static int destructorCount = 0;

    class TestClass {
    public:
        virtual ~TestClass() {
            destructorCount++;
        }
    };
}

using namespace SomeReferenceTests;

TEST(SomeReference, WeakReference) {
    auto shared = MAKE<TestClass>();

    EXPECT_EQ(shared.use_count(), 1);

    auto weakReference = WeakReference(shared);
    EXPECT_EQ(shared.use_count(), 1);

    auto value = weakReference.Value();
    EXPECT_EQ(value.use_count(), 2);
    EXPECT_EQ(shared.use_count(), 2);

    value.reset();
    EXPECT_EQ(shared.use_count(), 1);

    EXPECT_FALSE(weakReference.IsExpired());
    shared.reset();

    EXPECT_EQ(1, destructorCount);

    EXPECT_TRUE(weakReference.IsExpired());
}
