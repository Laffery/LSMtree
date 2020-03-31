#pragma once

#include "entry.hpp"

class sstable
{
private:
    uint64_t MAX_SIZE;

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
