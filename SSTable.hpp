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
        
        MAP_INDEX index;
        uint64_t offset = 0; // keys' offsets
        uint64_t key;
        string tmpdata = "";
        MAP_DATA::iterator iter = table.begin();
        while(iter != table.end()){
            key = iter->first;
            min_key = (min_key < key) ? min_key : key;
            max_key = (max_key > key) ? max_key : key;
            
            index.insert(pair<uint64_t, uint64_t>(key, offset));

            outfile.write((char*)&key, sizeof(uint64_t));

            tmpdata = iter->second;
            offset = offset + sizeof(uint64_t) + tmpdata.length();
            
            outfile.write(tmpdata.c_str(), sizeof(char) * tmpdata.size());

            MAP_DATA::iterator tmp = iter;
            iter++;
            table.erase(tmp);

            size += (sizeof(uint64_t) * 3 + tmpdata.size());
            if(size >= max_size)
                break;
        }

        this->index_offset = offset;

        MAP_INDEX::iterator it = index.begin();
        uint64_t tmpkey, tmpoft;
        while(it != index.end()){
            tmpkey = it->first;
            tmpoft = it->second;

            outfile.write((char*)&tmpkey, sizeof(uint64_t));
            outfile.write((char*)&tmpoft, sizeof(uint64_t));

            it++;
        }
        outfile.close();
    }

    bool SEARCH(uint64_t key){
        return SEARCH_OFFSET(key) != index_offset;
    }

    uint64_t SEARCH_OFFSET(uint64_t key){
        if(key < min_key || key > max_key)
            return index_offset;
        
        ifstream infile(dir, ios::binary);
        char gps;
        for(uint64_t i = 0; i < index_offset; ++i)
            infile.read(&gps, 1);
        
        uint64_t tmpkey, tmpoft;
        while(!infile.eof()){
            infile.read((char*)&tmpkey, sizeof(uint64_t));
            infile.read((char*)&tmpoft, sizeof(uint64_t));

            if(tmpkey == key){
                infile.close();
                return tmpoft;
            }
        }
        infile.close();
        return index_offset;
    }

    string GET(uint64_t key){
        if(key < min_key || key > max_key)
            return "";
        
        ifstream infile(dir, ios::binary);
        char gps;
        for(uint64_t i = 0; i < index_offset; ++i)
            infile.read(&gps, 1);
        
        uint64_t tmpkey, offset;
        while(!infile.eof()){
            infile.read((char*)&tmpkey, sizeof(uint64_t));
            infile.read((char*)&offset, sizeof(uint64_t));

            if(tmpkey == key)
                break;
        }
        
        if(infile.eof()){
            infile.close();
            return "";
        }

        offset += sizeof(uint64_t);
        uint64_t nextoft = index_offset;
        if(key < max_key){
            infile.read((char*)&nextoft, sizeof(uint64_t));
            infile.read((char*)&nextoft, sizeof(uint64_t));
        }
        infile.close();

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

        return res;
    }

    // compaction helpers
    MAP_INDEX GET_INDEX_MAP(){
        MAP_INDEX index;
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

        return index;
    }

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

    MAP_DATA GET_KV_MAP(){
        MAP_INDEX index = GET_INDEX_MAP();

        MAP_DATA table;
        MAP_INDEX::iterator iter = index.begin();
        uint64_t offset, nextoft, key;
        
        ifstream infile(dir, ios::binary);

        char ch;
        string res = "";
        uint64_t count = 0;

        while(iter != index.end()){
            infile.read((char*)&key, sizeof(uint64_t));
            offset = sizeof(uint64_t) + iter->second;
            iter++;
            if(iter == index.end())
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
        cout << index_offset <<":" <<offset<<endl;
        while(count < index_offset - offset){
            infile.read(&ch, 1);
            res.push_back(ch);
            count++;
        }

        table.insert(pair<uint64_t, string>(key, res));

        infile.close();
        return table;
    }

    void KV_TO_MAP(MAP_DATA &table){
        MAP_INDEX index = GET_INDEX_MAP();
        MAP_INDEX::iterator iter = index.begin();
        uint64_t offset, nextoft, key;
        
        ifstream infile(dir, ios::binary);

        char ch;
        string res = "";
        uint64_t count = 0;

        while(iter != index.end()){
            infile.read((char*)&key, sizeof(uint64_t));
            offset = sizeof(uint64_t) + iter->second;
            iter++;
            if(iter == index.end())
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
        // cout << index_offset <<":" <<offset<<endl;
        while(count < index_offset - offset){
            infile.read(&ch, 1);
            res.push_back(ch);
            count++;
        }

        table.insert(pair<uint64_t, string>(key, res));

        infile.close();
    }

    void RESET(){
        min_key = UINT64_MAX;
        max_key = 0;
        index_offset = 0;
        remove(dir.c_str());
    }
};

#endif // SSTable_hpp