#include "Utils.h"
#include <fstream>
#include <sstream>

int findK(vector_Rational2d &matrix) {
    auto n = matrix.size();
    if (n < 4) return -1;
    size_t i = 1;
    while (i < n - 3){
        if (matrix[i][0] != 0 && matrix[i + 2][0]){
            return i;
        }
        ++i;
    }
    return -1;
}

void BadMatrix() {
    throw std::invalid_argument("Матрица некорректна: не найдены k и k+2 строки");
}

void BadLine(size_t index) {
    std::string error = "Матрица некорректна: ";
    error += std::to_string(index);
    error += " строка имеет отличающийся размер";
    throw std::invalid_argument(error);
}

std::pair<vector_Rational2d, std::vector<Rational>> ReadFromFile() {
    std::ifstream in("in.txt");
    in.imbue(std::locale("en_US.UTF-8"));
    vector_Rational2d matrix;
    std::vector<Rational> f;
    bool end_matrix = false;
    int i = 0;
    int n = -1;
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
            if (n == -1){
                n = cur_line.size();
            }
            else if (cur_line.size() != n){
                BadLine(i);
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
        throw std::invalid_argument( "Нет столбца свободных членов");
    }
    return std::make_pair(std::move(matrix), std::move(f));
}
