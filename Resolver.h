#pragma once
#include <vector>
#include <boost/rational.hpp>

using Rational = boost::rational<std::int64_t>;
using vector_Rational = std::vector<Rational>;
using vector_Rational2d = std::vector<std::vector<Rational>>;
using vector_RefRational = std::vector<Rational*>;
using vector_RefRational2d = std::vector<std::vector<Rational*>>;


void Print(vector_Rational2d& matrix, std::vector<Rational>& f);

class Resolver{
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
    void PrintAll();
    bool HaveMissmatches();
    void Check();
    size_t ReverseIndex(size_t i);
    static size_t ReverseIndex(size_t i, size_t n);
    void SubLineDirect(size_t from, size_t to, Rational k);
    void SubLineEffUpToDown(size_t i);
    void SubLineEffUpToP(size_t index);
    void SubLineEffUpToQ(size_t index);
    void SubLineEffDownToUp(size_t i);
    void SubLineEffDownToP(size_t index);
    void SubLineEffDownToQ(size_t index);
    void NormalizeLineDirect(size_t index, Rational k);
    void NormalizeLineEffUpToDown(size_t i);
    void NormalizeLineEffDownToUp(size_t i);
    void FirstStep();
    void SecondStep();
    void ThirstStep();
    static void NormalizeLineCore(vector_RefRational2d& matrix3of3, vector_RefRational& f3, int index);
    static void SubLineCoreRight(vector_RefRational2d& matrix3of3, vector_RefRational& f3, int from, int to);
    static void SubLineCoreLeft(vector_RefRational2d& matrix3of3, vector_RefRational& f3, int from, int to);
    void FourthStep();
    void FiveStep();
public:
    Resolver(vector_Rational2d &matrix, vector_Rational &f, size_t k);
    void Solve();
};