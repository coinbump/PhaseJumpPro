#include "gtest/gtest.h"

#include "Data.h"

using namespace std;
using namespace PJ;

namespace DataTests {
}

using namespace DataTests;

TEST(Data, TestFailAllocator)
{
    Data<> sut([](size_t size) {
        return nullptr;
    });

    EXPECT_EQ(nullptr, sut.Pointer());
    EXPECT_EQ(0, sut.Size());

    sut.ResizeBytes(4);

    EXPECT_EQ(nullptr, sut.Pointer());
    EXPECT_EQ(0, sut.Size());

    Data<> data([](size_t size) {
        return CallocDataAllocator()(size);
    });
    data.ResizeBytes(4);
    EXPECT_EQ(4, data.Size());

    sut.CopyIn(data.Pointer(), data.Size());

    EXPECT_EQ(nullptr, sut.Pointer());
    EXPECT_EQ(0, sut.Size());
}

TEST(Data, TestCallocDataAllocator)
{
    Data<> sut([](size_t size) {
        return CallocDataAllocator()(size);
    });

    EXPECT_EQ(nullptr, sut.Pointer());
    EXPECT_EQ(0, sut.Size());

    sut.ResizeBytes(4);

    EXPECT_NE(nullptr, sut.Pointer());
    EXPECT_EQ(4, sut.Size());

    EXPECT_EQ(0, *((uint32_t*)sut.Pointer()));
}

TEST(Data, TestResize)
{
    Data<> sut([](size_t size) {
        return CallocDataAllocator()(size);
    });

    sut.ResizeBytes(4);

    EXPECT_NE(nullptr, sut.Pointer());
    EXPECT_EQ(4, sut.Size());

    sut.ResizeBytes(8);

    EXPECT_NE(nullptr, sut.Pointer());
    EXPECT_EQ(8, sut.Size());
}

TEST(Data, TestFlush)
{
    Data<> sut([](size_t size) {
        return CallocDataAllocator()(size);
    });

    sut.ResizeBytes(4);

    EXPECT_NE(nullptr, sut.Pointer());
    EXPECT_EQ(4, sut.Size());

    sut.Flush();

    EXPECT_EQ(nullptr, sut.Pointer());
    EXPECT_EQ(0, sut.Size());
}

TEST(Data, TestCopy)
{
    Data<> sut([](size_t size) {
        return CallocDataAllocator()(size);
    });

    sut.ResizeBytes(4);

    EXPECT_NE(nullptr, sut.Pointer());
    EXPECT_EQ(4, sut.Size());

    uint32_t testValue = 0xEF1fAABB;
    *((uint32_t*)sut.Pointer()) = testValue;

    auto copy = sut.Copy();

    EXPECT_NE(nullptr, copy->Pointer());
    EXPECT_EQ(4, copy->Size());
    EXPECT_EQ(testValue, *((uint32_t*)copy->Pointer()));
}

TEST(Data, TestCopyIn)
{
    Data<> sut([](size_t size) {
        return CallocDataAllocator()(size);
    });

    sut.ResizeBytes(4);
    uint32_t testValue = 0xEF1fAABB;
    *((uint32_t*)sut.Pointer()) = testValue;

    Data<> sut2([](size_t size) {
        return CallocDataAllocator()(size);
    });
    sut2.CopyIn(sut.Pointer(), sut.Size());
    EXPECT_NE(nullptr, sut2.Pointer());
    EXPECT_EQ(4, sut2.Size());
    EXPECT_EQ(testValue, *((uint32_t*)sut2.Pointer()));
}

TEST(Data, TestCopyInInvalid)
{
    Data<> sut([](size_t size) {
        return CallocDataAllocator()(size);
    });

    sut.ResizeBytes(4);
    uint32_t testValue = 0xEF1fAABB;
    *((uint32_t*)sut.Pointer()) = testValue;

    Data<> sut2([](size_t size) {
        return CallocDataAllocator()(size);
    });
    sut2.CopyIn(nullptr, -1);
    EXPECT_EQ(nullptr, sut2.Pointer());
    EXPECT_EQ(0, sut2.Size());
}

TEST(Data, TestCount)
{
    Data<int> sut([](size_t size) {
        return (int*)CallocDataAllocator()(size);
    });

    sut.ResizeCount(2);
    EXPECT_EQ(2 * sizeof(int), sut.Size());
    EXPECT_EQ(2, sut.Count());
}

