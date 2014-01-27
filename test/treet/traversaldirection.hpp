#ifndef TRAVERSALDIRECTION_H
#define TRAVERSALDIRECTION_H

#include <array>
#include <iostream>
#include "orientation.hpp"

template <uint Dimension>
struct TraversalDirection {
	std::array<orientation, Dimension> parent_direction {}; //direction one level up from cube-node
	int node {0}; //node in cube

	bool operator==(TraversalDirection<Dimension> rhs) const
	{
		for(uint i=0; i<Dimension; ++i)
			if(parent_direction[i] != rhs.parent_direction[i]) return false;

		if(node != rhs.node) return false;

		return true;
	}

	friend std::ostream& operator<< (std::ostream &stream, const TraversalDirection &obj)
	{
		return stream << "(" << obj.node << " :: " << obj.parent_direction << ")";
	}

	TraversalDirection<Dimension> fillParentDirection(const orientation v)
	{
		TraversalDirection<Dimension> r;
		for(uint i=0; i<Dimension; ++i)
			r.parent_direction[i] = v;

		return r;
	}
};

#endif