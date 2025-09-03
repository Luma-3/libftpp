#ifndef _OBSERVER_HPP
#define _OBSERVER_HPP

#include <any>
#include <functional>
#include <utility>
#include <vector>

class Observer
{
private:
    std::vector<std::pair<std::any, std::function<void()>*>> _subscribers;

public:
    template <typename TEvent>
    void subscribe(const TEvent& event, std::function<void()>& lambda)
    {
        auto tmp = std::make_pair(event, &lambda);
        _subscribers.push_back(tmp);
    }

    template <typename TEvent>
    void notify(const TEvent& event)
    {
        for (auto sub : _subscribers)
        {
            try
            {
                std::any_cast<TEvent>(sub.first);
            }
            catch (const std::bad_any_cast&)
            {
                continue;
            }
            if (event == std::any_cast<TEvent>(sub.first))
            {
                auto callback = sub.second;
                (*callback)();
            }
        }
    }
};

#endif // !_OBSERVER_HPP
