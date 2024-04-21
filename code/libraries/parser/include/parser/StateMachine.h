#pragma once

#include <functional>
#include <map>
#include <set>
#include <vector>

namespace parser {

template<class State, class Stimulus, class Data>
using StateTransitionAction = std::function<bool(State from, State to, Stimulus input, Data data)>;

template<class State, class Stimulus, class Data>
using StateInputAction = std::function<State(State from, Stimulus input, Data data)>;

template<class State, class Stimulus, class Data>
class StateMachineRule
{
public:
    using StimulusSet = std::set<Stimulus>;
    State from;
    State to;
    StimulusSet stimuli;
    StateTransitionAction<State, Stimulus, Data> transitionAction;
    StateInputAction<State, Stimulus, Data> handleInputAction;

    static StateMachineRule Transition(State from, State to, Stimulus input, StateTransitionAction<State, Stimulus, Data> action)
    {
        auto result = StateMachineRule(from, to, { input });
        result.transitionAction = action;
        return result;
    }
    static StateMachineRule Transition(State from, State to, StimulusSet input, StateTransitionAction<State, Stimulus, Data> action)
    {
        auto result = StateMachineRule(from, to, input);
        result.transitionAction = action;
        return result;
    }
    static StateMachineRule HandleInput(State from, Stimulus input, StateInputAction<State, Stimulus, Data> action)
    {
        auto result = StateMachineRule(from, from, { input });
        result.handleInputAction = action;
        return result;
    }
    static StateMachineRule HandleInput(State from, StimulusSet input, StateInputAction<State, Stimulus, Data> action)
    {
        auto result = StateMachineRule(from, from, input);
        result.handleInputAction = action;
        return result;
    }
    StateMachineRule(State from, State to, StimulusSet input)
        : from{ from }
        , to{ to }
        , stimuli{ input }
        , transitionAction{}
        , handleInputAction{}
    {
    }

    bool RunAction(Stimulus input, State& newState, Data data)
    {
        bool result = true;
        newState = to;
        if (handleInputAction)
            newState = handleInputAction(from, input, data);
        if (transitionAction)
            result = transitionAction(from, newState, input, data);
        return result;
    }
};

template<class State, class Stimulus>
using StateStimulusPair = std::pair<State, Stimulus>;

template<class State, class Stimulus, class Data>
using StateMachineRules = std::vector<StateMachineRule<State, Stimulus, Data>>;

template<class State, class Stimulus, class Data>
using HandleInputCallback = std::function<void(State from, State to, Stimulus inputs, Data data)>;

template<class State, class Stimulus, class Data>
class StateMachine
{
private:
	StateMachineRules<State, Stimulus, Data> m_rules;
	State m_state;
    std::map<StateStimulusPair<State, Stimulus>, StateMachineRule<State, Stimulus, Data>> m_lookupMap;
    HandleInputCallback<State, Stimulus, Data> m_handleInputCallback;

public:
	StateMachine();

    void SetCallback(HandleInputCallback<State, Stimulus, Data> callback);
    void SetupRules(const StateMachineRules<State, Stimulus, Data>& rules);
	State GetState() const;
	void SetState(State state);
	bool HandleInput(Stimulus input, Data data);
};

template<class State, class Stimulus, class Data>
StateMachine<State, Stimulus, Data>::StateMachine()
    : m_rules{}
    , m_state{}
    , m_lookupMap{}
{
}

template<class State, class Stimulus, class Data>
void StateMachine<State, Stimulus, Data>::SetupRules(const StateMachineRules<State, Stimulus, Data>& rules)
{
    m_rules.clear();
    m_lookupMap.clear();
    m_state = {};
    for (auto const& rule : rules)
    {
        m_rules.push_back(rule);
        for (auto const& stimulus : rule.stimuli)
        {
            m_lookupMap.emplace(StateStimulusPair<State, Stimulus>(rule.from, stimulus), rule);
        }
    }
}

template<class State, class Stimulus, class Data>
State StateMachine<State, Stimulus, Data>::GetState() const
{
    return m_state;
}

template<class State, class Stimulus, class Data>
void StateMachine<State, Stimulus, Data>::SetState(State state)
{
    m_state = state;
}

template<class State, class Stimulus, class Data>
void StateMachine<State, Stimulus, Data>::SetCallback(HandleInputCallback<State, Stimulus, Data> callback)
{
    m_handleInputCallback = callback;
}

template<class State, class Stimulus, class Data>
bool StateMachine<State, Stimulus, Data>::HandleInput(Stimulus input, Data data)
{
    auto it = m_lookupMap.find(StateStimulusPair<State, Stimulus>{m_state, input});
    if (it == m_lookupMap.end())
    {
        return false;
    }
    bool result = it->second.RunAction(input, m_state, data);
    if (m_handleInputCallback)
        m_handleInputCallback(it->second.from, m_state, input, data);
    return result;
}

} // namespace parser
