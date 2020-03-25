#include "SkipList.h"

int main(){

    cout << "test is starting ....." << endl;

    SkipList<int,int> list(20, 0.5);

    list.insert(1, 3);
    list.searchRes(1);

    srand(time(nullptr));

    for(int i = 0; i < 40; ++i)
        list.insert(rand()%40, i*2);


    cout<< "***********remove test***********\n\n\n";
    for(int i = 0; i <= 40; ++i)
        list.remove(i);

    cout << "test is finished ...." << endl;
    return 0;
}
