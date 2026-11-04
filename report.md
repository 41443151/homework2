# 41443151

## 作業二：多項式物件

### 解題說明

本題要讓程式能夠儲存多項式，並完成加法、乘法和代入 x 求值。另外使用 `>>` 和 `<<`，讓多項式可以直接輸入與輸出。

每一項分成係數和指數，例如 `3x²` 的係數是 3，指數是 2。程式用 `Term` 儲存這兩個數字，再用 `Polynomial` 裡的動態陣列存放所有非零項。

1. **加入項目**：使用 `newTerm()` 加入一項。係數為 0 就不存，陣列滿了就把容量增加成兩倍。
2. **加法**：從兩個多項式的第一項開始比較。指數相同就把係數相加，否則先放入指數較大的項，最後補上還沒處理的項。
3. **乘法**：第一個多項式的每一項，都要乘上第二個多項式的每一項。係數相乘，指數相加，再把相同次方合併。合併後係數變成 0，就刪掉那一項。
4. **代入求值**：每一項從係數開始，依照指數重複乘上 x，最後把各項加起來。例如 `3x²` 代入 x = 2，就是 `3 × 2 × 2 = 12`。
5. **輸入與輸出**：輸入時先讀取項數，再讀取各項的係數和指數。輸出時依序印出各項，沒有任何項目就印出 0。

例如：

```text
A(x) = 3x^2+2x+1
B(x) = 1x-1
A + B = 3x^2+3x
A * B = 3x^3-1x^2-1x-1
A(2) = 17
B(2) = 1
```

這個例子的輸入是：

```text
3 3 2 2 1 1 0
2 1 1 -1 0
2
```

第一行的 3 表示 A 有 3 項，接著是 `(3,2)`、`(2,1)`、`(1,0)` 三組係數與指數。第二行輸入 B，最後一行輸入要代入的 x。

輸入的項目必須依指數由大到小排列，不能有重複指數。項數與指數都必須是非負整數，零多項式只要輸入項數 0。程式沒有另外檢查輸入錯誤。

### 程式實作

```cpp
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
```

### 效能分析

以下用 m 表示 A 的非零項數，n 表示 B 的非零項數，E 表示求值時最大的指數。空間分析包含運算結果使用的陣列。

| 功能 | 時間複雜度 | 額外空間複雜度 | 說明 |
|---|---|---|---|
| `newTerm()` | 平均攤到每次加入為 O(1) | 擴充時 O(t) | t 是當時項數，擴充時需要複製舊資料 |
| `Add()` | O(m + n) | O(m + n) | 兩個多項式各看過一次 |
| `Mult()` | m、n > 0 時，最壞上界 O(m²n²) | O(mn) | 有 mn 組相乘，每次還可能搜尋及搬移結果中的項 |
| `Eval()` | O(m(E + 1)) | O(1) | 每一項依指數重複乘上 x |
| 輸入 k 筆項目 | O(k) | O(k + 1) | 逐筆讀取，必要時擴充陣列 |
| 輸出 m 項 | O(m + 1) | O(1) | 逐項印出，零多項式也需要印出 0 |

加法只需要依序比較，所以速度和輸入項數有關。乘法除了每兩項相乘，還要找到合併或插入的位置，所以不能只把時間寫成 O(mn)，且如果相同次方很多，合併後的項數會較少。

求值實際需要的乘法次數，是所有項目的指數加總。即使項數相同，指數越大，求值也可能越慢。

陣列容量每次加倍，所以雖然某一次擴充需要複製全部資料，連續加入多項時，平均每次的加入成本仍為 O(1)。單次擴充需要 O(t) 時間。

若其中一個多項式為零，乘法不會執行內層相乘，但仍可能檢查 m 次外層迴圈，時間為 O(m + 1)，結果只需要固定大小的陣列。以上將每次數值運算與單一數值的輸出視為固定成本。

### 測試與驗證

| 測試內容 | 預期結果 | 實際結果 |
|---|---|---|
| A = 3x² + 2x + 1，B = x − 1，相加 | 3x² + 3x | `3x^2+3x` |
| 上述 A、B 相乘 | 3x³ − x² − x − 1 | `3x^3-1x^2-1x-1` |
| 上述 A、B 代入 x = 2 | 17、1 | 17、1 |
| x 加上 −x | 0 | `0` |
| 零多項式乘上 3x² | 0 | `0` |
| 常數 5 代入 x = 0 | 5 | 5 |
| (x + 1)(x − 1) | x² − 1 | `1x^2-1` |
| (−2x³ + 4x)(2x² − 3) | −4x⁵ + 14x³ − 12x | `-4x^5+14x^3-12x` |

輸出保留係數 1，所以 `1x` 就是 x，`-1x^2` 就是 −x²，不會影響計算結果。測試使用小整數，沒有涵蓋所有浮點數誤差情況。

#### 效能量測

分別讓 A、B 都等於 `1 + x + ... + x^(n-1)`，並代入 x = 1。每種大小執行 5 次，把時間由小到大排列，取中間那次的數字。

| 每個多項式的項數 n | 完整執行時間（毫秒） |
|---|---|
| 10 | 8.230 |
| 50 | 8.706 |
| 100 | 9.529 |
| 200 | 11.856 |

時間是從外部量測，包含啟動程式、輸入、加法、乘法、求值與輸出，不是只量某個函式。資料量較小時，啟動程式的時間會占較大的比例。

### 心得討論

我認為這題比較難的地方是維持每一項的排列順序以及整理乘法的結果。輸入時要依照指數由大到小排列，這樣做加法時就能從前面一路比較，不用每次重新尋找相同次方的項目。

乘法的計算本身只需要把係數相乘、指數相加，但不同的項目相乘後，可能會得到相同次方。例如 x 乘上 x，以及 x² 乘上常數，都可能產生 x²，因此還要把這些項目的係數加起來，而如果合併後係數變成 0，就要刪掉那一項，將後面的資料往前移，並更新項數。這些步驟需要一起考慮，是我覺得比較容易寫錯的地方。

另外，輸出也有一些細節要注意，例如第一項前面不印加號、負係數直接印負號，常數項不印 x。雖然不影響計算，但處理好之後，結果會比較清楚，也比較容易檢查是否正確。
