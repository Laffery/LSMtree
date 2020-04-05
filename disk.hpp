#pragma once

#ifndef DISK_HPP
#define DISK_HPP

#include "SSLevel.hpp"

class disk
{
private:
    string dir;
    int depth = 0;
    int depest = 5;
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
        depest = deep;
        levels = new SSLevel[depest];
    }

    void SET_DIR_PATH(const string path){
        dir = path;
        for(int i = 0; i < depest; ++i){
            levels[i].SET_LEVEL(i);
            levels[i].SET_DIR_PATH(dir + "/C" + to_string(i) + "/");
        }
    }

    int GET_DEPTH(){
        return depth;
    }

    int GET_LEVEL_SST(int n){
        return levels[n].GET_SIZE();
    }

    void WRITE_TO_LEVEL(int n){
        // levels[n].WRITE_TO_SST(1);
    }

    string GET(uint64_t key){
        for(int i = 0; i < depth; ++i){
            string res = levels[i].GET(key);
            if(res != "")
                return res;
        }
        
        return "";
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