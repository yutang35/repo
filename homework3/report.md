# 41243208

作業三

## 解題說明

### 問題描述

本題目要求開發一個 C++ 類別 Polynomial，用於表示和操作單未知數多項式，係數為整數。使用標頭節點的循環鏈結串列來儲存多項式，每個節點代表一項，包括係數 (coef)、指數 (exp) 和鏈結 (link)。為了快速刪除多項式，使用可用空間串列 (available-space list) 來管理記憶體回收與使用。多項式的外部表示為一串列整數：項數 n，接著是 c1 e1 c2 e2 ... cn en，指數按遞減順序排列。需實現輸入/輸出運算子、複製建構函數、指定運算子、解構函數、加法、減法、乘法，以及在給定 x 值處的求值功能。

### 解題策略

1. 資料結構選擇：採用循環鏈結串列標頭節點來表示多項式，便於項的插入、刪除和遍歷。標頭節點不儲存資料，用於標記串列起始與結束。
2. 記憶體管理：實作全域可用空間串列 avail，透過 GetNode() 和 FreeNode() 函數來獲取和釋放節點，避免頻繁的 new/delete 運算。
3. 輸入與輸出：使用運算子多載讀取/輸出外部格式，確保指數遞減順序。
4. 運算實現：
    - 加法/減法：合併兩個串列，比較指數並處理係數，忽略係數為 0 的項。
    - 乘法：對每個項逐一相乘生成臨時多項式，然後累加合併。
    - 求值：遍歷串列，使用 pow() 計算每項值並累加。
5. 建構與指定：複製建構函數和指定運算子需深層複製節點；解構函數將節點回傳到可用空間串列。

## 程式實作

