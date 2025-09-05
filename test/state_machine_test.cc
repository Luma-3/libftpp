#include <gtest/gtest.h>

#include <iostream>
#include <string>

#include "state_machine.hpp"

TEST(StateMachineTestInit, AddState)
{
    StateMachine<std::string> fsm;
    EXPECT_NO_THROW(fsm.addState("A"));
    EXPECT_NO_THROW(fsm.addState("B"));
}

TEST(StateMachineTestInit, InitialState)
{
    StateMachine<std::string> fsm;
    fsm.addState("A");
    fsm.addState("B");
    EXPECT_NO_THROW(
        fsm.addTransition("B", "A", []() { std::cout << "Transition B->A" << std::endl; }));
    EXPECT_NO_THROW(
        fsm.addTransition("A", "B", []() { std::cout << "Transition A->B" << std::endl; }));

    testing::internal::CaptureStdout();
    EXPECT_NO_THROW(fsm.transitionTo("B"));
    EXPECT_NO_THROW(fsm.transitionTo("A"));
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Transition A->B\nTransition B->A\n");
}

TEST(StateMachineTestInit, AddAction)
{
    StateMachine<std::string> fsm;
    fsm.addState("A");
    testing::internal::CaptureStdout();
    EXPECT_NO_THROW(fsm.addAction("A", []() { std::cout << "Action A" << std::endl; }));
    EXPECT_NO_THROW(fsm.update());
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Action A\n");
}

TEST(StateMachineTestInit, DoubleAddState)
{
    StateMachine<std::string> fsm;
    fsm.addState("A");
    EXPECT_NO_THROW(fsm.addState("A"));
}

TEST(StateMachineTestInit, AddActionToNonExistentState)
{
    StateMachine<std::string> fsm;
    EXPECT_THROW(fsm.addAction("A", []() {}), std::runtime_error);
}

TEST(StateMachineTestInit, AddTransitionToNonExistentState)
{
    StateMachine<std::string> fsm;
    fsm.addState("A");
    EXPECT_THROW(fsm.addTransition("A", "B", []() {}), std::runtime_error);
    EXPECT_THROW(fsm.addTransition("B", "A", []() {}), std::runtime_error);
}

TEST(StateMachineTestInit, TransitionWithoutAction)
{
    StateMachine<std::string> fsm;
    fsm.addState("A");
    fsm.addState("B");
    EXPECT_NO_THROW(fsm.addTransition("A", "B", nullptr));
    EXPECT_NO_THROW(fsm.addTransition("B", "A", nullptr));
}

TEST(StateMachineTestInit, AddDoubleTransition)
{
    StateMachine<std::string> fsm;
    fsm.addState("A");
    fsm.addState("B");
    fsm.addTransition("A", "B", []() {});
    EXPECT_THROW(fsm.addTransition("A", "B", []() {}), std::runtime_error);
}

TEST(StateMachineTestInit, AddDoubleAction)
{
    StateMachine<std::string> fsm;
    fsm.addState("A");
    fsm.addAction("A", []() {});
    EXPECT_THROW(fsm.addAction("A", []() {}), std::runtime_error);
}
