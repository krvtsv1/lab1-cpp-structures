#ifndef DOUBLYLIST_H
#define DOUBLYLIST_H
#include <string>
#include <iostream>
using namespace std;

struct LNode {
    string data;
    LNode* next;
    LNode* prev;
};

struct DoublyList {
    LNode* head = nullptr;
    LNode* tail = nullptr;
    int size = 0;
};

void LADD_HEAD(DoublyList& list, const string& value); // Новое
void LADD_TAIL(DoublyList& list, const string& value);
void LDEL_HEAD(DoublyList& list); // Новое
void LDEL_TAIL(DoublyList& list); // Новое
void LPRINT_F(const DoublyList& list); // Новое
void LPRINT_B(const DoublyList& list); // Новое
#endif
