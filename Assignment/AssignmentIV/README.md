# 課堂作業四：雜湊函數設計與觀察 (C/C++ 版本)

This assignment focuses on the design and observation of hash functions using C/C++. 
Students are expected to implement and analyze the behavior of hash functions, 
evaluate their efficiency, and understand their applications in computer science. <br>

Developer: 張嘉祥 <br>

Email: s1121432 <br>

version : 288ccfd(string) & 00a2565 (integr) <br>

Build Environment
- Editor: Visual Studio Code  
- Operating System: Windows 10

## 簡單的前處理
### 編譯環境產生的問題
Added additional `.h` files so that the project can compile properly in the VSCode environment.

The `#include` statements in `main.cpp` have been updated to reference the `.h` versions accordingly.
  ```c
  #ifndef HASH_FN_H
  #define HASH_FN_H

  int myHashInt(int key, int m);
  int myHashString(const char *str, int m);

  #endif
  ```

  ```cxx
  #ifndef HASH_FUNC_H
  #define HASH_FUNC_H
  #include <string>

  int myHashInt(int key, int m);

  int myHashString(const std::string& str, int m);

  #endif
  ```


## 我的雜湊函數 (My Hash Function)

以下是三次實驗中使用的雜湊函數描述。

### 第一次雜湊函數 (First Hash Function)

#### 整數鍵 (Integer Keys)
- Formula / pseudocode:
  ```c
  // 第一次整數雜湊：純粹取餘數
  unsigned int hash_int_1(int key, unsigned int m) {
      return key % m;
  }
  ```
- Rationale: 這是最簡單的雜湊函數，直接使用模運算將鍵值映射到表格大小 $m$ 的範圍內。對於連續或等差數列的鍵值，會產生極差的分佈，碰撞率高。

#### 非整數鍵 (Non-integer Keys)
- Formula / pseudocode:
  ```c
  // 第一次字串雜湊：全部回傳 0
  unsigned int hash_str_1(const char* key, unsigned int m) {
      return 0;
  }
  ```
- Rationale: 這是最差的雜湊函數，所有鍵值都映射到索引 0，導致極高的碰撞率（除了第一個鍵值外，所有鍵值都會碰撞）。

### 第二次雜湊函數 (Second Hash Function)

#### 整數鍵 (Integer Keys)
- Formula / pseudocode:
  ```c
  // 第二次整數雜湊：邊界檢查與去負數
  unsigned int hash_int_2(int key, unsigned int m) {
      // 處理負數，確保結果為非負
      unsigned int abs_key = (key < 0) ? (unsigned int)(-key) : (unsigned int)key;
      return abs_key % m;
  }
  ```
- Rationale: 在第一次的基礎上增加了對負數的處理，確保雜湊結果在 $0$ 到 $m-1$ 之間。對於給定的正數測試集，其分佈與第一次相同，但提高了函數的健壯性。

#### 非整數鍵 (Non-integer Keys)
- Formula / pseudocode:
  ```c
  // 第二次字串雜湊：Polynomial Rolling Hash 的簡化版
  unsigned int hash_str_2(const char* key, unsigned int m) {
      unsigned long long hash = 0;
      for (int i = 0; key[i] != '\0'; i++) {
          // 假設 key 只有小寫英文字母
          hash = (hash + (unsigned long long)(key[i] - 'a' + 1)) % m;
      }
      return (unsigned int)hash;
  }
  ```
- Rationale: 採用一個簡化的多項式滾動雜湊（Polynomial Rolling Hash）概念，將每個字符轉換為一個數字（a=1, b=2, ...），並累加後取模。這比單純回傳 0 的方法有顯著改善，能根據字串內容產生不同的索引。

### 第三次雜湊函數 (Third Hash Function)

#### 整數鍵 (Integer Keys)
- Formula / pseudocode:
  ```c
  // 第三次整數雜湊：32-bit Knuth multiplication with XOR operation
  unsigned int hash_int_3(int key, unsigned int m) {
      // 32-bit Knuth multiplication constant (A = 2654435769)
      unsigned int A = 2654435769U; 
      unsigned int hash = (unsigned int)key;
      
      // 乘法與位移操作
      hash = hash * A;
      
      // 結合 XOR 增加混亂性
      hash = hash ^ (hash >> 16);
      
      // 取模
      return hash % m;
  }
  ```
