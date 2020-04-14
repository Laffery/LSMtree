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
        _mkdir(dir.c_str());
        for(int i = 0; i < depest; ++i){
            levels[i].SET_LEVEL(i);
            levels[i].SET_DIR_PATH(dir + "C" + to_string(i) + "/");
            if(!levels[i].GET_SIZE())
                depth++;
        }
    }

    int GET_DEPTH(){
        return depth;
    }

    int GET_LEVEL_SST(int n){
        return levels[n].GET_SIZE();
    }

    void WRITE_TO_SSL(int n, MAP_DATA &table){
        levels[n].WRITE_TO_SST(GET_LEVEL_SST(n), table);
        if(!depth)
            depth++;
    }

    void WRITE_TO_LV0(MAP_DATA &table){
        levels[0].WRITE_TO_SST(GET_LEVEL_SST(0), table);
        if(!depth)
            depth = 1;
    }

    void WRITE_TO_LEVELS(MAP_DATA &table){
        int i = 0;
        
        while(i < depth){
            LV_TO_MAP(i, table);
            if(levels[i].WRITE_TO_TABLES(table))
                return;
            ++i;
        }
        
        depth++;
        bool flag = levels[i].WRITE_TO_TABLES(table);
    }

    string GET(uint64_t key, bool &flag){
        flag = false;
        for(int i = 0; i < depth; ++i){
            string res = levels[i].GET(key, flag);
            
            if(res != "")
                return res;

            if(flag)
                return "";
        }
        
        return "";
    }

    void LV_TO_MAP(int n, MAP_DATA &table){
        levels[n].SSL_TO_MAP(table);
    }

    void FREE_DISK(){
        delete []levels;
    }

    void DISK_RESET(){
        for(int i = 0; i < depest; ++i)
            levels[i].LEVEL_RESET();
    }
};

#endif // disk_hpp