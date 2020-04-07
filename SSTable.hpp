#pragma once

#ifndef SSTABLE_HPP
#define SSTABLE_HPP

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include "entry.hpp"
#include <set>

using namespace std;

#define DEFAULT_MAX_SIZE 1024 * 1024 * 2 // 2 MB
typedef map<uint64_t, string> MAP_DATA;
typedef map<uint64_t, uint64_t> MAP_INDEX;

class SSTable
{
private:
    string dir;
    uint64_t size = 0;
    uint64_t max_size = DEFAULT_MAX_SIZE;
    uint64_t min_key  = UINT64_MAX;
    uint64_t max_key  = 0;
    uint64_t index_offset = 0;
    MAP_INDEX imap;

public:
    SSTable(){}
    SSTable(string path){SET_DIR_PATH(path);}
    ~SSTable(){}

public:
    void SET_MAX_SIZE(uint64_t msize){
        this->max_size = msize;
    }

    void SET_DIR_PATH(const string path){
        this->dir = path;
    }

    uint64_t GET_MIN_KEY(){
        return min_key;
    }

    uint64_t GET_MAX_KEY(){
        return max_key;
    }

    uint64_t GET_SIZE(){
        return size;
    }

    uint64_t GET_MAX_SIZE(){
        return max_size;
    }

    void WRITE_TO_DIR(MAP_DATA &table){
        size = 0;
        ofstream outfile(dir, ios::binary);
        if(!outfile)
            return;
        
        // MAP_INDEX index;
        uint64_t offset = 0; // keys' offsets
        uint64_t key;
        string tmpdata = "";
        MAP_DATA::iterator iter = table.begin();
        while(iter != table.end()){
            key = iter->first;
            min_key = (min_key < key) ? min_key : key;
            max_key = (max_key > key) ? max_key : key;
            
            imap.insert(pair<uint64_t, uint64_t>(key, offset));

            outfile.write((char*)&key, sizeof(uint64_t));

            tmpdata = iter->second;
            offset = offset + sizeof(uint64_t) + tmpdata.length();
            
            outfile.write(tmpdata.c_str(), sizeof(char) * tmpdata.size());

            MAP_DATA::iterator tmp = iter;
            iter++;
            table.erase(tmp);

            size += (sizeof(char) * tmpdata.size());
            if(size >= max_size)
                break;
        }

        this->index_offset = offset;

        MAP_INDEX::iterator it = imap.begin();
        uint64_t tmpkey, tmpoft;
        while(it != imap.end()){
            tmpkey = it->first;
            tmpoft = it->second;

            outfile.write((char*)&tmpkey, sizeof(uint64_t));
            outfile.write((char*)&tmpoft, sizeof(uint64_t));

            it++;
        }
        outfile.close();
    }

    string GET(uint64_t key, bool &flag){
        flag = false;
        if(key < min_key || key > max_key)
            return "";
        
        uint64_t tmpkey, offset;
        
        MAP_INDEX::iterator iter = imap.begin();
        while (iter != imap.end())
        {
            if(iter->first == key)
                break;
            iter++;
        }
        if(iter == imap.end())
            return "";
        
        offset = iter->second;
        offset += sizeof(uint64_t);

        iter++;
        uint64_t nextoft = (iter == imap.end()) ? index_offset : iter->second;

        if(nextoft == offset){
            flag = true;
            return "";
        }

        ifstream kfile(dir, ios::binary);
        
        char ch;
        string res = "";
        uint64_t count = 0;
        while(count < nextoft){
            kfile.read(&ch, 1);
            if(count >= offset)
                res.push_back(ch);
            
            count++;
        }
        kfile.close();

        flag = true;
        return res;
    }

    // compaction helpers
    void INDEX_TO_MAP(MAP_INDEX &index){
        ifstream infile(dir, ios::binary);
        char gps;
        for(uint64_t i = 0; i < index_offset; ++i)
            infile.read(&gps, 1);
        
        uint64_t tmpkey, tmpoft;
        while(!infile.eof()){
            infile.read((char*)&tmpkey, sizeof(uint64_t));
            infile.read((char*)&tmpoft, sizeof(uint64_t));
            index.insert(pair<uint64_t, uint64_t>(tmpkey, tmpoft));
        }
        infile.close();
    }

    void KV_TO_MAP(MAP_DATA &table){
        MAP_INDEX::iterator iter = imap.begin();
        uint64_t offset, nextoft, key;
        
        ifstream infile(dir, ios::binary);

        char ch;
        string res = "";
        uint64_t count = 0;

        while(iter != imap.end()){
            infile.read((char*)&key, sizeof(uint64_t));
            offset = sizeof(uint64_t) + iter->second;
            iter++;
            if(iter == imap.end())
                break;
            
            nextoft = iter->second;
            
            while(count < nextoft - offset){
                infile.read(&ch, 1);
                res.push_back(ch);
                count++;
            }

            table.insert(pair<uint64_t, string>(key, res));
            
            res = "";
            count = 0;
        }

        // last kv-pair
        while(count < index_offset - offset){
            infile.read(&ch, 1);
            res.push_back(ch);
            count++;
        }

        table.insert(pair<uint64_t, string>(key, res));

        infile.close();

        RESET();
    }

    void RESET(){
        min_key = UINT64_MAX;
        max_key = 0;
        index_offset = 0;
        imap.clear();
        remove(dir.c_str());
    }
};

#endif // SSTable_hpp