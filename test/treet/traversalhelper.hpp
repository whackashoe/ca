#ifndef TRAVERSALHELPER_H
#define TRAVERSALHELPER_H

#include <array>
#include "orientation.hpp"

/****************************************
 *
 *TRAVERSALHELPER :: MOVE DIRECTION / CONTINUE?
 * 
 ****************************************/

template <uint Dimension>
struct TraversalHelper {
	std::array<orientation, Dimension> way; //the direction of our traversal
	bool deeper; //should be set to true if we wish to go further up/down. if not, set to false. 
};

#endif