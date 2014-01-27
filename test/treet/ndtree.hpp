#ifndef NDTREE_H_
#define NDTREE_H_

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <queue>
#include <memory>
#include <string>
#include <sstream>
#include <limits>
#include <cassert>
#include <cmath>
#include <cstdint>
#include "prettyprint.hpp"

//#include "ndhash.hpp"
#include "cexp.hpp"
#include "orientation.hpp"
#include "traversaldirection.hpp"

/*
option to switch between pointerless (trade much higher computation for much less memory)
and pointer based (much higher memory, much less computation)
would be very useful for high dimensional simulations
*/


/****************************************
 *
 * NDTREE :: N-DIMENSIONAL TREE
 * 
 ****************************************/

template <uint Dimension>
class NDTree {
public:
	std::array<NDTree*, cexp::pow(2, Dimension)> nodes;
	NDTree * parent;
	uint state;
	uint position; //position in parents node list
	bool leaf;

	std::vector<NDTree*> getParents(const NDTree * node) const
	{
		std::vector<NDTree *> ret;
		NDTree * traverser = parent;

		while(traverser != nullptr) {
			ret.push_back(traverser);
			traverser = traverser->parent;
		}

		return ret;
	}

public:
	//static std::array<std::array<uint, Dimension>, cexp::pow(2, Dimension)> binary_mod_table;
	static std::array<uint, Dimension+1> moore_pow_table;
	static std::array<std::array<TraversalDirection<Dimension>, cexp::pow(3, Dimension)>, cexp::pow(2, Dimension)> moore_neighbor_table;
	static std::array<std::array<orientation, Dimension>, cexp::pow(3, Dimension)> moore_offset_table;
	static std::array<std::array<orientation, Dimension>, cexp::pow(2, Dimension)> node_orientation_table;
	static std::array<uint, cexp::pow(2, Dimension)> node_position_table;


	NDTree() : parent(nullptr), position(0), leaf(true) {}

	NDTree(NDTree * parent_, const uint position_) : parent(parent_), position(position_), leaf(true) {}
	
	~NDTree()
	{
		if(!leaf)
			for(auto & i : nodes)
				delete i;
	}

	std::string stringifyNodes(const int n=0) const
	{
		std::string m;

		if(!leaf)
			for(const auto & i : nodes)
				m += i->stringifyNodes(n+1);
		else
			m += state + '0';
		
		switch(n % 3) {
			case 0: return ("(" + m + ")"); break;
			case 1: return ("[" + m + "]"); break;
			case 2: return ("{" + m + "}"); break;
		}

		return 0;
	}
	
	friend std::ostream& operator<< (std::ostream &stream, const NDTree &obj) {
		return stream << obj.stringifyNodes();
	}

	NDTree * getAdjacentNode(const std::array<orientation, Dimension> direction) const
	{
		std::queue<std::array<orientation, Dimension>> stack; //holds our directions for going up and down the tree

		std::array<orientation, Dimension> pointing_direction = direction; 
		auto walk = [&](const uint position) {
			bool up_flag = false;

			for(uint d=0; d < Dimension; ++d) {
				if(pointing_direction[d] == CENTER) continue;

				if(pointing_direction[d] == LEFT) {
					if(node_orientation_table[position][d] == LEFT) {
						up_flag = true;
						pointing_direction[d] = RIGHT;
					}
				} else {
					if(node_orientation_table[position][d] == RIGHT) {
						up_flag = true;
						pointing_direction[d] = LEFT;
					}
				}
			}

			std::array<orientation, Dimension> results;
			for(uint i=0; i<pointing_direction.size(); ++i) {
				const auto m = pointing_direction[i] + node_orientation_table[position][i];
				results[i] = static_cast<orientation>(m == CENTER ? node_orientation_table[position][i] : m);
			}
			stack.push(results);
			std::cout << results << std::endl;

			return up_flag;
		};

		NDTree<Dimension> * treePointer = const_cast<NDTree *>(this);
		do {
			if(!walk(treePointer->position)) break;
			pointing_direction = stack.back();
			
			//if parent is nullptr we need to "expand" upwards by creating a new NDTree for the parent,
			if(treePointer->parent == nullptr)
				treePointer->reverseBirth(position);
			
			treePointer = treePointer->parent;
		} while(true);

		//walk back down to our node
		while(!stack.empty()) {
			std::cout << treePointer->position << std::endl;
			if(treePointer->leaf)
				return treePointer;

			auto m = stack.back();
			//ugly, fix with std::negate overload for orientation when available
			std::transform(m.begin(), m.end(), m.begin(), [](orientation o) { return static_cast<orientation>(static_cast<signed int>(o)); });
			treePointer = treePointer->nodes[getPositionFromOrientation<Dimension>(m)];
			stack.pop();
		}

		return treePointer;
	}

