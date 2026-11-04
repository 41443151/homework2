#include <iostream>
#include <algorithm>
using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream& s, Polynomial p);
private:
    float coef;
    int exp;
};

class Polynomial {
private:
    Term* termArray;
    int capacity;
    int terms;
public:
    Polynomial() : capacity(2), terms(0) {
        termArray = new Term[capacity];
    }

    void newTerm(const float theCoef, const int theExp) {
        if (theCoef == 0) return;
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            copy(termArray, termArray + terms, temp);
            delete[] termArray;
            termArray = temp;
        }
        termArray[terms].coef = theCoef;
        termArray[terms++].exp = theExp;
    }

    Polynomial Add(Polynomial b) {
        Polynomial c;
        int aPos = 0, bPos = 0;
        while ((aPos < terms) && (bPos < b.terms)) {
            if (termArray[aPos].exp == b.termArray[bPos].exp) {
                float t = termArray[aPos].coef + b.termArray[bPos].coef;
                if (t) c.newTerm(t, termArray[aPos].exp);
                aPos++;
                bPos++;
            } else if (termArray[aPos].exp < b.termArray[bPos].exp) {
                c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
                bPos++;
            } else {
                c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
                aPos++;
            }
        }
        for (; aPos < terms; aPos++)
            c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
        for (; bPos < b.terms; bPos++)
            c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
        return c;
    }

    Polynomial Mult(Polynomial b) {
        Polynomial c;
        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < b.terms; j++) {
                float theCoef = termArray[i].coef * b.termArray[j].coef;
                int theExp = termArray[i].exp + b.termArray[j].exp;
                int pos = 0;
                while (pos < c.terms && c.termArray[pos].exp > theExp)
                    pos++;
                if (pos < c.terms && c.termArray[pos].exp == theExp) {
                    c.termArray[pos].coef += theCoef;
                    // 同次方相消時，將後面的項目往前移。
                    if (c.termArray[pos].coef == 0) {
                        for (int k = pos; k < c.terms - 1; k++)
                            c.termArray[k] = c.termArray[k + 1];
                        c.terms--;
                    }
                } else if (theCoef != 0) {
                    // 配置空間，再搬移到正確位置。
                    c.newTerm(theCoef, theExp);
                    for (int k = c.terms - 1; k > pos; k--)
                        c.termArray[k] = c.termArray[k - 1];
                    c.termArray[pos].coef = theCoef;
                    c.termArray[pos].exp = theExp;
                }
            }
        }
        return c;
    }

    float Eval(float x) {
        float sum = 0;
        for (int i = 0; i < terms; i++) {
            float value = termArray[i].coef;
            for (int j = 0; j < termArray[i].exp; j++)
                value *= x;
            sum += value;
        }
        return sum;
    }

    friend istream& operator>>(istream& s, Polynomial& p);
    friend ostream& operator<<(ostream& s, Polynomial p);
};

istream& operator>>(istream& s, Polynomial& p) {
    int n;
    float coef;
    int exp;
    s >> n;
    p.terms = 0;
    // 輸入必須依指數遞減排列，而且不可有重複指數。
    for (int i = 0; i < n; i++) {
        s >> coef >> exp;
        p.newTerm(coef, exp);
    }
    return s;
}

ostream& operator<<(ostream& s, Polynomial p) {
    if (p.terms == 0) {
        s << 0;
        return s;
    }
    for (int i = 0; i < p.terms; i++) {
        float coef = p.termArray[i].coef;
        int exp = p.termArray[i].exp;
        if (i > 0 && coef > 0) s << '+';
        s << coef;
        if (exp > 0) {
            s << 'x';
            if (exp > 1) s << '^' << exp;
        }
    }
    return s;
}

int main() {
    Polynomial a, b;
    float x;
    cout << "Enter A: term count, then coefficient/exponent pairs.\n";
    cin >> a;
    cout << "Enter B: term count, then coefficient/exponent pairs.\n";
    cin >> b;
    cout << "Enter x: ";
    cin >> x;
    cout << "A(x) = " << a << '\n';
    cout << "B(x) = " << b << '\n';
    cout << "A + B = " << a.Add(b) << '\n';
    cout << "A * B = " << a.Mult(b) << '\n';
    cout << "A(" << x << ") = " << a.Eval(x) << '\n';
    cout << "B(" << x << ") = " << b.Eval(x) << '\n';
    return 0;
}

