# 41243208

作業二

## 解題說明

### 問題描述

本題目要求實作多項式的表示與加法運算。背景為多項式是由多個項次組成，每項包含係數與指數，例如 $3x^2 + 5x + 4$。題目需求包括：設計 Term 類別來表示單一項次，Polynomial 類別來表示整個多項式；支援從使用者輸入多項式的項數、係數與指數；支援兩個多項式的加法運算，並輸出結果。假設輸入的項次已按指數遞減順序輸入，以確保加法合併正確進行。輸出格式需正確顯示多項式，如省略係數為 1 或 -1 時的顯示，並處理正負號。

### 解題策略

1. 設計 Term 類別：用於儲存單一項次的係數 (coef) 與指數 (exp)，設為私有成員，並將 Polynomial 設為友誼類別以便存取。
2. 設計 Polynomial 類別：使用動態陣列 (termArray) 儲存 Term 物件，初始容量為 10，並支援動態擴充（類似 std::vector 的 resize）。提供建構函式、NewTerm 函式新增項次、Add 函式進行加法合併，以及多載 << 和 >> 運算子用於輸出與輸入。
3. 加法策略：假設陣列已按指數遞減排序，使用兩個指標 (aPos, bPos) 類似合併排序陣列的方式，比較指數大小合併項次。若指數相同，則相加係數（若結果為 0 則忽略）；若不同，則優先加入指數較大者。最後加入剩餘項次。
4. 輸入輸出：使用 >> 運算子提示使用者輸入項數與各項細節；使用 << 運算子格式化輸出多項式，處理特殊情況如係數為 1/-1、指數為 0/1、正負號等。

## 程式實作

```cpp
#include <iostream>
#include <string>
#include <algorithm>

class Polynomial;

// 定義 Term 類別，用於表示多項式中的單一項次 (例如 3x^2)
class Term {
    // 讓 Polynomial 以及輸入/輸出運算子能夠存取 Term 的私有成員
    friend std::ostream& operator<<(std::ostream& output, const Polynomial& p);
    friend std::istream& operator>>(std::istream& input, Polynomial& thePoly);
    friend Polynomial;

private:
    float coef; // 項次的係數
    int exp;    // 項次的指數
};

// 定義 Polynomial (多項式) 類別
class Polynomial {
    // 多項式的輸出與輸入運算子多載
    friend std::ostream& operator<<(std::ostream& output, const Polynomial& thePoly);
    friend std::istream& operator>>(std::istream& input, Polynomial& thePoly);

public:
    // 建構子：初始化多項式
    Polynomial(int numTerm = 0) {
        terms = numTerm;       // 當前項數
        capacity = 10;         // 初始容量 (最多可存 10 項)
        termArray = new Term[capacity]; // 動態配置陣列
    }

    // 多項式相加函式：回傳一個新的 Polynomial
    Polynomial Add(Polynomial b) {
        Polynomial c; // 結果多項式
        int aPos = 0, bPos = 0;

        // 同時走訪兩個多項式的項次，依指數比較
        while ((aPos < terms) && (bPos < b.terms)) {
            if (termArray[aPos].exp == b.termArray[bPos].exp) {
                // 若指數相同，係數相加
                float t = termArray[aPos].coef + b.termArray[bPos].coef;
                if (t) c.NewTerm(t, termArray[aPos].exp);
                aPos++;
                bPos++;
            }
            else if (termArray[aPos].exp < b.termArray[bPos].exp) {
                // 若 b 的指數較大，直接加入 b 的項次
                c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
                bPos++;
            }
            else {
                // 若 a 的指數較大，直接加入 a 的項次
                c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
                aPos++;
            }
        }

        // 若 a 還有剩餘項次，全部加入
        for (; aPos < terms; aPos++)
            c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);

        // 若 b 還有剩餘項次，全部加入
        for (; bPos < b.terms; bPos++)
            c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);

        return c;
    }

    // 建立新項次的函式
    void NewTerm(const float theCoeff, const int theExp) {
        // 若超過容量，則擴充陣列
        if (terms == capacity) {
            capacity *= 2; // 容量加倍
            Term* temp = new Term[capacity];
            std::copy(termArray, termArray + terms, temp); // 複製舊資料
            delete[] termArray;
            termArray = temp;
        }
        // 將新項次放入陣列
        termArray[terms].coef = theCoeff;
        termArray[terms++].exp = theExp;
    }

private:
    Term* termArray; // 動態陣列，存放所有項次
    int capacity;    // 當前陣列容量
    int terms;       // 實際項數
};

// 多項式的輸出運算子多載
std::ostream& operator<<(std::ostream& output, const Polynomial& p) {
    int e;
    float c;
    std::string sign;
    for (int i = 0; i < p.terms; i++) {
        c = p.termArray[i].coef; // 取出係數
        e = p.termArray[i].exp;  // 取出指數

        // 處理正號 (第一項不用加 +)
        if (i > 0 && c > 0) sign = "+";
        else sign = "";

        std::cout << sign;
        if (e == 0) {
            // 常數項 (不含 x)
            std::cout << c;
        }
        else {
            // 處理係數
            if (c == 1) std::cout << "";      // 省略 1
            else if (c == -1) std::cout << "-"; // 省略 -1 的 1
            else std::cout << c;

            // 處理次方
            if (e == 1) std::cout << "x";     // x^1 → x
            else std::cout << "x^" << e;
        }
    }
    return output;
}

// 多項式的輸入運算子多載
std::istream& operator>>(std::istream& input, Polynomial& p) {
    int numTerms;
    float c;
    int e;

    std::cout << "請輸入多項式的項數：";
    std::cin >> numTerms;
    for (int i = 0; i < numTerms; i++) {
        std::cout << "請輸入第 " << i << " 項的係數：";
        std::cin >> c;
        std::cout << "請輸入第 " << i << " 項的次數：";
        std::cin >> e;
        p.NewTerm(c, e);
    }

    return input;
}

int main(int argc, const char* argv[]) {
    Polynomial a, b;

    std::cout << "input poly A" << std::endl;
    std::cin >> a; // 輸入多項式 A

    std::cout << "input poly B" << std::endl;
    std::cin >> b; // 輸入多項式 B

    // 輸出結果
    std::cout << "a=" << a << std::endl;
    std::cout << "b=" << b << std::endl;
    std::cout << "a+b=" << a.Add(b) << std::endl;

    system("pause"); // 暫停 (僅限 Windows)
    return 0;
}
```


