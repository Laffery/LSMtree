#pragma once

#ifndef DISK_HPP
#define DISK_HPP

#include "SSLevel.hpp"

#define LSM_DEPTH 5

class disk
{
private:
    string dir;
    int depth = LSM_DEPTH;
    SSLevel *levels;

public:
    disk(){}

    disk(string path, int deep) : dir(path), depth(deep){
        levels = new SSLevel[depth];

        for(int i = 0; i < depth; ++i){
            // cout << dir << "C" << to_string(i) << "/\n";
            levels[i].SET_LEVEL(i);
            levels[i].SET_DIR_PATH(dir + "C" + to_string(i) + "/");
        }
    }

    ~disk(){
        delete []levels;
    }

public:

    void WRITE_TO_LEVEL(int n){
        levels[n].WRITE_TO_SST(1);
    }
};

#endif // disk_hpp