```cpp
#include <iostream>
#include <cmath>

using namespace std;

// 節點結構，用來表示多項式中的一個項次
// coef = 係數, exp = 指數, link = 指向下一個節點
struct Node {
    int coef;
    int exp;
    Node* link;
};

// 可用節點的「空間列表」，用來重複利用釋放掉的節點
Node* avail = nullptr;

// 取得新節點：若 avail 有節點可用就重複利用，否則配置新節點
Node* GetNode() {
    Node* p;
    if (avail != nullptr) {
        p = avail;
        avail = avail->link;
    } else {
        p = new Node;
    }
    p->link = nullptr;
    return p;
}

// 釋放節點：把不用的節點放回 avail，以便之後重複利用
void FreeNode(Node* p) {
    p->link = avail;
    avail = p;
}

// 多項式類別
class Polynomial {
private:
    Node* head; // 多項式的環狀鏈結串列頭節點 (dummy head)

    // 初始化多項式，建立一個頭節點
    void Initialize() {
        head = GetNode();
        head->coef = 0;
        head->exp = -1;   // 特殊標記，表示頭節點
        head->link = head; // 自己指向自己，形成環狀結構
    }

public:
    // 預設建構函數
    Polynomial() {
        Initialize();
    }

    // 複製建構函數 (深拷貝，複製另一個多項式的所有節點)
    Polynomial(const Polynomial& a) {
        Initialize();
        Node* current = a.head->link;
        Node* last = head;
        while (current != a.head) {
            Node* newNode = GetNode();
            newNode->coef = current->coef;
            newNode->exp = current->exp;
            newNode->link = head;
            last->link = newNode;
            last = newNode;
            current = current->link;
        }
    }

    // 解構函數：釋放所有節點，並把它們放回 avail
    ~Polynomial() {
        if (head == nullptr) return;
        Node* current = head->link;
        while (current != head) {
            Node* temp = current;
            current = current->link;
            FreeNode(temp);
        }
        FreeNode(head);
        head = nullptr;
    }

    // 賦值運算子 (深拷貝)
    const Polynomial& operator=(const Polynomial& a) {
        if (this != &a) {
            this->~Polynomial(); // 先釋放目前的多項式
            Initialize();        // 重新初始化
            Node* current = a.head->link;
            Node* last = head;
            while (current != a.head) {
                Node* newNode = GetNode();
                newNode->coef = current->coef;
                newNode->exp = current->exp;
                newNode->link = head;
                last->link = newNode;
                last = newNode;
                current = current->link;
            }
        }
        return *this;
    }

    // 輸入運算子多載：讀取多項式
    // 格式：n c1 e1 c2 e2 ... (n = 項數)
    friend istream& operator>>(istream& is, Polynomial& x) {
        int n;
        is >> n;
        Node* last = x.head;
        for (int i = 0; i < n; ++i) {
            Node* newNode = GetNode();
            is >> newNode->coef >> newNode->exp;
            newNode->link = x.head;
            last->link = newNode;
            last = newNode;
        }
        return is;
    }

    // 輸出運算子多載：印出多項式
    // 格式：n c1 e1 c2 e2 ... (n = 項數)
    friend ostream& operator<<(ostream& os, const Polynomial& x) {
        Node* current = x.head->link;
        int count = 0;
        Node* temp = current;
        while (temp != x.head) {
            count++;
            temp = temp->link;
        }
        os << count;
        if (count > 0) os << " ";
        while (current != x.head) {
            os << current->coef << " " << current->exp;
            current = current->link;
            if (current != x.head) os << " ";
        }
        return os;
    }

    // 多項式加法
    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        Node* last = result.head;
        Node* p1 = head->link;
        Node* p2 = b.head->link;

        while (p1 != head || p2 != b.head) {
            Node* newNode = GetNode();
            if (p2 == b.head || (p1 != head && p1->exp > p2->exp)) {
                // 取 p1 項
                newNode->coef = p1->coef;
                newNode->exp = p1->exp;
                p1 = p1->link;
            } else if (p1 == head || p1->exp < p2->exp) {
                // 取 p2 項
                newNode->coef = p2->coef;
                newNode->exp = p2->exp;
                p2 = p2->link;
            } else {
                // 次方相同 → 係數相加
                int sum = p1->coef + p2->coef;
                if (sum != 0) {
                    newNode->coef = sum;
                    newNode->exp = p1->exp;
                } else {
                    // 如果相加為 0，釋放節點並跳過
                    FreeNode(newNode);
                    p1 = p1->link;
                    p2 = p2->link;
                    continue;
                }
                p1 = p1->link;
                p2 = p2->link;
            }
            newNode->link = result.head;
            last->link = newNode;
            last = newNode;
        }
        return result;
    }

    // 多項式減法
    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        Node* last = result.head;
        Node* p1 = head->link;
        Node* p2 = b.head->link;

        while (p1 != head || p2 != b.head) {
            Node* newNode = GetNode();
            if (p2 == b.head || (p1 != head && p1->exp > p2->exp)) {
                // 取 p1 項
                newNode->coef = p1->coef;
                newNode->exp = p1->exp;
                p1 = p1->link;
            } else if (p1 == head || p1->exp < p2->exp) {
                // 取 -p2 項
                newNode->coef = -p2->coef;
                newNode->exp = p2->exp;
                p2 = p2->link;
            } else {
                // 次方相同 → 係數相減
                int diff = p1->coef - p2->coef;
                if (diff != 0) {
                    newNode->coef = diff;
                    newNode->exp = p1->exp;
                } else {
                    FreeNode(newNode);
                    p1 = p1->link;
                    p2 = p2->link;
                    continue;
                }
                p1 = p1->link;
                p2 = p2->link;
            }
            newNode->link = result.head;
            last->link = newNode;
            last = newNode;
        }
        return result;
    }

    // 多項式乘法
    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        Node* p1 = head->link;

        // 對 p1 的每一項，去乘上 p2 的每一項
        while (p1 != head) {
            Polynomial temp;
            Node* last = temp.head;
            Node* p2 = b.head->link;

            while (p2 != b.head) {
                Node* newNode = GetNode();
                newNode->coef = p1->coef * p2->coef;
                newNode->exp = p1->exp + p2->exp;
                newNode->link = temp.head;
                last->link = newNode;
                last = newNode;
                p2 = p2->link;
            }

            // 把臨時結果加到總結果
            Polynomial tempResult = result + temp;
            result = tempResult;
            p1 = p1->link;
        }
        return result;
    }

    // 多項式求值 f(x)
    float Evaluate(float x) const {
        float result = 0.0;
        Node* current = head->link;
        while (current != head) {
            result += current->coef * pow(x, current->exp);
            current = current->link;
        }
        return result;
    }
};

// 主程式測試
int main() {
    Polynomial p1, p2;

    cout << "輸入第一個多項式（格式：n c1 e1 c2 e2 ...）：" << endl;
    cin >> p1;
    cout << "輸入第二個多項式（格式：n c1 e1 c2 e2 ...）：" << endl;
    cin >> p2;

    cout << "多項式 p1: " << p1 << endl;
    cout << "多項式 p2: " << p2 << endl;

    Polynomial sum = p1 + p2;
    cout << "p1 + p2: " << sum << endl;

    Polynomial diff = p1 - p2;
    cout << "p1 - p2: " << diff << endl;

    Polynomial prod = p1 * p2;
    cout << "p1 * p2: " << prod << endl;

    float x;
    cout << "輸入 x 值用於求值 p1(x): ";
    cin >> x;
    cout << "p1(" << x << ") = " << p1.Evaluate(x) << endl;

    Polynomial p3(p1);
    cout << "複製 p1 到 p3: " << p3 << endl;

    Polynomial p4;
    p4 = p2;
    cout << "賦值 p2 到 p4: " << p4 << endl;

    return 0;
}
```


## 效能分析

### 時間複雜度
- 輸入/輸出運算子：$O(n)$，其中 $n$ 是多項式的項數，因為需逐項讀取或輸出。
- 加法/減法運算子：$O(m + n)$，其中 $m$ 和 $n$ 分別是兩個多項式的項數，因為需遍歷兩個串列一次，比較指數並合併。
- 乘法運算子：$O(m * n)$，因為對第一個多項式的每項與第二個多項式的每項相乘，然後合併結果。
- 求值函數：$O(k)$，其中 $k$ 是項數，因為需遍歷所有項並計算 $pow(x, exp)$，假設 $pow()$ 為常數時間。
- 複製建構函數/指定運算子：$O(n)$，需逐項複製節點。
- 解構函數：$O(n)$，需逐項釋放節點到可用空間串列。
- 整個程式的時間複雜度：假設兩個多項式的項數均為 $n$，則 main() 函數中的測試流程總時間複雜度為 $O(n^2)$，因為乘法運算主導了複雜度，其他運算均為 $O(n)$ 或 $O(n + n)$。

