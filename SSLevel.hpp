#pragma once

#ifndef LEVEL_H
#define LEVEL_H

#include <direct.h>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <cmath>
#include "SSTable.hpp"

using namespace std;

class SSLevel 
{
private:
    string dir;
    int level = 0;
    int sst_size = 0;
    int sst_max = 0;
    uint64_t sst_size_max = 1024;
    SSTable *tables;
    uint64_t min_key = UINT64_MAX;
    uint16_t max_key = 0;                 

public:

    SSLevel(){}

    SSLevel(const string path, int lv){
        SET_LEVEL(lv);
        SET_DIR_PATH(path);
    }

    ~SSLevel(){
        delete []tables;
    }

public:
    void SET_LEVEL(int lv){
        level = lv;
        sst_max = pow(2, level + 1);
        sst_size_max *= sst_max;
        tables = new SSTable[sst_max];
    }

    void SET_DIR_PATH(const string path){
        dir = path;
        _mkdir(dir.c_str());

        for(int i = 0; i < sst_max; ++i){
            tables[i].SET_DIR_PATH(dir + "/sstable" + to_string(i) + ".dat");
            cout <<dir << "sstable" << to_string(i) << ".txt\n";
        }
    }

    int GET_SIZE(){
        return sst_size;
    }

    uint64_t GET_SST_SIZE(){
        return sst_size_max;
    }

    void WRITE_TO_SST(int n, MAP_DATA table){
        if(n < 0 || n > sst_max)
            return;
        
        tables[n].WRITE_TO_DIR(table);

        uint64_t min = tables[n].GET_MIN_KEY();
        uint64_t max = tables[n].GET_MAX_KEY();
        min_key = (min_key < min) ? min_key : min;
        max_key = (max_key > max) ? max_key : max;

        sst_size++;
    }

    string GET(const uint64_t key){
        if(key < min_key || key > max_key)
            return "";

        uint64_t min, max;
        for(int i = sst_size - 1; i >= 0; --i){
            min = tables[i].GET_MIN_KEY();
            max = tables[i].GET_MAX_KEY();

            if(key > max)
                return "";
            else if(key < min) 
                continue;
            
            else
                return tables[i].GET(key);
        }

        return "";
    }

    void LEVEL_RESET(){
        for(int i = 0; i < sst_size; ++i)
            tables[i].RESET();
    }
};

#endif // level_h