- Rationale: 採用 Knuth 乘法雜湊（Multiplicative Hash）的變體，利用一個大質數常數 $A$ 進行乘法，並結合位移和 XOR 運算來最大化鍵值的位元差異，以達到更好的雪崩效應（Avalanche Effect）和均勻分佈。

#### 非整數鍵 (Non-integer Keys)
- Formula / pseudocode:
  ```c
  // 第三次字串雜湊：Polynomial Rolling Hash
  unsigned int hash_str_3(const char* key, unsigned int m) {
      unsigned long long hash = 0;
      unsigned long long p = 31; // 質數基數
      unsigned long long p_pow = 1;
      
      for (int i = 0; key[i] != '\0'; i++) {
          hash = (hash + (key[i] - 'a' + 1) * p_pow) % m;
          p_pow = (p_pow * p) % m;
      }
      return (unsigned int)hash;
  }
  ```
- Rationale: 採用標準的多項式滾動雜湊（Polynomial Rolling Hash），利用質數基數 $p$ 和模數 $m$ 來計算字串的雜湊值。這是一種廣泛使用的字串雜湊方法，能夠有效分散字串鍵值，顯著降低碰撞。

## 實驗設置 (Experimental Setup)
- Table sizes tested (m): 10, 11, 37
- Test dataset:
  - Integers: 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60
  - Strings: "cat", "dog", "bat", "cow", "ant", "owl", "bee", "hen", "pig", "fox"
- Compiler: GCC and G++
- Standard: C23 and C++23

## 結果 (Results)

### 整數鍵結果分析 (Integer Key Results Analysis)

| 雜湊版本 | 表格大小 (m) | 索引序列 (前10個) | 碰撞次數 (總共20個鍵) | 碰撞率 | 觀察與分析 |
| :---: | :---: | :---: | :---: | :---: | :--- |
| **第一次** | 10 | 1, 2, 3, 4, 5, 6, 7, 8, 9, 0... | 10 | 50.0% | 鍵值與索引呈線性關係，每 10 個鍵值重複一次，碰撞率極高。 |
| **(純取餘數)** | 11 | 10, 0, 1, 2, 3, 4, 5, 6, 7, 8... | 9 | 45.0% | 碰撞次數略有下降，但仍有規律性。 |
| | 37 | 21, 22, 23, 24, 25, 26, 27, 28, 29, 30... | 3 | 15.0% | 由於 $m=37$ 遠大於鍵值間隔，分佈較為均勻，碰撞次數最低。 |
| **第二次** | 10 | 1, 2, 3, 4, 5, 6, 7, 8, 9, 0... | 10 | 50.0% | 與第一次結果相同，因為測試鍵值皆為正數。 |
| **(邊界檢查)** | 11 | 10, 0, 1, 2, 3, 4, 5, 6, 7, 8... | 9 | 45.0% | 與第一次結果相同。 |
| | 37 | 21, 22, 23, 24, 25, 26, 27, 28, 29, 30... | 3 | 15.0% | 與第一次結果相同。 |
| **第三次** | 10 | 5, 2, 4, 9, 1, 6, 8, 3, 9, 8... | 10 | 50.0% | 雖然碰撞次數仍高，但索引分佈看起來更隨機，無明顯規律。 |
| **(Knuth 乘法)** | 11 | 3, 1, 4, 7, 8, 5, 8, 7, 6, 7... | 10 | 50.0% | 碰撞次數與前兩次相似，但分佈更為混亂，無規律性。 |
| | 37 | 29, 3, 33, 10, 2, 14, 10, 8, 6, 33... | 4 | 20.0% | 在 $m=37$ 時，分佈相對均勻，碰撞次數低，顯示出 Knuth 乘法雜湊的優勢。 |

### 字串鍵結果分析 (String Key Results Analysis)