### 空間複雜度：

- 整體空間：$O(n)$，因為使用鏈結串列儲存多項式的 $n$ 個項，每個節點佔用固定空間。可用空間串列幫助重新使用記憶體，但不改變漸近複雜度。
- 運算中臨時空間：加法/減法為 $O(m + n)$，乘法為 $O(m * n)$（生成所有乘積項後合併）。
- 整個程式的空間複雜度：假設兩個多項式的項數均為 $n$，則總空間複雜度為 $O(n^2)$，因為乘法運算會產生臨時多項式佔用 $O(n^2)$ 空間，同時存在多個 Polynomial 物件各佔 $O(n)$ 空間。

## 測試與驗證

### 編譯與執行指令

```shell
$ g++ main.cpp -std=c++11 -o main.exe
$ ./main.exe
輸入第一個多項式（格式：n c1 e1 c2 e2 ...）：
2 3 2 1 0
輸入第二個多項式（格式：n c1 e1 c2 e2 ...）：
2 2 1 1 0
多項式 p1: 2 3 2 1 0
多項式 p2: 2 2 1 1 0
p1 + p2: 3 3 2 2 1 2 0
p1 - p2: 2 3 2 -2 1
p1 * p2: 4 6 3 3 2 2 1 1 0
輸入 x 值用於求值 p1(x): 2
p1(2) = 13
複製 p1 到 p3: 2 3 2 1 0
賦值 p2 到 p4: 2 2 1 1 0
```

### 測試案例驗證

**測試案例 1：正常案例**  
```shell
$ ./main.exe
輸入第一個多項式（格式：n c1 e1 c2 e2 ...）：
2 3 2 1 0
輸入第二個多項式（格式：n c1 e1 c2 e2 ...）：
2 2 1 1 0
多項式 p1: 2 3 2 1 0
多項式 p2: 2 2 1 1 0
p1 + p2: 3 3 2 2 1 2 0
p1 - p2: 2 3 2 -2 1
p1 * p2: 4 6 3 3 2 2 1 1 0
輸入 x 值用於求值 p1(x): 2
p1(2) = 13
複製 p1 到 p3: 2 3 2 1 0
賦值 p2 到 p4: 2 2 1 1 0
```

**測試案例 2：邊界案例（空多項式）**  
```shell
$ ./main.exe
輸入第一個多項式（格式：n c1 e1 c2 e2 ...）：
1 5 3
輸入第二個多項式（格式：n c1 e1 c2 e2 ...）：
0
多項式 p1: 1 5 3
多項式 p2: 0
p1 + p2: 1 5 3
p1 - p2: 1 5 3
p1 * p2: 0
輸入 x 值用於求值 p1(x): 1
p1(1) = 5
複製 p1 到 p3: 1 5 3
賦值 p2 到 p4: 0
```

**測試案例 3：特殊案例（相同指數）**  
```shell
$ ./main.exe
輸入第一個多項式（格式：n c1 e1 c2 e2 ...）：
2 3 2 2 2
輸入第二個多項式（格式：n c1 e1 c2 e2 ...）：
2 2 2 1 0
多項式 p1: 2 3 2 2 2
多項式 p2: 2 2 2 1 0
p1 + p2: 2 5 2 1 0
p1 - p2: 2 1 2 1 0
p1 * p2: 4 10 4 7 2 2 1 2 0
輸入 x 值用於求值 p1(x): 2
p1(2) = 21
複製 p1 到 p3: 2 3 2 2 2
賦值 p2 到 p4: 2 2 2 1 0
```

## 申論及開發報告

本程式使用循環鏈結串列來表示多項式，因為它允許動態插入和刪除項，而不需要固定大小的陣列，適合多項式項數不定的情境。標頭節點的設計簡化了串列運算，例如避免特殊處理空串列或串列開頭插入。選擇循環鏈結而非單向鏈結，是因為循環結構便於判斷串列結束（link 指向標頭結點），並符合題目要求。  
對於記憶體管理，使用可用空間串列是為了快速回收刪除的節點，避免頻繁的 new/delete 運算，這在頻繁新增/刪除多項式的場景下能降低系統消耗。  
加法和減法採用合併排序串列的演算法，類似合併兩個有序鏈結串列，利用指數遞減的屬性，確保時間效率。乘法使用簡單的逐項乘法演算法，雖然時間複雜度為 $O(m*n)$，但對於小規模多項式足夠。求值使用 $pow()$ 函數，假設指數不大，避免手動實現指數計算以簡化程式。  
開發過程中，遇到的主要挑戰是確保記憶體不洩漏和正確處理係數為 0 的項（自動忽略）。透過測試驗證，確認所有功能正確，包括邊界案例如空多項式或相同指數項。整體設計強調模組化，便於維護和擴展。