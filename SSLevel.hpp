#pragma once

#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include "SSTable.hpp"
#include "entry.hpp"

using namespace std;

class SSLevel 
{
private:
    string dir;
    int level = 0;
    int sstables = 2;
    SSTable *tables;

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
        sstables = 2 * (level + 1);
        tables = new SSTable[sstables];
    }

    void SET_DIR_PATH(const string path){
        dir = path;
        for(int i = 0; i < sstables; ++i){
            tables[i].SET_DIR_PATH(dir + "sstable" + to_string(i) + ".txt");
            // cout <<dir << "sstable" << to_string(i) << ".txt\n";
        }
    }

    void WRITE_TO_SST(int n){
        map<uint64_t, string> mapStudent;  
  
        for(uint64_t i = 0; i < 1002; ++i){
            mapStudent.insert(pair<uint64_t, string>(i, "student_"+std::to_string(i))); 
        }
  
        tables[n].WRITE_TO_DIR(mapStudent);

            cout << tables[n].GET_SCALE_MAX()<<endl;
    }

    void LEVEL_RESET(){
        for(int i = 0; i < sstables; ++i)
            tables[i].RESET();
    }
};

#endif // level_h