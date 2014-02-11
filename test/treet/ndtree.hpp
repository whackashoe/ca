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
#include <utility>
#include <numeric>
#include "prettyprint.hpp"

//#include "ndhash.hpp"
#include "cexp.hpp"
#include "orientation.hpp"
#include "traversaldirection.hpp"
#include "traversalhelper.hpp"

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

template <uint Dimension, typename StateType>
class NDTree {
public:
	std::array<NDTree*, cexp::pow(2, Dimension)> * nodes;
	NDTree * parent;
	StateType state;
	char position; //position in parents node list
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


	NDTree() : 
		nodes(new std::array<NDTree*, cexp::pow(2, Dimension)>), 
		parent(nullptr),
		position(0),
		leaf(true) {}

	NDTree(NDTree * parent_, const uint position_) :
		nodes(new std::array<NDTree*, cexp::pow(2, Dimension)>),
		parent(parent_),
		position(position_),
		leaf(true) {}
	
	//copy constructor
	NDTree(const NDTree & rhs) :
		parent(rhs.parent),
		state(rhs.state),
		position(rhs.position),
		leaf(rhs.leaf)
	{
		if(!rhs.leaf)
			nodes = rhs.nodes;
	}


	//move constructor
	NDTree(NDTree && rhs) :
		parent(std::move(rhs.parent)),
		state(std::move(rhs.state)),
		position(std::move(rhs.position)),
		leaf(std::move(rhs.leaf))
	{
		rhs.parent = nullptr;
		if(!rhs.leaf) {
			nodes = std::move(rhs.nodes);
			rhs.nodes = nullptr;
		}

	}

	//destructor
	~NDTree()
	{
		if(!leaf)
			for(auto & i : (*nodes))
				delete i;
	}

	// assignment
	NDTree const & operator=(NDTree & rhs)
	{
		if(!rhs.leaf)
			nodes = rhs.nodes;

		parent   = rhs.parent;
		state    = rhs.state;
		position = rhs.position;
		leaf     = rhs.leaf;

		return (*this);
	}

	NDTree const & operator=(const NDTree & rhs)
	{
		if(!rhs.leaf)
			nodes = rhs.nodes;

		parent   = rhs.parent;
		state    = rhs.state;
		position = rhs.position;
		leaf     = rhs.leaf;

		return (*this);
	}

	//pointerized assignment
	NDTree const & operator=(const NDTree * rhs)
	{
		if(!rhs->leaf)
			nodes = rhs->nodes;

		parent   = rhs->parent;
		state    = rhs->state;
		position = rhs->position;
		leaf     = rhs->leaf;

		return (*this);
	}

	/* get node value */
	NDTree const &operator[](const int i) const
	{
		assert("array access out of bounds" && i >= 0 && i < cexp::pow(2, Dimension));
		return (*(*nodes)[i]);
	}

	/* set node value */
	NDTree &operator[](const int i)
	{
		assert("array access out of bounds" && i >= 0 && i < cexp::pow(2, Dimension));
		return (*(*nodes)[i]);
	}

	/* we can pretty print our tree out*/
	friend std::ostream& operator<< (std::ostream &stream, const NDTree &obj)
	{
		std::function<std::string(const int, const NDTree &)> stringifyNodes = [&](const int n, const NDTree & obj)
		{
			std::stringstream m;

			if(!obj.leaf)
				for(int i=0; i<cexp::pow(2, Dimension); ++i)
					m << stringifyNodes(n+1, obj[i]);
			else
				m << static_cast<int>(obj.state);
			
			switch(n % 4) {
				case 0: return ("(" + m.str() + ")"); break;
				case 1: return ("[" + m.str() + "]"); break;
				case 2: return ("{" + m.str() + "}"); break;
				case 3: return ("<" + m.str() + ">"); break;
			}

			return std::string();
		};

		return stream << stringifyNodes(0, obj);
	}

	const int size()
	{
		return (*nodes).size();
	}

