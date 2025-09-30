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