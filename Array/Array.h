#ifndef ARRAY_H
#define ARRAY_H
#include <string>
#include <iostream>
using namespace std;

struct Array {
    string* data;
    int size;
    int capacity;
};

void arrInit(Array& arr, int cap = 10);
void arrPush(Array& arr, string value);
void arrPopBack(Array& arr);  // Новое
void arrPopFront(Array& arr); // Новое
void arrFree(Array& arr);
string arrGet(const Array& arr, int index);
void arrPrint(const Array& arr); // Новое
#endif
