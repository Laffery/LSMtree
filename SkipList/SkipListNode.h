#ifndef SKIPLISTNODE_H
#define SKIPLISTNODE_H

template <typename K, typename V>
class Node
{
private:
    K key;
    V val = 0;

public:
    Node<K, V> *succ = nullptr;
    Node<K, V> *pred = nullptr;
    Node<K, V> *above = nullptr;
    Node<K, V> *below = nullptr;

    Node() {}

    Node(K k, V v) : key(k), val(v) {}
    
    Node(Node* base){
        this->below = base;
        base->above = this;

        this->key = base->key;
        this->val = base->val;
    }

    ~Node() {}

    K getKey() const {return key;}

    V getVal() const {return val;}
};

#endif // SkipListNode_h