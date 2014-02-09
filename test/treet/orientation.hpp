#ifndef ORIENTATION_H
#define ORIENTATION_H

#include <iostream>

/****************************************
 *
 *ORIENTATION :: ONE DIMENSION DIRECTION
 * 
 ****************************************/

enum orientation : signed int {LEFT = -1, CENTER = 0, RIGHT = 1};
std::ostream& operator<< (std::ostream &stream, const orientation &obj)
{
	return stream << ((obj == LEFT) ? "LEFT" : (obj == CENTER) ? "CENTER" : (obj == RIGHT) ? "RIGHT" : "BAD_VAL");
}

/****************************************
 *
 * BASIC HELPER FUNCTIONS
 * 
 ****************************************/

 orientation flip(const orientation o)
 {
 	return static_cast<orientation>(static_cast<signed int>(o)*-1);
 }

template <uint Dimension>
std::array<orientation, Dimension> getOrientationFromPosition(const uint position)
{
	std::array<orientation, Dimension> r;
	
	for(uint i = 0; i < Dimension; ++i)
		if(position % cexp::pow(2, i+1) < cexp::pow(2, i))
			r[i] = LEFT;
		else
			r[i] = RIGHT;

	return r;
}

template <uint Dimension>
uint getPositionFromOrientation(const std::array<orientation, Dimension> orientation)
{
	uint r { 0 };
	
	for(uint i = 0; i < Dimension; ++i)
		if(orientation[i] == RIGHT) 
			r += cexp::pow(2, i);
	
	return r;
}

#endif