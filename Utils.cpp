#include "Utils.h"
#include <fstream>
#include <sstream>

int findK(std::ifstream &file) {
    std::string temp;
    std::getline(file, temp);
    return std::stoi(temp);
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

std::tuple<vector_Rational2d, std::vector<Rational>, int> ReadFromFile() {
    std::ifstream in("in.txt");
    in.imbue(std::locale("en_US.UTF-8"));
    int k = findK(in);
    if (k == -1) {
        BadMatrix();
    }
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
                if (!ss.fail()) {
                    cur_line.emplace_back(temp);
                }
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
    return std::make_tuple(std::move(matrix), std::move(f), k);
}
