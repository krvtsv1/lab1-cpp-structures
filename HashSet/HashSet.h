#ifndef HASHSET_H
#define HASHSET_H

#include <string>

// Узел для цепочек коллизий (односвязный список)
struct HashNode {
    std::string key;
    HashNode* next;
};

struct HashSet {
    HashNode** buckets; // Массив указателей на узлы
    int capacity;       // Размер массива
    int size;           // Количество элементов
};

// Функции
HashSet* createHashSet(int capacity);
void deleteHashSet(HashSet* set);

void setAdd(HashSet* set, std::string key);
void setDel(HashSet* set, std::string key);
bool setAt(HashSet* set, std::string key);

// Хеш-функция (вспомогательная)
unsigned int hashFunction(const std::string& key, int capacity);

#endif
