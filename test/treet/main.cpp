#include "ndtree.hpp"
#include "prettyprint.hpp"
#include "unittest.hpp"


int main() {
	const uint Dimension = 2;
	NDTree<Dimension, char> tree;
	
	tree.subdivide();
	for(int i=0; i<tree.size(); i++)
		tree[i].subdivide();

	for(int y=0; y<cexp::pow(2, Dimension); y++) {
		for(int x=0; x<cexp::pow(2, Dimension); x++) {
			tree[y][x].state = ((y)*10)+(x);
		}
	}

	std::cout << tree <<std::endl;

	/* incorrect results:
	[3][0]{ RIGHT, LEFT }
	[0][0]{ LEFT, LEFT }

	*/

	NDTree<Dimension, char> * result = tree[3][0].getAdjacentNode({ RIGHT, LEFT });

	
	std::cout << "state " << (int) result->state << std::endl;
	std::cout << "position ";

	while(result->parent != nullptr) {
		std::cout << (int) result->position << " ";
		result = result->parent;
	}
	std::cout << std::endl;

	//exit(1);
	//std::cout << "NODE_ORIENTATION_TABLE" << std::endl << NDTree<Dimension, uint>::node_orientation_table << std::endl;
	//std::cout << "MOORE_POW_TABLE" << std::endl << NDTree<Dimension>::moore_pow_table << std::endl;
	//std::cout << "MOORE_OFFSET_TABLE" << std::endl << NDTree<Dimension>::moore_offset_table << std::endl;
	//std::cout << "NEIGHBOR_TABLE" << std::endl << NDTree<Dimension>::moore_neighbor_table << std::endl;
	//run_unit_tests();
	return 0;
}