	TraversalHelper<Dimension> traverse(const std::array<orientation, Dimension> dir, const std::array<orientation, Dimension> cmp) const
	{
		TraversalHelper<Dimension> depth;

		for(uint d=0; d < Dimension; ++d) {
			switch(dir[d]) {
				case CENTER:
					depth.way[d] = CENTER;
					break;

				case LEFT:
					if(cmp[d] == RIGHT) {
						depth.way[d] = LEFT;
					} else {
						depth.way[d] = RIGHT;
						depth.deeper = true;
					}
					break;

				case RIGHT:
					if(cmp[d] == LEFT) {
						depth.way[d] = RIGHT;
					} else {
						depth.way[d] = LEFT;
						depth.deeper = true;
					}
					break;
			}
		}

		return depth;
	}

	std::array<orientation, Dimension> uncenter(const std::array<orientation, Dimension> dir, const std::array<orientation, Dimension> cmp) const
	{
		std::array<orientation, Dimension> way;

		for(uint d=0; d < Dimension; ++d)
			way[d] = (dir[d] == CENTER) ? cmp[d] : dir[d];
		
		return way;
	}

	NDTree * getAdjacentNode(const std::array<orientation, Dimension> direction) const
	{
		//our first traversal pass
		TraversalHelper<Dimension> pass = traverse(direction, node_orientation_table[position]);

		//if we are lucky the direction results in one of our siblings
		if(!pass.deeper)
			return (*(*parent).nodes)[getPositionFromOrientation<Dimension>(pass.way)];


		std::vector<std::array<orientation, Dimension>> up;   //holds our directions for going up the tree
		std::vector<std::array<orientation, Dimension>> down; //holds our directions for going down
		NDTree<Dimension, StateType> * tp = parent;           //tp is our tree pointer
		up.push_back(pass.way); //initialize with our first pass we did above

		while(true) {
			//continue going up as long as it takes, baby
			pass = traverse(up.back(), node_orientation_table[tp->position]);
			std::cout << pass.way << " :: " << uncenter(pass.way, node_orientation_table[tp->position]) << std::endl;

			if(!pass.deeper) //we've reached necessary top
				break;
			up.push_back(pass.way);
			
			//if we don't have any parent we must explode upwards
			if(tp->parent == nullptr)
				tp->reverseBirth(tp->position);
			
			tp = tp->parent;
		}

		//line break ups and downs
		std::cout << std::endl;

		//traverse upwards combining the matrices to get our actual position in cube
		tp = const_cast<NDTree *>(this);
		for(int i=1; i<up.size(); i++) {
			std::cout << up[i] << " :: " << uncenter(up[i], node_orientation_table[tp->position]) << std::endl;
			down.push_back(uncenter(up[i], node_orientation_table[tp->parent->position]));
			tp = tp->parent;
		}

		//make our way back down (tp is still set to upmost parent from above)
		for(const auto & i : down) {
			int pos = 0; //we need to get the position from an orientation list

			for(int d=0; d<i.size(); d++)
				if(i[d] == RIGHT)
					pos += cexp::pow(2, d); //consider left as 0 and right as 1 << dimension

			//grab the child of treepointer via position we just calculated
			tp = (*(*tp).nodes)[pos];
		}

		return tp;
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
			node = (*(*node).nodes)[cell_pos];
		}
	}

	void removeNode(const std::array<double, cexp::pow(2, Dimension)> pos)
	{

	}

	void subdivide()
	{
		if(!leaf) return;

		for(uint i=0; i < cexp::pow(2, Dimension); ++i)
			(*nodes)[i] = new NDTree(this, i);

		leaf = false;
	}

	void collapse()
	{
		if(leaf) return;
		(*nodes).clear();
		leaf = true;
	}

	void reverseBirth(uint position)
	{
		assert(parent == nullptr);
		parent = new NDTree<Dimension, StateType>();
		parent->subdivide();
		//(*(*parent).nodes)[position] = this;
		(*parent)[position] = this;
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

template <uint Dimension, typename StateType>
std::array<uint, cexp::pow(2, Dimension)> generateNodePositionTable()
{	/*calculates the position / index of each node based on its orientation*/
	std::array<uint, cexp::pow(2, Dimension)> r;
	
	for(uint i=0; i < r.size(); ++i)
		r[i] = NDTree<Dimension, StateType>::getPositionFromOrientation<Dimension>(NDTree<Dimension, StateType>::node_orientation_table[i]);

	return r;
}


template <uint Dimension, typename StateType>
std::array<std::array<orientation, Dimension>, cexp::pow(3, Dimension)> generateMooreOffsetTable()
{	/*calculates an offset from center of each node in each dimension (center of hypercube of length 3)*/
	std::array<std::array<orientation, Dimension>, cexp::pow(3, Dimension)> r;

	NDTree<Dimension, StateType>::moore_pow_table = generatePowTable<Dimension, 3>();

	for(uint i=0; i < r.size(); ++i) {
		for(uint j=0; j < Dimension; ++j) {
			const int moore_mod = i % NDTree<Dimension, StateType>::moore_pow_table[j+1];
			if(moore_mod < NDTree<Dimension, StateType>::moore_pow_table[j+1]-(NDTree<Dimension, StateType>::moore_pow_table[j]*2))
				r[r.size()-1-i][j] = RIGHT;
			else if(moore_mod < NDTree<Dimension, StateType>::moore_pow_table[j+1]-NDTree<Dimension, StateType>::moore_pow_table[j])
				r[r.size()-1-i][j] = CENTER;
			else
				r[r.size()-1-i][j] = LEFT;
		}
	}

	return r;
}

template <uint Dimension, typename StateType>
std::array<std::array<TraversalDirection<Dimension>, cexp::pow(3, Dimension)>, cexp::pow(2, Dimension)> generateMooreNeighborTable()
{	/*calculate offset via overlaying a moore 3 length hypercubes center over each position of the base nodes(hypercube)
	 and calculating the offset including if we need to go up one level(parent) to access adjacent node*/
	std::array<std::array<TraversalDirection<Dimension>, cexp::pow(3, Dimension)>, cexp::pow(2, Dimension)> r;

	for(uint c=0; c < r.size(); ++c) { //cube nodes			
		for(uint m=0; m < r[c].size(); ++m) { //moore-nodes

			TraversalDirection<Dimension> tdir;

			for(uint d=0; d < Dimension; ++d) {
				if(NDTree<Dimension, StateType>::node_orientation_table[c][d] == LEFT) {
					switch(NDTree<Dimension, StateType>::moore_offset_table[m][d]) {
						case LEFT:
							tdir.parent_direction[d] = LEFT;
							tdir.node += cexp::pow(2, d);
							break;
						case RIGHT: tdir.node += cexp::pow(2, d); break;
						case CENTER: break;
						default: /*todo:: add bad memory error handling*/ break;
					}
				} else {
					switch(NDTree<Dimension, StateType>::moore_offset_table[m][d]) {
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
//NDTree<Dimension, StateType>::binary_mod_table = generateModTable<Dimension, 2>();

//initialized in generate_moore_offset_table due to compiler running these out of order
template <uint Dimension, typename StateType>
std::array<uint, Dimension+1>
NDTree<Dimension, StateType>::moore_pow_table = generatePowTable<Dimension, 3>();

template <uint Dimension, typename StateType>
std::array<std::array<orientation, Dimension>, cexp::pow(2, Dimension)> 
NDTree<Dimension, StateType>::node_orientation_table = generateNodeOrientationTable<Dimension>();

template <uint Dimension, typename StateType>
std::array<uint, cexp::pow(2, Dimension)>
NDTree<Dimension, StateType>::node_position_table = generateNodePositionTable<Dimension, StateType>();


template <uint Dimension, typename StateType>
std::array<std::array<orientation, Dimension>, cexp::pow(3, Dimension)>
NDTree<Dimension, StateType>::moore_offset_table = generateMooreOffsetTable<Dimension, StateType>();

template <uint Dimension, typename StateType>
std::array<std::array<TraversalDirection<Dimension>, cexp::pow(3, Dimension)>, cexp::pow(2, Dimension)>
NDTree<Dimension, StateType>::moore_neighbor_table = generateMooreNeighborTable<Dimension, StateType>();

#endif