#pragma once

#ifndef DISK_HPP
#define DISK_HPP

#include "SSLevel.hpp"

class disk
{
private:
    string dir;
    int depth = 5;
    SSLevel *levels;

public:
    disk(){}

    disk(string path, int deep){
        SET_DEPTH(deep);
        SET_DIR_PATH(path);
    }

    ~disk(){
        FREE_DISK();
    }

public:

    void SET_DEPTH(int deep){
        depth = deep;
        levels = new SSLevel[depth];
    }

    void SET_DIR_PATH(const string path){
        dir = path;
        for(int i = 0; i < depth; ++i){
            levels[i].SET_LEVEL(i);
            levels[i].SET_DIR_PATH(dir + "C" + to_string(i) + "/");
        }
    }

    void WRITE_TO_LEVEL(int n){
        levels[n].WRITE_TO_SST(1);
    }

    void FREE_DISK(){
        delete []levels;
    }

    void DISK_RESET(){
        for(int i = 0; i < depth; ++i){
            levels[i].LEVEL_RESET();
        }
    }
};

#endif // disk_hpp