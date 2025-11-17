/*
   ========================================
   hash_fn.c — implement your hash functions
   ========================================

   Description:
    This file contains the hash functions for integer and string keys.

   Development History:
    - 2025/11/13: Initial implementation and completed the first version.
    - 2025/11/14: Finished the currently final hash_func and updated the C++ version.
    - 2025/11/17: Wrote the README.

   Developer: Eason Zhang(張嘉祥) <s1121432@mail.yzu.edu.tw>
 */
#include "hash_fn.h"
#include <string.h>

int myHashInt(int key, int m) {
    //return key % m;  // division method example

    /* 第一個版本 - 改良原先的邏輯 */
    // 1. 處理邊界情況：如果雜湊表大小無效，回傳 0。
    if (m <= 0)
        return 0;
    
    // 使用 Knuth 建議的乘法雜湊常數
    unsigned int x = (unsigned int) key;
    x = x * 2654435761u;   // 乘法雜湊
    x ^= (x >> 16);        // 混合高位與低位

    // 2. 計算原本邏輯下的INDEX
    //int index = key % m;
    int index = (int)(x % m);

    // 3. 處理負數情況：如果INDEX是負的，將其調整到 [0, m-1] 的範圍內。
    // 例如，-1 % 10 = -1，調整後應為 9。
    //return (index < 0) ? (index + m) : index;
    return index;
}

int myHashString(const char* str, int m) {
    unsigned long hash = 0;
    // TODO: replace with your own design
    //return (int)(hash % m); // basic division method
    /* 第一個版本 - 基本處理*/

    // 1. 處理空字串 + edge case
    if (m <= 0 || str == NULL || str[0] == '\0') {
        return 0;
    }

    const unsigned long long base = 31; // base : 31 37 131

    // 3. 遍歷字串中的每個字元
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
         hash = (hash * base + (unsigned char)c);
    }

    // 4. 回傳結果 (確保是 int 範圍)
    return (int)(hash % m);
}
