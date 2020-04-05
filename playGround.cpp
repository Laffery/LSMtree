#include "MemTable.hpp"
#include "SSTable.hpp"
#include "SSLevel.hpp"
#include "disk.hpp"
#include "kvstore.h"
#include <map>
#include <string>
#include <iostream>
#include <ctime>
#include <cstdio>

using namespace std;  
typedef map<uint64_t, string> MAP_DATA;
typedef map<uint64_t, uint64_t> MAP_INDEX;  

void SkipListTest(int levelsize, float rate, int num){
    SkipList<int, char> list;
    list.init_handle(levelsize, rate, -32768, 32767, 0);

    for(int i = 0; i < num; ++i)
        list.insert(rand()%num, 'k');

    list.traverse();

    cout<< "***********remove test***********\n";

    for(int i = 0; i <= num/2; ++i)
        list.remove(i);

    list.traverse();

    cout<< "***********search test***********\n";
    for(int i = 0; i < num; ++i)
        cout <<"search "<< i <<" result : "<< list.search(i) << endl;
}

void memTableTest(int size, uint64_t num){
    MemTable mem(size);

    string str("K");
    for(uint64_t i = 0; i < num; ++i)
        mem.PUT((uint64_t)rand()%num, str);

    for(uint64_t i = 0; i <= num/2; ++i)
        mem.DELETE(i);

    // mem.TRAVERSE();

    // cout<< "***********search test***********\n";
    for(uint64_t i = 0; i < num; ++i)
        cout <<"search "<< i <<" result : "<< mem.GET(i) << endl;

    map<uint64_t, string> mem_map = mem.IMM_MEMTABLE();
    
    map<uint64_t, string>::iterator iter; 
  
    for(iter = mem_map.begin(); iter != mem_map.end(); iter++)  
        cout<<iter->first<<' '<<iter->second<<endl; 
    
}

void SSTtest(){  
  
    map<uint64_t, string> mapStudent;  
  
    for(uint64_t i = 0; i < 66666; ++i){
        mapStudent.insert(pair<uint64_t, string>(i, "student_"+std::to_string(i))); 
    }

    SSTable sst("sstable3.dat");
    sst.WRITE_TO_DIR(mapStudent);
    MAP_DATA::iterator iter = mapStudent.begin();
    while(iter != mapStudent.end()){
        cout <<iter->first <<": " << iter->second <<endl;
        iter++;
    }
    // cout << sst.SEARCH(102) <<endl;
    // cout << sst.GET(112) <<endl;
    // sst.RESET();

    // MAP_INDEX index = sst.GET_INDEX_MAP();
    // MAP_INDEX index;
    // sst.INDEX_TO_MAP(index);
    // MAP_INDEX::iterator iter = index.begin();
    // while(iter != index.end()){
    //     cout<<iter->first<<' '<<iter->second<<endl; 
    //     iter++;
    // }

    // MAP_DATA table = sst.GET_KV_MAP();
    // MAP_DATA table;
    // sst.KV_TO_MAP(table);
    // MAP_DATA::iterator it = table.begin();
    // while(it != table.end()){
    //     cout <<it->first << ":" <<it->second<<endl;
    //     it++;
    // }
}

void levelTest(){
    map<uint64_t, string> mapStudent0;
    map<uint64_t, string> mapStudent1;
    map<uint64_t, string> mapStudent2;
    map<uint64_t, string> mapStudent3;
  
    for(uint64_t i = 0; i < 102; ++i)
        mapStudent0.insert(pair<uint64_t, string>(i, "student_"+std::to_string(i))); 

    for(uint64_t i = 200; i < 302; ++i)
        mapStudent1.insert(pair<uint64_t, string>(i, "student_"+std::to_string(i))); 

    for(uint64_t i = 400; i < 502; ++i)
        mapStudent2.insert(pair<uint64_t, string>(i, "student_"+std::to_string(i))); 

    for(uint64_t i = 600; i < 702; ++i)
        mapStudent3.insert(pair<uint64_t, string>(i, "student_"+std::to_string(i))); 

    SSLevel lv("./data/C1", 1);
    lv.WRITE_TO_SST(0, mapStudent0);
    lv.WRITE_TO_SST(1, mapStudent1);
    lv.WRITE_TO_SST(2, mapStudent2);
    lv.WRITE_TO_SST(3, mapStudent3);

    for(uint64_t i = 0; i < 800; i += 20)
        cout << i << ": " << lv.GET(i) <<endl;
    return;
}

void diskTest(){
    disk disc("./data", 5);
    // disc.WRITE_TO_LEVEL(4);
}

void kvstoreTest(){
    KVStore("./data");
}

int main(){

    cout << "test is starting ......" << endl;

    // SkipListTest(25, 0.85, 32600);
    
    // fileTest();

    // memTableTest(1024, 30000);

    SSTtest();

    // levelTest();

    // diskTest();

    // kvstoreTest();

    cout << "test is finished ......" << endl;
    
    return 0;
}