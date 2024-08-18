#include "gtest/gtest.h"
#include "_String.h"

#include "Modules.h"

using namespace PJ;

TEST(Module, Core_Foo) {
    ClassRegistry classRegistry;
    CoreModule sut(classRegistry);
    sut.Go();

    auto value = ContainsKey(classRegistry.map, ClassId::Foo);
    EXPECT_TRUE(value);
    EXPECT_NE(nullptr, DCAST<_Foo>(classRegistry.NewBase(ClassId::Foo)));
    EXPECT_EQ(nullptr, DCAST<_MacFoo>(classRegistry.NewBase(ClassId::Foo)));
}

TEST(Module, MacPlatform_Foo) {
    ClassRegistry classRegistry;
    MacPlatformModule sut(classRegistry);
    sut.Go();

    auto value = ContainsKey(classRegistry.map, ClassId::Foo);
    EXPECT_TRUE(value);
    EXPECT_NE(nullptr, DCAST<_Foo>(classRegistry.NewBase(ClassId::Foo)));
    EXPECT_NE(nullptr, DCAST<_MacFoo>(classRegistry.NewType<_MacFoo>(ClassId::Foo)));
}
