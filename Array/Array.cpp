#include "Array.h"

void arrInit(Array& arr, int cap) {
    arr.capacity = cap;
    arr.size = 0;
    arr.data = new string[cap];
}

void arrPush(Array& arr, string value) {
    if (arr.size >= arr.capacity) {
        arr.capacity *= 2;
        string* newData = new string[arr.capacity];
        for (int i = 0; i < arr.size; i++) newData[i] = arr.data[i];
        delete[] arr.data;
        arr.data = newData;
    }
    arr.data[arr.size++] = value;
}

// Удаление с конца (для MPOP B)
void arrPopBack(Array& arr) {
    if (arr.size > 0) arr.size--;
}

// Удаление с начала (для MPOP F) - сдвиг массива
void arrPopFront(Array& arr) {
    if (arr.size == 0) return;
    for (int i = 0; i < arr.size - 1; i++) {
        arr.data[i] = arr.data[i + 1];
    }
    arr.size--;
}

void arrFree(Array& arr) {
    delete[] arr.data;
}

string arrGet(const Array& arr, int index) {
    if (index < 0 || index >= arr.size) return "";
    return arr.data[index];
}

void arrPrint(const Array& arr) {
    if (arr.size == 0) { cout << "empty" << endl; return; }
    for (int i = 0; i < arr.size; i++) cout << arr.data[i] << " ";
    cout << endl;
}
