#include "Resolver.h"
#include <iostream>

bool Resolver::HaveMissmatches() {
    auto n = matrix.size();
    bool have_missmatch = false;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (matrix[i][j] != matrix2[i][j]){
                std::cout << "missmatch in M : " << i << ' ' << j << ' ' << matrix[i][j] << ' ' << matrix2[i][j] << '\n';
                have_missmatch = true;
            }
        }
        if (f[i] != f2[i]){
            std::cout << "missmatch in f : " << i << ' ' << f[i] << ' ' << f2[i] << '\n';
            have_missmatch = true;
        }
    }
    return have_missmatch;
}

void Resolver::PrintAll() {
    std::cout << "First matrix\n";
    Print(matrix, f);
    std::cout << "Second matrix\n";
    Print(matrix2, f2);
    std::cout << "==============================\n";
}

void Resolver::Check() {
    if (HaveMissmatches()) PrintAll();
}

size_t Resolver::ReverseIndex(size_t i) {
    return n - 1 - i;
}

size_t Resolver::ReverseIndex(size_t i, size_t n) {
    return n - 1 - i;
}

void Resolver::SubLineDirect(size_t from, size_t to, Rational k) {
    for (int i = 0; i < matrix2.size(); ++i){
        matrix2[to][i] += k * matrix2[from][i];
    }
    f2[to] += f2[from] * k;
    Check();
}

void Resolver::SubLineEffUpToDown(size_t i) {
    *b[i] -= *a[i-1] * *c[i-1];
    f[i] -= f[i-1] * *c[i-1];
    *c[i-1] = 0;
}

void Resolver::SubLineEffUpToP(size_t index) {
    *p[index + 1] -= *a[index] * *p[index];
    f[k] -= f[index] * *p[index];
    *p[index] = 0;
}

void Resolver::SubLineEffUpToQ(size_t index) {
    *q[index + 1] -= *a[index] * *q[index];
    f[k+2] -= f[index] * *q[index];
    *q[index] = 0;
}

void Resolver::SubLineEffDownToUp(size_t i) {
    *b[i] -= *c[i] * *a[i];
    f[i] -= f[i + 1] * *a[i];
    *a[i] = 0;
}

void Resolver::SubLineEffDownToP(size_t index) {
    *p[index - 1] -= *c[index - 1] * *p[index];
    f[k] -= f[index] * *p[index];
    *p[index] = 0;
}

void Resolver::SubLineEffDownToQ(size_t index) {
    *q[index - 1] -= *c[index - 1] * *q[index];
    f[k + 2] -= f[index] * *q[index];
    *q[index] = 0;
}

void Resolver::NormalizeLineDirect(size_t index, Rational k) {
    for (int i = 0; i < matrix2.size(); ++i){
        matrix2[index][i] /= k;
    }
    f2[index] /= k;
    Check();
}

void Resolver::NormalizeLineEffUpToDown(size_t i) {
    *a[i] /= *b[i];
    f[i] /= *b[i];
    *b[i] = 1;
}

void Resolver::NormalizeLineEffDownToUp(size_t i) {
    f[i] /= *b[i];
    *c[i - 1] /= *b[i];
    *b[i] = 1;
}

void Resolver::FirstStep() {
    //делаем единичку на побочной диагонали в первой строке
    std::cout << "Шаг 1\n";
    std::cout << "==============================\n";
    NormalizeLineEffUpToDown(0);
    NormalizeLineDirect(0, matrix2[0][n - 1]);
    for (auto i = 1; i < k; ++i){
        //зануляем i столбец в строке под текущей
        SubLineEffUpToDown(i);
        SubLineDirect(i - 1, i, -matrix2[i][n - i]);
        //нормируем строку
        NormalizeLineEffUpToDown(i);
        NormalizeLineDirect(i, matrix2[i][n - i - 1]);
        //зануляем i столбец в p
        SubLineEffUpToP(i - 1);
        SubLineDirect(i - 1, k, -matrix2[k][n - i]);
        //зануляем i столбец в q
        SubLineEffUpToQ(i - 1);
        SubLineDirect(i - 1, k + 2, -matrix2[k + 2][n - i]);
    }
    PrintAll();
}

void Resolver::SecondStep() {
    std::cout << "Шаг 2\n";
    std::cout << "==============================\n";
    NormalizeLineEffDownToUp(n - 1);
    NormalizeLineDirect(n - 1, matrix2[n - 1][0]);
    //идём до q
    for (auto i = n - 2; i > k + 2; --i){
        SubLineEffDownToUp(i);
        SubLineDirect(i + 1, i, -matrix2[i][ReverseIndex(i+1)]);
        //зануляем i столбец в p
        if (i != k + 2){
            //нормируем строку
            NormalizeLineEffDownToUp(i);
            NormalizeLineDirect(i, matrix2[i][ReverseIndex(i)]);
            SubLineEffDownToP(i + 1);
            SubLineDirect(i + 1, k, -matrix2[k][ReverseIndex(i + 1)]);
            //зануляем i столбец в q
            SubLineEffDownToQ(i + 1);
            SubLineDirect(i + 1, k + 2, -matrix2[k + 2][ReverseIndex(i + 1)]);
        }
    }
    PrintAll();
}

