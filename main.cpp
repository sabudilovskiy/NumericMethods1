#include <fstream>
#include <vector>
#include <iostream>
#include <Windows.h>
#include "Resolver.h"
#include "Utils.h"

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    try{
        auto [matrix, f] = ReadFromFile();
        int k = findK(matrix);
        if (k == -1) {
            BadMatrix();
        }
        Resolver resolver(matrix, f, k);
        resolver.Solve();
    }
    catch (std::exception& exc){
        std::cout << exc.what();
    }
    std::cin.get();
    return 0;
}
