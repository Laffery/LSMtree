#pragma once

#include <set>
#include "entry.h"

class sstable
{
private:
    uint64_t MAX_SIZE;
    set<entry<uint64_t, uint64_t>> index;

public:
    sstable(uint64_t size);
    ~sstable();
};

sstable::sstable(uint64_t size) : MAX_SIZE(size)
{
    
}

sstable::~sstable()
{
}
