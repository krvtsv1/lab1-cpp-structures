#include "SinglyLinkedList.h"
using namespace std;

void FPUSH_HEAD(SinglyLinkedList& list, const string& key) {
    FNode* newNode = new FNode{key, list.head};
    list.head = newNode;
    list.size++;
}

void FPUSH_TAIL(SinglyLinkedList& list, const string& key) {
    FNode* newNode = new FNode{key, nullptr};
    if (!list.head) {
        list.head = newNode;
    } else {
        FNode* curr = list.head;
        while (curr->next) curr = curr->next;
        curr->next = newNode;
    }
    list.size++;
}

void FDEL_HEAD(SinglyLinkedList& list) {
    if (!list.head) return;
    FNode* temp = list.head;
    list.head = list.head->next;
    delete temp;
    list.size--;
}

void FDEL_TAIL(SinglyLinkedList& list) {
    if (!list.head) return;
    if (!list.head->next) {
        delete list.head;
        list.head = nullptr;
    } else {
        FNode* curr = list.head;
        while (curr->next->next) curr = curr->next;
        delete curr->next;
        curr->next = nullptr;
    }
    list.size--;
}

void FDEL(SinglyLinkedList& list, const string& value) {
    if (!list.head) return;
    if (list.head->key == value) {
        FDEL_HEAD(list);
        return;
    }
    FNode* prev = list.head;
    FNode* curr = list.head->next;
    while (curr && curr->key != value) {
        prev = curr;
        curr = curr->next;
    }
    if (!curr) return;
    prev->next = curr->next;
    delete curr;
    list.size--;
}

void FPRINT(const SinglyLinkedList& list) {
    if (!list.head) { cout << "empty" << endl; return; }
    FNode* curr = list.head;
    while (curr) {
        cout << curr->key << " ";
        curr = curr->next;
    }
    cout << endl;
}
