#ifndef _SINGLETON_HPP_
#define _SINGLETON_HPP_

#include <memory>
#include <stdexcept>
template <typename TType>
class Singleton
{

private:
    static std::unique_ptr<TType> _instance;

public:
    TType* instance()
    {
        if (!_instance)
        {
            return nullptr;
        }
        return _instance.get();
    }

    template <typename... TArgs>
    void instanciate(TArgs... p_args)
    {
        if (_instance)
        {
            throw std::runtime_error("Double Instanciating Error");
        }

        _instance = std::make_unique<TType>((p_args)...);
    }
};

template <typename TType>
std::unique_ptr<TType> Singleton<TType>::_instance = nullptr;

#endif // !_SINGLETON_HPP_
