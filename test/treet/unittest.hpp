#ifndef UNITTEST_H
#define UNITTEST_H

#include <cstdint>
#include "lest.hpp"
#include "ndtree.hpp"


/****************************************
 *
 * UNIT TESTING SPECIFICATION
 * 
 ****************************************/

/*
const lest::test specification[] =
{
	"Brand new tree should be a leaf", []
	{
		EXPECT( NDTree<1, uint>().leaf );
	},
	"moore pow table is incorrect", []
	{
		auto m = NDTree<2, uint>::moore_pow_table;
		m = {1, 3, 9};
		EXPECT( NDTree<2, uint>::moore_pow_table == m )
	},
	"node orientation table is incorrect", []
	{
		auto m = NDTree<2, uint>::node_orientation_table;
		m = {{{LEFT, LEFT}, {RIGHT, LEFT}, {LEFT, RIGHT}, {RIGHT, RIGHT}}};
		EXPECT( NDTree<2, uint>::node_orientation_table == m )
	},
	"moore offset table is incorrect", []
	{
		auto m = NDTree<2, uint>::moore_offset_table;
		m = {{{LEFT, LEFT }, {CENTER, LEFT }, {RIGHT, LEFT}, {LEFT, CENTER}, {CENTER, CENTER}, {RIGHT, CENTER}, {LEFT, RIGHT}, {CENTER, RIGHT}, {RIGHT, RIGHT}}};
		EXPECT( NDTree<2, uint>::moore_offset_table == m)
	},
	"moore neighborhood table is incorrect", []
	{
		auto m = NDTree<1, uint>::moore_neighbor_table;
		m[0][0].parent_direction[0] = LEFT;   m[0][0].node = 1;
		m[0][1].parent_direction[0] = CENTER; m[0][1].node = 0;
		m[0][2].parent_direction[0] = CENTER; m[0][2].node = 1;
		m[1][0].parent_direction[0] = CENTER;   m[1][0].node = 1;
		m[1][1].parent_direction[0] = CENTER; m[1][1].node = 0;
		m[1][2].parent_direction[0] = RIGHT; m[1][2].node = 1;

		EXPECT( NDTree<1, uint>::moore_neighbor_table == m )
	},
	"subdivide turns tree into non leaf", []
	{
		NDTree<1, uint> tree;
		tree.subdivide();

		EXPECT( !tree.leaf )
	},
	"position does not correspond with orientation", []
	{
		EXPECT( getPositionFromOrientation<2>({RIGHT, LEFT}) == 1 );
	},
	"orientation does not correspond with position", []
	{
		auto m = getOrientationFromPosition<2>(1);
		m = {RIGHT, LEFT};
		EXPECT( getOrientationFromPosition<2>(1) == m );
	}
};
*/

int run_unit_tests()
{
	//return lest::run( specification );
	return 0;
}

#endif