#include "gtest/gtest.h"
#include "StringUtils.h"

#include "Modules.h"

using namespace PJ;

TEST(Module, Core_Foo) {
    CoreModule::ClassRegistry classRegistry;
    CoreModule sut(classRegistry);
    sut.Go();

    auto value = ContainsKey(classRegistry.Map(), ClassId::Foo);
    EXPECT_TRUE(value);
    EXPECT_NE(nullptr, dynamic_cast<_Foo*>(classRegistry.MakeType<Base>(ClassId::Foo).get()));
    EXPECT_EQ(nullptr, dynamic_cast<_MacFoo*>(classRegistry.MakeType<Base>(ClassId::Foo).get()));
}

TEST(Module, MacPlatform_Foo) {
    CoreModule::ClassRegistry classRegistry;
    MacPlatformModule sut(classRegistry);
    sut.Go();

    auto value = ContainsKey(classRegistry.Map(), ClassId::Foo);
    EXPECT_TRUE(value);
    EXPECT_NE(nullptr, dynamic_cast<_Foo*>(classRegistry.MakeType<Base>(ClassId::Foo).get()));
    EXPECT_NE(nullptr, dynamic_cast<_MacFoo*>(classRegistry.MakeType<Base>(ClassId::Foo).get()));
}
