#include <cstring>
#include <string>
#include <iostream>
#include <iomanip>
#include <sys/timeb.h>
#include "kvstore.h"

using namespace std;

// performanceTest
void delayTest(uint64_t max){
    KVStore store("./data");
    uint64_t i;
    clock_t start,finish;

    start = clock();
    for(i = 0; i < max; ++i){
        store.put(i, std::string(i + 1, 't'));
    }
    finish = clock();
    cout << "Average time for 'put' is " << setprecision(3) << (finish - start)/(double)max << " msec\n";

    start = clock();
    for(i = 0; i < max; ++i){
        string tmp = store.get(i);
    }
    finish = clock();
    cout << "Average time for 'get' is " << setprecision(3) << (finish - start)/(double)max << " msec\n";

    start = clock();
    for(i = 0; i < max; ++i){
        bool flag = store.del(i);
    }
    finish = clock();
    cout << "Average time for 'del' is " << setprecision(3) << (finish - start)/(double)max << " msec\n";
}

void throughputTest(uint64_t max){
    KVStore store("./data");
    uint64_t i, j = 0;
    int second = 0;
    clock_t start,finish;

    start = clock();
    for(i = 0; i < max; ++i){
        store.put(i, std::string(i + 1 , 's'));

        finish = clock();
        if((finish - start)/1000 > second)
        {
            second++;
            cout << second << '\t' << i << '\t' << i - j << endl;
            j = i;
        }
    }
}

int main(){
    // delayTest(2000);

    // throughputTest(1024 * 16);
    return 0;
}