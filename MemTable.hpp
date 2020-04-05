#pragma once 

#ifndef MEMTABLE_HPP
#define MEMTABLE_HPP

#include "SkipList.hpp"

using namespace std;

#define MEM_MAX_LV 15
#define MEM_RATE 0.5

class MemTable
{
private:
    int mem_maxlv = 25;
    float mem_rate = 0.6;
    uint64_t max_size;
    SkipList<uint64_t, string> list;
    
public:
    MemTable(uint64_t size) : max_size(size){
        string str("default");
        list.init_handle(mem_maxlv, mem_rate, 0, UINT64_MAX, str);
    }

    ~MemTable(){
        MEM_FREE();
    }

    void PUT(const uint64_t key, const string &s){
        if(!IS_FULL())
            list.insert(key, s);
    }

    string GET(const uint64_t key){
        Node<uint64_t, string> *target;
        target = list.searchNode(key);
        return (target == nullptr) ? "" : target->getVal();
    }

    map<uint64_t, string> IMM_MEMTABLE(){
        return list.SELECT_TO_MAP();
    }

    bool SEARCH(const uint64_t key){
        return list.search(key);
    }

    void DELETE(const uint64_t key){
        list.remove(key);
    }

    void TRAVERSE(){
        list.traverse();
    }

    void MEM_FREE(){
        list.freeList();
    }

    void MEM_RESET(){
        list.reset();
    }

    bool IS_FULL() const {
        return list.size() == max_size;
    }

    bool IS_EMPTY() const {
        return list.size() == 0;
    }
};

#endif //MemTable_hpp