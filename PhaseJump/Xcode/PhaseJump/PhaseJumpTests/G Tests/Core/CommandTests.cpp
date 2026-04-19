#include "gtest/gtest.h"
#include "Command.h"

using namespace PJ;
using namespace std;

namespace CommandTests {
}

using namespace CommandTests;

TEST(Command, Test)
{
    Command<int> sut(Command<int>::Config{.runFunc = [](auto& command) { command.core++; }, .undoFunc = [](auto& command) { command.core--; }});
    sut.core = 0;

    EXPECT_EQ(SomeCommand::StateType::Default, sut.State());
    sut.Run();
    EXPECT_EQ(SomeCommand::StateType::Complete, sut.State());
    EXPECT_EQ(1, sut.core);
    sut.Run();
    EXPECT_EQ(SomeCommand::StateType::Complete, sut.State());
    EXPECT_EQ(1, sut.core);
    
    sut.Undo();
    EXPECT_EQ(SomeCommand::StateType::Reversed, sut.State());
    EXPECT_EQ(0, sut.core);
    sut.Undo();
    EXPECT_EQ(SomeCommand::StateType::Reversed, sut.State());
    EXPECT_EQ(0, sut.core);
}
