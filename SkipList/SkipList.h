#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include "SkipListNode.h"

/* 
 * 2020.3.24 debug:
 * The class template function's defination can't be placed in a .cpp file.
 * or will feedback with error: "Undefined reference to 'xxx'".
 * 2020.3.25 debug:
 * bugs in func: insert,remove,free
 */

using namespace std;

#define headKey -10000
#define footKey  10000

template<typename K, typename V>
class SkipList 
{   
private:
    Node<K, V> *header = nullptr;
    Node<K, V> *footer = nullptr;

    float p   = 0; // grow rate
    int count = 0; // number of nodes
    int maxlv = 0; // max level
    int level = 0; // current max level (except header and footer)

public:
    SkipList(int lv, float rate);

    ~SkipList() {freeList();}
    
    Node<K, V> *searchNode(K key);

    bool search(K key);

    int RandomLevel();

    void insertHelper(K key, V value, int addlv, Node<K, V> *before, Node<K, V> *after);

    void insert(K key, V value);

    void remove(K key);
    
    void traverse();

    void freeList();

    void freeTower(Node<K, V> *base);
    
    bool isEmpty() const {return count == 0;}

    int size() const {return count;}

    int getlevel() const {return level;}

    void changeLevel(int tmplv) {level = (level > tmplv) ? level : tmplv;}

public:
    Node<K, V> *getTop(Node<K, V> *ptr) const {
        Node<K, V> *curr = ptr;
        while(curr->above != nullptr)
            curr = curr->above;
        
        return curr;
    }

    Node<K, V> *getLevel(Node<K, V> *ptr, int lv) const {
        Node<K, V> *curr = ptr;
        while(lv > 1){
            curr = curr->above;
            lv--;
        }

        return curr;
    }

    Node<K, V> *getBottom(Node<K, V> *ptr) const {
        Node<K, V> *curr = ptr;
        while(curr->below != nullptr)
            curr = curr->below;

        return curr;
    }

    int getLayer(Node<K, V> *ptr){ /* down to up */
        int layer = 0;
        Node<K, V> *curr = ptr;
        while(curr != nullptr)
        {
            curr = curr->above;
            layer++;
        }

        return layer;
    }

    int getHight(Node<K, V> *ptr){ /* up to down */
        int hight = 0;
        Node<K, V> *curr = ptr;
        while(curr != nullptr)
        {
            curr = curr->below;
            hight++;
        }

        return hight;
    }

    /* debug helpers */
    void nmsl(){cout<<"nmsl\n";}

    void awsl(){cout<<"awsl\n";}

    void searchRes(K key) {
        bool res = search(key);
        
        if(res)
            cout <<"key " << key << " is found out!\n";
        else
            cout <<"key " << key << " is not in list\n";
    }
};

template<typename K, typename V>
SkipList<K, V>::SkipList(int lv, float rate){
    this->maxlv = lv;
    this->p = rate;
    srand(time(nullptr));

    header = new Node<K, V>(headKey, 0);
    footer = new Node<K, V>(footKey, 0);
    
    Node<K, V> *head = header;
    Node<K, V> *foot = footer;
    
    for(int i = 1; i < maxlv; ++i){
        Node<K, V> *addHead = new Node<K, V>(head);
        Node<K, V> *addFoot = new Node<K, V>(foot);
        addHead->succ = addFoot;
        addFoot->pred = addHead;

        head = addHead;
        foot = addFoot;
    }
}

template<typename K, typename V>
Node<K, V> *SkipList<K, V>::searchNode(K key){
    Node<K, V> *curr = getLevel(header, level);
    // cout << getHight(curr) << "___" << level<<endl;
    
    for(int i = level; i > 0; i--){
        K tmpkey = curr->succ->getKey();
        // cout<<tmpkey << " : tmpkey\n";
        while (tmpkey < key)
        {
            /*  tmpkey != footKey <==> curr->succ != footer*/
            curr = curr->succ;
            tmpkey = curr->succ->getKey();
        }

        /* now curr->succ->key >= key && curr->key < key */
        if(tmpkey == key) /* find it! */
            return getBottom(curr->succ);
        
        /* curr->key < key && curr->succ->key > key */
        curr = curr->below;
    }

    return nullptr;
}

template<typename K, typename V>
bool SkipList<K, V>::search(K key){
    if(isEmpty()){
        cout << "The list is empty!\n";
        return false;
    }

    return searchNode(key) != nullptr;
}

template<typename K, typename V>
int SkipList<K, V>::RandomLevel(){
    int addlv = 0;

    float r = ((float)rand()/RAND_MAX);

    while (r < p) {
        addlv++;
        r = rand()/((float)RAND_MAX);
    }

    return addlv;
}

