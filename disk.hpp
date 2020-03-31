#pragma once

#include "disk_lv.h"

#define LSM_DEPTH 5

class disk
{
private:
    disk_level **level;

public:
    disk(){
        **level = new disk_level*[LSM_DEPTH];

        for(int i = 0; i < LSM_DEPTH; ++i)
            level[i] = new disk_level(i+1);
    }
    ~disk(){
        for(int i = 0; i < LSM_DEPTH; ++i)
            delete []level[i];
        delete []level;
    }
};