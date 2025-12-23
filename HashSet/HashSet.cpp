#include "HashSet.h"
#include <iostream>

using namespace std;

// Хеш-функция (алгоритм DJB2 - простой и эффективный для строк)
unsigned int hashFunction(const string& key, int capacity) {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c; 
    }
    return hash % capacity;
}

HashSet* createHashSet(int capacity) {
    HashSet* set = new HashSet;
    set->capacity = capacity;
    set->size = 0;
    set->buckets = new HashNode*[capacity];
    for (int i = 0; i < capacity; i++) {
        set->buckets[i] = nullptr;
    }
    return set;
}

void deleteHashSet(HashSet* set) {
    for (int i = 0; i < set->capacity; i++) {
        HashNode* current = set->buckets[i];
        while (current != nullptr) {
            HashNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] set->buckets;
    delete set;
}

void setAdd(HashSet* set, string key) {
    if (setAt(set, key)) return; // Элемент уже есть, дубликаты не нужны

    unsigned int index = hashFunction(key, set->capacity);
    HashNode* newNode = new HashNode;
    newNode->key = key;
    newNode->next = set->buckets[index]; // Вставляем в начало цепочки
    set->buckets[index] = newNode;
    set->size++;
}

void setDel(HashSet* set, string key) {
    unsigned int index = hashFunction(key, set->capacity);
    HashNode* current = set->buckets[index];
    HashNode* prev = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (prev == nullptr) {
                // Удаляем первый элемент в цепочке
                set->buckets[index] = current->next;
            } else {
                // Удаляем из середины или конца
                prev->next = current->next;
            }
            delete current;
            set->size--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

bool setAt(HashSet* set, string key) {
    unsigned int index = hashFunction(key, set->capacity);
    HashNode* current = set->buckets[index];
    while (current != nullptr) {
        if (current->key == key) {
            return true;
        }
        current = current->next;
    }
    return false;
}
