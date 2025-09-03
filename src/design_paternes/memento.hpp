#ifndef _MEMENTO_HPP
#define _MEMENTO_HPP

#include <any>
#include <string>
#include <unordered_map>

class Memento
{
public:
    class Snapshot
    {
    private:
        std::unordered_map<std::string, std::any> _data;

    public:
        template <typename TType>
        void set(const std::string& name, TType data)
        {
            _data[name] = data;
        }

        template <typename TType>
        TType get(const std::string& name)
        {
            return std::any_cast<TType>(_data.at(name));
        }
    };

    Snapshot save();
    void     load(const Memento::Snapshot& state);

private:
    virtual void _saveToSnapshot(Memento::Snapshot& state) const = 0;
    virtual void _loadToSnapshot(Snapshot& state) const          = 0;
};

#endif // !_MEMENTO_HPP
