#ifndef SKIPLISTNODE_HPP
#define SKIPLISTNODE_HPP
#include "entry.hpp"

template <typename K, typename V>
class Node
{
private:
    Entry<K, V> entry;

public:
    Node<K, V> *succ = nullptr;
    Node<K, V> *pred = nullptr;
    Node<K, V> *above = nullptr;
    Node<K, V> *below = nullptr;

    Node(K k, V v) {
        this->entry = new Entry<K, V>(k, v);
    }
    
    Node(Node* base) {
        this->below = base;
        base->above = this;
        this->entry.key = base->entry.key;
        this->entry.val = base->entry.val;
    }

    ~Node() {}
    
    K getKey() const {return entry.key;}
    V getVal() const {return entry.val;}
    void setKey(K k) {this->entry.key = k;}
    void setVal(V v) {this->entry.val = v;}
    void setKV(K k, V v) {setKey(k);setVal(v);}
};

#endif // SkipListNode_hpp