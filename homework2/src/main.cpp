#include <iostream>
#include <string>
#include <algorithm>

class Polynomial;

// �w�q Term ���O�A�Ω��ܦh����������@���� (�Ҧp 3x^2)
class Term {
    // �� Polynomial �H�ο�J/��X�B��l����s�� Term ���p������
    friend std::ostream& operator<<(std::ostream& output, const Polynomial& p);
    friend std::istream& operator>>(std::istream& input, Polynomial& thePoly);
    friend Polynomial;

private:
    float coef; // �������Y��
    int exp;    // ����������
};

// �w�q Polynomial (�h����) ���O
class Polynomial {
    // �h��������X�P��J�B��l�h��
    friend std::ostream& operator<<(std::ostream& output, const Polynomial& thePoly);
    friend std::istream& operator>>(std::istream& input, Polynomial& thePoly);

public:
    // �غc�l�G��l�Ʀh����
    Polynomial(int numTerm = 0) {
        terms = numTerm;       // ��e����
        capacity = 10;         // ��l�e�q (�̦h�i�s 10 ��)
        termArray = new Term[capacity]; // �ʺA�t�m�}�C
    }

    // �h�����ۥ[�禡�G�^�Ǥ@�ӷs�� Polynomial
    Polynomial Add(Polynomial b) {
        Polynomial c; // ���G�h����
        int aPos = 0, bPos = 0;

        // �P�ɨ��X��Ӧh�����������A�̫��Ƥ��
        while ((aPos < terms) && (bPos < b.terms)) {
            if (termArray[aPos].exp == b.termArray[bPos].exp) {
                // �Y���ƬۦP�A�Y�Ƭۥ[
                float t = termArray[aPos].coef + b.termArray[bPos].coef;
                if (t) c.NewTerm(t, termArray[aPos].exp);
                aPos++;
                bPos++;
            }
            else if (termArray[aPos].exp < b.termArray[bPos].exp) {
                // �Y b �����Ƹ��j�A�����[�J b ������
                c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
                bPos++;
            }
            else {
                // �Y a �����Ƹ��j�A�����[�J a ������
                c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
                aPos++;
            }
        }

        // �Y a �٦��Ѿl�����A�����[�J
        for (; aPos < terms; aPos++)
            c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);

        // �Y b �٦��Ѿl�����A�����[�J
        for (; bPos < b.terms; bPos++)
            c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);

        return c;
    }

    // �إ߷s�������禡
    void NewTerm(const float theCoeff, const int theExp) {
        // �Y�W�L�e�q�A�h�X�R�}�C
        if (terms == capacity) {
            capacity *= 2; // �e�q�[��
            Term* temp = new Term[capacity];
            std::copy(termArray, termArray + terms, temp); // �ƻs�¸��
            delete[] termArray;
            termArray = temp;
        }
        // �N�s������J�}�C
        termArray[terms].coef = theCoeff;
        termArray[terms++].exp = theExp;
    }

private:
    Term* termArray; // �ʺA�}�C�A�s��Ҧ�����
    int capacity;    // ��e�}�C�e�q
    int terms;       // ��ڶ���
};

// �h��������X�B��l�h��
std::ostream& operator<<(std::ostream& output, const Polynomial& p) {
    int e;
    float c;
    std::string sign;
    for (int i = 0; i < p.terms; i++) {
        c = p.termArray[i].coef; // ���X�Y��
        e = p.termArray[i].exp;  // ���X����

        // �B�z���� (�Ĥ@�����Υ[ +)
        if (i > 0 && c > 0) sign = "+";
        else sign = "";

        std::cout << sign;
        if (e == 0) {
            // �`�ƶ� (���t x)
            std::cout << c;
        }
        else {
            // �B�z�Y��
            if (c == 1) std::cout << "";      // �ٲ� 1
            else if (c == -1) std::cout << "-"; // �ٲ� -1 �� 1
            else std::cout << c;

            // �B�z����
            if (e == 1) std::cout << "x";     // x^1 �� x
            else std::cout << "x^" << e;
        }
    }
    return output;
}

// �h��������J�B��l�h��
std::istream& operator>>(std::istream& input, Polynomial& p) {
    int numTerms;
    float c;
    int e;

    std::cout << "�п�J�h���������ơG";
    std::cin >> numTerms;
    for (int i = 0; i < numTerms; i++) {
        std::cout << "�п�J�� " << i << " �����Y�ơG";
        std::cin >> c;
        std::cout << "�п�J�� " << i << " �������ơG";
        std::cin >> e;
        p.NewTerm(c, e);
    }

    return input;
}

int main(int argc, const char* argv[]) {
    Polynomial a, b;

    std::cout << "input poly A" << std::endl;
    std::cin >> a; // ��J�h���� A

    std::cout << "input poly B" << std::endl;
    std::cin >> b; // ��J�h���� B

    // ��X���G
    std::cout << "a=" << a << std::endl;
    std::cout << "b=" << b << std::endl;
    std::cout << "a+b=" << a.Add(b) << std::endl;

    system("pause"); // �Ȱ� (�ȭ� Windows)
    return 0;
}
