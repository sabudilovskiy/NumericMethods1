#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <boost/rational.hpp>
#include <Windows.h>
#include <functional>

using Rational = boost::rational<std::int64_t>;
using vector_Rational = std::vector<Rational>;
using vector_Rational2d = std::vector<std::vector<Rational>>;
using vector_RefRational = std::vector<Rational*>;
using vector_RefRational2d = std::vector<std::vector<Rational*>>;


void Print(vector_Rational2d& matrix, std::vector<Rational>& f){
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


class Solution{
    vector_RefRational a;
    vector_RefRational b;
    vector_RefRational c;
    vector_RefRational p;
    vector_RefRational q;
    vector_Rational& f;
    vector_Rational2d& matrix;
    vector_Rational2d matrix2;
    vector_Rational f2;
    size_t n;
    size_t k;
    void PrintAll(){
        std::cout << "First matrix\n";
        Print(matrix, f);
        std::cout << "Second matrix\n";
        Print(matrix2, f2);
    }
    bool HaveMissmatches(){
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
    void Check(){
        if (HaveMissmatches()) PrintAll();
    }
    void SubLineDirect(size_t from, size_t to, Rational k){
        for (int i = 0; i < matrix2.size(); ++i){
            matrix2[to][i] += k * matrix2[from][i];
        }
        f2[to] += f2[from] * k;
        Check();
    }
    void SubLineEffUpToDown(size_t i){
        *b[i] -= *a[i-1] * *c[i-1];
        f[i] -= f[i-1] * *c[i-1];
        *c[i-1] = 0;
    }
    void SubLineEffUpToLineP(size_t i){
        *p[i] -= *a[i - 1] * *p[i - 1];
        f[k] -= f[i-1] * *p[i - 1];
        *p[i - 1] = 0;
    }

    void SubLineEffUpToLineQ(size_t i){
        *q[i] -= *a[i - 1] * *q[i - 1];
        f[k+2] -= f[i-1] * *q[i - 1];
        *q[i - 1] = 0;
    }

    void SubLineEffDownToUp(size_t i){
        *b[i] -= *c[i] * *a[i];
        f[i] -= f[i + 1] * *a[i];
        *a[i] = 0;
    }

    void SubLineEffDownToP(size_t i){
        *p[i] -= *c[i] * *p[i + 1];
        f[k] -= f[i + 1] * *p[i + 1];
        *p[i + 1] = 0;
    }
    void SubLineEffDownToQ(size_t i){
        *q[i] -= *c[i] * *q[i + 1];
        f[k + 2] -= f[i + 1] * *q[i + 1];
        *q[i + 1] = 0;
    }

    void NormalizeLineDirect(size_t index, Rational k){
        for (int i = 0; i < matrix2.size(); ++i){
            matrix2[index][i] /= k;
        }
        f2[index] /= k;
        Check();
    }
    void NormalizeLineEffUpToDown(size_t i){
        *a[i] /= *b[i];
        f[i] /= *b[i];
        *b[i] = 1;
    }
    void NormalizeLineEffDownToUp(size_t i){
        f[i] /= *b[i];
        *c[i - 1] /= *b[i];
        *b[i] = 1;
    }
    void FirstStep(){
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
            SubLineEffUpToLineP(i);
            SubLineDirect(i - 1, k, -matrix2[k][n - i]);
            //зануляем i столбец в q
            SubLineEffUpToLineQ(i);
            SubLineDirect(i - 1, k + 2, -matrix2[k + 2][n - i]);
        }
    }
    void SecondStep(){
        std::cout << "Шаг 2\n";
        std::cout << "==============================\n";
        NormalizeLineEffDownToUp(n - 1);
        NormalizeLineDirect(n - 1, matrix2[n - 1][0]);
        //идём до q
        for (auto i = n - 2; i > k + 2; --i){
            SubLineEffDownToUp(i);
            SubLineDirect(i + 1, i, -matrix2[i][n - i - 2]);
            //зануляем i столбец в p
            if (i != k + 2){
                //нормируем строку
                NormalizeLineEffDownToUp(i);
                NormalizeLineDirect(i, matrix2[i][n - i - 1]);
                SubLineEffDownToP(i);
                SubLineDirect(i + 1, k, -matrix2[k][n - i - 2]);
                //зануляем i столбец в q
                SubLineEffDownToQ(i);
                SubLineDirect(i + 1, k + 2, -matrix2[k + 2][n - i - 2]);
            }
        }
    }
    void ThirstStep(){
        std::cout << "Шаг 3\n";
        std::cout << "==============================\n";
        PrintAll();
        SubLineEffDownToP(k+3);
        SubLineDirect(k+3, k, -matrix2[k][k+3]);
//        SubLineDirect(k+3, k+2, -matrix2[k+2][k+3]);
//        SubLineEffDownToQ(k+3);
    }
    void FourthStep(){
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
        *matrix3of3[0][1] /= *matrix3of3[0][0]; *matrix3of3[0][2] /= *matrix3of3[0][0]; *f3[0] /= *matrix3of3[0][0]; *matrix3of3[0][0] = 1;
    }
public:
    Solution(vector_Rational2d &matrix, vector_Rational &f, size_t k) : matrix(matrix), f(f), k(k) {
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
    void Solve(){
        FirstStep();
        SecondStep();
        ThirstStep();
        FourthStep();
    }
};



int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    std::ifstream in("in.txt");
    std::ofstream out("out.txt");
    in.imbue(std::locale("en_US.UTF-8"));
    vector_Rational2d matrix, matrix2;
    std::vector<Rational> f, f2;
    bool end_matrix = false;
    int k = 3;
    int i = 0;
    std::cout << std::setprecision(2);
    while (!in.eof() && !end_matrix){
        std::string line;
        std::getline(in, line);
        if (!line.empty()){
            std::stringstream ss(line);
            auto& cur_line = matrix.emplace_back();
            while (!ss.eof()){
                int64_t temp;
                ss >> temp;
                cur_line.emplace_back(temp);
            }
            ++i;
        }
        else end_matrix = true;
    }
    while (!in.eof()){
        std::int64_t temp;
        in >> temp;
        f.emplace_back(temp);
    }
    if (matrix.size() != f.size()) {
        out << "А где столбец корректный?";
        return -1;
    }
    Solution solution(matrix, f, k);
    solution.Solve();
    std::cin.get();
    return 0;
}
