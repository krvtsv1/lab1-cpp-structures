#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include "Array/Array.h"
#include "LinkedList/SinglyLinkedList.h"
#include "DoubleLinkedList/DoubleLinkedList.h"
#include "Stack/Stack.h"
#include "Queue/Queue.h"
#include "FullBinaryTree/FullBinaryTree.h"

using namespace std;

// Глобальные переменные
Array myArray;
SinglyLinkedList mySList;
DoublyList myDList;
Stack* myStack = nullptr;
Queue* myQueue = nullptr;
BinaryTree myTree;

void initGlobals() {
    arrInit(myArray); 
    myStack = createStack();
    myQueue = createQueue();
}

void cleanGlobals() {
    arrFree(myArray);
    deleteStack(myStack);
    deleteQueue(myQueue);
}

// === СОХРАНЕНИЕ И ЗАГРУЗКА ===
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
    for(int i=0; i<=myStack->top; i++) file << myStack->data[i] << " ";
    file << endl;

    // Queue
    file << "Q ";
    QNode* qcurr = myQueue->front;
    while(qcurr) { file << qcurr->data << " "; qcurr = qcurr->next; }
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
            // Tree load skip for simplicity
            
            val = strtok(NULL, " ");
        }
        delete[] buf;
    }
    file.close();
}

// === ВЫВОД СПРАВКИ ===
void printHelp() {
    cout << "=== Интерфейс для работы со структурами данных ===" << endl << endl;
    cout << "=== СПРАВКА ПО КОМАНДАМ ===" << endl << endl;

    cout << "МАССИВ (M):" << endl;
    cout << "  MCREATE            - создать массив" << endl;
    cout << "  MPUSH B <value>    - добавить в конец" << endl;
    cout << "  MPUSH F <value>    - добавить в начало" << endl;
    cout << "  MPOP B             - удалить с конца" << endl;
    cout << "  MPOP F             - удалить с начала" << endl;
    cout << "  MPRINT             - вывести массив" << endl << endl;

    cout << "ОДНОСВЯЗНЫЙ СПИСОК (F):" << endl;
    cout << "  FCREATE            - создать список" << endl;
    cout << "  FPUSH B <value>    - добавить в хвост" << endl;
    cout << "  FPUSH F <value>    - добавить в голову" << endl;
    cout << "  FPOP B             - удалить хвост" << endl;
    cout << "  FPOP F             - удалить голову" << endl;
    cout << "  FPRINT             - вывести список" << endl << endl;

    cout << "ДВУСВЯЗНЫЙ СПИСОК (L):" << endl;
    cout << "  LCREATE            - создать список" << endl;
    cout << "  LPUSH B <value>    - добавить в хвост" << endl;
    cout << "  LPUSH F <value>    - добавить в голову" << endl;
    cout << "  LPOP B             - удалить хвост" << endl;
    cout << "  LPOP F             - удалить голову" << endl;
    cout << "  LPRINT F           - вывести список (вперед)" << endl;
    cout << "  LPRINT B           - вывести список (назад)" << endl << endl;

    cout << "СТЕК (S):" << endl;
    cout << "  SCREATE            - создать стек" << endl;
    cout << "  SPUSH <value>      - положить в стек" << endl;
    cout << "  SPOP               - взять из стека" << endl;
    cout << "  SPEEK              - посмотреть вершину" << endl;
    cout << "  SPRINT             - вывести стек" << endl << endl;

    cout << "ОЧЕРЕДЬ (Q):" << endl;
    cout << "  QCREATE            - создать очередь" << endl;
    cout << "  QPUSH <value>      - добавить в очередь" << endl;
    cout << "  QPOP               - взять из очереди" << endl;
    cout << "  QFRONT             - посмотреть первый элемент" << endl;
    cout << "  QPRINT             - вывести очередь" << endl << endl;

    cout << "ПОЛНОЕ БИНАРНОЕ ДЕРЕВО (T):" << endl;
    cout << "  TCREATE            - создать дерево" << endl;
    cout << "  TINSERT <number>   - добавить элемент" << endl;
    cout << "  TFIND <number>     - найти элемент" << endl;
    cout << "  TCOMPLETE          - проверить на полноту" << endl;
    cout << "  TPRINT             - вывести дерево" << endl << endl;

    cout << "ОБЩИЕ КОМАНДЫ:" << endl;
    cout << "  HELP               - показать эту справку" << endl;
    cout << "  CLEAR              - очистить все структуры" << endl;
    cout << "  EXIT               - выход из программы" << endl;
    cout << "==========================================================" << endl;
}

int main(int argc, char* argv[]) {
    initGlobals();
    loadData(); // Загружаем состояние

    // Парсинг аргументов
    char* myArgs[10]; 
    int argsCount = 0;
    for(int k=0; k<10; k++) myArgs[k] = nullptr;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--query") == 0) {
            if (i + 1 < argc) {
                char* q = argv[++i];
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
        } else {
            if(argsCount < 10) {
                myArgs[argsCount] = new char[strlen(argv[i]) + 1];
                strcpy(myArgs[argsCount], argv[i]);
                argsCount++;
            }
        }
    }

    if (argsCount == 0) {
        printHelp();
        // При простом вызове справки не сохраняем (чтобы не портить файл)
        cleanGlobals();
        return 0;
    }

    string command = (myArgs[0] ? myArgs[0] : "");
    string arg1 = (argsCount > 1 && myArgs[1]) ? myArgs[1] : "";
    string arg2 = (argsCount > 2 && myArgs[2]) ? myArgs[2] : "";
    string val = (!arg2.empty()) ? arg2 : arg1;

    // --- ARRAY ---
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
    else if (command == "MPRINT") { cout << "-> "; arrPrint(myArray); }

    // --- SINGLY LIST ---
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

    // --- DOUBLY LIST ---
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

    // --- STACK ---
    else if (command == "SPUSH") { push(myStack, val); cout << "-> " << val << endl; }
    else if (command == "SPOP") { cout << "-> " << pop(myStack) << endl; }
    else if (command == "SPEEK") { cout << "-> " << peek(myStack) << endl; }
    else if (command == "SPRINT") { cout << "-> "; printStack(myStack); }

    // --- QUEUE ---
    else if (command == "QPUSH") { push(myQueue, val); cout << "-> " << val << endl; }
    else if (command == "QPOP") { cout << "-> " << pop(myQueue) << endl; }
    else if (command == "QFRONT") { cout << "-> " << front(myQueue) << endl; }
    else if (command == "QPRINT") { cout << "-> "; printQueue(myQueue); }

    // --- TREE ---
    else if (command == "TINSERT") { TINSERT(myTree, val); cout << "-> " << val << endl; }
    else if (command == "TCOMPLETE") { cout << "-> " << (TCHECK(myTree) ? "TRUE" : "FALSE") << endl; }
    else if (command == "TPRINT") { cout << "-> "; TPRINT(myTree); }
    
    // --- COMMON ---
    else if (command == "HELP") { printHelp(); }
    else if (command == "CLEAR") { 
        // Очищаем файл и память
        ofstream file("file.data"); file.close();
        cout << "-> CLEARED" << endl;
    }
    else if (command == "EXIT") { return 0; }

    saveData(); // Сохраняем состояние

    for(int k=0; k<argsCount; k++) delete[] myArgs[k];
    cleanGlobals();
    return 0;
}
