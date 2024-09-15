#include "gtest/gtest.h"

#undef _DISABLE_MIRRORS_

#include "Pool.h"
#include "Utils.h"

using namespace PJ;
using namespace std;

namespace PoolTests {
    struct TestAgent : public Poolable {
        String value;

        TestAgent() {
            resetFunc = [](auto& p) {
                ((TestAgent*)(&p))->value = "reset";
            };
        }
    };
}

using namespace PoolTests;

TEST(Pool, Test)
{
    Pool<TestAgent> sut(2);
    EXPECT_EQ(2, sut.PoolSize());
    EXPECT_EQ(0, sut.ActiveCount());

    EXPECT_EQ(-1, sut.LastActiveIndex());

    auto first = sut.Add();
    EXPECT_NE(nullptr, first);
    EXPECT_EQ(1, sut.ActiveCount());
    EXPECT_EQ(0, sut.Value()[0]->index);
    EXPECT_TRUE(sut.Value()[0]->IsActive());
    EXPECT_EQ(0, sut.LastActiveIndex());
    EXPECT_EQ(0, sut.InactiveAvailableCount());

    auto second = sut.Add();
    EXPECT_NE(nullptr, second);
    EXPECT_EQ(2, sut.ActiveCount());
    EXPECT_EQ(1, sut.Value()[1]->index);
    EXPECT_TRUE(sut.Value()[1]->IsActive());
    EXPECT_EQ(1, sut.LastActiveIndex());
    EXPECT_EQ(2, sut.PoolSize());

    sut.Remove(first);
    EXPECT_EQ(1, sut.LastActiveIndex());
    EXPECT_EQ(1, sut.InactiveAvailableCount());
    EXPECT_EQ(0, sut.Value()[0]->index);
    EXPECT_FALSE(sut.Value()[0]->IsActive());
    EXPECT_EQ(1, sut.Value()[1]->index);
    EXPECT_TRUE(sut.Value()[1]->IsActive());
    EXPECT_FALSE(first->IsActive());

    sut.Remove(second);
    EXPECT_EQ(-1, sut.LastActiveIndex());
    EXPECT_EQ(2, sut.InactiveAvailableCount());
    EXPECT_EQ(0, sut.Value()[0]->index);
    EXPECT_FALSE(sut.Value()[0]->IsActive());
    EXPECT_EQ(1, sut.Value()[1]->index);
    EXPECT_FALSE(sut.Value()[1]->IsActive());
    EXPECT_FALSE(second->IsActive());

    auto third = sut.Add();
    EXPECT_EQ(2, sut.PoolSize());
    EXPECT_EQ(1, sut.InactiveAvailableCount());
    EXPECT_NE(-1, sut.LastActiveIndex());
    EXPECT_TRUE(third->IsActive());
    EXPECT_EQ("reset", third->value);

    sut.Add();
    EXPECT_EQ(2, sut.PoolSize());
    EXPECT_EQ(0, sut.InactiveAvailableCount());
    EXPECT_EQ(1, sut.LastActiveIndex());
    
    sut.Add();
    EXPECT_EQ(4, sut.PoolSize());
    EXPECT_EQ(0, sut.InactiveAvailableCount());
    EXPECT_EQ(2, sut.LastActiveIndex());
}

TEST(Pool, RemoveInactive)
{
    Pool<TestAgent> sut(2);
    auto first = sut.Add();
    sut.Remove(first);
    EXPECT_EQ(0, sut.ActiveCount());
    sut.Remove(first);
    EXPECT_EQ(0, sut.ActiveCount());
}

TEST(Pool, MaxSize)
{
    Pool<TestAgent> sut(2);
    sut.maxSize = 2;
    EXPECT_NE(nullptr, sut.Add());
    EXPECT_NE(nullptr, sut.Add());
    EXPECT_EQ(nullptr, sut.Add());
}

TEST(Pool, At)
{
    Pool<TestAgent> sut(2);

    TestAgent* first = sut.Add();
    EXPECT_NE(nullptr, first);
    EXPECT_EQ(1, sut.ActiveCount());
    EXPECT_EQ(first, &sut.at(0));

    EXPECT_ANY_THROW(sut.at(-1));
    EXPECT_ANY_THROW(sut.at(1));
}
