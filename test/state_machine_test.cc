#include <gtest/gtest.h>

#include <string>

#include "state_machine.hpp"

class StateMachineTest : public ::testing::Test
{
protected:
    StateMachine<int> fsm;

    void SetUp() override
    {
        fsm.addState(0);
        fsm.addState(1);
        fsm.addState(2);

        fsm.addAction(0, []() { std::cout << "In State 0" << std::endl; });
        fsm.addAction(1, []() { std::cout << "In State 1" << std::endl; });
        fsm.addAction(2, []() { std::cout << "In State 2" << std::endl; });

        fsm.addTransition(0, 1, []() { std::cout << "Transitioning from 0 to 1" << std::endl; });
        fsm.addTransition(1, 2, []() { std::cout << "Transitioning from 1 to 2" << std::endl; });
        fsm.addTransition(2, 0, []() { std::cout << "Transitioning from 2 to 0" << std::endl; });
    }
};

TEST_F(StateMachineTest, InitialState)
{
    EXPECT_NO_THROW(fsm.update());
}

TEST_F(StateMachineTest, ValidTransition)
{
    EXPECT_NO_THROW(fsm.transitionTo(1));
    EXPECT_NO_THROW(fsm.update());
    EXPECT_NO_THROW(fsm.transitionTo(2));
    EXPECT_NO_THROW(fsm.update());
    EXPECT_NO_THROW(fsm.transitionTo(0));
    EXPECT_NO_THROW(fsm.update());
}

TEST_F(StateMachineTest, InvalidTransition)
{
    EXPECT_NO_THROW(fsm.update());
    EXPECT_THROW(fsm.transitionTo(2), std::runtime_error);
}

TEST_F(StateMachineTest, InvalidStateAction)
{
    StateMachine<int> fsm_invalid;
    fsm_invalid.addState(0);
    EXPECT_THROW(fsm_invalid.addAction(1, []() {}), std::runtime_error);
}
