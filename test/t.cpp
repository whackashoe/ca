#include <array>
#include <iostream>
#include "prettyprint.hpp"
#include "cexp.hpp"

template <int Dimension>
std::array<std::array<int, Dimension>, cexp::exp2(Dimension)> generateModTable()
{
	std::array<std::array<int, Dimension>, (2 << Dimension)> r;

	for(int i=0; i < (2 << Dimension); ++i)
		for(int j=0; j < Dimension; ++j)
			r[i][j] = i % (2 << j);

	return r;
}

int main() {
	std::cout << generateModTable<2>() << std::endl;
}