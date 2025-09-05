#include <gtest/gtest.h>

#include <string>

#include "memento.hpp"

class TestMemento : public Memento
{
private:
    int         _intValue;
    std::string _strValue;

public:
    TestMemento(int intValue = 0, const std::string& strValue = "")
        : _intValue(intValue), _strValue(strValue)
    {
    }

    int getIntValue() const
    {
        return _intValue;
    }
    std::string getStrValue() const
    {
        return _strValue;
    }

protected:
    void _saveToSnapshot(Memento::Snapshot& state) const override
    {
        state.set("intValue", _intValue);
        state.set("strValue", _strValue);
    }

    void _loadToSnapshot(Memento::Snapshot& state) const override
    {
        const_cast<int&>(_intValue)         = state.get<int>("intValue");
        const_cast<std::string&>(_strValue) = state.get<std::string>("strValue");
    }
};

TEST(Memento, SaveLoad)
{

    TestMemento       obj1(42, "Hello, World!");
    Memento::Snapshot snap = obj1.save();

    TestMemento obj2;
    obj2.load(snap);

    EXPECT_EQ(obj2.getIntValue(), 42);
    EXPECT_EQ(obj2.getStrValue(), "Hello, World!");
}

TEST(Memento, IndependentSnapshots)
{
    TestMemento       obj1(100, "First");
    Memento::Snapshot snap1 = obj1.save();

    TestMemento       obj2(200, "Second");
    Memento::Snapshot snap2 = obj2.save();

    TestMemento obj3;
    obj3.load(snap1);
    EXPECT_EQ(obj3.getIntValue(), 100);
    EXPECT_EQ(obj3.getStrValue(), "First");

    obj3.load(snap2);
    EXPECT_EQ(obj3.getIntValue(), 200);
    EXPECT_EQ(obj3.getStrValue(), "Second");
}

TEST(Memento, ModifyAfterSave)
{
    TestMemento       obj1(10, "Initial");
    Memento::Snapshot snap = obj1.save();

    // Modify obj1 after saving
    obj1 = TestMemento(20, "Modified");

    TestMemento obj2;
    obj2.load(snap);

    EXPECT_EQ(obj2.getIntValue(), 10);
    EXPECT_EQ(obj2.getStrValue(), "Initial");
}
