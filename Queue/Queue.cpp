#include "Queue.h"

Queue* createQueue() {
    return new Queue{nullptr, nullptr};
}

void deleteQueue(Queue* q) {
    while (q->front) {
        QNode* temp = q->front;
        q->front = q->front->next;
        delete temp;
    }
    delete q;
}

void push(Queue* q, string val) {
    QNode* temp = new QNode{val, nullptr};
    if (!q->rear) {
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}

string pop(Queue* q) {
    if (!q->front) return "EMPTY";
    QNode* temp = q->front;
    string val = temp->data;
    q->front = q->front->next;
    if (!q->front) q->rear = nullptr;
    delete temp;
    return val;
}

string front(Queue* q) {
    if (!q->front) return "EMPTY";
    return q->front->data;
}

void printQueue(Queue* q) {
    if (!q->front) { cout << "empty" << endl; return; }
    QNode* curr = q->front;
    while (curr) { cout << curr->data << " "; curr = curr->next; }
    cout << endl;
}
