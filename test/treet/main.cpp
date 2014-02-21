#include "ndtree.hpp"
#include "prettyprint.hpp"
#include "unittest.hpp"


int main() {
	const uint Dimension = 1;
	NDTree<Dimension, char> tree;
	
	tree.subdivide();
	for(int i=0; i<tree.size(); i++) {
		tree[i].subdivide();
		for(int j=0; j<tree.size(); j++) {
			tree[i][j].subdivide();
		}
	}

	for(int z=0; z<cexp::pow(2, Dimension); z++) {
		for(int y=0; y<cexp::pow(2, Dimension); y++) {
			for(int x=0; x<cexp::pow(2, Dimension); x++) {
				tree[z][y][x].state = (z*100)+(y*10)+(x);
			}
		}
	}

	std::cout << tree <<std::endl;

	/* incorrect results:
	[3][0]{ RIGHT, LEFT }
	[0][0]{ LEFT, LEFT }
	*/

	auto m = [](int n) {
		int rval = 0;
		for(int i=2; i>0; i--) {
			int pw = cexp::pow(2, i);
			if(n < pw) {
				rval += pw;
				n -= pw;
			}
		}

		return rval;
	};

	/*std::cout << m(0) << std::endl;
	std::cout << m(1) << std::endl;
	std::cout << m(2) << std::endl;
	std::cout << m(3) << std::endl;
	exit(0);*/

	for(int z=0; z<cexp::pow(2, Dimension); z++) {
		for(int y=0; y<cexp::pow(2, Dimension); y++) {
			for(int x=0; x<cexp::pow(2, Dimension); x++) {
				std::cout << z << y << x << std::endl;
				std::cout << "LEFT   :: "; tree[z][y][x].getAdjacentNode({ LEFT });/*->state   << */std::cout << std::endl;
				std::cout << "CENTER :: "; tree[z][y][x].getAdjacentNode({ CENTER });/*->state << */std::cout << std::endl;
				std::cout << "RIGHT  :: "; tree[z][y][x].getAdjacentNode({ RIGHT });/*->state  << */std::cout << std::endl;
			}
		}
	}
	/*std::cout << "000" << std::endl;
	std::cout << "LEFT   :: " << (int) tree[0][0][0].getAdjacentNode({ LEFT })->state   << std::endl;//
	//std::cout << "CENTER :: " << (int) tree[0][0][0].getAdjacentNode({ CENTER })->state << std::endl;
	//std::cout << "RIGHT  :: " << (int) tree[0][0][0].getAdjacentNode({ RIGHT })->state  << std::endl;

	std::cout << "001" << std::endl;
	//std::cout << "LEFT   :: " << (int) tree[0][0][1].getAdjacentNode({ LEFT })->state   << std::endl;
	std::cout << "CENTER :: " << (int) tree[0][0][1].getAdjacentNode({ CENTER })->state << std::endl;
	std::cout << "RIGHT  :: " << (int) tree[0][0][1].getAdjacentNode({ RIGHT })->state  << std::endl;//
	
	std::cout << "010" << std::endl;
	std::cout << "LEFT   :: " << (int) tree[0][1][0].getAdjacentNode({ LEFT })->state   << std::endl;
	std::cout << "CENTER :: " << (int) tree[0][1][0].getAdjacentNode({ CENTER })->state << std::endl;
	std::cout << "RIGHT  :: " << (int) tree[0][1][0].getAdjacentNode({ RIGHT })->state  << std::endl;

	std::cout << "011" << std::endl;
	//std::cout << "LEFT   :: " << (int) tree[0][1][1].getAdjacentNode({ LEFT })->state   << std::endl;
	std::cout << "CENTER :: " << (int) tree[0][1][1].getAdjacentNode({ CENTER })->state << std::endl;//
	std::cout << "RIGHT  :: " << (int) tree[0][1][1].getAdjacentNode({ RIGHT })->state  << std::endl;//
	
	std::cout << "100" << std::endl;
	std::cout << "LEFT   :: " << (int) tree[1][0][0].getAdjacentNode({ LEFT })->state   << std::endl;//
	//std::cout << "CENTER :: " << (int) tree[1][0][0].getAdjacentNode({ CENTER })->state << std::endl;
	//std::cout << "RIGHT  :: " << (int) tree[1][0][0].getAdjacentNode({ RIGHT })->state  << std::endl;

	std::cout << "101" << std::endl;
	//std::cout << "LEFT   :: " << (int) tree[1][0][1].getAdjacentNode({ LEFT })->state   << std::endl;
	std::cout << "CENTER :: " << (int) tree[1][0][1].getAdjacentNode({ CENTER })->state << std::endl;//
	std::cout << "RIGHT  :: " << (int) tree[1][0][1].getAdjacentNode({ RIGHT })->state  << std::endl;//

	std::cout << "110" << std::endl;
	std::cout << "LEFT   :: " << (int) tree[1][1][0].getAdjacentNode({ LEFT })->state   << std::endl;//
	//std::cout << "CENTER :: " << (int) tree[1][1][0].getAdjacentNode({ CENTER })->state << std::endl;
	//std::cout << "RIGHT  :: " << (int) tree[1][1][0].getAdjacentNode({ RIGHT })->state  << std::endl;

	std::cout << "111" << std::endl;
	//std::cout << "LEFT   :: " << (int) tree[1][1][1].getAdjacentNode({ LEFT })->state   << std::endl;
	std::cout << "CENTER :: " << (int) tree[1][1][1].getAdjacentNode({ CENTER })->state << std::endl;//
	std::cout << "RIGHT  :: " << (int) tree[1][1][1].getAdjacentNode({ RIGHT })->state  << std::endl;//*/
	

	
	/*std::cout << "state " << (int) result->state << std::endl;
	std::cout << "position ";

	while(result->parent != nullptr) {
		std::cout << (int) result->position << " ";
		result = result->parent;
	}
	std::cout << std::endl;*/

	//exit(1);
	//std::cout << "NODE_ORIENTATION_TABLE" << std::endl << NDTree<Dimension, uint>::node_orientation_table << std::endl;
	//std::cout << "MOORE_POW_TABLE" << std::endl << NDTree<Dimension, uint>::moore_pow_table << std::endl;
	//std::cout << "MOORE_OFFSET_TABLE" << std::endl << NDTree<Dimension, uint>::moore_offset_table << std::endl;
	//std::cout << "NEIGHBOR_TABLE" << std::endl << NDTree<Dimension, uint>::moore_neighbor_table << std::endl;
	//run_unit_tests();
	return 0;
}