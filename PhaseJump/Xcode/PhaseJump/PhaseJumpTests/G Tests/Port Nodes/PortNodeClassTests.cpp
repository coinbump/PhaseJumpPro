#include "gtest/gtest.h"
#include "PortNodeClass.h"
#include "PortEvaluation.h"
#include "PortNode.h"

using namespace PJ;
using namespace std;

namespace PortNodeClassTests {
}

using namespace PortNodeClassTests;

TEST(PortNodeClass, Tests)
{
    auto sut = MAKE<PortNodeClass>("test", PortNodeClass::Type::Data, [](PortNode& node) {
        return PortEvaluation();
    });
    sut->AddInput<String>("istring", "string", "default");
    sut->AddInput<int>("iint", "float", 10);
    sut->AddOutput<String>("ostring", "string");

    EXPECT_EQ(2, sut->inputs.size());
    EXPECT_NE(nullptr, sut->inputs["istring"]);
    EXPECT_NE(nullptr, sut->inputs["iint"]);
    EXPECT_EQ(1, sut->outputs.size());
    EXPECT_NE(nullptr, sut->outputs["ostring"]);
}
