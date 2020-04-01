#pragma once

#ifndef SSTABLE_HPP
#define SSTABLE_HPP

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include "entry.hpp"

using namespace std;

#define DEFAULT_MAX_SIZE 1024
typedef map<uint64_t, string> MAP_DATA;
typedef map<uint64_t, uint64_t> MAP_INDEX;

class SSTable
{
private:
    string dir;
    uint64_t max_size = DEFAULT_MAX_SIZE;
    uint64_t min_key  = UINT64_MAX;
    uint64_t max_key  = 0;
    MAP_INDEX index;
    uint64_t max_offset = 0;

public:
    SSTable(){}
    SSTable(string path){SET_DIR_PATH(path);}
    ~SSTable(){}

public:
    void SET_MAX_SIZE(uint64_t size){
        this->max_size = size;
    }

    void SET_DIR_PATH(const string path){
        this->dir = path;
    }

    uint64_t GET_SCALE_MIN(){
        return min_key;
    }

    uint64_t GET_SCALE_MAX(){
        return max_key;
    }

    void WRITE_TO_DIR(MAP_DATA table){
        ofstream outfile(dir);
        if(!outfile){
            cout << "Failed to write to file" << dir << endl;
            return;
        }

        uint64_t offset = 0;
        uint64_t key;
        string tmp = "";
        MAP_DATA::iterator iter = table.begin();
        while(iter != table.end()){
            index.insert(pair<uint64_t, uint64_t>(iter->first, offset));
            
            key = iter->first;
            tmp = std::to_string(key) + "," + iter->second;
            offset += tmp.length();
            
            outfile <<tmp;

            min_key = (min_key < key) ? min_key : key;
            max_key = (max_key > key) ? max_key : key;

            iter++;
        }

        iter--;
        key = iter->first;
        tmp = std::to_string(key) + "," + iter->second;
        max_offset = offset;
        outfile.close();
    }

    void SHOW_INDEX(){
        MAP_INDEX::iterator iter = index.begin();
        while(iter != index.end()){
            cout <<iter->first << ":"<<iter->second<<endl;
            iter++;
        }
    }

    bool SEARCH(uint64_t key){
        MAP_INDEX::iterator iter = index.find(key);
        return (iter != index.end());
    }

    string GET(uint64_t key){
        MAP_INDEX::iterator iter = index.find(key);
        if(iter == index.end())
            return "";
        
        uint64_t offset = iter->second + std::to_string(key).length() + 1;
        iter++;
        uint64_t next = (iter != index.end()) ? iter->second : max_offset;

        ifstream infile(dir);
        if(!infile){
            cout << "Failed to open file " << dir <<endl;
            return "";
        }

        char* helper;
        helper = new char[next - offset];
        uint64_t count = 0;
        while(count < next){
            char ch;
            infile.read(&ch, 1);
            if(count >= offset){
                helper[count - offset] = ch;
            }
            count++;
        }

        string res = helper;
        infile.close();
        return res;
    }
};

#endif // SSTable_hpp