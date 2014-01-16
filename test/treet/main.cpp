#include "ndtree.hpp"
#include "prettyprint.hpp"

int main() {
	const uint Dimension = 5;
	NDTree<Dimension> tree;
	for(uint j=0; j<100000; j++) {
		NDTree<Dimension> * t = &tree;
		for(uint i=0; i<100000; ++i) {
			t->subdivide();
			t = t->nodes[i%10];
		}
	}
	//std::cout << tree << std::endl;

	//exit(1);
	//std::cout << "NODE_ORIENTATION_TABLE" << std::endl << NDTree<Dimension>::node_orientation_table << std::endl;
	//std::cout << "MOORE_POW_TABLE" << std::endl << NDTree<Dimension>::moore_pow_table << std::endl;
	//std::cout << "MOORE_OFFSET_TABLE" << std::endl << NDTree<Dimension>::moore_offset_table << std::endl;
	//std::cout << "NEIGHBOR_TABLE" << std::endl << NDTree<Dimension>::moore_neighbor_table << std::endl;
	//run_unit_tests();
	return 0;
}