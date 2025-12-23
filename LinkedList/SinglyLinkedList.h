#ifndef SINGLYLIST_H
#define SINGLYLIST_H
#include <string>
#include <iostream>

struct FNode {
    std::string key;
    FNode* next;
};

struct SinglyLinkedList {
    FNode* head = nullptr;
    int size = 0;
};

void FPUSH_HEAD(SinglyLinkedList& list, const std::string& key);
void FPUSH_TAIL(SinglyLinkedList& list, const std::string& key); // Новое
void FDEL_HEAD(SinglyLinkedList& list);
void FDEL_TAIL(SinglyLinkedList& list); // Новое
void FDEL(SinglyLinkedList& list, const std::string& value);
void FPRINT(const SinglyLinkedList& list); // Новое
#endif
