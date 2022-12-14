#pragma once
#include <utility>
#include "Resolver.h"

void BadLine(size_t index);
void BadMatrix();
int findK(vector_Rational2d& matrix);
std::pair<vector_Rational2d, std::vector<Rational>>  ReadFromFile();