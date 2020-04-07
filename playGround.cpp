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

    // list.traverse();

    // cout<< "***********remove test***********\n";

    // for(int i = 0; i <= num/2; ++i)
    //     list.remove(i);

    // list.traverse();

    // cout<< "***********search test***********\n";
    // for(int i = 0; i < num; ++i)
    //     cout <<"search "<< i <<" result : "<< list.search(i) << endl;

    // list.reset();
    // list.insert(10000, 's');
    // list.traverse();
    map<int, char> table = list.SELECT_TO_MAP();
    list.traverse();
}

void memTableTest(uint64_t num){
    MemTable mem;

    string str("K");
    for(uint64_t i = 0; i < num; ++i)
        mem.PUT((uint64_t)rand()%num, str);

    // for(uint64_t i = 0; i <= num/2; ++i)
    //     mem.DELETE(i);

    // mem.TRAVERSE();

    // cout<< "***********search test***********\n";
    // for(uint64_t i = 0; i < num; ++i)
    //     cout <<"search "<< i <<" result : "<< mem.GET(i) << endl;

    map<uint64_t, string> mem_map = mem.IMM_MEMTABLE();
    
    map<uint64_t, string>::iterator iter = mem_map.begin(); 

    while(iter != mem_map.end()){  
        cout<<iter->first<<' '<<iter->second<<endl;
        iter++;
    }
}

void SSTtest(){  
  
    // map<uint64_t, string> mapStudent;  
  
    // for(uint64_t i = 0; i < 666; ++i){
    //     mapStudent.insert(pair<uint64_t, string>(i, "student_"+std::to_string(i))); 
    // }

    SSTable sst("./data/C0/sstable0.dat");
    // sst.WRITE_TO_DIR(mapStudent);
    // MAP_DATA::iterator iter = mapStudent.begin();
    // while(iter != mapStudent.end()){
    //     cout <<iter->first <<": " << iter->second <<endl;
    //     iter++;
    // }
    for(uint64_t i = 0; i < 8; ++i){
        bool flag;
        cout << sst.GET(i, flag) << endl;
    }
    // cout << sst.SEARCH(102) <<endl;
    // bool flag;
    // cout << sst.GET(112, flag) <<endl;
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
  
    for(uint64_t i = 0; i < 300; ++i)
        mapStudent0.insert(pair<uint64_t, string>(i, "student_"+std::to_string(i))); 

    for(uint64_t i = 200; i < 500; ++i)
        mapStudent1.insert(pair<uint64_t, string>(i, "student_"+std::to_string(i))); 

    for(uint64_t i = 400; i < 500; ++i)
        mapStudent2.insert(pair<uint64_t, string>(i, "student_"+std::to_string(i))); 

    for(uint64_t i = 600; i < 700; ++i)
        mapStudent3.insert(pair<uint64_t, string>(i, "student_"+std::to_string(i))); 

    SSLevel lv("./data/C1", 1);
    lv.WRITE_TO_SST(0, mapStudent0);
    lv.WRITE_TO_SST(1, mapStudent1);
    lv.WRITE_TO_SST(2, mapStudent2);
    lv.WRITE_TO_SST(3, mapStudent3);

    bool flag;
    for(uint64_t i = 0; i < 800; i += 20)
        cout << i << ": " << lv.GET(i, flag) <<endl;
    return;
}

void diskTest(){
    map<uint64_t, string> mapStudent0;
    map<uint64_t, string> mapStudent1;
    map<uint64_t, string> mapStudent2;
    map<uint64_t, string> mapStudent3;
  
    for(uint64_t i = 0; i < 300; ++i)
        mapStudent0.insert(pair<uint64_t, string>(i, "student_"+std::to_string(i))); 

    for(uint64_t i = 200; i < 500; ++i)
        mapStudent1.insert(pair<uint64_t, string>(i, "student_"+std::to_string(i))); 

    for(uint64_t i = 400; i < 500; ++i)
        mapStudent2.insert(pair<uint64_t, string>(i, "student_"+std::to_string(i))); 

    for(uint64_t i = 600; i < 700; ++i)
        mapStudent3.insert(pair<uint64_t, string>(i, "student_"+std::to_string(i)));
    disk disc("./data", 5);
    disc.WRITE_TO_SSL(2, mapStudent0);
    disc.WRITE_TO_SSL(2, mapStudent1);
    disc.WRITE_TO_SSL(2, mapStudent2);
    disc.WRITE_TO_SSL(2, mapStudent3);
}

void kvstoreTest(int max){
    KVStore store("./data");
    uint64_t i;
    ofstream outf("log.txt", ios::out);

    // outf << "get 1" << ": " << store.get(1) <<endl;
	// store.put(1, "SE");
    // outf << "get 1" << ": " << store.get(1) <<endl;
	// outf << "del 1" << ": " << store.del(1) <<endl;
    // outf << "get 1" << ": " << store.get(1) <<endl;
    // outf << "del 1" << ": " << store.del(1) <<endl;

	outf << "Test multiple key-value pairs\n";
	for (i = 0; i < max; ++i) {
	    store.put(i, std::string(i+1, 's'));
		outf << "get " << i << ": " << store.get(i) <<endl;
	}
	
	outf << "Test after all insertions\n";
	for (i = 0; i < max; ++i)
		outf <<"get" << i <<": "<<  store.get(i)<<endl;

	outf << "Test deletions\n";
	for (i = 0; i < max; i+=2)
		outf << "del " << i << ": " << store.del(i) <<endl;

	for (i = 0; i < max; ++i)
		outf << "get " << i << ": " << store.get(i) <<endl;

	for (i = 1; i < max; ++i)
		outf << "del " << i << ": " << store.del(i)<<endl;
    outf.close();
}

int main(){

    cout << "test is starting ......" << endl;

    // SkipListTest(25, 0.85, 300);

    // memTableTest(300000);

    SSTtest();

    // levelTest();

    // diskTest();

    // kvstoreTest(2100);

    cout << "test is finished ......" << endl;
    
    return 0;
}