#include "gtest/gtest.h"

#include "SelectHandler.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

namespace SelectHandlerTests {
    class TestSelectListener : public WorldComponent {
    public:
        int signalCount = 0;
        bool lastIsSelected = false;

        TestSelectListener() {
            AddSignalHandler<SelectEvent>(
                { .id = SignalId::Select, .func = [this](auto& component, auto& event) {
                     signalCount++;
                     lastIsSelected = event.isSelected;
                 } }
            );
        }

        String TypeName() const override {
            return "TestSelectListener";
        }
    };
} // namespace SelectHandlerTests

using namespace SelectHandlerTests;

TEST(SelectHandler, Init_DefaultsToUnselected) {
    SelectHandler sut;
    EXPECT_FALSE(sut.IsSelected());
    EXPECT_TRUE(sut.IsSelectable());
}

TEST(SelectHandler, SetIsSelected_True) {
    SelectHandler sut;
    sut.SetIsSelected(true);
    EXPECT_TRUE(sut.IsSelected());
}

TEST(SelectHandler, SetIsSelected_FalseAfterTrue) {
    SelectHandler sut;
    sut.SetIsSelected(true);
    sut.SetIsSelected(false);
    EXPECT_FALSE(sut.IsSelected());
}

TEST(SelectHandler, CustomCallback_FiresOnTransition) {
    SelectHandler sut;

    int callCount = 0;
    bool lastValue = false;
    sut.onIsSelectedChangeFunc = [&](auto& handler) {
        callCount++;
        lastValue = handler.IsSelected();
    };

    sut.SetIsSelected(true);
    EXPECT_EQ(1, callCount);
    EXPECT_TRUE(lastValue);

    sut.SetIsSelected(false);
    EXPECT_EQ(2, callCount);
    EXPECT_FALSE(lastValue);
}

TEST(SelectHandler, CustomCallback_NoFireOnSameValue) {
    SelectHandler sut;

    int callCount = 0;
    sut.onIsSelectedChangeFunc = [&](auto& handler) { callCount++; };

    sut.SetIsSelected(false);
    EXPECT_EQ(0, callCount);

    sut.SetIsSelected(true);
    sut.SetIsSelected(true);
    EXPECT_EQ(1, callCount);
}

TEST(SelectHandler, DefaultCallback_FiresSelectedSignalOnOwner) {
    auto node = MAKE<WorldNode>();
    auto& handler = node->AddComponent<SelectHandler>();
    auto& listener = node->AddComponent<TestSelectListener>();

    handler.SetIsSelected(true);
    EXPECT_EQ(1, listener.signalCount);
    EXPECT_TRUE(listener.lastIsSelected);

    handler.SetIsSelected(false);
    EXPECT_EQ(2, listener.signalCount);
    EXPECT_FALSE(listener.lastIsSelected);
}

TEST(SelectHandler, CustomCallback_OverridesDefaultSignal) {
    auto node = MAKE<WorldNode>();
    auto& handler = node->AddComponent<SelectHandler>();
    auto& listener = node->AddComponent<TestSelectListener>();

    handler.onIsSelectedChangeFunc = [](auto&) {};
    handler.SetIsSelected(true);

    EXPECT_TRUE(handler.IsSelected());
    EXPECT_EQ(0, listener.signalCount);
}

TEST(SelectHandler, NullCallback_NoCrashOnTransition) {
    SelectHandler sut;
    sut.onIsSelectedChangeFunc = nullptr;
    sut.SetIsSelected(true);
    EXPECT_TRUE(sut.IsSelected());
}

TEST(SelectHandler, SetIsSelectable_FalseRejectsSelectRequest) {
    SelectHandler sut;
    sut.SetIsSelectable(false);

    sut.SetIsSelected(true);
    EXPECT_FALSE(sut.IsSelected());
}

TEST(SelectHandler, SetIsSelectable_FalseStillAllowsDeselect) {
    SelectHandler sut;
    sut.SetIsSelected(true);
    EXPECT_TRUE(sut.IsSelected());

    sut.SetIsSelectable(false);
    sut.SetIsSelected(false);
    EXPECT_FALSE(sut.IsSelected());
}

TEST(SelectHandler, SetIsSelectable_RestoresAbilityToSelect) {
    SelectHandler sut;
    sut.SetIsSelectable(false);
    sut.SetIsSelected(true);
    EXPECT_FALSE(sut.IsSelected());

    sut.SetIsSelectable(true);
    sut.SetIsSelected(true);
    EXPECT_TRUE(sut.IsSelected());
}

TEST(SelectHandler, SetIsSelectable_RejectedSelectFiresNoCallback) {
    SelectHandler sut;
    sut.SetIsSelectable(false);

    int callCount = 0;
    sut.onIsSelectedChangeFunc = [&](auto&) { callCount++; };

    sut.SetIsSelected(true);
    EXPECT_EQ(0, callCount);
}
