#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include "Array/Array.h"
#include "LinkedList/SinglyLinkedList.h"
#include "DoubleLinkedList/DoubleLinkedList.h"
#include "Stack/Stack.h"
#include "Queue/Queue.h"
#include "FullBinaryTree/FullBinaryTree.h"
#include "HashSet/HashSet.h"

using namespace std;

// Глобальные переменные
Array myArray;
SinglyLinkedList mySList;
DoublyList myDList;
Stack* myStack = nullptr;
Queue* myQueue = nullptr;
BinaryTree myTree;
HashSet* mySet = nullptr;

// --- ФУНКЦИИ ДЛЯ КАЛЬКУЛЯТОРА (ЗАДАНИЕ 1) ---
bool isOperator(const string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/";
}

long long performOp(const string& op, long long a, long long b) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") return b != 0 ? a / b : 0;
    return 0;
}

void runRPN(const string& expression) {
    Stack* rpnStack = createStack(); 

    stringstream ss(expression);
    string token;
    bool error = false;

    while (ss >> token) {
        if (isOperator(token)) {
            if (rpnStack->top == -1) { cout << "Error: Invalid Expression" << endl; error = true; break; }
            long long val2 = stoll(pop(rpnStack)); 

            if (rpnStack->top == -1) { cout << "Error: Invalid Expression" << endl; error = true; break; }
            long long val1 = stoll(pop(rpnStack)); 

            long long res = performOp(token, val1, val2);
            push(rpnStack, to_string(res)); 
        } else {
            push(rpnStack, token);
        }
    }

    if (!error) {
        if (rpnStack->top == 0) {
            cout << "Result: " << pop(rpnStack) << endl;
        } else {
            cout << "Error: Too many operands" << endl;
        }
    }
    
    delete[] rpnStack->data;
    delete rpnStack;
}

// --- ИНИЦИАЛИЗАЦИЯ И ОЧИСТКА ---
void initGlobals() {
    arrInit(myArray); 
    myStack = createStack();
    myQueue = createQueue();
    // mySet инициализируется отдельно в main
}

void cleanGlobals() {
    arrFree(myArray);
    deleteStack(myStack);
    deleteQueue(myQueue);
    // mySet удаляется отдельно
}

// --- СОХРАНЕНИЕ И ЗАГРУЗКА (ОБЩИЕ) ---
void saveTreeRec(TNode* node, ofstream& file) {
    if (!node) return;
    // Используем Pre-Order для сохранения (чтобы потом восстановить структуру)
    file << node->value << " "; 
    saveTreeRec(node->left, file);
    saveTreeRec(node->right, file);
}
void saveData() {
    ofstream file("file.data");
    if (!file.is_open()) return;

    // Array
    file << "M ";
    for(int i=0; i<myArray.size; i++) file << myArray.data[i] << " ";
    file << endl;

    // Singly List
    file << "F ";
    FNode* fcurr = mySList.head;
    while(fcurr) { file << fcurr->key << " "; fcurr = fcurr->next; }
    file << endl;

    // Doubly List
    file << "L ";
    LNode* lcurr = myDList.head;
    while(lcurr) { file << lcurr->data << " "; lcurr = lcurr->next; }
    file << endl;

    // Stack
    file << "S ";
    if (myStack) {
        for(int i=0; i<=myStack->top; i++) file << myStack->data[i] << " ";
    }
    file << endl;

    // Queue
    file << "Q ";
    if (myQueue) {
        QNode* qcurr = myQueue->front;
        while(qcurr) { file << qcurr->data << " "; qcurr = qcurr->next; }
    }
    file << endl;
    // TREE
    file << "T ";
    saveTreeRec(myTree.root, file);
    file << endl;
    
    file.close();
}

void loadData() {
    ifstream file("file.data");
    if (!file.is_open()) return;

    string line;
    while(getline(file, line)) {
        if(line.empty()) continue;
        
        char* buf = new char[line.length() + 1];
        strcpy(buf, line.c_str());
        
        char* type = strtok(buf, " ");
        if(!type) { delete[] buf; continue; }

        char* val = strtok(NULL, " ");
        while(val) {
            string sVal = val;
            if (strcmp(type, "M") == 0) arrPush(myArray, sVal);
            else if (strcmp(type, "F") == 0) FPUSH_TAIL(mySList, sVal);
            else if (strcmp(type, "L") == 0) LADD_TAIL(myDList, sVal);
            else if (strcmp(type, "S") == 0) push(myStack, sVal);
            else if (strcmp(type, "Q") == 0) push(myQueue, sVal);
            else if (strcmp(type, "T") == 0) TINSERT(myTree, sVal);
            
            val = strtok(NULL, " ");
        }
        delete[] buf;
    }
    file.close();
}

