#include "Stack.h"

Stack* createStack(int initialCapacity) {
    Stack* stack = new Stack;
    stack->capacity = initialCapacity;
    stack->data = new string[stack->capacity];
    stack->top = -1;
    return stack;
}

void deleteStack(Stack* stack) {
    if (stack) {
        delete[] stack->data;
        delete stack;
    }
}

void resizeStack(Stack* stack) {
    if (stack->top >= stack->capacity - 1) {
        int newCapacity = stack->capacity * 2;
        string* newData = new string[newCapacity];
        for (int i = 0; i <= stack->top; ++i) newData[i] = stack->data[i];
        delete[] stack->data;
        stack->data = newData;
        stack->capacity = newCapacity;
    }
}

void push(Stack* stack, const string& value) {
    resizeStack(stack);
    stack->data[++stack->top] = value;
}

string pop(Stack* stack) {
    if (isEmpty(stack)) return "EMPTY";
    return stack->data[stack->top--];
}

string peek(Stack* stack) {
    if (isEmpty(stack)) return "EMPTY";
    return stack->data[stack->top];
}

bool isEmpty(Stack* stack) {
    return stack->top == -1;
}

void printStack(Stack* stack) {
    if (isEmpty(stack)) { cout << "empty" << endl; return; }
    // Вывод от дна к вершине (или наоборот, как удобно)
    for (int i = 0; i <= stack->top; ++i) cout << stack->data[i] << " ";
    cout << endl;
}
