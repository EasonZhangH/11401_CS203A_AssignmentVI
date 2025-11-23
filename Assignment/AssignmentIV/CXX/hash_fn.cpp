/*
   ========================================
   hash_fn.cpp — implement your hash functions
   ========================================

   Description:
    This file contains the hash functions for integer and string keys.

   Development History:
    - 2025/11/13: Initial implementation and completed the first version.
    - 2025/11/14: Finished the currently final hash_func and updated the C++ version.
    - 2025/11/17: Wrote the README.

   Developer: Eason Zhang(張嘉祥) <s1121432@mail.yzu.edu.tw>
 */
#include <string>

int myHashInt(int key, int m) {
    // TODO: replace with your own design
    if (m <= 0) return 0;

    unsigned int x = static_cast<unsigned int>(key);
    x = x * 2654435761u;
    x ^= (x >> 16);

    // 2. 計算原本邏輯下的INDEX
    int index = x % m;

    // 3. 處理負數情況：如果INDEX是負的，將其調整到 [0, m-1] 的範圍內。
    //return (index < 0) ? (index + m) : index;
    return static_cast<int>(index);
}

int myHashString(const std::string& str, int m) {
    unsigned long hash = 0;
    // TODO: replace with your own design
    //return static_cast<int>(hash % m);  // basic division method

    if ( m <= 0 || str.empty() )
        return 0; // edge case 

    for (char c : str) {
        //65599 是一個經過大量統計測試後的一個好用的常數，經過了大量測試
        // hash = c + hash * 64 + hash * 65536 - hash
        // hash = c + hash * (65536 + 64 - 1) = c + hash * 65599
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash % m;
}