// --- СОХРАНЕНИЕ И ЗАГРУЗКА (ДЛЯ ЗАДАНИЯ 2 - HashSet) ---
void loadSetFromFile(HashSet* set, const string& filename) {
    ifstream file(filename);
    string key;
    if (file.is_open()) {
        while (file >> key) {
            setAdd(set, key);
        }
        file.close();
    }
}

void saveSetToFile(HashSet* set, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < set->capacity; i++) {
            HashNode* current = set->buckets[i];
            while (current != nullptr) {
                file << current->key << endl;
                current = current->next;
            }
        }
        file.close();
    }
}

// задание 3
int* partNumbers = nullptr;
int partCount = 0;
bool* partUsed = nullptr;

struct Subset {
    int* data;
    int size;
    int capacity;
};
Subset* currentSubsets = nullptr;
int globalTargetSum = 0;

void sortNumbersDesc(int* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] < arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void addToSubset(int subsetIdx, int val) {
    Subset& s = currentSubsets[subsetIdx];
    s.data[s.size++] = val;
}

void popFromSubset(int subsetIdx) {
    if (currentSubsets[subsetIdx].size > 0) {
        currentSubsets[subsetIdx].size--;
    }
}

bool canPartition(int k, int currentSum, int startDateIndex, int subsetIndex) {
    if (k == 1) return true;
    
    if (currentSum == globalTargetSum) {
        return canPartition(k - 1, 0, 0, subsetIndex + 1);
    }
    
    for (int i = startDateIndex; i < partCount; i++) {
        if (!partUsed[i] && currentSum + partNumbers[i] <= globalTargetSum) {
            partUsed[i] = true;
            addToSubset(subsetIndex, partNumbers[i]);
            
            if (canPartition(k, currentSum + partNumbers[i], i + 1, subsetIndex)) return true;
            
            partUsed[i] = false;
            popFromSubset(subsetIndex);
        }
    }
    return false;
}

void solvePartition() {
    int count = 0;
    for (int i = 0; i < mySet->capacity; i++) {
        HashNode* curr = mySet->buckets[i];
        while (curr) { count++; curr = curr->next; }
    }

    if (count == 0) { cout << "Error: Set is empty" << endl; return; }

    if (partNumbers) delete[] partNumbers;
    partNumbers = new int[count];
    partCount = 0;

    int totalSum = 0;
    for (int i = 0; i < mySet->capacity; i++) {
        HashNode* curr = mySet->buckets[i];
        while (curr) {
            try {
                int val = stoi(curr->key);
                partNumbers[partCount++] = val;
                totalSum += val;
            } catch (...) {}
            curr = curr->next;
        }
    }

    sortNumbersDesc(partNumbers, partCount);

    if (partUsed) delete[] partUsed;
    partUsed = new bool[partCount];

    bool foundAny = false;

    for (int k = 2; k <= partCount; k++) {
        if (totalSum % k != 0) continue;
        
        globalTargetSum = totalSum / k;

        for(int i=0; i<partCount; i++) partUsed[i] = false;

        currentSubsets = new Subset[k];
        for(int i=0; i<k; i++) {
            currentSubsets[i].data = new int[partCount]; 
            currentSubsets[i].capacity = partCount;
            currentSubsets[i].size = 0;
        }
        
        if (canPartition(k, 0, 0, 0)) {
            foundAny = true;
            cout << "Found partition into " << k << " subsets (Sum=" << globalTargetSum << "):" << endl;
            
            for (int i = 0; i < partCount; i++) {
                if (!partUsed[i]) addToSubset(k-1, partNumbers[i]);
            }

            for (int i = 0; i < k; i++) {
                cout << "  Subset " << i+1 << ": { ";
                for (int j = 0; j < currentSubsets[i].size; j++) {
                    cout << currentSubsets[i].data[j] << " ";
                }
                cout << "}" << endl;
            }
        }
        
        for(int i=0; i<k; i++) delete[] currentSubsets[i].data;
        delete[] currentSubsets;
        currentSubsets = nullptr;
    }

    if (!foundAny) {
        cout << "Error: No partition possible" << endl;
    }
}

