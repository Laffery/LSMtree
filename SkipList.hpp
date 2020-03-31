#pragma once

#ifndef SKIPLIST_HPP
#define SKIPLIST_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include "SkipListNode.hpp"

/* 
 * 2020.3.24 debug:
 * The class template function's defination can't be placed in a .cpp file.
 * or will feedback with error: "Undefined reference to 'xxx'".
 * 2020.3.25 debug:
 * bugs in func: insert,remove,free
 * 2020.3.29 update:
 * api supports to typename entry
 * 2020.3.31 update:
 * initial function and recreate function to support memtable
 */

typedef std::string V;

using namespace std;

template <typename K, typename V>
class SkipList 
{   
private:
    K headKey = -32768;
    K footKey = 32767;
    V defVal;
    Node<K, V> *header;
    Node<K, V> *footer;

    float p   = 0.5; // grow rate
    int count = 0; // number of nodes
    int maxlv = 8; // max level
    int level = 0; // current max level (except header and footer)

public:
    SkipList(){}

    SkipList(int lv, float rate) {init_argc_2(lv, rate);}

    SkipList(int lv, float rate, K hKey, K fKey) : headKey(hKey), footKey(fKey){
        init_argc_2(lv, rate);
    }

    ~SkipList() {freeList();}

    void init_lv_rate(int lv, float rate){
        this->maxlv = lv;
        this->p = rate;
    }

    void init_argc_0();

    void init_argc_2(int lv, float rate);

    void init_handle(int lv, float rate, K hKey, K fKey, V defV){
        init_lv_rate(lv, rate);
        this->headKey = hKey;
        this->footKey = fKey;
        this->defVal  = defV;

        init_argc_0();
    }

    void recreate(int lv, float rate);
    
    Node<K, V> *searchNode(K key);

    bool search(K key);

    int RandomLevel();

    void insertHelper(K key, V val, int addlv, Node<K, V> *before, Node<K, V> *after);

    void insert(K key, V val);

    void remove(K key);
    
    void traverse();

    void freeList();

    void freeTower(Node<K, V> *base);
    
    bool isEmpty() const {return count == 0;}

    int size() const {return count;}

    int getRate() const {return p;}

    int getMaxlevel() const {return maxlv;}

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
};

template <typename K, typename V>
void SkipList<K, V>::init_argc_0(){
    srand(time(nullptr));

    header = new Node<K, V>(headKey, defVal);
    footer = new Node<K, V>(footKey, defVal);
    
    
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

template <typename K, typename V>
void SkipList<K, V>::init_argc_2(int lv, float rate){
    init_lv_rate(lv, rate);
    init_argc_0();
}

template <typename K, typename V>
void SkipList<K, V>::recreate(int lv, float rate){
    this->p = rate;
    if(lv == maxlv || lv <= 0)
        return;
    
    Node<K, V> *head = getTop(header);
    Node<K, V> *foot = getTop(footer);

    if(lv > maxlv){
        for(int i = maxlv; i < lv; ++i){
            Node<K, V> *addHead = new Node<K, V>(head);
            Node<K, V> *addFoot = new Node<K, V>(foot);
            addHead->succ = addFoot;
            addFoot->pred = addHead;

            head = addHead;
            foot = addFoot;
        }
    }

    else
    {   Node<K, V> *headHelper = head->below;
        Node<K, V> *footHelper = foot->below;
        for(int i = lv; i < maxlv; ++i){
            
            delete head;
            delete foot;
            headHelper = headHelper->below;
            footHelper = footHelper->below;
        }
    }
    
    this->maxlv = lv;
}

template <typename K, typename V>
Node<K, V> *SkipList<K, V>::searchNode(K key){
    Node<K, V> *curr = getLevel(header, level);
    
    for(int i = level; i > 0; i--){
        K tmpkey = curr->succ->getKey();
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

template <typename K, typename V>
bool SkipList<K, V>::search(K key){
    if(isEmpty()){
        cout << "The list is empty!\n";
        return false;
    }

    return searchNode(key) != nullptr;
}

template <typename K, typename V>
int SkipList<K, V>::RandomLevel(){
    int addlv = 0;

    float r = ((float)rand()/RAND_MAX);

    while (r < p) {
        addlv++;
        r = rand()/((float)RAND_MAX);
    }

    return addlv;
}

template <typename K, typename V>
void SkipList<K, V>::insertHelper(K key, V val, int addlv, Node<K, V> *before, Node<K, V> *after)
{
    // cout<<"\ninsert :"<< key << "; randlv: " <<addlv <<endl;
    Node<K, V> *helper = new Node<K, V>(key, val);
    changeLevel(addlv);

    while (addlv) {
        helper->pred = before;
        before->succ = helper;
        helper->succ = after;
        after->pred  = helper;

        addlv--;
        if(!addlv)
            break;

        while(before->above == nullptr)
            before = before->pred;

        while(after->above == nullptr)
            after = after->succ;

        Node<K, V> *subHlper = new Node<K, V>(helper);
        helper = subHlper;
        before = before->above;
        after = after->above;
    }

    count++;
    // cout << "key " << key <<" successfully insert!\n";
}

template <typename K, typename V>
void SkipList<K, V>::insert(K key, V val) {
    int addlv = RandomLevel();
    if(!addlv || addlv > maxlv){
        // cout <<"key " << key << " unluckily can't grow!\n";
        return;
    }

    if(isEmpty()){
        insertHelper(key, val, addlv, header, footer);
        return;
    }

    Node<K, V> *curr = getLevel(header, level);

    for(int i = level - 1; i >= 0; i--){
        K tmpkey = curr->succ->getKey();

        while (tmpkey < key)
        {
            /* tmpkey != footKey <==> curr->succ != footer*/
            curr = curr->succ;
            tmpkey = curr->succ->getKey();
        }
        
        /* <==> curr->succ->key >= key && curr->key < key */
        if(tmpkey == key){ /* find it! */
            // cout << key << " has already existed!\n";
            return;
        }
        
        /* curr->key < key && curr->succ->key > key */
        if(i)
            curr = curr->below;
    }

    /* now we are on the bottom level and where we will insert is between curr and curr->succ*/
    Node<K, V> *before = curr;
    Node<K, V> *after = curr->succ;

    insertHelper(key, val, addlv, before, after);
}

template <typename K, typename V>
void SkipList<K, V>::remove(K key) {
    if(isEmpty()){
        cout <<"The list is empty!\n";
        return;
    }

    Node<K, V> *target = searchNode(key);
    if(target == nullptr){
        // cout << "key " <<key<< " does not exist!\n";
        return;
    }
    
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
}

template <typename K, typename V>
void SkipList<K, V>::traverse(){
    if(isEmpty()){
        cout << "The list is empty!\n";
        return;
    }
    
    cout << "*******SkipListDisplay*******"<<endl;
    Node<K, V> *curr = header;

    while(curr != nullptr){
        cout << curr->getKey() << " : ";
        cout << curr->getVal() << " : ";
        cout << getLayer(curr) <<endl;
        curr = curr->succ;
    }
}

template <typename K, typename V>
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

template <typename K, typename V>
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

#endif //SkipList.hpp