void Resolver::ThirstStep() {
    std::cout << "Шаг 3\n";
    std::cout << "==============================\n";
    SubLineEffDownToP(ReverseIndex(k));
    SubLineDirect(k+3, k, -matrix2[k][k]);
    SubLineEffDownToQ(ReverseIndex(k));
    SubLineDirect(k+3, k+2, -matrix2[k+2][k]);

    SubLineEffUpToP(k - 1);
    SubLineDirect(k - 1, k, -matrix2[k][ReverseIndex(k - 1)]);
    SubLineEffUpToQ(k - 1);
    SubLineDirect(k - 1, k + 2, -matrix2[k + 2][ReverseIndex(k - 1)]);

    PrintAll();
}

void Resolver::NormalizeLineCore(vector_RefRational2d &matrix3of3, vector_RefRational &f3, int index) {
    int reverse_index = ReverseIndex(index, 3);
    for (int i = reverse_index - 1; i >= 0; i--){
        *matrix3of3[index][i] /= *matrix3of3[index][reverse_index];
    }
    *f3[index] /= *matrix3of3[index][reverse_index];
    *matrix3of3[index][reverse_index] = 1;
}

void Resolver::SubLineCoreRight(vector_RefRational2d &matrix3of3, vector_RefRational &f3, int from, int to) {
    int reverse_index = ReverseIndex(from, 3);
    for (int j = reverse_index - 1; j >= 0; j--){
        *matrix3of3[to][j] -=  *matrix3of3[from][j] * *matrix3of3[to][reverse_index];
    }
    *f3[to] -= *f3[from] * *matrix3of3[to][reverse_index];
    *matrix3of3[to][reverse_index] = 0;
}

void Resolver::SubLineCoreLeft(vector_RefRational2d &matrix3of3, vector_RefRational &f3, int from, int to) {
    *f3[to] -= *f3[from] * *matrix3of3[to][ReverseIndex(from, 3)];
    *matrix3of3[to][ReverseIndex(from, 3)] = 0;
}

void Resolver::FourthStep() {
    std::cout << "Шаг 4\n";
    std::cout << "==============================\n";
    vector_RefRational2d matrix3of3 = {
            {p[k+2], p[k+1], p[k] }, // f[k]
            {a[k+1], b[k+1], c[k]},  // f[k+1]
            {q[k+2], q[k+1], q[k]}   // f[k+2]
    };
    vector_RefRational f3 = {
            &f[k],
            &f[k+1],
            &f[k+2]
    };
    for (int i = 0; i < 3; i++){
        NormalizeLineCore(matrix3of3, f3, i);
        NormalizeLineDirect(k + i, matrix2[k + i][ReverseIndex(k+i)]);
        for (int j = i + 1; j < 3; j++){
            SubLineCoreRight(matrix3of3, f3, i, j);
            SubLineDirect(k + i, k + j, -matrix2[k+j][ReverseIndex(k + i)]);
        }
    }
    for (int i = 2; i >= 1; i--){
        for (int j = i - 1; j >= 0; j--){
            SubLineCoreLeft(matrix3of3, f3, i, j);
            SubLineDirect(k+i, k+j,-matrix2[k+j][ReverseIndex(k+i)]);
        }
    }
    PrintAll();
}

Resolver::Resolver(vector_Rational2d &matrix, vector_Rational &f, size_t k) : matrix(matrix), f(f), k(k) {
    n = matrix.size();
    a.resize(n-1);
    b.resize(n);
    c.resize(n-1);
    for (auto i = 0; i <= n - 2; ++i){
        a[i] = &matrix[i][n - i - 2];
    }
    for (auto i = 0; i <= n - 1; ++i){
        b[i] = &matrix[i][n-i-1];
    }
    for (auto i = 0; i <= n - 2; ++i) {
        c[i] = &matrix[i+1][n-i-1];
    }
    p.resize(n);
    q.resize(n);
    for (auto i = 0; i < n; ++i){
        p[i] = &matrix[k][n - i - 1];
        q[i] = &matrix[k+2][n - i - 1];
    }
    matrix2 = matrix;
    f2 = f;
    Check();
}

void Resolver::FiveStep() {
    std::cout << "Шаг 5\n";
    std::cout << "==============================\n";
    for (auto i = k + 3; i < n; ++i){
        SubLineEffUpToDown(i);
        SubLineDirect(i - 1, i, -matrix2[i][ReverseIndex(i - 1)]);
    }
    for (int i = k - 1; i>= 0; --i){
        SubLineEffDownToUp(i);
        SubLineDirect(i + 1, i, -matrix2[i][ReverseIndex(i + 1)]);
    }
    PrintAll();
}

void Resolver::Solve() {
    FirstStep();
    SecondStep();
    ThirstStep();
    FourthStep();
    FiveStep();
}

void Print(vector_Rational2d &matrix, std::vector<Rational> &f) {
    auto it = f.begin();
    for (auto& line : matrix){
        for (auto row: line) {
            if (row.denominator() == 1) std::cout << row.numerator() << ' ';
            else std::cout << row << ' ';
        }
        std::cout << '|';
        if (it->denominator() == 1) std::cout << it->numerator();
        else std::cout << *it;
        std::cout << '\n';
        ++it;
    }
    std::cout << '\n';
}