| 雜湊版本 | 表格大小 (m) | 索引序列 (全部10個鍵) | 碰撞次數 (總共10個鍵) | 碰撞率 | 觀察與分析 |
| :---: | :---: | :---: | :---: | :---: | :--- |
| **第一次** | 10 | 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 | 9 | 90.0% | 所有鍵值都映射到 0，碰撞率最高。 |
| **(回傳 0)** | 11 | 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 | 9 | 90.0% | 所有鍵值都映射到 0，碰撞率最高。 |
| | 37 | 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 | 9 | 90.0% | 所有鍵值都映射到 0，碰撞率最高。 |
| **第二次** | 10 | 4, 6, 3, 1, 5, 0, 2, 7, 2, 5 | 2 | 20.0% | 碰撞次數顯著下降，僅 `bee` 和 `pig` 碰撞 (索引 2)，`ant` 和 `fox` 碰撞 (索引 5)。 |
| **(簡化滾動)** | 11 | 2, 4, 1, 8, 2, 6, 1, 5, 10, 1 | 3 | 30.0% | 碰撞次數略有增加，`cat` 和 `ant` 碰撞 (索引 2)，`bat`, `bee`, `fox` 碰撞 (索引 1)。 |
| | 37 | 24, 26, 23, 4, 35, 13, 12, 27, 32, 8 | 0 | 0.0% | 在 $m=37$ 時，所有鍵值均未碰撞，分佈極為均勻。 |
| **第三次** | 10 | 2, 4, 1, 9, 3, 8, 0, 5, 0, 3 | 2 | 20.0% | 碰撞次數低，`bee` 和 `pig` 碰撞 (索引 0)，`ant` 和 `fox` 碰撞 (索引 3)。 |
| **(標準滾動)** | 11 | 8, 4, 7, 8, 4, 1, 10, 3, 0, 1 | 3 | 30.0% | 碰撞次數低，`cat` 和 `cow` 碰撞 (索引 8)，`dog` 和 `ant` 碰撞 (索引 4)，`owl` 和 `fox` 碰撞 (索引 1)。 |
| | 37 | 31, 32, 1, 18, 9, 9, 29, 33, 13, 35 | 1 | 10.0% | 碰撞次數最低，僅 `ant` 和 `owl` 碰撞 (索引 9)。分佈極為優異。 |

## 編譯、建置、執行與輸出 (Compilation, Build, Execution, and Output)

### 編譯 (Compilation)
- Command for C:
  ```bash
  gcc -std=c23 -o hash_function main.c
  ```
- Command for C++:
  ```bash
  g++ -std=c++23 -o hash_function_cpp main.cpp
  ```

### 建置二進制檔 (Make Binary)
- Use the following `Makefile` to automate the build process:
  ```makefile
  all:
      gcc -std=c23 -o hash_function main.c
      g++ -std=c++23 -o hash_function_cpp main.cpp
  clean:
      rm -f hash_function hash_function_cpp
  ```

### 執行 (Execution)
- Run the compiled binary:
  ```bash
  ./hash_function
  ```
  or
  ```bash
  ./hash_function_cpp
  ```

### 結果快照 (Result Snapshot)
- Example output for integers:
  ```
  === Hash Function Observation (C Version) ===

  === Table Size m = 10 ===
  Key     Index
  -----------------
  21      5
  22      2
  23      4
  24      9
  25      1
  26      6
  27      8
  28      3
  29      9
  30      8
  51      6
  52      2
  53      9
  54      7
  55      0
  56      6
  57      3
  58      9
  59      6
  60      4
  ...

  === Table Size m = 11 ===
  Key     Index
  -----------------
  21      3
  22      1
  23      4
  24      7
  25      8
  26      5
  27      8
  28      7
  29      6
  30      7
  51      2
  52      10
  53      4
  54      0
  55      7
  56      3
  57      10
  58      7
  59      4
  60      1
  ...

  === Table Size m = 37 ===
  Key     Index
  -----------------
  21      29
  22      3
  23      33
  24      10
  25      2
  26      14
  27      10
  28      8
  29      6
  30      33
  51      2
  52      28
  53      27
  54      23
  55      15
  56      16
  57      7
  58      15
  59      26
  60      13
  ...

  === Hash Function Observation (C++ Version) ===

  === Table Size m = 10 ===
  Key     Index
  -----------------
  21      5
  22      2
  23      4
  24      9
  25      1
  26      6
  27      8
  28      3
  29      9
  30      8
  51      6
  52      2
  53      9
  54      7
  55      0
  56      6
  57      3
  58      9
  59      6
  60      4
  ...

  === Table Size m = 11 ===
  Key     Index
  -----------------
  21      3
  22      1
  23      4
  24      7
  25      8
  26      5
  27      8
  28      7
  29      6
  30      7
  51      2
  52      10
  53      4
  54      0
  55      7
  56      3
  57      10
  58      7
  59      4
  60      1
  ...

  === Table Size m = 37 ===
  Key     Index
  -----------------
  21      29
  22      3
  23      33
  24      10
  25      2
  26      14
  27      10
  28      8
  29      6
  30      33
  51      2
  52      28
  53      27
  54      23
  55      15
  56      16
  57      7
  58      15
  59      26
  60      13
  ...
  ```

