#include "parser/StateMachine.h"

#include "test-platform/GoogleTest.h"

#include "utility/Console.h"
#include "parser/Token.h"

using namespace parser;

using State = int;
using Stimulus = int;
using Data = int;
enum Terminal
{
    A,
    B,
};
using Input = TokenType<Terminal>;

class StateMachineTest
    : public ::testing::Test
{
public:
    static utility::Console console;
    static const State state1;
    static const State state2;
    static const State state3;

    StateMachineTest()
    {
        SetupTokenDefinitions<Terminal>({
            { Terminal::A, "A" },
            { Terminal::B, "B" },
        });
    }

    static bool TransitionAction(State from, State to, Stimulus input, Data data)
    {
        console << "From " << from << " to " << to << " input " << input << " data " << data << std::endl;
        return true;
    }
    static State HandleInputAction(State from, Input input, Data data)
    {
        State newState = from;
        console << "From " << from << " input " << input << " data " << data;
        if (from == state1)
        {
            switch (input.m_type)
            {
            case Terminal::A:
                newState = state1;
                break;
            case Terminal::B:
                newState = state2;
                break;
            }
        }
        else if (from == state2)
        {
            switch (input.m_type)
            {
            case Terminal::A:
                newState = state1;
                break;
            case Terminal::B:
                newState = state3;
                break;
            }
        }
        else if (from == state3)
        {
            switch (input.m_type)
            {
            case Terminal::A:
                newState = state2;
                break;
            case Terminal::B:
                newState = state3;
                break;
            }
        }
        
        console << "-> " << newState << std::endl;
        return newState;
    }
};

utility::Console StateMachineTest::console{};
const State StateMachineTest::state1 = 0;
const State StateMachineTest::state2 = 1;
const State StateMachineTest::state3 = 2;

TEST_F(StateMachineTest, Construct)
{
    StateMachineRules<State, Stimulus, Data> rules{};
    StateMachine<State, Stimulus, Data> stateMachine;
    stateMachine.SetupRules(rules);

    EXPECT_EQ(state1, stateMachine.GetState());
}

TEST_F(StateMachineTest, ConstructWithRules)
{
    StateMachineRules<State, Stimulus, Data> rules{
        StateMachineRule<State, Stimulus, Data>::Transition(state1, state1, 0, TransitionAction)
    };
    StateMachine<State, Stimulus, Data> stateMachine;
    stateMachine.SetupRules(rules);

    EXPECT_EQ(state1, stateMachine.GetState());
}

TEST_F(StateMachineTest, SetState)
{
    StateMachineRules<State, Stimulus, Data> rules{};
    StateMachine<State, Stimulus, Data> stateMachine;
    stateMachine.SetupRules(rules);
    stateMachine.SetState(state2);

    EXPECT_EQ(state2, stateMachine.GetState());
}

TEST_F(StateMachineTest, HandleInput)
{
    StateMachineRules<State, Stimulus, Data> rules{
        StateMachineRule<State, Stimulus, Data>::Transition(state1, state1, 0, TransitionAction),
        StateMachineRule<State, Stimulus, Data>::Transition(state1, state2, 1, TransitionAction),
        StateMachineRule<State, Stimulus, Data>::Transition(state2, state1, 0, TransitionAction),
        StateMachineRule<State, Stimulus, Data>::Transition(state2, state3, 1, TransitionAction),
        StateMachineRule<State, Stimulus, Data>::Transition(state3, state2, 0, TransitionAction),
        StateMachineRule<State, Stimulus, Data>::Transition(state3, state3, 1, TransitionAction),
    };
    StateMachine<State, Stimulus, Data> stateMachine;
    stateMachine.SetupRules(rules);
    stateMachine.SetState(state1);
    EXPECT_TRUE(stateMachine.HandleInput(0, 0));
    EXPECT_EQ(state1, stateMachine.GetState());

    EXPECT_TRUE(stateMachine.HandleInput(1, 1));
    EXPECT_EQ(state2, stateMachine.GetState());

    EXPECT_TRUE(stateMachine.HandleInput(0, 0));
    EXPECT_EQ(state1, stateMachine.GetState());

    EXPECT_TRUE(stateMachine.HandleInput(1, 1));
    EXPECT_EQ(state2, stateMachine.GetState());

    EXPECT_TRUE(stateMachine.HandleInput(1, 1));
    EXPECT_EQ(state3, stateMachine.GetState());

    EXPECT_TRUE(stateMachine.HandleInput(1, 1));
    EXPECT_EQ(state3, stateMachine.GetState());

    EXPECT_TRUE(stateMachine.HandleInput(0, 0));
    EXPECT_EQ(state2, stateMachine.GetState());

    EXPECT_TRUE(stateMachine.HandleInput(0, 0));
    EXPECT_EQ(state1, stateMachine.GetState());
}

TEST_F(StateMachineTest, HandleInputParser)
{
    StateMachineRules<State, Input, Data> rules
    {
        StateMachineRule<State, Input, Data>::HandleInput(state1, Input(Terminal::A), HandleInputAction),
        StateMachineRule<State, Input, Data>::HandleInput(state1, Input(Terminal::B), HandleInputAction),
        StateMachineRule<State, Input, Data>::HandleInput(state2, Input(Terminal::A), HandleInputAction),
        StateMachineRule<State, Input, Data>::HandleInput(state2, Input(Terminal::B), HandleInputAction),
        StateMachineRule<State, Input, Data>::HandleInput(state3, { Input(Terminal::A), Input(Terminal::B) }, HandleInputAction),
    };
    StateMachine<State, Input, Data> stateMachine;
    stateMachine.SetupRules(rules);
    stateMachine.SetState(state1);
    EXPECT_TRUE(stateMachine.HandleInput(Terminal::A, 0));
    EXPECT_EQ(state1, stateMachine.GetState());

    EXPECT_TRUE(stateMachine.HandleInput(Terminal::B, 1));
    EXPECT_EQ(state2, stateMachine.GetState());

    EXPECT_TRUE(stateMachine.HandleInput(Terminal::A, 0));
    EXPECT_EQ(state1, stateMachine.GetState());

    EXPECT_TRUE(stateMachine.HandleInput(Terminal::B, 1));
    EXPECT_EQ(state2, stateMachine.GetState());

    EXPECT_TRUE(stateMachine.HandleInput(Terminal::B, 1));
    EXPECT_EQ(state3, stateMachine.GetState());

    EXPECT_TRUE(stateMachine.HandleInput(Terminal::B, 1));
    EXPECT_EQ(state3, stateMachine.GetState());

    EXPECT_TRUE(stateMachine.HandleInput(Terminal::A, 0));
    EXPECT_EQ(state2, stateMachine.GetState());

    EXPECT_TRUE(stateMachine.HandleInput(Terminal::A, 0));
    EXPECT_EQ(state1, stateMachine.GetState());
}

