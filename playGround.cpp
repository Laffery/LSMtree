#include "SkipList.hpp"
#include "MemTable.hpp"
#include <string>
#include <iostream>
#include <io.h>

using namespace std;

void SkipListTest(int levelsize, float rate, int num){
    SkipList<int, char> list;
    list.init_handle(15, 0.5, -32768, 32767, 0);

    for(int i = 0; i < num; ++i){
        list.insert(rand()%num, 'k');
    }

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

    cout<<"nmdl\n";
    string str("K");
    for(uint64_t i = 0; i < num; ++i){
        mem.PUT((uint64_t)rand()%num, str);
    }

    cout<< "***********remove test***********\n";

    for(uint64_t i = 0; i <= num/2; ++i)
        mem.DELETE(i);

    mem.TRAVERSE();

    cout<< "***********search test***********\n";
    for(uint64_t i = 0; i < num; ++i)
        cout <<"search "<< i <<" result : "<< mem.SEARCH(i) << endl;
}

void fileTest(){
    ofstream outfile("./demo/A/");//声明一个写文件流对象outfile并打开文件
    if(!outfile){//检查是否打开成功
        cout << "Failed to create file...\n";
        exit(0);
    }
    outfile << "Create a file!\n";//向文件写数据
    outfile.close();//关闭文件
    remove("./demo/B.txt");
    rename("./demo/C.txt", "./demo/E.txt");
    cout << "test is finished ...." << endl;
}

int main(){

    cout << "test is starting ......" << endl;

    // SkipListTest(15, 0.5, 60);
    
    // fileTest();

    memTableTest(1024, 3000);

    cout << "test is finished ......" << endl;
    
    return 0;
}