	void insertNode(const std::array<double, Dimension> pos, const uint depth_precision)
	{
		NDTree * node { this };

		for(uint i=0; i<depth_precision; ++i) {
			if(node->leaf)
				node->subdivide();

			const double half_dist = 1.0 / cexp::pow(2, i+1);
			uint cell_pos = 0;

			for(uint j=0; j<Dimension; ++j)
				if(pos[j] > half_dist) {
					pos[j] -= half_dist;
					cell_pos += cexp::pow(2, j);
				}

			std::cout << "depth: " << i << " cell pos: " << cell_pos << std::endl;
			node = node->nodes[cell_pos];
		}
	}

	void removeNode(const std::array<double, cexp::pow(2, Dimension)> pos)
	{

	}

	void subdivide()
	{
		if(!leaf) return;

		for(uint i=0; i < cexp::pow(2, Dimension); ++i)
			nodes[i] = new NDTree(this, i);

		leaf = false;
	}

	void collapse()
	{
		if(leaf) return;
		nodes.clear();
		leaf = true;
	}

	void reverseBirth(uint position)
	{
		assert(parent == nullptr);
		parent = new NDTree<Dimension>();
		parent->subdivide();
		parent->nodes[position] = this;
		parent->leaf = false;
	}

	void calculateHash()
	{

	}
};


/****************************************
 *
 * TABLE GENERATING FUNCTIONS
 * 
 ****************************************/


/*
//unused
template <uint Dimension, uint Number>
std::array<std::array<uint, Dimension>, cexp::pow(Number, Dimension)> generateModTable()
{	/\*calculate a mod table so we can easily see if node values per dimension are on left or right side e.g. in 2d (node-val 3 % 4 > 2 so bottom Y axis and node-val 3 % 2 > 1)*\/
	std::array<std::array<uint, Dimension>, cexp::pow(Number, Dimension)> r;

	for(uint i=0; i < r.size(); ++i)
		for(uint j=0; j < Dimension; ++j)
			r[i][j] = i % cexp::pow(Number, j);

	return r;
}*/

template <uint Dimension, uint Power>
std::array<uint, Dimension+1> generatePowTable()
{	/*calculate orientation (left/right) of each node numbered sequentially in an n-dimensional cube*/
	std::array<uint, Dimension+1> r;

	for(uint i=0; i < r.size(); ++i)
		r[i] = cexp::pow(Power, i);

	return r;
}

template <uint Dimension>
std::array<std::array<orientation, Dimension>, cexp::pow(2, Dimension)> generateNodeOrientationTable()
{	/*calculate orientation (left/right) of each node numbered sequentially in an n-dimensional cube*/
	std::array<std::array<orientation, Dimension>, cexp::pow(2, Dimension)> r;
	
	for(uint i=0; i < r.size(); ++i)
		r[i] = getOrientationFromPosition<Dimension>(i);

	return r;
}

template <uint Dimension>
std::array<uint, cexp::pow(2, Dimension)> generateNodePositionTable()
{	/*calculates the position / index of each node based on its orientation*/
	std::array<uint, cexp::pow(2, Dimension)> r;
	
	for(uint i=0; i < r.size(); ++i)
		r[i] = NDTree<Dimension>::getPositionFromOrientation<Dimension>(NDTree<Dimension>::node_orientation_table[i]);

	return r;
}


