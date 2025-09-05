#ifndef _STATE_MACHINE_HPP
#define _STATE_MACHINE_HPP

#include <functional>
#include <map>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <utility>

template <typename TState>
class StateMachine
{
private:
    TState                                                     _current;
    std::unordered_set<TState>                                 _states;
    std::unordered_map<TState, std::function<void()>>          _action;
    std::map<std::pair<TState, TState>, std::function<void()>> _transition;

public:
    void addState(const TState& state)
    {
        if (_states.empty())
        {
            _current = state;
        }
        _states.insert(state);
    }

    void addTransition(const TState&                startState,
                       const TState&                finalState,
                       const std::function<void()>& lambda)
    {
        if (_states.find(startState) == _states.end() || _states.find(finalState) == _states.end())
        {
            throw std::runtime_error("State Not Found");
        }

        auto tr = std::make_pair(startState, finalState);
        if (_transition.find(tr) != _transition.end())
        {
            throw std::runtime_error("Transition Already Exists");
        }
        _transition[tr] = lambda;
    }

    void addAction(const TState& state, const std::function<void()>& lambda)
    {
        if (_states.find(state) == _states.end())
        {
            throw std::runtime_error("State Not Found");
        }
        if (_action.find(state) != _action.end())
        {
            throw std::runtime_error("Action Already Exists for this state");
        }
        _action[state] = lambda;
    }

    void transitionTo(const TState& state)
    {
        auto tr = std::make_pair(_current, state);

        if (_transition.find(tr) == _transition.end())
        {
            throw std::runtime_error("transition not Found");
        }
        _current = state;
        _transition[tr]();
    }

    void update()
    {

        if (_action.find(_current) == _action.end())
        {
            throw std::runtime_error("No Action for this state");
        }
        _action[_current]();
    }
};

#endif // !_STATE_MACHINE_HPP
