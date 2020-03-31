#pragma once

#include <set>
#include "sstable.h"
#include "entry.h"

class disk_level 
{
private:
    int level = 0;
    sstable *tables;

public:
    disk_level(int lv) : level(lv){
        tables = new sstable[2 * lv];
    }
    ~disk_level(){
        delete []tables;
    }
};