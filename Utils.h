#pragma once
#include <utility>
#include "Resolver.h"

void BadLine(size_t index);
void BadMatrix();
int findK(std::ifstream& file);
std::tuple<vector_Rational2d, std::vector<Rational>, int>  ReadFromFile();