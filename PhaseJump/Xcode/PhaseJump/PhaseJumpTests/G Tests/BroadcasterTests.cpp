#include "gtest/gtest.h"
#include "Broadcaster.h"
#include "SomeSignal.h"
#include "StandardEventCore.h"

using namespace PJ;
using namespace std;

namespace BroadcasterTests {
    class TestListener : public SomeListener {
    public:
        int listenCount = 0;
        String lastMessage;

        void OnSignal(SomeSignal& signal) override {
            listenCount++;

            auto standardEvent = As<Event<StandardEventCore>>(&signal);
            GUARD(standardEvent);
            lastMessage = standardEvent->core.id;
        }
    };
}

using namespace BroadcasterTests;

TEST(Broadcaster, Test) {
    Broadcaster sut;
    auto listener = MAKE<TestListener>();
    auto listener2 = MAKE<TestListener>();
    sut.Add(listener);
    EXPECT_EQ(1, sut.Count());
    sut.Remove(listener);
    EXPECT_EQ(0, sut.Count());

    sut.Add(listener);
    sut.Add(listener2);
    Event<StandardEventCore> event({"hello"});
    sut.Broadcast(event);
    EXPECT_EQ("hello", listener->lastMessage);
    EXPECT_EQ("hello", listener2->lastMessage);

    sut.Remove(listener);
    Event<StandardEventCore> event2({"goodbye"});
    sut.Broadcast(event2);
    EXPECT_EQ("hello", listener->lastMessage);
    EXPECT_EQ("goodbye", listener2->lastMessage);

    sut.Clear();
    EXPECT_EQ(0, sut.Count());
}
