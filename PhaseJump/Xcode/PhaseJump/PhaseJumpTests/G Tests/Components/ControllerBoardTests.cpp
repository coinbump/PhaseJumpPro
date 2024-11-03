#include "gtest/gtest.h"
#include "ControllerBoard.h"

using namespace PJ;
using namespace std;

namespace ControllerBoardTests {
}

using namespace ControllerBoardTests;

TEST(ControllerBoard, Set)
{
    ControllerBoard sut;

    Color color;
    sut.colorFuncs.insert_or_assign("color", [&](String id, Color value) {
        color = value;
    });

    bool switchValue{};
    sut.switchFuncs.insert_or_assign("switch", [&](String id, bool value) {
        switchValue = value;
    });

    String state;
    sut.stateFuncs.insert_or_assign("state", [&](String id, String value) {
        state = value;
    });

    float floatValue{};
    sut.valueFuncs.insert_or_assign("value", [&](String id, float value) {
        floatValue = value;
    });

    sut.SetColor("color", Color::red);
    EXPECT_EQ(Color::red, color);
    sut.SetColor("", Color::blue);
    EXPECT_EQ(Color::red, color);

    sut.SetSwitch("switch", true);
    EXPECT_TRUE(switchValue);
    sut.SetSwitch("", false);
    EXPECT_TRUE(switchValue);

    sut.SetState("state", "test");
    EXPECT_EQ("test", state);
    sut.SetState("", "t");
    EXPECT_EQ("test", state);

    sut.SetValue("value", 3);
    EXPECT_EQ(3, floatValue);
    sut.SetValue("", 30);
    EXPECT_EQ(3, floatValue);
}

TEST(ControllerBoard, SetAll)
{
    ControllerBoard sut;
    float floatValue{};
    sut.valueFuncs.insert_or_assign(ControllerBoardId::All, [&](String id, float value) {
        floatValue = value;
    });

    sut.SetValue("value", 3);
    EXPECT_EQ(3, floatValue);
    sut.SetValue("", 30);
    EXPECT_EQ(30, floatValue);
}
