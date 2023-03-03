#include <fstream>
#include <vector>
#include <iostream>
#include <Windows.h>
#include "Resolver.h"
#include "Utils.h"
#include "Generator.hpp"
#include <functional>
#include <algorithm>
size_t reverse_index(size_t index, size_t size){
    return size - index - 1;
}

template <typename Solution>
void PrintGeneratedSolution(Solution&& solution){
    std::cout << "Сгенерированное решение: \n";
    for (auto& x : solution){
        std::cout << x << ' ';
    }
    std::cout << '\n';
}

template<typename Result>
void PrintGenerateResult(Result&& result){
    const auto& [matrix, free_column, solution] = result;
    std::cout << "Сгенерированная матрица: \n";
    for (auto& row: matrix) {
        for (auto elem : row) std::cout << elem << ' ';
        std::cout << '\n';
    }
    std::cout << "Сгенерированный столбец свободных членов: \n";
    for (auto& elem_column : free_column){
        std::cout << elem_column << '\n';
    }
    PrintGeneratedSolution(solution);
}
template <typename Result>
void SaveToFile(Result&& result, size_t k){
    const auto& [matrix, free_column, solution] = result;
    std::ofstream file("in.txt");
    file << k << '\n';
    for (auto& row: matrix) {
        for (auto elem : row) file << elem << '\t';
        file << '\n';
    }
    for (auto& elem_column : free_column){
        file << '\n' << elem_column;
    }
}

template <typename Solution>
void PrintSolution(Solution&& solution){
    std::cout << "Решение: \n";
    for (auto& x : solution){
        std::cout << x << ' ';
    }
    std::cout << '\n';
}

template<typename ExpectedSolution, typename GetSolution>
void CompareSolutions(ExpectedSolution&& expected_solution, GetSolution&& get_solution){
    size_t N = expected_solution.size();
    std::vector<double> arr;
    arr.resize(N);
    for (size_t i = 0; i < N; i++){
        arr[i] = std::abs(expected_solution[i] - get_solution[i]);
    }
    std::cout << "dif: \n";
    for (auto& x : arr){
        std::cout << x << '\n';
    }
    auto max = std::max_element(arr.begin(),  arr.end());
    std::cout << "error: " << *max << '\n';
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    while (true){
        static size_t constexpr k = 3;
        auto checker = [](size_t i, size_t j, size_t size){
            static std::array<std::function<bool(size_t, size_t, size_t)>, 5> checkers{
                    [](size_t i, size_t j, size_t size){
                        return i == k;
                    },
                    [](size_t i, size_t j, size_t size){
                        return i == k + 2;
                    },
                    [](size_t i, size_t j, size_t size){
                        return j == reverse_index(i, size) - 1;
                    },
                    [](size_t i, size_t j, size_t size){
                        return j == reverse_index(i, size);
                    },
                    [](size_t i, size_t j, size_t size){
                        return j  == reverse_index(i, size) + 1;
                    },
            };
            return std::any_of(checkers.begin(), checkers.end(), [=](auto& cur_checker){
                return cur_checker(i,j,size);
            });
        };
        constexpr auto range = 10000;
        constexpr auto gen_traits = Traits<std::int64_t>{
                .kSize = 10,
                .kMin = -range,
                .kMax = range};
        Generator<std::int64_t, gen_traits, RandomSeed::Yes> generator{checker};
        generator.Generate();
        PrintGenerateResult(generator.GetResult());
        SaveToFile(generator.GetResult(), k);
        try{
            auto [matrix, f, kReaded] = ReadFromFile();
            Resolver resolver(matrix, f, kReaded);
            resolver.Solve();
            PrintGeneratedSolution(generator.GetSolution());
            PrintSolution(resolver.GetSolution());
            CompareSolutions(generator.GetSolution(), resolver.GetSolution());
        }
        catch (std::exception& exc){
            std::cout << exc.what();
        }
        std::cin.get();
    }
    return 0;
}
