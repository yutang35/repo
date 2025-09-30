# 41243208

作業一

## 解題說明

### 問題描述

本作業包含兩個演算法問題：

**Problem 1: Ackermann函數實作**
- 需要實作Ackermann函數的遞迴版本和非遞迴版本
- Ackermann函數定義如下：
  - $A(m,n) = n + 1$, 若 $m = 0$
  - $A(m,n) = A(m-1, 1)$, 若 $n = 0$  且 $m > 0$
  - $A(m,n) = A(m-1, A(m, n-1))$, 若 $m, n > 0$
- 此函數以快速增長特性著稱，即使在較小的 $m$ 和 $n$ 值下也會產生極大的結果

**Problem 2: 冪集產生**
- 實作一個遞迴函數來計算集合S的冪集
- 冪集是包含S所有可能子集合的集合
- 例如：若S = {a,b,c}，則powerset(S) = {∅, {a}, {b}, {c}, {a,b}, {a,c}, {b,c}, {a,b,c}}

### 解題策略

**Problem 1解決方案：**
1. **遞迴版本**：直接按照數學定義實作遞迴函數
2. **非遞迴版本**：使用手動堆疊模擬遞迴過程，避免系統堆疊溢位問題

**Problem 2解決方案：**
1. 使用遞迴與回溯法產生所有子集合
2. 對於每個元素，有兩種選擇：包含在子集合中或不包含
3. 遞迴終止條件：處理完所有元素時輸出當前子集合

## 程式實作

```cpp
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

const int N = 3; // 集合大小，可自行修改
string S[N] = {"a", "b", "c"}; 
string current[N]; // 暫存子集合
int curSize = 0;   // 目前子集合大小

// 遞迴計算 Ackermann 函數
unsigned long long AckermannRecursive(unsigned int m, unsigned int n) {
    if (m == 0) {
        return n + 1;
    } else if (n == 0) {
        return AckermannRecursive(m - 1, 1);
    } else {
        return AckermannRecursive(m - 1, AckermannRecursive(m, n - 1));
    }
}

// 模擬堆疊的資料結構
struct Node {
    unsigned int m;
    unsigned long long n;
    bool waiting;  // 標記這個節點是否在等待子結果
};

// 非遞迴版 Ackermann (手動堆疊)
unsigned long long AckermannNonRecursive(unsigned int m, unsigned int n) {
    const int MAXSIZE = 100000;  // 最大堆疊大小
    Node stack[MAXSIZE];
    int top = -1;

    stack[++top] = {m, n, false};
    unsigned long long result = 0;

    while (top >= 0) {
        Node cur = stack[top--]; // pop

        if (cur.m == 0) {
            result = cur.n + 1;
        } else if (cur.n == 0) {
            stack[++top] = {cur.m - 1, 1, false};
            continue;
        } else if (!cur.waiting) {
            // A(m-1, A(m, n-1))，先算 A(m, n-1)
            stack[++top] = {cur.m - 1, 0, true};   // 等待結果
            stack[++top] = {cur.m, cur.n - 1, false}; // 先處理內層
            continue;
        } else {
            // cur.waiting == true → 前一步的子結果已算好存到 result
            stack[++top] = {cur.m, result, false};
            continue;
        }

        // 若有等待節點，就把結果丟回去
        if (top >= 0 && stack[top].waiting) {
            Node tmp = stack[top--];
            stack[++top] = {tmp.m, result, false};
        }
    }

    return result;
}


// 遞迴產生冪集
void generatePowerSet(int index) {
    if (index == N) {
        cout << "{";
        for (int i = 0; i < curSize; i++) {
            cout << current[i];
            if (i != curSize - 1) cout << ",";
        }
        cout << "}" << endl;
        return;
    }

    // 不選 S[index]
    generatePowerSet(index + 1);

    // 選 S[index]
    current[curSize++] = S[index];
    generatePowerSet(index + 1);
    curSize--; // 回溯
}

int main() {
    unsigned int m, n;
    int choice;
    
    cout << "Problem 1:" << endl;

    cout << "請輸入 m 與 n: ";
    cin >> m >> n;
    
    cout << "[遞迴版] Ackermann(" << m << ", " << n << ") = "
     << AckermannRecursive(m, n) << endl;
    cout << "[非遞迴版] Ackermann(" << m << ", " << n << ") = "
     << AckermannNonRecursive(m, n) << endl << endl;
     
    cout << "Problem 2:" << endl;
    
    cout << "Power set:" << endl;
    generatePowerSet(0);
    return 0;
}
```

