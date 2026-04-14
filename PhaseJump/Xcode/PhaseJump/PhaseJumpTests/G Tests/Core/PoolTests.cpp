#include "gtest/gtest.h"

#undef _DISABLE_MIRRORS_

#include "Pool.h"
#include "Utils.h"

using namespace PJ;
using namespace std;

namespace PoolTests {
    class PoolTest : public ::testing::Test {
    protected:
        void SetUp() override {
            // Any shared setup can go here
        }
    };
}

using namespace PoolTests;

TEST_F(PoolTest, Poolable_ActivationTriggersReset) {
    bool isResetCalled = false;

    Pool<String>::ResetFunc resetFunc = [&](auto& p) {
        isResetCalled = true;
    };

    Pool<String> sut(2, resetFunc);

    sut.Add("test1");
    sut.RemoveAt(0);
    sut.Add("test2");
    
    EXPECT_TRUE(isResetCalled);
}

TEST(Pool, Test)
{
    Pool<String>::ResetFunc resetFunc = [](auto& p) {
        p = "reset";
    };

    Pool<String> sut(2, resetFunc);
    EXPECT_EQ(2, sut.Size());
    EXPECT_EQ(0, sut.ActiveCount());

    auto first = sut.Add();
    EXPECT_TRUE(first);
    EXPECT_EQ(1, sut.ActiveCount());
    EXPECT_TRUE(sut.IsActive(0));
    EXPECT_EQ(0, sut.InactiveAvailableCount());

    auto second = sut.Add();
    EXPECT_TRUE(second);
    EXPECT_EQ(2, sut.ActiveCount());
    EXPECT_TRUE(sut.IsActive(1));
    EXPECT_EQ(2, sut.Size());

    sut.Remove(first);
    EXPECT_EQ(1, sut.InactiveAvailableCount());
    EXPECT_FALSE(sut.IsActive(0));
    EXPECT_TRUE(sut.IsActive(1));

    sut.Remove(second);
    EXPECT_EQ(2, sut.InactiveAvailableCount());
    EXPECT_FALSE(sut.IsActive(0));
    EXPECT_FALSE(sut.IsActive(1));

    auto third = sut.Add();
    EXPECT_EQ(2, sut.Size());
    EXPECT_EQ(1, sut.InactiveAvailableCount());
    EXPECT_TRUE(sut.IsActive(1));
    EXPECT_EQ("reset", third->Get());

    sut.Add();
    EXPECT_EQ(2, sut.Size());
    EXPECT_EQ(0, sut.InactiveAvailableCount());
    
    sut.Add();
    EXPECT_EQ(2, sut.Size());
    EXPECT_EQ(0, sut.InactiveAvailableCount());
}

TEST(Pool, RemoveInactive)
{
    Pool<String>::ResetFunc resetFunc = [](auto& p) {
        p = "reset";
    };

    Pool<String> sut(2, resetFunc);
    auto first = sut.Add();
    sut.Remove(first);
    EXPECT_EQ(0, sut.ActiveCount());
    sut.Remove(first);
    EXPECT_EQ(0, sut.ActiveCount());
}

//TEST(Pool, MaxSize)
//{
//    Pool<String> sut(2);
//    sut.maxSize = 2;
//    EXPECT_NE(nullptr, sut.Add());
//    EXPECT_NE(nullptr, sut.Add());
//    EXPECT_EQ(nullptr, sut.Add());
//}

TEST(Pool, At)
{
    Pool<String>::ResetFunc resetFunc = [](auto& p) {
        p = "reset";
    };

    Pool<String> sut(2, resetFunc);

    auto first = sut.Add();
    EXPECT_TRUE(first);
    EXPECT_EQ(1, sut.ActiveCount());
    EXPECT_EQ(first->Get(), sut.at(0));

    EXPECT_ANY_THROW(sut.at(-1));
    EXPECT_ANY_THROW(sut.at(1));
}

TEST(Pool, ActiveEnd)
{
    Pool<int>::ResetFunc resetFunc = [](auto& p) {
        p = 0;
    };

    Pool<int> sut(4, resetFunc);

    auto first = sut.Add();
    sut.Add();
    sut.Add();
    
    EXPECT_EQ(3, sut.ActiveScanEnd());
    
    sut.Remove(first);
    
    EXPECT_EQ(3, sut.ActiveScanEnd());
}

TEST(Pool, AtEnd)
{
    Pool<int>::ResetFunc resetFunc = [](auto& p) {
        p = 0;
    };

    Pool<int> sut(4, resetFunc);

    auto first = sut.Add();
    sut.Add();
    sut.Add();
    
    // Decrements activeCount, make sure we can still access past that
    sut.Remove(first);
    
    EXPECT_NO_THROW(sut.at(2));
}

TEST(Pool, ForActiveAtEnd)
{
    Pool<int>::ResetFunc resetFunc = [](auto& p) {
        p = 0;
    };

    Pool<int> sut(4, resetFunc);

    auto first = sut.Add();
    auto second = sut.Add();
    auto third = sut.Add();
    
    // Decrements activeCount, make sure we can still access past that
    sut.Remove(first);
    
    sut.ForActive([](auto& item) {
        item++;
    });

    EXPECT_EQ(0, first->Get());
    EXPECT_EQ(1, second->Get());
    EXPECT_EQ(1, third->Get());
}

TEST(Pool, ForActive)
{
    Pool<int>::ResetFunc resetFunc = [](auto& p) {
        p = 0;
    };

    Pool<int> sut(2, resetFunc);

    auto first = sut.Add();
    EXPECT_TRUE(first);
    EXPECT_EQ(first->Get(), sut.at(0));
    EXPECT_EQ(first->Get(), 0);
    
    sut.ForActive([](auto& item) {
        item++;
    });

    EXPECT_EQ(first->Get(), 1);
    
    auto second = sut.Add();
    EXPECT_EQ(second->Get(), 0);
    
    sut.ForActive([](auto& item) {
        item++;
    });
    
    EXPECT_EQ(first->Get(), 2);
    EXPECT_EQ(second->Get(), 1);
}
