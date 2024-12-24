#include "gtest/gtest.h"
#include "StringUtils.h"

#include "Modules.h"

using namespace PJ;

//TEST(Module, Core_Foo) {
//    ClassRegistry classRegistry;
//    CoreModule sut(classRegistry);
//    sut.Go();
//
//    auto value = ContainsKey(classRegistry.Map(), ClassId::Foo);
//    EXPECT_TRUE(value);
//    EXPECT_NE(nullptr, DCAST<_Foo>(classRegistry.MakeBase(ClassId::Foo)));
//    EXPECT_EQ(nullptr, DCAST<_MacFoo>(classRegistry.MakeBase(ClassId::Foo)));
//}
//
//TEST(Module, MacPlatform_Foo) {
//    ClassRegistry classRegistry;
//    MacPlatformModule sut(classRegistry);
//    sut.Go();
//
//    auto value = ContainsKey(classRegistry.Map(), ClassId::Foo);
//    EXPECT_TRUE(value);
//    EXPECT_NE(nullptr, DCAST<_Foo>(classRegistry.MakeBase(ClassId::Foo)));
//    EXPECT_NE(nullptr, DCAST<_MacFoo>(classRegistry.MakeType<_MacFoo>(ClassId::Foo)));
//}
