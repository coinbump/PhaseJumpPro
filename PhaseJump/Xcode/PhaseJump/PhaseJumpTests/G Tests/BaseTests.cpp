#include "gtest/gtest.h"
#include "Base.h"
#include "GeoTransform.h"
#include <memory>

using namespace std;
using namespace PJ;

namespace BaseTests {
    class TestBase : public Base {
    public:
        uint32_t goCount{};
        
        void OnGo() override {
            goCount++;
        }
    };
}

using namespace BaseTests;

TEST(Base, TestInit) {
    TestBase sut;
    EXPECT_EQ(0, sut.goCount);
    EXPECT_FALSE(sut.DidGo());
    sut.Go();
    EXPECT_EQ(1, sut.goCount);
    EXPECT_TRUE(sut.DidGo());
    sut.Go();
    EXPECT_EQ(1, sut.goCount);
}
