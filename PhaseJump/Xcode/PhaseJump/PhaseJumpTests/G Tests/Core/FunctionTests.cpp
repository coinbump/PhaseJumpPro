#include "gtest/gtest.h"
#include "Function.h"

using namespace PJ;
using namespace std;

namespace FunctionTests {
    struct Core {
        int value = 5;
    };
}

using namespace FunctionTests;

TEST(Function, IntResult)
{
    Function<int()> func([]() { return 10; });
    EXPECT_EQ(10, func());
}

TEST(Function, VoidResult)
{
    int value = 0;
    Function<void()> func([&]() { value = 10; });
    func();
    EXPECT_EQ(10, value);
}

TEST(Function, NullFunc)
{
    Function<int()> func(nullptr);
    EXPECT_EQ(0, func());
}

TEST(Function, WithArguments)
{
    Function<int(int, int)> func([](int a, int b) { return a + b; });
    EXPECT_EQ(10, func(4, 6));
}

TEST(CoreFunction, IntResult)
{
    CoreFunction<Core, int> func([](Core& core) { return core.value; });
    EXPECT_EQ(5, func());
}

TEST(CoreFunction, VoidResult)
{
    CoreFunction<Core, void> func([&](Core& core) { core.value = 10; });
    func();
    EXPECT_EQ(10, func.core.value);
}

TEST(CoreFunction, WithArguments)
{
    CoreFunction<Core, int, int, int> func([](Core& core, int a, int b) { return core.value + a + b; });
    EXPECT_EQ(15, func(4, 6));
}

TEST(CoreFunction, NullFunc)
{
    CoreFunction<Core, int> func(nullptr);
    EXPECT_EQ(0, func());
}
