#include "gtest/gtest.h"

#include "Data.h"

using namespace std;
using namespace PJ;

TEST(Data, TestCallocAllocator)
{
    Data<CallocDataAllocator> sut;

    EXPECT_EQ(nullptr, sut.DataPointer());
    EXPECT_EQ(0, sut.DataSize());

    sut.Resize(4);

    EXPECT_NE(nullptr, sut.DataPointer());
    EXPECT_EQ(4, sut.DataSize());

    EXPECT_EQ(0, *((uint32_t*)sut.DataPointer()));
}

TEST(Data, TestResize)
{
    Data<CallocDataAllocator> sut;

    sut.Resize(4);

    EXPECT_NE(nullptr, sut.DataPointer());
    EXPECT_EQ(4, sut.DataSize());

    sut.Resize(8);

    EXPECT_NE(nullptr, sut.DataPointer());
    EXPECT_EQ(8, sut.DataSize());
}

TEST(Data, TestFlush)
{
    Data<CallocDataAllocator> sut;

    sut.Resize(4);

    EXPECT_NE(nullptr, sut.DataPointer());
    EXPECT_EQ(4, sut.DataSize());

    sut.Flush();

    EXPECT_EQ(nullptr, sut.DataPointer());
    EXPECT_EQ(0, sut.DataSize());
}

TEST(Data, TestCopy)
{
    Data<CallocDataAllocator> sut;

    sut.Resize(4);

    EXPECT_NE(nullptr, sut.DataPointer());
    EXPECT_EQ(4, sut.DataSize());

    uint32_t testValue = 0xEF1fAABB;
    *((uint32_t*)sut.DataPointer()) = testValue;

    auto copy = sut.Copy();

    EXPECT_NE(nullptr, copy->DataPointer());
    EXPECT_EQ(4, copy->DataSize());
    EXPECT_EQ(testValue, *((uint32_t*)copy->DataPointer()));
}
