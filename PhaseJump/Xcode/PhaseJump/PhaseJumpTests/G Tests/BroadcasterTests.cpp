#include "gtest/gtest.h"
#include "Broadcaster.h"
#include "SomeListener.h"

using namespace PJ;
using namespace std;

namespace BroadcasterTests {
    class TestListener : public SomeListener {
    public:
        int listenCount = 0;
        String lastMessage;

        void OnListen(EventPtr event) override {
            listenCount++;
            lastMessage = event->id;
        }
    };
}

using namespace BroadcasterTests;

TEST(Broadcaster, Factory) {
    Broadcaster sut;
    auto listener = make_shared<TestListener>();
    auto listener2 = make_shared<TestListener>();
    sut.AddListener(listener);
    EXPECT_EQ(1, sut.listeners.size());
    sut.RemoveListener(listener);
    EXPECT_EQ(0, sut.listeners.size());

    sut.AddListener(listener);
    sut.AddListener(listener2);
    sut.Broadcast(make_shared<Event>("hello"));
    EXPECT_EQ("hello", listener->lastMessage);
    EXPECT_EQ("hello", listener2->lastMessage);

    sut.RemoveListener(listener);
    sut.Broadcast(make_shared<Event>("goodbye"));
    EXPECT_EQ("hello", listener->lastMessage);
    EXPECT_EQ("goodbye", listener2->lastMessage);

    sut.Clear();
    EXPECT_EQ(0, sut.listeners.size());
}
