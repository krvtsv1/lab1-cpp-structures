#ifndef LRUCACHE_H
#define LRUCACHE_H

#include <unordered_map>
#include <string>
#include "../DoubleLinkedList/DoubleLinkedList.h"

using namespace std;

class LRUCache {
private:
    int capacity;
    DoublyList list;
    unordered_map<int, LNode*> map;

    void removeNode(LNode* node);

public:
    LRUCache(int capacity);
    ~LRUCache();
    
    int get(int key);
    void put(int key, int value);
};

#endif
