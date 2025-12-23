#include <iostream>
#include "LRUCache.h" 

using namespace std;

int main() {
    cout << "LRU Test Start" << endl;
    LRUCache cache(2);

    cache.put(1, 10);
    cout << "Put(1, 10)" << endl;

    cache.put(2, 20);
    cout << "Put(2, 20)" << endl;

    cout << "Get(1): " << cache.get(1) << endl; // вернет 10

    cache.put(3, 30); // вытеснит 2
    cout << "Put(3, 30)" << endl;

    cout << "Get(2): " << cache.get(2) << endl; // вернет -1 (не найден)
    cout << "Get(3): " << cache.get(3) << endl; // вернет 30
    
    return 0;
}
