#include "SkipList.h"

int main(){

    cout << "test is starting ....." << endl;

    SkipList<int, int> list(20, 0.5);

    srand(time(nullptr));

    for(int i = 0; i < 800; ++i){
        list.insert(rand()%800, i*2);
    }

    list.traverse();

    cout<< "***********remove test***********\n\n\n";
    for(int i = 0; i <= 400; ++i)
        list.remove(i);

    list.traverse();

    for(int i = 0; i < 800; ++i)
        cout <<"search "<<i <<" is "<<list.search(i)<<endl;

    cout << "test is finished ...." << endl;
    return 0;
}
