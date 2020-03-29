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
        table = new sstable[level];
    }
    ~disk_level(){
        for(int i = 0; i < level; ++ i)
            tables[i].freetable();
        delete []tables;
    }
}