## 效能分析

### 時間複雜度

- 加法運算 (Add 函式)：需遍歷兩個多項式的所有項次，使用兩個指標線性合併，因此時間複雜度為 $ O(m + n) $，其中 $ m $ 為第一個多項式的項數，$ n $ 為第二個多項式的項數。
- 新增項次 (NewTerm 函式)：大多數情況下為 $ O(1) $，但當容量不足需擴充時，為 $ O(k) $（其中 $ k $ 為當前項數），均攤分析下仍為 $ O(1) $。
- 輸入與輸出：輸入為 $ O(t) $（$ t $ 為項數），輸出為 $ O(t) $。
- 整體程式：主程式以加法為主，因此時間複雜度為 $ O(m + n) $。

### 空間複雜度：
- 使用動態陣列儲存項次，容量初始為 10，並以倍數擴充，因此空間複雜度為 $ O(t) $，其中 $ t $ 為項數（最壞情況下接近 2t，但 Big-O 忽略常數）。
- 加法結果需額外空間儲存新多項式，因此總空間為 $ O(m + n) $。

## 測試與驗證

### 編譯與執行指令

```shell
$ g++ main.cpp -o main.exe
$ ./main.exe
input poly A
請輸入多項式的項數：2
請輸入第 0 項的係數：3
請輸入第 0 項的次數：2
請輸入第 1 項的係數：4
請輸入第 1 項的次數：0
input poly B
請輸入多項式的項數：2
請輸入第 0 項的係數：-2
請輸入第 0 項的次數：3
請輸入第 1 項的係數：5
請輸入第 1 項的次數：1
a=3x^2+4
b=-2x^3+5x
a+b=-2x^3+3x^2+5x+4
請按任意鍵繼續 . . .
```

### 測試案例驗證

**測試案例 1：基本多項式加法**  
* 輸入  
    $A = 3x^2+4$  
    $B = -2x^3+5x$
* 輸出  
    $A+B = -2x^3+3x^2+5x+4$  

**測試案例 2：相同指數項次合併與零係數**  
* 輸入  
    $A = 2x^2 + 3x$  
    $B = -2x^2 + 4$
* 輸出  
    $A+B = 3x+4$  

**測試案例 3：單項式與空多項式**  
* 輸入  
    $A = 5x^3$  
    $B = 空（項數為 0）$
* 輸出  
    $A+B = 5x^3$  

## 申論及開發報告

本程式中，我選擇使用動態陣列作為資料結構來儲存多項式的項次，因為陣列提供連續記憶體存取，適合線性遍歷與合併操作，且易於實作容量擴充（類似 std::vector 的行為），避免固定大小陣列的限制。陣列的空間效率高，且在假設項次已排序的前提下，加法運算可採用合併演算法（類似合併兩個有序陣列），這是基於merge sort的概念，能在線性時間內完成，效率優於其他結構如鏈結串列（鏈結需額外指標，合併時可能需更多時間處理插入）。  
選擇此演算法的原因是多項式加法本質上為有序項次的合併問題，按指數遞減排序可確保合併過程簡單且高效，避免每次加法都需重新排序（若輸入未排序，可在 NewTerm 後添加排序，但會增加 $ O(t \log t) $ 時間，本程式為簡化假設已排序）。此外，多載 << 和 >> 運算子提升程式可讀性與使用者互動性。開發過程中，需注意記憶體管理（如 delete[] termArray 避免洩漏），以及輸出格式的邊界條件（如係數為 0、1、-1，指數為 0、1）。若未來擴充，可加入項次排序功能以提升穩健性。