template<typename K, typename V>
void SkipList<K, V>::insertHelper(K key, V value, int addlv, Node<K, V> *before, Node<K, V> *after)
{
    cout<<"\ninsert :"<< key << "; randlv: " <<addlv <<endl;
    Node<K, V> *helper = new Node<K, V>(key, value);
    changeLevel(addlv);

    while (addlv) {
        // cout <<"addlv: "<<addlv<<"---------\n";
        // cout << "before: "<<before->getKey()<<" after: "<<after->getKey()<<endl;
        helper->pred = before;
        before->succ = helper;
        helper->succ = after;
        after->pred  = helper;
        // cout << "before: "<<helper->pred->getKey()<<" after: "<<helper->succ->getKey()<<endl;

        addlv--;
        if(!addlv)
            break;

        while(before->above == nullptr){
            // nmsl();
            before = before->pred;
        }

        while(after->above == nullptr){
            // awsl();
            after = after->succ;
        }
        // cout << "before: "<<before->getKey()<<" after: "<<after->getKey()<<endl;

        Node<K, V> *subHlper = new Node<K, V>(helper);
        helper = subHlper;
        before = before->above;
        after = after->above;
    }

    count++;
    cout << "key " << key <<" successfully insert!\n";
    traverse();
    cout <<endl;
}

template<typename K, typename V>
void SkipList<K, V>::insert(K key, V value) {
    int addlv = RandomLevel();
    if(!addlv || addlv > maxlv){
        cout <<"key " << key << " unluckily can't grow!\n";
        return;
    }

    if(isEmpty()){
        cout << "first insert!\n";
        insertHelper(key, value, 8, header, footer);
        return;
    }

    Node<K, V> *curr = getLevel(header, level);


    for(int i = level - 1; i >= 0; i--){
        K tmpkey = curr->succ->getKey();

        while (tmpkey < key)
        {
            /*  tmpkey != footKey <==> curr->succ != footer*/
            curr = curr->succ;
            tmpkey = curr->succ->getKey();
        }
        
        /* <==> curr->succ->key >= key && curr->key < key */
        if(tmpkey == key){ /* find it! */
            cout << key << " has already existed!\n";
            return;
        }
        
        /* curr->key < key && curr->succ->key > key */
        if(i)
            curr = curr->below;
    }

    /* now we are on the bottom level and where we will insert is between curr and curr->succ*/
    Node<K, V> *before = curr;
    Node<K, V> *after = curr->succ;

    insertHelper(key, value, addlv, before, after);
}

template<typename K, typename V>
void SkipList<K, V>::remove(K key) {
    if(isEmpty()){
        cout <<"The list is empty!\n";
        return;
    }

    Node<K, V> *target = searchNode(key);
    if(target == nullptr){
        cout << "key " <<key<< " does not exist!\n";
        return;
    }
    
    cout <<"hight is :" <<getLayer(target)<<endl;

    /* now we are on the bottom of target tower */
    Node<K, V> *helper = target->above;
    while (target != nullptr)
    {
        target->succ->pred = target->pred;
        target->pred->succ = target->succ;
        delete target;
        
        if(helper == nullptr)
            break;
        target = helper;
        helper = helper->above;
    }

    count--;
    cout <<"key " << key <<" Delete Successly!" <<endl;
    traverse();
    cout <<endl;
}

template<typename K, typename V>
void SkipList<K, V>::traverse(){
    if(isEmpty()){
        cout << "The list is empty!\n";
        return;
    }
    
    cout << "*******SkipListDisplay*******"<<endl;
    Node<K, V> *curr = header;

    while(curr != nullptr){
        cout << curr->getKey() << " : ";

        for(int i = getLayer(curr); i > 0; --i)
            cout << ' '<< i << ' ';

        cout <<endl;
        curr = curr->succ;
    }
}

template<typename K, typename V>
void SkipList<K, V>::freeList(){
    Node<K, V> *curr = header;
    Node<K, V> *helper = curr->succ;

    while (helper != nullptr)
    {   
        freeTower(curr);
        curr   = helper;
        helper = helper->succ;
    }

    /* helper == nullptr <==> curr->succ == nullptr <==> curr is footer */
    freeTower(curr);
    count = 0;
}

template<typename K, typename V>
void SkipList<K, V>::freeTower(Node<K, V> *base){
    Node<K, V> *curr = base;
    Node<K, V> *helper = curr->above;

    while (helper != nullptr)
    {   
        delete curr;
        curr   = helper;
        helper = helper->above;
    }

    /* helper == nullptr <==> curr->above == nullptr */
    delete curr;
}