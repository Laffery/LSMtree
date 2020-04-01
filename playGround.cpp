#include "MemTable.hpp"
#include "SSTable.hpp"
#include "SSLevel.hpp"
#include "disk.hpp"
#include <map>
#include <string>
#include <iostream>

using namespace std;

void SkipListTest(int levelsize, float rate, int num){
    SkipList<int, char> list;
    list.init_handle(15, 0.5, -32768, 32767, 0);

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
  
    for(uint64_t i = 0; i < 1002; ++i){
        mapStudent.insert(pair<uint64_t, string>(i, "student_"+std::to_string(i))); 
    }
  
    map<uint64_t, string>::iterator iter;   

    SSTable sst("./demo/B.txt");
    sst.WRITE_TO_DIR(mapStudent);
    // sst.SHOW_INDEX();
    cout << sst.GET(0) <<endl;
}

void levelTest(int n){
    SSLevel lv("./data/C3", n);
    lv.WRITE_TO_SST(3);
    return;
}

void diskTest(){
    disk disc("./data/", 5);
    disc.WRITE_TO_LEVEL(4);
    return;
}

int main(){

    cout << "test is starting ......" << endl;

    // SkipListTest(15, 0.5, 60);
    
    // fileTest();

    // memTableTest(1024, 300);

    // SSTtest();

    // levelTest(1);

    diskTest();

    cout << "test is finished ......" << endl;
    
    return 0;
}

