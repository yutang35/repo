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
