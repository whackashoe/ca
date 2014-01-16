#ifndef NDTREE_H_
#define NDTREE_H_

#include <iostream>
#include <vector>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <sstream>
#include "ndhash.hpp"


template <int Dimension>
class NDTree {
friend class NDHash;

private:
	std::array<std::unique_ptr<NDTree>, (1 << Dimension)> nodes;
	NDTree * parent;
	uint32_t state;
	bool leaf;

	void NDTree::subdivide(const uint32_t dimension)
	{
		assert(leaf);

		for(uint32_t i=0; i < (1 << Dimension); ++i)
			nodes[i] = std::unique_ptr<NDTree>(new NDTree(this));

		leaf = false;
	}

	void collapse()
	{
		assert(!leaf);
		nodes.clear();
		leaf = true;
	}

	std::vector<NDTree*> getParents(const NDTree * node) const
	{
		std::vector<NDTree *> ret;
		NDTree * traverser = parent;

		while(traverser) {
			ret.push_back(traverser);
			traverser = traverser->parent;
		}

		return ret;
	}

	std::array<std::array<int, Dimension>, (1 << Dimension)> generateModTable()
	{
		std::array<std::array<int, Dimension>, (1 << Dimension)> r;

		for(int i=0; i < (1 << Dimension); ++i)
			for(int j=0; j < Dimension; ++j)
				r[i][j] = i % (2 << j);

		return r;
	}

public:
	NDTree() : leaf(true) {}
	NDTree(NDTree * parent_) : parent(parent_), leaf(true) {}

	void insertNode(std::vector<double> pos,
					const uint32_t depth_precision)
	{
		NDTree * node = this;

		for(uint32_t i=0; i<depth_precision; ++i) {
			const double half_dist = 1.0 / (1 << (i+1));
			uint32_t cell_pos = 0;

			if(node->leaf)
				node->subdivide(pos.size());

			for(uint32_t j=0; j<pos.size(); ++j)
				if(pos[j] > half_dist) {
					pos[j] -= half_dist;
					cell_pos += (1 << j);
				}

			std::cout << "depth: " << i << " cell pos: " << cell_pos << std::endl;
			node = node->nodes[cell_pos].get();
		}
	}

	void removeNode(std::vector<double> pos)
	{

	}

	void calculateHash()
	{

	}

	int getChildOrientation(int n, int dimensions)
	{
		std::vector<int> r;
		for(int i=0; i<dimensions; ++i) {
			if(n % std::pow(2, i+1) < std::pow(2, i)) //left
			else //right
	}

	std::vector<std::vector<unsigned int>> getMooreNeighborhoodOrientations(int dimension)
	{
		//local testing
		std::vector<std::vector<unsigned int>> orientations;
		for(int i=0; i<nodes.size(); i++)
			orientations.push_back(getChildOrientation(n, dimension));
		return orientations;
	}

	std::vector<NDTree*> getMooreNeighborhoodBranches(int n, int dimension)
	{
		left of 0 is 1, right of 0 is 1
		up of 0 is 2 right of 0 is 2

		std::vector<std::vector<std::pair<NDTree*, NDTree*>>> orientations;//pair is left right per dimension tree

		for(int i=0; i<nodes.size(); ++i) {
			std::vector<std::pair<unsigned int, unsigned int>> d_orientations; //pair is left right 
			orientations.push_back(std::vector<std::pair<unsigned int, unsigned int>>());
		}

		left, left = parent->left node->right child node
		left, right = right node
		right, left = left node
		right, right = parent->right node->left child node
	}
};

#endif