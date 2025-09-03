#include "memento.hpp"

Memento::Snapshot Memento::save()
{
    Memento::Snapshot snap;
    _saveToSnapshot(snap);
    return snap;
}

void Memento::load(const Memento::Snapshot& state)
{
    Memento::Snapshot cpy = state;
    _loadToSnapshot(cpy);
}
