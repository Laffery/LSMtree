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
        FREE_LEVEL();
    }

public:
    void SET_LEVEL(int lv){
        level = lv;
        sst_max = pow(2, level + 1);
        tables = new SSTable[sst_max];
    }

    void SET_DIR_PATH(const string path){
        dir = path;
        _mkdir(dir.c_str());

        for(int i = 0; i < sst_max; ++i){
            string sst_dir = dir + "/sstable" + to_string(i) + ".dat";
            if(_access(sst_dir.c_str(), 0) == 0)
                sst_size++;

            tables[i].SET_DIR_PATH(sst_dir);
            uint64_t min = tables[i].GET_MIN_KEY();
            uint64_t max = tables[i].GET_MAX_KEY();
            min_key = (min_key < min) ? min_key : min;
            max_key = (max_key > max) ? max_key : max;
        }
    }

    int GET_SIZE(){
        return sst_size;
    }

    void WRITE_TO_SST(int n, MAP_DATA table){
        if(n < 0 || n >= sst_max)
            return;
        
        tables[n].WRITE_TO_DIR(table);

        uint64_t min = tables[n].GET_MIN_KEY();
        uint64_t max = tables[n].GET_MAX_KEY();
        min_key = (min_key < min) ? min_key : min;
        max_key = (max_key > max) ? max_key : max;

        sst_size++;
    }

    bool WRITE_TO_TABLES(MAP_DATA &table){
        for(int i = 0; i < sst_max; ++i){
            tables[i].WRITE_TO_DIR(table);
            sst_size++;

            uint64_t min = tables[i].GET_MIN_KEY();
            uint64_t max = tables[i].GET_MAX_KEY();
            min_key = (min_key < min) ? min_key : min;
            max_key = (max_key > max) ? max_key : max;

            if(table.empty())
                return true;
        }
        return false;
    }

    string GET(const uint64_t key, bool &flag){
        flag = false;
        if(key < min_key || key > max_key){
            return "";
        }

        // C0 has different index constucture
        if(level == 0){
            for(int i = sst_size - 1; i >= 0; --i){
                string res = tables[i].GET(key, flag);
                if(res != "")
                    return res;

                else if(flag){// key exists but val is "" :deleted
                    return "";
                }
            }
            return "";
        }

        uint64_t min, max;
        for(int i = sst_size - 1; i >= 0; --i){
            min = tables[i].GET_MIN_KEY();
            max = tables[i].GET_MAX_KEY();
            
            if(key > max)
                return "";
            else if(key < min) 
                continue;
            
            else
                return tables[i].GET(key, flag);
        }
        return "";
    }

    void SSL_TO_MAP(MAP_DATA &table){
        for(int i = sst_size - 1; i >= 0; --i){
            tables[i].KV_TO_MAP(table);
        }
        LEVEL_RESET();
    }

    void FREE_LEVEL(){
        delete []tables;
    }

    void LEVEL_RESET(){
        for(int i = 0; i < sst_size; ++i)
            tables[i].RESET();
        sst_size = 0;
        min_key = UINT64_MAX;
        max_key = 0;
    }
};

#endif // level_h