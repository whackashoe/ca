#ifndef TRAVERSALHELPER_H
#define TRAVERSALHELPER_H

#include <array>
#include "orientation.hpp"

template <uint Dimension>
struct TraversalHelper {
	std::array<orientation, Dimension> way;
	bool deeper;
};

#endif