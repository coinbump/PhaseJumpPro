#include "gtest/gtest.h"
#include "Types/_String.h"

#include "Modules.h"

using namespace PJ;

TEST(Module, Core_Foo) {
    ClassRegistry classRegistry;
    Modules::Core sut(classRegistry);
    sut.Go();

    EXPECT_TRUE(classRegistry.ContainsKey(ClassIds::foo));
    EXPECT_NE(nullptr, DCAST<_Foo>(classRegistry.New(ClassIds::foo)));
    EXPECT_EQ(nullptr, DCAST<_MacFoo>(classRegistry.New(ClassIds::foo)));
}

TEST(Module, MacPlatform_Foo) {
    ClassRegistry classRegistry;
    Modules::MacPlatform sut(classRegistry);
    sut.Go();

    EXPECT_TRUE(classRegistry.ContainsKey(ClassIds::foo));
    EXPECT_NE(nullptr, DCAST<_Foo>(classRegistry.New(ClassIds::foo)));
    EXPECT_NE(nullptr, DCAST<_MacFoo>(classRegistry.NewType<_MacFoo>(ClassIds::foo)));
}
