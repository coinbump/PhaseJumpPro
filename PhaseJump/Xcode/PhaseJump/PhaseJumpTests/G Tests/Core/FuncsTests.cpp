#include "gtest/gtest.h"
#include "Funcs.h"
#include "StringUtils.h"

using namespace PJ;
using namespace std;

namespace FuncsTests {
}

using namespace FuncsTests;

TEST(Funcs, Repeat)
{
    int value = 0;
    Repeat(10, [&]() { value++; });
    EXPECT_EQ(10, value);
}

TEST(Funcs, RepeatCount)
{
    int value = 0;
    RepeatCount(3, [&](auto count) { value += count; });
    EXPECT_EQ(3, value);
}

TEST(Funcs, For)
{
    int value = 0;
    For(0, 10, [&]() { value++; });
    EXPECT_EQ(10, value);
}

TEST(Funcs, ForCount)
{
    int value = 0;
    ForCount(0, 3, [&](auto count) { value += count; });
    EXPECT_EQ(3, value);
}

TEST(Funcs, Override)
{
    String value;
    using Func = std::function<void(int)>;

    Func baseFunc = [&](auto input) { value += "base"; };
    Func overrideFunc = [&](auto input) { value += MakeString(input); };
    baseFunc = OverrideFunc(baseFunc, overrideFunc);

    baseFunc(10);
    EXPECT_EQ("base10", value);
}

TEST(Funcs, OverrideNoBase)
{
    String value;
    using Func = std::function<void(int)>;

    Func baseFunc;
    Func overrideFunc = [&](auto input) { value += MakeString(input); };
    baseFunc = OverrideFunc(baseFunc, overrideFunc);

    baseFunc(10);
    EXPECT_EQ("10", value);
}

TEST(Funcs, OverrideNoOverride)
{
    String value;
    using Func = std::function<void(int)>;

    Func baseFunc = [&](auto input) { value += "base"; };
    Func overrideFunc;
    baseFunc = OverrideFunc(baseFunc, overrideFunc);

    baseFunc(10);
    EXPECT_EQ("base", value);
}

TEST(Funcs, OverrideMoveArgsDontCopy)
{
    String value = "base";
    using Func = std::function<void(String&)>;

    Func baseFunc = [&](String& input) { value += input; };
    Func overrideFunc = [&](String& input) { value += input; };
    baseFunc = OverrideFunc(baseFunc, overrideFunc);

    baseFunc(value);
    EXPECT_EQ("basebasebasebase", value);
}