template <uint Dimension>
std::array<std::array<orientation, Dimension>, cexp::pow(3, Dimension)> generateMooreOffsetTable()
{	/*calculates an offset from center of each node in each dimension (center of hypercube of length 3)*/
	std::array<std::array<orientation, Dimension>, cexp::pow(3, Dimension)> r;

	NDTree<Dimension>::moore_pow_table = generatePowTable<Dimension, 3>();

	for(uint i=0; i < r.size(); ++i) {
		for(uint j=0; j < Dimension; ++j) {
			const int moore_mod = i % NDTree<Dimension>::moore_pow_table[j+1];
			if(moore_mod < NDTree<Dimension>::moore_pow_table[j+1]-(NDTree<Dimension>::moore_pow_table[j]*2))
				r[r.size()-1-i][j] = RIGHT;
			else if(moore_mod < NDTree<Dimension>::moore_pow_table[j+1]-NDTree<Dimension>::moore_pow_table[j])
				r[r.size()-1-i][j] = CENTER;
			else
				r[r.size()-1-i][j] = LEFT;
		}
	}

	return r;
}

template <uint Dimension>
std::array<std::array<TraversalDirection<Dimension>, cexp::pow(3, Dimension)>, cexp::pow(2, Dimension)> generateMooreNeighborTable()
{	/*calculate offset via overlaying a moore 3 length hypercubes center over each position of the base nodes(hypercube)
	 and calculating the offset including if we need to go up one level(parent) to access adjacent node*/
	std::array<std::array<TraversalDirection<Dimension>, cexp::pow(3, Dimension)>, cexp::pow(2, Dimension)> r;

	for(uint c=0; c < r.size(); ++c) { //cube nodes			
		for(uint m=0; m < r[c].size(); ++m) { //moore-nodes

			TraversalDirection<Dimension> tdir;

			for(uint d=0; d < Dimension; ++d) {
				if(NDTree<Dimension>::node_orientation_table[c][d] == LEFT) {
					switch(NDTree<Dimension>::moore_offset_table[m][d]) {
						case LEFT:
							tdir.parent_direction[d] = LEFT;
							tdir.node += cexp::pow(2, d);
							break;
						case RIGHT: tdir.node += cexp::pow(2, d); break;
						case CENTER: break;
						default: /*todo:: add bad memory error handling*/ break;
					}
				} else {
					switch(NDTree<Dimension>::moore_offset_table[m][d]) {
						case LEFT: tdir.node += cexp::pow(2, d); break;
						case RIGHT:
							tdir.parent_direction[d] = RIGHT;
							tdir.node += cexp::pow(2, d);
							break;
						case CENTER: break;
						default: break;
					}
				}
			}

			r[c][m] = tdir;
		}
	}

	return r;
}

/****************************************
 *
 * STATIC MEMBER INITIALIZATION
 * 
 ****************************************/

//template <uint Dimension>
//std::array<std::array<uint,  Dimension>, cexp::pow(2, Dimension)>
//NDTree<Dimension>::binary_mod_table = generateModTable<Dimension, 2>();

//initialized in generate_moore_offset_table due to compiler running these out of order
template <uint Dimension>
std::array<uint, Dimension+1>
NDTree<Dimension>::moore_pow_table = generatePowTable<Dimension, 3>();

template <uint Dimension>
std::array<std::array<orientation, Dimension>, cexp::pow(2, Dimension)> 
NDTree<Dimension>::node_orientation_table = generateNodeOrientationTable<Dimension>();

template <uint Dimension>
std::array<uint, cexp::pow(2, Dimension)>
NDTree<Dimension>::node_position_table = generateNodePositionTable<Dimension>();


template <uint Dimension>
std::array<std::array<orientation, Dimension>, cexp::pow(3, Dimension)>
NDTree<Dimension>::moore_offset_table = generateMooreOffsetTable<Dimension>();

template <uint Dimension>
std::array<std::array<TraversalDirection<Dimension>, cexp::pow(3, Dimension)>, cexp::pow(2, Dimension)>
NDTree<Dimension>::moore_neighbor_table = generateMooreNeighborTable<Dimension>();

#endif