void printSubsets() {
    // Используем глобальный массив myArray
    if (myArray.size == 0) {
        cout << "Array is empty. Only subset: {}" << endl;
        return;
    }

    unsigned long long count = 1 << myArray.size;

    cout << "All distinct subsets (" << count << "):" << endl;

    for (unsigned long long i = 0; i < count; i++) {
        cout << "{ ";
        for (int j = 0; j < myArray.size; j++) {
            // Проверяем j-й бит числа i
            if ((i >> j) & 1) {
                cout << myArray.data[j] << " ";
            }
        }
        cout << "}" << endl;
    }
}

TNode** inOrderNodes = nullptr;
int inOrderCount = 0;
int inOrderCapacity = 100;

void traverseInOrder(TNode* node) {
    if (!node) return;
    traverseInOrder(node->left);
    
    // Добавляем узел в массив
    if (inOrderCount >= inOrderCapacity) {
        inOrderCapacity *= 2;
        TNode** temp = new TNode*[inOrderCapacity];
        for(int i=0; i<inOrderCount; i++) temp[i] = inOrderNodes[i];
        delete[] inOrderNodes;
        inOrderNodes = temp;
    }
    inOrderNodes[inOrderCount++] = node;

    traverseInOrder(node->right);
}

void recoverTree() {
    if (!myTree.root) {
        cout << "Tree is empty" << endl;
        return;
    }

    if (inOrderNodes) delete[] inOrderNodes;
    inOrderCapacity = 100;
    inOrderNodes = new TNode*[inOrderCapacity];
    inOrderCount = 0;

    traverseInOrder(myTree.root);

    TNode* first = nullptr;
    TNode* second = nullptr;
    
    for (int i = 0; i < inOrderCount - 1; i++) {
        bool violation = false;
        try {
            if (stoi(inOrderNodes[i]->value) > stoi(inOrderNodes[i+1]->value)) violation = true;
        } catch (...) {
            if (inOrderNodes[i]->value > inOrderNodes[i+1]->value) violation = true;
        }

        if (violation) {
            if (first == nullptr) {
                first = inOrderNodes[i];
                second = inOrderNodes[i+1]; 
            } else {
                second = inOrderNodes[i+1];
            }
        }
    }

    if (first && second) {
        cout << "Swapped nodes found: " << first->value << " and " << second->value << endl;
        string temp = first->value;
        first->value = second->value;
        second->value = temp;
        cout << "Tree fixed!" << endl;
    } else {
        cout << "Tree is already correct." << endl;
    }
}
void printTreeInOrder(TNode* node) {
    if (!node) return;
    printTreeInOrder(node->left);
    cout << node->value << " ";
    printTreeInOrder(node->right);
}
string intToRoman(int num) {
    if (num <= 0) return "Invalid number";
    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const string symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    
    string result = "";
    for (int i = 0; i < 13; i++) {
        while (num >= values[i]) {
            num -= values[i];
            result += symbols[i];
        }
    }
    return result;
}
// 7 задание
struct CacheNode {
    int key;
    int value;
    CacheNode* prev;
    CacheNode* next;
    CacheNode(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

// Простой LRU кэш
struct LRUCache {
    int capacity;
    int size;
    CacheNode* head;
    CacheNode* tail;

    LRUCache(int cap) : capacity(cap), size(0), head(nullptr), tail(nullptr) {}

    // Перемещение узла в голову (он стал самым свежим)
    void moveToHead(CacheNode* node) {
        if (node == head) return;

        // Отцепляем
        if (node->prev) node->prev->next = node->next;
        if (node->next) node->next->prev = node->prev;
        
        if (node == tail && node->prev) tail = node->prev;

        // Прицепляем в начало
        node->next = head;
        node->prev = nullptr;
        if (head) head->prev = node;
        head = node;
        if (!tail) tail = head;
    }

    // Удаление хвоста (самого старого)
    void removeTail() {
        if (!tail) return;
        CacheNode* temp = tail;
        if (tail->prev) {
            tail = tail->prev;
            tail->next = nullptr;
        } else {
            head = tail = nullptr;
        }
        delete temp;
        size--;
    }

    int get(int key) {
        CacheNode* curr = head;
        while (curr) {
            if (curr->key == key) {
                moveToHead(curr);
                return curr->value;
            }
            curr = curr->next;
        }
        return -1;
    }

    void put(int key, int value) {
        // Проверяем наличие
        CacheNode* curr = head;
        while (curr) {
            if (curr->key == key) {
                curr->value = value;
                moveToHead(curr);
                return;
            }
            curr = curr->next;
        }

        // Если не нашли
        if (size >= capacity) {
            removeTail();
        }

        CacheNode* newNode = new CacheNode(key, value);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        size++;
    }
};

LRUCache* myCache = nullptr;

// === ВЫВОД СПРАВКИ ===
void printHelp() {
    cout << "=== ИНТЕРФЕЙС СТРУКТУР ДАННЫХ (LAB 1 & LAB 2) ===" << endl << endl;

    cout << "--- МАССИВ (M) ---" << endl;
    cout << "  MPUSH F <value>    : Добавить в начало" << endl;
    cout << "  MPUSH B <value>    : Добавить в конец" << endl;
    cout << "  MPOP F             : Удалить с начала" << endl;
    cout << "  MPOP B             : Удалить с конца" << endl;
    cout << "  MPRINT             : Показать массив" << endl << endl;

    cout << "--- ОДНОСВЯЗНЫЙ СПИСОК (F) ---" << endl;
    cout << "  FPUSH F <value>    : Добавить в начало" << endl;
    cout << "  FPUSH B <value>    : Добавить в конец" << endl;
    cout << "  FPOP F             : Удалить с начала" << endl;
    cout << "  FPOP B             : Удалить с конца" << endl;
    cout << "  FPRINT             : Показать список" << endl << endl;

    cout << "--- ДВУСВЯЗНЫЙ СПИСОК (L) ---" << endl;
    cout << "  LPUSH F <value>    : Добавить в начало" << endl;
    cout << "  LPUSH B <value>    : Добавить в конец" << endl;
    cout << "  LPOP F             : Удалить с начала" << endl;
    cout << "  LPOP B             : Удалить с конца" << endl;
    cout << "  LPRINT F           : Показать (прямой порядок)" << endl;
    cout << "  LPRINT B           : Показать (обратный порядок)" << endl << endl;

    cout << "--- СТЕК (S) ---" << endl;
    cout << "  SPUSH <value>      : Положить (Push)" << endl;
    cout << "  SPOP               : Взять (Pop)" << endl;
    cout << "  SPEEK              : Посмотреть вершину" << endl;
    cout << "  SPRINT             : Показать стек" << endl << endl;

    cout << "--- ОЧЕРЕДЬ (Q) ---" << endl;
    cout << "  QPUSH <value>      : Встать в очередь" << endl;
    cout << "  QPOP               : Выйти из очереди" << endl;
    cout << "  QFRONT             : Посмотреть первого" << endl;
    cout << "  QPRINT             : Показать очередь" << endl << endl;

    cout << "--- ДЕРЕВО (T) ---" << endl;
    cout << "  TINSERT <val>      : Вставить элемент" << endl;
    cout << "  TCOMPLETE          : Проверить на полноту" << endl;
    cout << "  TPRINT             : Показать дерево" << endl << endl;

    cout << "--- ЗАДАНИЕ 1: КАЛЬКУЛЯТОР ОПЗ ---" << endl;
    cout << "  CALC_RPN <expr>    : Вычислить (напр: '3 4 + 2 *')" << endl << endl;

    cout << "--- ЗАДАНИЕ 2: МНОЖЕСТВО (SET) ---" << endl;
    cout << "  SETADD <value>     : Добавить элемент" << endl;
    cout << "  SETDEL <value>     : Удалить элемент" << endl;
    cout << "  SET_AT <value>     : Проверить наличие (YES/NO)" << endl << endl;

    cout << "--- ОБЩИЕ ---" << endl;
    cout << "  CLEAR              : Очистить все данные" << endl;
    cout << "  EXIT               : Выход" << endl;
    cout << "=================================================" << endl;
}


int main(int argc, char* argv[]) {
    // 1. Инициализация всего
    initGlobals();
    mySet = createHashSet(100);

    // 2. Поиск имени файла для множества
    string dataFileName = "set.data"; // Имя по умолчанию
    for (int i = 0; i < argc; i++) {
        if (string(argv[i]) == "--file" && i + 1 < argc) {
            dataFileName = argv[i + 1];
        }
    }

    // 3. Загрузка данных
    loadData();                  // Общие структуры
    loadSetFromFile(mySet, dataFileName); // Множество

    // 4. Парсинг аргументов командной строки
    char* myArgs[10]; 
    int argsCount = 0;
    for(int k=0; k<10; k++) myArgs[k] = nullptr;

    for (int i = 1; i < argc; ++i) {
        // Игнорируем флаг --file и его значение
        if (strcmp(argv[i], "--file") == 0) {
            i++; 
            continue;
        }

        if (strcmp(argv[i], "--query") == 0) {
            if (i + 1 < argc) {
                char* q = argv[++i];
                // Создаем копию строки, чтобы strtok не портил argv
                char* buf = new char[strlen(q) + 1];
                strcpy(buf, q);
                
                char* token = strtok(buf, " ");
                while (token && argsCount < 10) {
                    myArgs[argsCount] = new char[strlen(token) + 1];
                    strcpy(myArgs[argsCount], token);
                    argsCount++;
                    token = strtok(NULL, " ");
                }
                delete[] buf; 
            }
        } 
    }

    // Если команд нет - показываем помощь и выходим
    if (argsCount == 0) {
        printHelp();
        cleanGlobals();
        deleteHashSet(mySet);
        return 0;
    }

    string command = (myArgs[0] ? myArgs[0] : "");
    string arg1 = (argsCount > 1 && myArgs[1]) ? myArgs[1] : "";
    string arg2 = (argsCount > 2 && myArgs[2]) ? myArgs[2] : "";
    string val = (!arg2.empty()) ? arg2 : arg1;

    // --- ВЫПОЛНЕНИЕ КОМАНД ---

    // ARRAY
    if (command == "MPUSH") {
        if (arg1 == "F") { /* arrPushFront */ } 
        else arrPush(myArray, val);
        cout << "-> " << val << endl;
    }
    else if (command == "MPOP") {
        if (arg1 == "F") arrPopFront(myArray);
        else arrPopBack(myArray);
        cout << "-> POPPED" << endl;
    }
    else if (command == "ARRAY_SUBSETS") {
        printSubsets();
    }
    else if (command == "MPRINT") { cout << "-> "; arrPrint(myArray); }
    
    // CALCULATOR RPN (LAB 2 TASK 1)
    else if (command == "CALC_RPN") {
        string expr = "";
        for(int k=1; k < argsCount; k++) {
            if (myArgs[k]) expr += string(myArgs[k]) + " ";
        }
        runRPN(expr);
    }

    // SINGLY LIST
    else if (command == "FPUSH") {
        if (arg1 == "B") FPUSH_TAIL(mySList, val);
        else FPUSH_HEAD(mySList, val);
        cout << "-> " << val << endl;
    }
    else if (command == "FPOP") {
        if (arg1 == "B") FDEL_TAIL(mySList);
        else FDEL_HEAD(mySList);
        cout << "-> POPPED" << endl;
    }
    else if (command == "FPRINT") { cout << "-> "; FPRINT(mySList); }

    // DOUBLY LIST
    else if (command == "LPUSH") {
        if (arg1 == "F") LADD_HEAD(myDList, val);
        else LADD_TAIL(myDList, val);
        cout << "-> " << val << endl;
    }
    else if (command == "LPOP") {
        if (arg1 == "B") LDEL_TAIL(myDList);
        else LDEL_HEAD(myDList);
        cout << "-> POPPED" << endl;
    }
    else if (command == "LPRINT") {
        if (arg1 == "B") { cout << "-> "; LPRINT_B(myDList); }
        else { cout << "-> "; LPRINT_F(myDList); }
    }

    // STACK
    else if (command == "SPUSH") { push(myStack, val); cout << "-> " << val << endl; }
    else if (command == "SPOP") { cout << "-> " << pop(myStack) << endl; }
    else if (command == "SPEEK") { cout << "-> " << peek(myStack) << endl; }
    else if (command == "SPRINT") { cout << "-> "; printStack(myStack); }

    // QUEUE
    else if (command == "QPUSH") { push(myQueue, val); cout << "-> " << val << endl; }
    else if (command == "QPOP") { cout << "-> " << pop(myQueue) << endl; }
    else if (command == "QFRONT") { cout << "-> " << front(myQueue) << endl; }
    else if (command == "QPRINT") { cout << "-> "; printQueue(myQueue); }

    // TREE
    else if (command == "TINSERT") { TINSERT(myTree, val); cout << "-> " << val << endl; }
    else if (command == "TCOMPLETE") { cout << "-> " << (TCHECK(myTree) ? "TRUE" : "FALSE") << endl; }
    else if (command == "TPRINT") { cout << "-> "; TPRINT(myTree); }
    else if (command == "TREE_FIX") {
        recoverTree();
    }
    else if (command == "TREE_BREAK") {
        if (myTree.root && myTree.root->left) {
            string temp = myTree.root->value;
            myTree.root->value = myTree.root->left->value;
            myTree.root->left->value = temp;
            cout << "Tree broken (swapped root and left child)" << endl;
        } else {
            cout << "Cannot break (need root and left child)" << endl;
        }
    }
    else if (command == "MY_TPRINT") {
        printTreeInOrder(myTree.root);
        cout << endl;
    }


    // HASH SET (LAB 2 TASK 2)
    else if (command == "SETADD") {
        if (argsCount >= 2) {
            setAdd(mySet, myArgs[1]);
        } else {
            cout << "Error: No value provided" << endl;
        }
    }
    else if (command == "SETDEL") {
        if (argsCount >= 2) {
            setDel(mySet, myArgs[1]);
        } else {
            cout << "Error: No value provided" << endl;
        }
    }
    else if (command == "SET_AT") {
        if (argsCount >= 2) {
            if (setAt(mySet, myArgs[1])) {
                cout << "YES" << endl;
            } else {
                cout << "NO" << endl;
            }
        } else {
            cout << "Error: No value provided" << endl;
        }
    }
    // task 3
    else if (command == "SET_PARTITION") {
        solvePartition();
    }
    // task 6
    else if (command == "CALC_ROMAN" || (argsCount > 0 && string(myArgs[0]) == "CALC_ROMAN")) {
        int valIdx = -1;
        
        // Определяем, где лежит число
        if (argsCount > 0 && string(myArgs[0]) != "CALC_ROMAN") {
            valIdx = 0;
        } else if (argsCount > 1 && string(myArgs[0]) == "CALC_ROMAN") {
            valIdx = 1;
        }

        if (valIdx != -1) {
            try {
                int val = stoi(myArgs[valIdx]);
                cout << "Roman: " << intToRoman(val) << endl;
            } catch (...) {
                cout << "Error: Invalid number format" << endl;
            }
        } else {
            cout << "Error: No number provided" << endl;
        }
    }
    // 7 задание
    else if (command == "LRU_INIT" || (argsCount > 0 && string(myArgs[0]) == "LRU_INIT")) {
        // Пытаемся найти число в аргументах
        int cap = -1;
        for(int i=0; i<argsCount; i++) {
            try { cap = stoi(myArgs[i]); break; } catch(...) {}
        }
        
        if (cap > 0) {
            myCache = new LRUCache(cap);
            cout << "LRU Init: " << cap << endl;
        } else {
             cout << "Error: No capacity found in args. Args count: " << argsCount << endl;
        }
    }
    else if (command == "LRU_PUT" || (argsCount > 0 && string(myArgs[0]) == "LRU_PUT")) {
        int k = -1, v = -1;
        int found = 0;
        for(int i=0; i<argsCount; i++) {
            try { 
                int val = stoi(myArgs[i]); 
                if (found == 0) k = val;
                else if (found == 1) v = val;
                found++;
            } catch(...) {}
        }

        if (myCache && found >= 2) {
            myCache->put(k, v);
            cout << "SET " << k << " -> " << v << endl;
        } else {
            cout << "Error: Need 2 ints. Found: " << found << endl;
        }
    }
    else if (command == "LRU_GET" || (argsCount > 0 && string(myArgs[0]) == "LRU_GET")) {
        int k = -1;
        int found = 0;
        for(int i=0; i<argsCount; i++) {
             try { 
                int val = stoi(myArgs[i]); 
                k = val;
                found++;
                break; // берем первое число
            } catch(...) {}
        }

        if (myCache && found > 0) {
            cout << "GET " << k << ": " << myCache->get(k) << endl;
        } else {
            cout << "Error: Need key. Found: " << found << endl;
        }
    }
    // COMMON
    else if (command == "HELP") { printHelp(); }
    else if (command == "CLEAR") { 
        ofstream file("file.data"); file.close();
        ofstream fileSet(dataFileName); fileSet.close(); // Очищаем и файл сета
        cout << "-> CLEARED" << endl;
    }
    else if (command == "EXIT") { /* ничего не делаем, просто выходим */ }
    
    // 5. Сохранение и очистка
    saveData();
    saveSetToFile(mySet, dataFileName);

    for(int k=0; k<argsCount; k++) delete[] myArgs[k];
    
    cleanGlobals();
    deleteHashSet(mySet);

    return 0;
}
