#ifndef QUEUE_H
#define QUEUE_H
#include <iostream>
#include <string>
using namespace std;

struct QNode {
    string data;
    QNode* next;
};

struct Queue {
    QNode* front;
    QNode* rear;
};

Queue* createQueue();
void deleteQueue(Queue* q);
void push(Queue* q, string val);
string pop(Queue* q);
string front(Queue* q); // Новое
void printQueue(Queue* q); // Новое
#endif