- Example output for strings:
  ```
  === String Hash (m = 10) ===
  Key     Index
  -----------------
  cat     2
  dog     4
  bat     1
  cow     9
  ant     3
  owl     8
  bee     0
  hen     5
  pig     0
  fox     3
  ...

  === String Hash (m = 11) ===
  Key     Index
  -----------------
  cat     8
  dog     4
  bat     7
  cow     8
  ant     4
  owl     1
  bee     10
  hen     3
  pig     0
  fox     1
  ...

  === String Hash (m = 37) ===
  Key     Index
  -----------------
  cat     31
  dog     32
  bat     1
  cow     18
  ant     9
  owl     9
  bee     29
  hen     33
  pig     13
  fox     35
  ...
  ```
  === screen shot ===
  ![Integer 1](integer_1(CXX).png)
  
  ![Integer 2](integer_2(CXX).png)
  
  ![Integer 3](integer_3(CXX).png)
  
  ![String 1](string_1(CXX).png)
  
  ![String 2](string_2(CXX).png)
  
  ![String 3](string_3(CXX).png)

- Observations: Outputs align with the analysis, showing better distribution with prime table sizes.
- Example output for integers:
  ```
  Hash table (m=10): [1, 2, 3, 4, 5, 6, 7, 8, 9, 0]
  Hash table (m=11): [10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
  Hash table (m=37): [20, 21, 22, 23, 24, 25, 26, 27, 28, 29, ...]
  ```
- Example output for strings:
  ```
  Hash table (m=10): ["cat", "dog", "bat", "cow", "ant", ...]
  Hash table (m=11): ["fox", "cat", "dog", "bat", "cow", ...]
  Hash table (m=37): ["bee", "hen", "pig", "fox", "cat", ...]
  ```
- Observations: Outputs align with the analysis, showing better distribution with prime table sizes.

## 總結分析 (Analysis)
- **質數 vs. 非質數 $m$**: 
  - 觀察結果顯示，當表格大小 $m$ 為質數（如 $m=11$ 和 $m=37$）時，雜湊分佈通常優於非質數 $m=10$。
  - 特別是對於較好的雜湊函數（第二次和第三次），$m=37$ 的碰撞率顯著低於 $m=10$ 和 $m=11$。
- **規律性或碰撞 (Patterns or Collisions)**: 
  - **第一次雜湊**：整數鍵值產生高度規律的線性分佈，字串鍵值則完全集中在索引 0，碰撞率極高。
  - **第二次雜湊**：整數鍵值仍有規律性。字串鍵值的分佈顯著改善，尤其在 m=37 時達到零碰撞。
  - **第三次雜湊**：整數鍵值的分佈最為混亂（隨機），規律性最低，這對於雜湊函數來說是理想的特性。字串鍵值在 $m=37$ 時僅有一次碰撞，表現最佳。
- **改進建議 (Improvements)**: 
  - **整數鍵**: 應使用如 Knuth 乘法雜湊（第三次）這類能有效打亂鍵值位元的分佈型雜湊函數，而不是簡單的取模運算。
  - **字串鍵**: 應使用標準的多項式滾動雜湊（第三次）或類似的算法，確保每個字符的貢獻和位置都被考慮，以最小化碰撞。
  - **表格大小**: 盡可能選擇一個**質數**作為表格大小 $m$，以提高雜湊函數的均勻分佈性能。

## 反思 (Reflection)
1. **雜湊函數設計**：設計一個好的雜湊函數需要平衡**計算效率**和**分佈均勻性**。簡單的取模雖然快速，但分佈差；而 Knuth 乘法和 Polynomial Rolling Hash 雖然計算略複雜，但能顯著提高分佈質量，減少碰撞。
2. **表格大小的影響**：表格大小 $m$ 對雜湊分佈的影響至關重要。當 $m$ 是一個質數且與雜湊函數的設計參數互質時，通常能產生更均勻的索引序列。
3. **最佳設計**：在本次實驗中，**第三次雜湊函數**（整數使用 Knuth 乘法，字串使用標準 Polynomial Rolling Hash）配合**質數表格大小 $m=37$** 產生了最優異的結果，碰撞次數相對較少，分佈最均勻。
