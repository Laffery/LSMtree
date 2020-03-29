#ifndef SKIPLISTNODE_H
#define SKIPLISTNODE_H
#include "entry.h"

template <typename K, typename V>
class Node
{
public:
    Entry<K, V> entry;

    Node<K, V> *succ = nullptr;
    Node<K, V> *pred = nullptr;
    Node<K, V> *above = nullptr;
    Node<K, V> *below = nullptr;

    Node(K k, V v) {
        entry = new Entry<K, V>(k, v);
    }
    
    Node(Node* base) {
        this->below = base;
        base->above = this;
        this->entry = base->entry;
    }

    ~Node() {}

    K getKey() const {
        return entry.key;
    }
};

#endif // SkipListNode_h