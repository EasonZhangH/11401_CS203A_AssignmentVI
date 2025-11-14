/*
   ========================================
   hash_fn.cpp — implement your hash functions
   ========================================

   Description:
    This file contains the hash functions for integer and string keys.

   Development History:
    - 2025/11/11: Initial implementation

   Developer: Yu-Feng Huang <yfhuang@saturn.yzu.edu.tw>
 */
#include <string>

int myHashInt(int key, int m) {
    // TODO: replace with your own design
    if (m <= 0) return 0;

    // 2. 計算原本邏輯下的INDEX
    int index = key % m;

    // 3. 處理負數情況：如果INDEX是負的，將其調整到 [0, m-1] 的範圍內。
    return (index < 0) ? (index + m) : index;
}

int myHashString(const std::string& str, int m) {
    unsigned long hash = 0;
    // TODO: replace with your own design
    //return static_cast<int>(hash % m);  // basic division method

    if ( m <= 0 || str.empty() )
        return 0; // edge case 

    for (char c : str)
        // 將字元轉換為 1-26 的數字 (假設只有小寫字母)
        hash = (hash + (c - 'a' + 1) ) % m;

    //確保結果為正數並回傳
    return static_cast<int>(hash);
}
