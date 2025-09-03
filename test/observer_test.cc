#include <gtest/gtest.h>

#include <string>

#include "observer.hpp"

class ObserverTest : public ::testing::Test
{
protected:
    Observer observer;
};

TEST_F(ObserverTest, SubscribeAndNotify)
{
    bool                  notified = false;
    int                   event    = 1;
    std::function<void()> callback = [&notified]() { notified = true; };

    observer.subscribe(1, callback);
    observer.notify(1);

    EXPECT_TRUE(notified);
}

TEST_F(ObserverTest, NoNotificationForDifferentEvent)
{
    bool                  notified = false;
    int                   event    = 1;
    std::function<void()> callback = [&notified]() { notified = true; };

    observer.subscribe(1, callback);
    observer.notify(2);

    EXPECT_FALSE(notified);
}

TEST_F(ObserverTest, MultipleSubscribers)
{
    int                   notifyCount1 = 0;
    int                   notifyCount2 = 0;
    std::function<void()> callback1    = [&notifyCount1]() { notifyCount1++; };
    std::function<void()> callback2    = [&notifyCount2]() { notifyCount2++; };

    observer.subscribe(1, callback1);
    observer.subscribe(1, callback2);
    observer.notify(1);

    EXPECT_EQ(notifyCount1, 1);
    EXPECT_EQ(notifyCount2, 1);
}

TEST_F(ObserverTest, SubscribeDifferentEventTypes)
{
    bool                  notifiedInt    = false;
    bool                  notifiedString = false;
    std::function<void()> callbackInt    = [&notifiedInt]() { notifiedInt = true; };
    std::function<void()> callbackString = [&notifiedString]() { notifiedString = true; };

    observer.subscribe(1, callbackInt);
    observer.subscribe(std::string("event"), callbackString);

    observer.notify(1);
    EXPECT_TRUE(notifiedInt);
    EXPECT_FALSE(notifiedString);

    notifiedInt = false; // Reset for next test
    observer.notify(std::string("event"));
    EXPECT_FALSE(notifiedInt);
    EXPECT_TRUE(notifiedString);
}

TEST_F(ObserverTest, NoSubscribers)
{
    // Should not crash or throw
    observer.notify(1);
    SUCCEED();
}
