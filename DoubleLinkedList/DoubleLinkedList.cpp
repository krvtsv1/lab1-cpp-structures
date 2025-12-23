#include "DoubleLinkedList.h"

void LADD_HEAD(DoublyList& list, const string& value) {
    LNode* newNode = new LNode{value, list.head, nullptr};
    if (list.head) list.head->prev = newNode;
    else list.tail = newNode;
    list.head = newNode;
    list.size++;
}

void LADD_TAIL(DoublyList& list, const string& value) {
    LNode* newNode = new LNode{value, nullptr, list.tail};
    if (list.tail) list.tail->next = newNode;
    else list.head = newNode;
    list.tail = newNode;
    list.size++;
}

void LDEL_HEAD(DoublyList& list) {
    if (!list.head) return;
    LNode* temp = list.head;
    list.head = list.head->next;
    if (list.head) list.head->prev = nullptr;
    else list.tail = nullptr;
    delete temp;
    list.size--;
}

void LDEL_TAIL(DoublyList& list) {
    if (!list.tail) return;
    LNode* temp = list.tail;
    list.tail = list.tail->prev;
    if (list.tail) list.tail->next = nullptr;
    else list.head = nullptr;
    delete temp;
    list.size--;
}

void LPRINT_F(const DoublyList& list) {
    if (!list.head) { cout << "empty" << endl; return; }
    LNode* curr = list.head;
    while (curr) { cout << curr->data << " "; curr = curr->next; }
    cout << endl;
}

void LPRINT_B(const DoublyList& list) {
    if (!list.tail) { cout << "empty" << endl; return; }
    LNode* curr = list.tail;
    while (curr) { cout << curr->data << " "; curr = curr->prev; }
    cout << endl;
}
