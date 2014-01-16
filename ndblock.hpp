#ifndef NDBLOCK_H
#define NDBLOCK_H

#include "ndtree.hpp"
#include "ndhash.hpp"

class NDBlock {
public:
	NDTree tree;
	NDHash hash;
	NDHash next;
	unsigned int count;
};

#endif