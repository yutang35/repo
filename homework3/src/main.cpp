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

