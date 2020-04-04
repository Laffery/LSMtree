#include "MemTable.hpp"
#include "SSTable.hpp"
#include "SSLevel.hpp"
#include "disk.hpp"
#include "log.hpp"
#include <map>
#include <string>
#include <iostream>
#include <ctime>
#include <cstdio>

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

void binFileTest(const string path){
    ofstream fout(path, ios::binary | ios::app);
    int nNum = 80;

    string str("Hello,world");

    fout.write((char*)&nNum, sizeof(int));

    fout.write(str.c_str(), sizeof(char) * (str.size()));

    fout.close();  

    ifstream fin(path, ios::binary);
    int num;  
    char szBuf[256] = {0};

    fin.read((char*)&num, sizeof(int));

    fin.read(szBuf, sizeof(char)*256);

    cout <<"int = "<<num<<endl;

    cout <<"str = "<<szBuf<<endl;
    fin.close();
}

void LogTest(const string path){
    Log log(path);
    log.REGARD('i', 65,"hahah");
    log.REGARD('d', 65,"hahah");
}

int main(){

    cout << "test is starting ......" << endl;

    // SkipListTest(15, 0.5, 60);
    
    // fileTest();

    // memTableTest(1024, 300);

    // SSTtest();

    // levelTest(1);

    // diskTest();

    // binFileTest("./data/my.log");

    // time_t rawtime;
    // struct tm *ptminfo;
    clock_t start, finish;

    start = clock();
 
    LogTest("./data/my.log");

    finish = clock();
    cout <<start <<' ' << finish <<endl;
    // time(&rawtime);
    // ptminfo = localtime(&rawtime);
    // printf("current: %02d-%02d-%02d %02d:%02d:%02d\n",
    //         ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);
    cout << "test is finished ......" << endl;
    
    return 0;
}

