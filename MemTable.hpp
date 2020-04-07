#pragma once 

#ifndef MEMTABLE_HPP
#define MEMTABLE_HPP

#include "SkipList.hpp"

using namespace std;

#define MEM_MAX_LV 15
#define MEM_RATE 0.5
#define DEFAULT_MAX_SIZE 1024 * 1024 * 2 // 2 MB

class MemTable
{
private:
    int mem_maxlv = 25;
    float mem_rate = 0.6;
    uint64_t size = 0;
    SkipList<uint64_t, string> list;
    
public:
    MemTable(){
        string str("default");
        list.init_handle(mem_maxlv, mem_rate, 0, UINT64_MAX, str);
    }

    ~MemTable(){
        MEM_FREE();
    }

    void PUT(const uint64_t key, const string &s){
        size += (s.size() * sizeof(char));
        list.insert(key, s);
    }

    string GET(const uint64_t key, bool &flag){
        Node<uint64_t, string> *target;
        target = list.searchNode(key);
        flag = (target != nullptr);
        return flag ? target->getVal() : "";
    }

    map<uint64_t, string> IMM_MEMTABLE(){
        map<uint64_t, string> imm = list.SELECT_TO_MAP();
        MEM_RESET();
        return imm;
    }

    bool SEARCH(const uint64_t key){
        return list.search(key);
    }

    void DELETE(const uint64_t key){
        bool flag;
        string val = GET(key, flag);
        list.remove(key);
        size -= (val.size() * sizeof(char));
    }

    void TRAVERSE(){
        list.traverse();
    }

    void MEM_FREE(){
        list.freeList();
    }

    void MEM_RESET(){
        size = 0;
        list.resetList();
    }

    bool IS_FULL() const {
        return size >= DEFAULT_MAX_SIZE;
    }

    bool IS_EMPTY() const {
        return size == 0;
    }
};

#endif //MemTable_hpp