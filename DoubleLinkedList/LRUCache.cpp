#include "LRUCache.h"
#include <iostream>

void LRUCache::removeNode(LNode* node) {
    if (!node) return;

    if (node->prev) {
        node->prev->next = node->next;
    } else {
        list.head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        list.tail = node->prev;
    }

    list.size--;
    delete node;
}

LRUCache::LRUCache(int cap) : capacity(cap) {
    list.head = nullptr;
    list.tail = nullptr;
    list.size = 0;
}

LRUCache::~LRUCache() {
    while(list.head) {
        LDEL_HEAD(list);
    }
}

int LRUCache::get(int key) {
    if (map.find(key) == map.end()) {
        return -1;
    }

    LNode* node = map[key];
    int val = stoi(node->data);
    
    removeNode(node);
    map.erase(key);
    
    LADD_HEAD(list, to_string(val));
    map[key] = list.head;
    
    return val;
}

void LRUCache::put(int key, int value) {
    if (map.find(key) != map.end()) {
        LNode* oldNode = map[key];
        removeNode(oldNode);
        map.erase(key);
    } 
    else {
        if (list.size >= capacity) {
            LNode* tail = list.tail;
            if (tail) {
                int keyToDelete = -1;
                for(auto const& item : map) {
                    if(item.second == tail) {
                        keyToDelete = item.first;
                        break;
                    }
                }
                
                if (keyToDelete != -1) {
                    map.erase(keyToDelete);
                }
                
                LDEL_TAIL(list);
            }
        }
    }

    LADD_HEAD(list, to_string(value));
    map[key] = list.head;
}
