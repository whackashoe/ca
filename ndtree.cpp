#include <cstdint>
#include <memory>
#include <iostream>
#include <cassert>
#include "ndtree.hpp"

void NDTree::subdivide(const uint32_t dimension)
{
	assert(leaf);
	const uint32_t grid_size = 1 << dimension;
	for(uint32_t i=0; i<grid_size; ++i)
		nodes.push_back(std::unique_ptr<NDTree>(new NDTree(this)));

	leaf = false;
}

void NDTree::collapse()
{
	assert(!leaf);
	nodes.clear();
	leaf = true;
}

std::vector<NDTree *> NDTree::getParents(const NDTree * node) const
{
	std::vector<NDTree *> ret;

	NDTree * traverser = parent;
	while(traverser) {
		ret.push_back(traverser);
		traverser = traverser->parent;
	}

	return ret;
}

void NDTree::insertNode(std::vector<double> pos,
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

void NDTree::removeNode(std::vector<double> pos)
{

}

std::vector<unsigned int> getChildOrientation(int n, int dimensions)
{
	std::vector<int> r;
	for(int i=0; i<dimensions; ++i) {
		if(n % std::pow(2, i+1) < std::pow(2, i)) //left
		else //right


	}
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