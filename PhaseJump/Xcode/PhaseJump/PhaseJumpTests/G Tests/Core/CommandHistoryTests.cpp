#include "gtest/gtest.h"
#include "CommandHistory.h"

using namespace PJ;
using namespace std;

namespace CommandHistoryTests {
    SP<Command<int>> MakeCommand(String name) {
        return MAKE<Command<int>>(name, [](auto& command) { command.core++; }, [](auto& command) { command.core--; });
    }
}

using namespace CommandHistoryTests;

TEST(CommandHistory, BuildPastMaxSize)
{
    CommandHistory sut(2);

    EXPECT_EQ(-1, sut.ActiveCommandIndex());
    sut.Run(MakeCommand("a"));
    EXPECT_EQ(0, sut.ActiveCommandIndex());
    EXPECT_EQ(1, sut.Size());
    sut.Run(MakeCommand("b"));
    EXPECT_EQ(1, sut.ActiveCommandIndex());
    ASSERT_EQ(2, sut.Size());

    EXPECT_EQ("a", sut.At(0).name);
    EXPECT_EQ("b", sut.At(1).name);
    EXPECT_ANY_THROW(sut.At(-1));
    EXPECT_ANY_THROW(sut.At(2));

    sut.Run(MakeCommand("c"));
    EXPECT_EQ(1, sut.ActiveCommandIndex());
    ASSERT_EQ(2, sut.Size());

    EXPECT_EQ("b", sut.At(0).name);
    EXPECT_EQ("c", sut.At(1).name);
}

TEST(CommandHistory, UndoRedo)
{
    CommandHistory sut(2);

    EXPECT_EQ(-1, sut.ActiveCommandIndex());
    auto commandA = MakeCommand("a");
    auto commandB = MakeCommand("b");
    sut.Run(commandA);
    sut.Run(commandB);

    EXPECT_EQ(1, commandA->core);
    EXPECT_EQ(1, commandB->core);

    sut.Undo();
    EXPECT_EQ(0, sut.ActiveCommandIndex());
    sut.Undo();
    EXPECT_EQ(-1, sut.ActiveCommandIndex());

    EXPECT_EQ(0, commandA->core);
    EXPECT_EQ(0, commandB->core);

    sut.Redo();
    EXPECT_EQ(0, sut.ActiveCommandIndex());
    sut.Redo();
    EXPECT_EQ(1, sut.ActiveCommandIndex());

    EXPECT_EQ(1, commandA->core);
    EXPECT_EQ(1, commandB->core);

    sut.Undo();
}

TEST(CommandHistory, UndoThenRun)
{
    CommandHistory sut(2);

    EXPECT_EQ(-1, sut.ActiveCommandIndex());
    auto commandA = MakeCommand("a");
    auto commandB = MakeCommand("b");
    auto commandC = MakeCommand("c");
    sut.Run(commandA);
    sut.Run(commandB);

    EXPECT_EQ(1, commandA->core);
    EXPECT_EQ(1, commandB->core);

    sut.Undo();
    EXPECT_EQ(0, sut.ActiveCommandIndex());

    sut.Run(commandC);
    EXPECT_EQ(1, sut.ActiveCommandIndex());

    EXPECT_EQ("a", sut.At(0).name);
    EXPECT_EQ("c", sut.At(1).name);

    EXPECT_EQ(1, commandA->core);
    EXPECT_EQ(0, commandB->core);
    EXPECT_EQ(1, commandC->core);
}

TEST(CommandHistory, Helpers)
{
    CommandHistory sut(2);

    EXPECT_EQ(-1, sut.ActiveCommandIndex());
    auto commandA = MakeCommand("a");
    auto commandB = MakeCommand("b");
    auto commandC = MakeCommand("c");

    EXPECT_FALSE(sut.CanUndo());
    EXPECT_FALSE(sut.CanRedo());

    sut.Run(commandA);
    EXPECT_EQ("a", sut.UndoCommandName());
    EXPECT_TRUE(sut.CanUndo());
    EXPECT_FALSE(sut.CanRedo());

    sut.Run(commandB);
    EXPECT_EQ("b", sut.UndoCommandName());
    EXPECT_TRUE(sut.CanUndo());
    EXPECT_FALSE(sut.CanRedo());
    EXPECT_EQ("", sut.RedoCommandName());

    sut.Undo();
    EXPECT_EQ("a", sut.UndoCommandName());
    EXPECT_EQ("b", sut.RedoCommandName());

    sut.Run(commandC);
    EXPECT_EQ(1, sut.ActiveCommandIndex());
    EXPECT_EQ("c", sut.UndoCommandName());
    EXPECT_EQ("", sut.RedoCommandName());
}