## 效能分析

### Problem 1: Ackermann函數

**時間複雜度：**
- **遞迴版本**：$$O(A(m,n))$$ 
    - 極快增長函數，實際上無法用多項式表示
- **非遞迴版本**：$$O(A(m,n))$$ 
    - 與遞迴版本相同的時間複雜度，但避免了系統堆疊溢位

**空間複雜度：**
- **遞迴版本**：$$O(A(m,n-1))$$ 
    - 遞迴深度極深，容易造成堆疊溢位
- **非遞迴版本**：$$O(A(m,n-1))$$ 
    - 使用手動堆疊，空間使用更可控

### Problem 2: 冪集產生

**時間複雜度：**
$$O(2^n)$$  
- 需要產生所有 $2^n$ 個子集合

**空間複雜度：**
$$O(n)$$ 
- 遞迴深度最多為 $n$，加上暫存陣列空間

## 測試與驗證

### 編譯與執行指令

```shell
$ g++ main.cpp --std=c++11 -o main.exe
$ ./main.exe
Problem 1:
請輸入 m 與 n: 2 3
[遞迴版] Ackermann(2, 3) = 9
[非遞迴版] Ackermann(2, 3) = 9

Problem 2:
Power set:
{}
{c}
{b}
{b,c}
{a}
{a,c}
{a,b}
{a,b,c}
```

### 測試案例驗證

**Ackermann函數測試：**
- $A(0,0)$ = 1 ✓
- $A(0,5)$ = 6 ✓  
- $A(1,3)$ = 5 ✓
- $A(2,3)$ = 9 ✓

**冪集測試：**
對於集合 S = {a, b, c}，產生的冪集包含 8 個子集合，符合 $2^3 = 8$ 的預期結果。

## 申論及開發報告

### 資料結構與演算法選擇理由

**Problem 1 - Ackermann函數：**

1. **遞迴實作**：直接對應數學定義，程式碼簡潔易懂，但受限於系統堆疊大小
2. **非遞迴實作**：使用**自定義堆疊**模擬遞迴過程，主要優點：
   - 避免系統堆疊溢位問題
   - 可處理更大的參數值
   - 堆疊大小可動態調整

**關鍵設計：**
- 使用 `Node` 結構體儲存函數調用狀態
- `waiting` 標記用於處理巢狀調用的結果傳遞
- 手動管理堆疊指標實現LIFO操作

**Problem 2 - 冪集產生：**

選擇**遞迴與回溯法**的理由：
1. **自然對應**：冪集的定義本身就具有遞迴性質
2. **狀態管理**：使用全域陣列 `current` 和 `curSize` 追蹤當前子集合狀態
3. **完整搜索**：確保不遺漏任何可能的子集合組合

**演算法流程：**
1. 對每個元素決定是否包含在當前子集合中
2. 遞迴處理下一個元素
3. 回溯時恢復狀態，探索另一種選擇

### 實作挑戰與解決方案

1. **Ackermann函數的堆疊管理**：非遞迴版本需要精確模擬遞迴調用順序，透過 `waiting` 標記確保結果正確傳遞

2. **冪集的狀態維護**：使用回溯法確保每次遞迴後都能正確恢復到前一狀態

3. **記憶體使用優化**：避免創建大量臨時物件，重複使用相同的儲存空間

此實作展示了遞迴與非遞迴演算法的差異，以及如何運用適當的資料結構解決複雜的計算問題。

