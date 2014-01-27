#include "ndtree.hpp"
#include "prettyprint.hpp"
#include "unittest.hpp"


int main() {
	const uint Dimension = 2;
	NDTree<Dimension, uint> tree;
	tree.subdivide();
	for(auto & i : tree.nodes)
		i->subdivide();

	tree.nodes[2]->nodes[1]->state = 1;
	std::cout << tree << std::endl;

	std::array<orientation, Dimension> direction;
	direction[0] = RIGHT;
	direction[1] = CENTER;
	
	NDTree<Dimension, uint> * result = tree.nodes[3]->nodes[0]->getAdjacentNode(direction);
	std::cout << "state " << result->state << std::endl;
	std::cout << "position ";
	while(result->parent != nullptr) {
		std::cout << result->position << " ";
		result = result->parent;
	}
	std::cout << std::endl;

	std::cout << tree << std::endl;

	//exit(1);
	std::cout << "NODE_ORIENTATION_TABLE" << std::endl << NDTree<Dimension, uint>::node_orientation_table << std::endl;
	//std::cout << "MOORE_POW_TABLE" << std::endl << NDTree<Dimension>::moore_pow_table << std::endl;
	//std::cout << "MOORE_OFFSET_TABLE" << std::endl << NDTree<Dimension>::moore_offset_table << std::endl;
	//std::cout << "NEIGHBOR_TABLE" << std::endl << NDTree<Dimension>::moore_neighbor_table << std::endl;
	//run_unit_tests();
	return 0;
}