#pragma once
#include <iostream>

/* this is entry class of kv-pair */
template <typename K, typename V>
class Entry
{
public: 
    K key = 0;
    V val;

    Entry(){}
    Entry(K k, V v): key(k), val(v){}
    Entry(const Entry<K, V> *e): key(e->key), val(e->val){}
    ~Entry(){}
    
    bool operator< (const Entry<K, V> &e){return key < e->key;}
    bool operator> (const Entry<K, V> &e){return key > e->key;}
    bool operator==(const Entry<K, V> &e){return key ==e->key;}
    bool operator!=(const Entry<K, V> &e){return key !=e->key;}
};