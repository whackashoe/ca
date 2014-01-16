#include <vector>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <random>
#include <chrono>
#include <cstdint>
#include <sstream>
#include <map>
#include <memory>

#include <unistd.h>
#include <gmp.h>
#include <gmpxx.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "main.hpp"
#include "neighborrule.hpp"
#include "inputparser.hpp"
#include "glfw-ui.hpp"
#include "color.hpp"


#define VERSION 0.1


std::mt19937 rng;

#define ITERATOR \
	if(input.print_f) { \
		for(uint64_t i=0; i<cur.size(); i++) \
				std::cout << cur[i]; \
 \
		std::cout << std::endl; \
	} \
 \
	if(input.use_builtin_rule_f) { \
		cur = iterate_builtin_ruleset(	ref, \
										built_in_rules[input.selected_neighbor_rule], \
										input.grid_size, \
										input.states, \
										input.dimension); \
	} else { \
		cur = iterate_integer_ruleset(	ref, \
										input.rule, \
										input.grid_size, \
										input.states, \
										input.dimension); \
	} \
 \
	ref = cur; \
	usleep(1000*100); \

/*
grids can be infinite in dimension if we have a generalized nd-tree (e.g. quadtree)
we can then merely make the top leveled node encased in a new top level node when an object 
goes outside the old top level nodes boundaries

this also could lead to a hashlife esque implementation
*/

/*
negative dimensions should be explored for traversal, the dwrap subfunction
leads itself to curious results
*/

/*
genetic algorithm to generate ca rulesets to fit specific criterion
*/

/*MDCats - Multi Dimensional Cellular AuTomaton Simulator
HaRDCASh	  HigheR Dimensional Cellular Automata Simulator
CASTO	  Cellular Automata SimulaTOr
CATS	  Cellular AuTomata SimulaTor
CASTOR	  Cellular Automata SimulaTOR
CLAST	  CeLlular Automata SimulaTor

CELLULAR	  CELLULar Automata Renderer
CARE	  Cellular Automata REnderer
CELLAR	  CELLular Automata Renderer*/

/*
what is the rule number of conways game of life?

0 0 0 = 0
0 0 0
0 0 0

0 0 0 = 0
0 0 0
0 0 1 

0 0 0 = 0
0 0 0
0 1 0 

0 0 0 = 0
0 0 0
0 1 1 

0 0 0 = 0
0 0 0
1 0 0

0 0 0 = 0
0 0 0
1 0 1

0 0 0 = 1
0 0 0
1 1 1   



function that can generate a numeric rule from a built in rule via..
brute force??
or...
via some structured rules (e.g. 1 neighbor and state=0 = 1, 2 neighbor and state=0 = 1, 2 neighbor and state=0 = 0 etc etc.)

*/

int main(int argc, char ** argv)
{
	uint64_t milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>
											(std::chrono::system_clock::now().time_since_epoch()).count();
	rng.seed(milliseconds_since_epoch);
	
	std::map<std::string, NeighborRule> built_in_rules;
	initialize_built_in_rules(built_in_rules);

	
	InputParser input;
	input.parse(argc, argv, built_in_rules);

	auto cur = input.source;
	auto ref = input.source;


	GLFWwindow* window = nullptr;
	if(input.use_ui_f) {
		window = init_gl(window);

		std::vector<Color> color_by_state;
		for(uint32_t i=0; i<input.states; ++i)
			color_by_state.push_back(Color());

		const GLfloat box_size = 2.0 / input.grid_size;

		std::vector<GLfloat> g_vertex_buffer_data;
		std::vector<GLfloat> g_color_buffer_data;
		g_color_buffer_data.resize(input.source.size()*6*3);

		for(uint32_t i=0; i<input.source.size(); i++) {
			const GLfloat x = -1.0 + (box_size*(i % input.grid_size));
			const GLfloat y = 1.0 - (box_size*(std::floor(i / input.grid_size)));

			g_vertex_buffer_data.push_back(x);
			g_vertex_buffer_data.push_back(y);
			g_vertex_buffer_data.push_back(0.0);

			g_vertex_buffer_data.push_back(x+box_size);
			g_vertex_buffer_data.push_back(y);
			g_vertex_buffer_data.push_back(0.0);

			g_vertex_buffer_data.push_back(x);
			g_vertex_buffer_data.push_back(y-box_size);
			g_vertex_buffer_data.push_back(0.0);


			g_vertex_buffer_data.push_back(x+box_size);
			g_vertex_buffer_data.push_back(y);
			g_vertex_buffer_data.push_back(0.0);

			g_vertex_buffer_data.push_back(x+box_size);
			g_vertex_buffer_data.push_back(y-box_size);
			g_vertex_buffer_data.push_back(0.0);

			g_vertex_buffer_data.push_back(x);
			g_vertex_buffer_data.push_back(y-box_size);
			g_vertex_buffer_data.push_back(0.0);
		}

		// This will identify our vertex buffer
		GLuint vertexbuffer, colorbuffer;
		glGenBuffers(1, &vertexbuffer);
		glGenBuffers(1, &colorbuffer);

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(	GL_ARRAY_BUFFER,
						sizeof(g_vertex_buffer_data[0]) * g_vertex_buffer_data.size(),
						&g_vertex_buffer_data[0],
						GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(	GL_ARRAY_BUFFER,
						sizeof(g_color_buffer_data[0]) * g_color_buffer_data.size(),
						&g_color_buffer_data[0],
						GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint programID = loadShaders("glsl/simple.vs", "glsl/simple.fs");
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);




		while (!glfwWindowShouldClose(window)) {
			// Keep running
			double gtime = glfwGetTime();
			float ratio;
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			ratio = width / (float) height;
			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(programID);

			ITERATOR

			uint32_t cnt = 0;
			for(uint32_t i=0; i<cur.size(); ++i) {
				for(uint32_t j=0; j<6; ++j) {
					g_color_buffer_data[cnt++] = (color_by_state[cur[i]].r);
					g_color_buffer_data[cnt++] = (color_by_state[cur[i]].g);
					g_color_buffer_data[cnt++] = (color_by_state[cur[i]].b);
				}
			}
			glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
			glBufferData(	GL_ARRAY_BUFFER,
						sizeof(g_color_buffer_data[0]) * g_color_buffer_data.size(),
						&g_color_buffer_data[0],
						GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);


			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			 

			glDrawArrays(GL_TRIANGLES, 0, input.source.size()*6);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glUseProgram(0);


			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_SUCCESS);
	} else {
		for(; ;) {
			ITERATOR
		}
	}
}

std::vector<uint32_t> & iterate(const InputParser & input, 
								const std::vector<uint32_t> & ref,
								std::map<std::string, NeighborRule> & built_in_rules)
{
	std::vector<uint32_t> ret;
	return ret;
}

std::vector<uint32_t> generate_rule(const uint64_t states,
									const uint64_t dimension, 
									const mpz_class ruleset)
{
	assert("Ruleset larger than possibilities" || (ruleset > std::pow(states, std::pow(3, dimension))));
	
	const uint32_t pad = std::pow(states, std::pow(3, dimension));	//length of rules
	std::vector<uint32_t> ret; 										//return val
	ret.reserve(pad);
	uint64_t cnt = 1;												//current counter of individual rules

	const uint32_t printer = 1000000;

	mpz_class bcnt1a, bcnt1b, bcnt2, bcnt2_m;
	for(uint64_t i=pad; i>0; --i) {
		for(uint64_t j=0; j<states; ++j) {
			mpz_ui_pow_ui(bcnt1b.get_mpz_t(), states, cnt);	
			mpz_mod(bcnt1a.get_mpz_t(), ruleset.get_mpz_t(), bcnt1b.get_mpz_t());	
			mpz_ui_pow_ui(bcnt2.get_mpz_t(), states, cnt-1);
			mpz_fdiv_q_2exp(bcnt1b.get_mpz_t(), bcnt1b.get_mpz_t(), 1);
			mpz_mul_ui(bcnt2_m.get_mpz_t(), bcnt2.get_mpz_t(), j+1);

			if(mpz_cmp(bcnt1a.get_mpz_t(),bcnt2_m.get_mpz_t()) < 0) {
				cnt++;
				ret.push_back(j);
				if(i % printer == 0) std::cout << floor(ret.size()/printer) << "/" << floor(pad/printer) << std::endl;
				break;
			}
		}
	}

	//std::reverse(ret.begin(), ret.end());
	return ret;
}

/*uint32_t query_rule(const uint64_t states,
					const uint64_t dimension, 
					const mpz_class ruleset,
					const mpz_class specific)
{
	if(((ruleset % pow(states, cnt)) << 1)  < (pow(states, cnt-1) * (j+1)))
}*/

std::vector<uint32_t> iterate_integer_ruleset(	const std::vector<uint32_t> & source,
												const std::vector<uint32_t> & rules,
												const uint32_t grid_size,
												const uint32_t states,
												const uint32_t dimension)
{
	assert("Make sure rules are set after dimension and states" || rules.size() == static_cast<uint64_t>(std::pow(states, std::pow(3, dimension))));
	std::vector<uint32_t> cur;
	cur.resize(source.size());

	//#pragma omp parallel for
	//for(uint64_t i=0; i<cur.size(); i++)
	//	cur[i] = rules[calc_rule_position_from_adjacent_cells(get_moore_neighborhood(source, grid_size, dimension, i), states)];

	return cur;
}

std::vector<uint32_t> iterate_builtin_ruleset(	const std::vector<uint32_t> & source,
												const NeighborRule & rules,
												const uint32_t grid_size,
												const uint32_t states,
												const uint32_t dimension)
{
	std::vector<uint32_t> cur;
	cur.resize(source.size());

	//#pragma omp parallel for
	//for(uint64_t i=0; i<cur.size(); i++) {
	//	auto cells = get_moore_neighborhood(source, grid_size, dimension, i);
	//	uint64_t count = 0;
	//	for(uint64_t j=0; j<cells.size(); ++j)
	//		if(cells[j] > 0)
	//			++count;
//
	//	cur[i] = rules.apply(source[i], count, states, dimension);
	//}

	return cur;
}

uint32_t compare_grid(const std::vector<int32_t> & target, const std::vector<int32_t> & src)
{
	assert("Comparison grids not same size" && (target.size() != src.size()));
	
	uint32_t matches = 0;
	for(uint32_t i=0; i<target.size(); i++)
		if(target[i] == src[i]) ++matches;
	
	return matches;
}

std::vector<uint64_t> generate_grid_dimension_sizes(const uint32_t dimension, const uint32_t grid_size)
{
	//precompute the offsets per dimension for 1d vector representation
	std::vector<uint64_t> grid_dimension_sizes;
	for(uint32_t i=0; i<=dimension; ++i)
		grid_dimension_sizes.push_back(static_cast<uint64_t>(std::pow(grid_size, i)));

	return grid_dimension_sizes;
}

std::vector<std::vector<int32_t>> generate_moore_offsets(	const uint32_t dimension,
															const uint32_t grid_size,
															const std::vector<uint64_t> & grid_dimension_sizes)
{
	const uint32_t range = 1;
	std::vector<std::vector<int32_t>> offsets;
	offsets.resize(static_cast<uint32_t>(std::pow((range*2)+1, dimension)));

	std::vector<uint32_t> dfield;
	for(uint32_t i=0; i<=dimension; ++i)
		dfield.push_back(static_cast<uint32_t>(std::pow(3, i)));

	for(uint32_t i=0; i<offsets.size(); ++i)
		for(uint32_t j=0; j<grid_dimension_sizes.size()-1; ++j)
			if(i % dfield[j+1] < dfield[j])
				offsets[i].push_back(grid_dimension_sizes[j]*-1);
			else if(i % dfield[j+1] < dfield[j]*2)
				offsets[i].push_back(0);
			else
				offsets[i].push_back(grid_dimension_sizes[j]);

	return offsets;
}


void get_moore_neighborhood(const std::vector<uint32_t> & grid, 
							std::vector<uint32_t> & cells,
							const std::vector<std::vector<int32_t>> & offsets,   
							const std::vector<uint64_t> & grid_dimension_sizes,
							const uint32_t position)
{
	std::vector<int32_t> cmp;
	cmp.resize(grid_dimension_sizes.size());

	for(uint32_t i=0; i<grid_dimension_sizes.size(); i++)
		cmp[i] = std::floor(position / grid_dimension_sizes[i]);

	for(auto i : cmp)
	std::cout << i << std::endl;
	std::cout << "---" << std::endl;

	for(uint32_t offset_id=0; offset_id<cells.size(); ++offset_id) {
		int32_t rval = position;
		
		for(uint32_t i=0; i<offsets[offset_id].size(); ++i) {
			rval += offsets[offset_id][i];

			const int32_t newp = std::floor(rval % grid_dimension_sizes[i+1]);
			std::cout << "i:" << i << " rval:" << rval << " newp: " << newp << " cmp: " << cmp[i] << std::endl;

			//if(newp < cmp[i])
			//	rval += grid_dimension_sizes[i+1];
			//else if(newp > cmp[i])
			//	rval -= grid_dimension_sizes[i+1];
		}

		cells[offset_id] = grid[rval];
	}
}

/*
std::vector<int32_t> get_moore_neighborhood(const std::vector<uint32_t> & grid, 
											const uint32_t grid_size, 
											const uint32_t dimension,
											const uint32_t position)
{
	
	//possible optimizations:
	//
	//pre generate grid offsets per dimension
//
	//pre generate a list of offsets for grid, e.g. d2 =  ((grid_size*-1) -1) (-(grid_size*1) 0) 	(-(grid_size*1) + 1)
	//													((0*1) -1) 			((0*1) 0) 			((0*1) + 1)
	//													((grid_size*1) -1) 	((grid_size*1) 0) 	((grid_size*1) + 1)
	//pre generate cells vector, pass reference in (just overwrite all data instead of create & delete)
//
	//mod numbers by grid dimension size per dimension, check if they are <0 or >griddimensionsize and fix them

	
	assert("grid_size must be larger than 0" 	&& grid_size > 0);
	assert("dimension must be larger than 0" 	&& dimension > 0);
	assert("grid must contain atleast pow(grid_size, dimension) elements" && grid.size() >= static_cast<uint64_t>(std::pow(grid_size, dimension)));
	assert("position must be smaller than size of grid" && position < grid.size());

	const int32_t chebyshev_distance = 1;	//used for the field, e.g. moving one below and one above each element per dimension

	std::vector<int32_t> cells;	//holds return values
	cells.resize(static_cast<uint32_t>(std::pow((chebyshev_distance*2)+1, dimension)));

	//precompute the offsets per dimension for 1d vector representation
	std::vector<uint64_t> grid_dimension_sizes;
	for(uint32_t i=0; i<=dimension; ++i)
		grid_dimension_sizes.push_back(static_cast<uint64_t>(std::pow(grid_size, i)));

	//takes into account grid dimensions for wrapping on an axis across grid
	const std::function<int32_t (	const int64_t, 
									const int64_t, 
									const uint32_t)> dwrap = [&](	const int64_t oldpos, 
																	const int64_t newpos, 
																	const uint32_t d) -> int32_t {
		const auto old_rem = oldpos / grid_dimension_sizes[d];
		const auto new_rem = newpos / grid_dimension_sizes[d];

		int64_t check;	//guarantee [under/over]flows wrap to correct plane
		if(old_rem < new_rem) 
			check = newpos - grid_dimension_sizes[d];
		else if(old_rem > new_rem) 
			check = newpos + grid_dimension_sizes[d];
		else
			check = newpos;

		//TODO:: optimize out mods
		if(check < 0)			return (check + grid_dimension_sizes[d]) % grid.size();
		if(check > grid.size())	return (check - grid_dimension_sizes[d]) % grid.size();
		else					return check;
	};

	uint64_t cell_pos = 0;	//used to insert into vector at correct position

	//traverse grid by moving axis by axis recursively to grab all cells in dimensional field
	const std::function<void (	const int64_t,
								const uint32_t)> traverse = [&](const int64_t pos,
																const uint32_t d) {
		if(d-1 > 0) 
			for(int i = (-chebyshev_distance); i <= chebyshev_distance; ++i)
				traverse(dwrap(pos, pos + (i * grid_dimension_sizes[d-1]), d), d-1);
		else
			for(int i = (-chebyshev_distance); i <= chebyshev_distance; ++i)
				cells[cell_pos++] = grid[dwrap(pos, pos + (i * grid_dimension_sizes[d-1]), d)];
	};
	
	traverse(position, dimension);

	return cells;
}*/

/*
std::vector<int32_t> get_vonneumann_neighborhood(	const std::vector<uint32_t> & grid, 
													const uint32_t grid_size, 
													const uint32_t dimension,
													const uint32_t position)
{
	assert("grid_size must be larger than 0" 	&& grid_size > 0);
	assert("dimension must be larger than 0" 	&& dimension > 0);
	assert("grid must contain atleast pow(grid_size, dimension) elements" && grid.size() >= static_cast<uint64_t>(std::pow(grid_size, dimension)));
	assert("position must be smaller than size of grid" && position < grid.size());

	const int32_t chebyshev_distance = 1;	//used for the field, e.g. moving one below and one above each element per dimension

	std::vector<int32_t> cells;	//holds return values
	cells.resize(static_cast<uint32_t>(std::pow((chebyshev_distance*2)+1, dimension)));

	//precompute the offsets per dimension for 1d vector representation
	std::vector<uint64_t> grid_dimension_sizes;
	grid_dimension_sizes.resize(static_cast<uint32_t>(std::pow(grid_size, dimension)));
	for(uint32_t i=0; i<=dimension; ++i)
		grid_dimension_sizes[i] = (static_cast<uint64_t>(std::pow(grid_size, i)));

	//takes into account grid dimensions for wrapping on an axis across grid
	const std::function<int32_t (	const int64_t, 
									const int64_t, 
									const uint32_t)> dwrap = [&](	const int64_t oldpos, 
																	const int64_t newpos, 
																	const uint32_t d) -> int32_t {
		const auto old_rem = oldpos / grid_dimension_sizes[d];
		const auto new_rem = newpos / grid_dimension_sizes[d];

		int64_t check;	//guarantee [under/over]flows wrap to correct plane
		if(old_rem < new_rem) 
			check = newpos - grid_dimension_sizes[d];
		else if(old_rem > new_rem) 
			check = newpos + grid_dimension_sizes[d];
		else
			check = newpos;

		if(check < 0)	return check + grid_dimension_sizes[d];
			 			return check;
	};

	uint64_t cell_pos = 0;	//used to insert into vector at correct position

	//traverse grid by moving axis by axis recursively to grab all cells in dimensional field
	const std::function<void (	const int64_t,
								const uint32_t)> traverse = [&](const int64_t pos,
																const uint32_t d) {
		if(d-1 > 0) 
			for(int i = (-chebyshev_distance); i <= chebyshev_distance; ++i)
				traverse(dwrap(pos, pos + (i * grid_dimension_sizes[d-1]), d), d-1);
		else
			for(int i = (-chebyshev_distance); i <= chebyshev_distance; ++i)
				cells[cell_pos++] = grid[dwrap(pos, pos + (i * grid_dimension_sizes[d-1]), d)];
	};
	
	traverse(position, dimension);

	return cells;
}


3:

-3 -0
-2 -1
-1 -2
-0 -3
+1 -2
+2 -1
+3 -0
+2 +1
+1 +2
+0 +3
-1 +2
-2 +1
//-3 +0
*/

uint64_t calc_rule_position_from_adjacent_cells(std::vector<int32_t> input,
												const uint32_t states)
{
	std::reverse(input.begin(), input.end());

	mpz_class pos(0);
	for(uint32_t i=0; i<input.size(); i++) {
		if(input[i] > 0) {
			mpz_class tmp(0);
			mpz_ui_pow_ui(tmp.get_mpz_t(), states, i);
			tmp *= input[i];
			pos += tmp;
		}
	}

	if(pos.fits_ulong_p()) return pos.get_ui();
	else throw std::overflow_error("Result is too big for unsigned long");
}


void initialize_built_in_rules(std::map<std::string, NeighborRule> & built_in_rules)
{
	std::function<void (const std::string &,
						const std::vector<uint32_t>,
						const std::vector<uint32_t>)> add_neighbor_count_rule = [&](const std::string & name,
																					const std::vector<uint32_t> alivestates,
																					const std::vector<uint32_t> birthstates) {
		NeighborRule trule;
		trule.add_rule([=](	const uint32_t val, 
							const uint32_t neighbors, 
							const uint32_t states, 
							const uint32_t dimension) {
			if(val > 0) {
				for(const auto & n : alivestates) {
					if(n > neighbors) break;
					else if(neighbors == n) return 1;
				}
			} else {
				for(const auto & n : birthstates) {
					if(n > neighbors) break;
					else if(neighbors == n) return 1;
				}
			}

			return 0;
		});
		built_in_rules[name] = trule;
	};

	add_neighbor_count_rule("Gnarl", {1}, {1});											//A simple exploding rule that forms complex patterns from even a single live cell.
	add_neighbor_count_rule("Replicator", {1, 3, 5, 7}, {1, 3, 5, 7});					//A rule in which every pattern is a replicator.
	add_neighbor_count_rule("Fredkin", {0, 2, 4, 6, 8}, {1, 3, 5, 7});					//A rule in which, like Replicator, every pattern is a replicator.
	add_neighbor_count_rule("Seeds", {}, {2});											//An exploding rule in which every cell dies in every generation. It has many simple orthogonal spaceships, though it is in general difficult to create patterns that don't explode.
	add_neighbor_count_rule("LiveFreeorDie", {0}, {2});									//An exploding rule in which only cells with no neighbors survive. It has many spaceships, puffers, and oscillators, some of infinitely extensible size and period.
	add_neighbor_count_rule("Serviettes", {}, {2, 3, 4});								//An exploding rule in which every cell dies every generation (like seeds). This rule is of interest because of the fabric-like beauty of the patterns that it produces.
	add_neighbor_count_rule("DotLife", {0, 2, 3}, {3});									//An exploding rule closely related to Conway's Life. The B-heptomino is a common infinite growth pattern in this rule, though it can be stabilized into a spaceship.
	add_neighbor_count_rule("Lifewithoutdeath", {0, 1, 2, 3, 4, 5, 6, 7, 8}, {3});		//An expanding rule that produces complex flakes. It also has important ladder patterns.
	add_neighbor_count_rule("Mazectric", {1, 2, 3, 4}, {3});							//An expanding rule that crystalizes to form maze-like designs that tend to be straighter (ie. have longer "halls") than the standard maze rule.
	add_neighbor_count_rule("Maze", {1, 2, 3, 4, 5}, {3});								//An expanding rule that crystalizes to form maze-like designs.
	add_neighbor_count_rule("ConwaysLife", {2, 3}, {3});								//A chaotic rule that is by far the most well-known and well-studied. It exhibits highly complex behavior.
	add_neighbor_count_rule("Coral", {4, 5, 6, 7, 8}, {3});								//An exploding rule in which patterns grow slowly and form coral-like textures.
	add_neighbor_count_rule("34Life", {3, 4}, {3, 4});									//An exploding rule that was initially thought to be a stable alternative to Conway's Life, until computer simulation found that most patterns tend to explode. It has many small oscillators and simple period 3 orthogonal and diagonal spaceships.
	add_neighbor_count_rule("Assimilation", {4, 5, 6, 7}, {3, 4, 5});					//A very stable rule that forms permanent diamond-shaped patterns with partially filled interiors.
	add_neighbor_count_rule("LongLife", {5}, {3, 4, 5});								//A stable rule that gets its name from the fact that it has many simple extremely high period oscillators.
	add_neighbor_count_rule("Diamoeba", {5, 6, 7, 8}, {3, 5, 6, 7, 8});					//A chaotic pattern that forms large diamonds with chaotically oscillating boundaries. Known to have quadratically-growing patterns.
	add_neighbor_count_rule("Amoeba", {1, 3, 5, 8}, {3, 5, 7});							//A chaotic rule that is well balanced between life and death; it forms patterns with chaotic interiors and wildly moving boundaries.
	add_neighbor_count_rule("PseudoLife", {2, 3, 8}, {3, 5, 7});						//A chaotic rule with evolution that resembles Conway's Life, but few patterns from Life work in this rule because the glider is unstable.
	add_neighbor_count_rule("2x2", {1, 2, 5}, {3, 6});									//A chaotic rule with many simple still lifes, oscillators and spaceships. Its name comes from the fact that it sends patterns made up of 2x2 blocks to patterns made up of 2x2 blocks.
	add_neighbor_count_rule("HighLife", {2, 3}, {3, 6});								//A chaotic rule very similar to Conway's Life that is of interest because it has a simple replicator.
	add_neighbor_count_rule("Move", {2, 4, 5}, {3, 6, 8});								//A rule in which random patterns tend to stabilize extremely quickly. Has a very common slow-moving spaceship and slow-moving puffer.
	add_neighbor_count_rule("Stains", {2, 3, 5, 6, 7, 8}, {3, 6, 7, 8});				//A stable rule in which most patterns tend to "fill in" bounded regions. Most nearby rules (such as coagulations) tend to explode.
	add_neighbor_count_rule("Day&Night", {3, 4, 6, 7, 8}, {3, 6, 7, 8});				//A stable rule that is symmetric under on-off reversal. Many patterns exhibiting highly complex behavior have been found for it.
	add_neighbor_count_rule("DryLife", {2, 3}, {3, 7});									//An exploding rule closely related to Conway's Life, named after the fact that the standard spaceships bigger than the glider do not function in the rule. Has a small puffer based on the R-pentomino, which resembles the switch engine in the possibility of combining several to form a spaceship.
	add_neighbor_count_rule("Coagulations", {2, 3, 5, 6, 7, 8}, {3, 7, 8});				//An exploding rule in which patterns tend to expand forever, producing a thick "goo" as it does so.
	add_neighbor_count_rule("Walledcities", {2, 3, 4, 5}, {4, 5, 6, 7, 8});				//A stable rule that forms centers of pseudo-random activity separated by walls.
	add_neighbor_count_rule("Vote4/5", {3, 5, 6, 7, 8}, {4, 6, 7, 8});					//A modification of the standard Gérard Vichniac voting rule, also known as "Anneal", used as a model for majority voting.
	add_neighbor_count_rule("Vote", {4, 5, 6, 7, 8}, {5, 6, 7, 8});						//Standard Gérard Vichniac voting rule, also known as "Majority", used as a model for majority voting.
	add_neighbor_count_rule("Seascape", {1, 2, 3, 4}, {1, 2, 3, 4});					//Productive undulator with bubbs of happiness
}

void calculate_conways_rulenumber()
{
	/*mpz_class tmp(0);
	
	std::vector<uint32_t> cells;
	for(uint64_t i=0; i<512; i++) {
		uint32_t cnt = 0;
		for(uint32_t j=0; 1 << j < 512; j++) {
			if(1 << j > )
		}
	}*/
}

void display_help(const std::map<std::string, NeighborRule> & built_in_rules)
{
	std::cout 	<< "Usage: hardcash [OPTIONS...]" << std::endl
				<< "Higher Dimensional Cellular Automaton Simulator" << std::endl
				<< std::endl
				<< "  -s					   set amount of possible states (default: 2)" << std::endl
				<< "  -d					   set amount of dimensions to run on (default: 1)" << std::endl
				<< "  -g					   set grid size (default: 10)" << std::endl
				<< "  -i					   grab starting grid from stdin rather than random configuration" << std::endl
				<< "  -u					   render graphics" << std::endl
				<< "  -p					   print out results" << std::endl
				<< "  -r					   set rule number or preset, see below for options (required)" << std::endl;

	std::cout 	<< "Examples:" << std::endl
				<< "  hdcas -s 3 -d 1 -g 128 -r 3374586492785" << std::endl
				<< "	  1 dimensional ternary cellular automaton with rule set and random input" << std::endl
				<< std::endl
				<< "  cat conway_input.txt | hdcas -2 -d 2 -g 128 -r ConwaysLife -i" << std::endl
				<< "	  run conways game of life on a grid of 128x128 with input from file" << std::endl
				<< std::endl;

	std::cout	<< "Possible options for rule:" << std::endl
				<< "	   integer in range of 0 - pow(states, pow(states, pow(3, dimension)))" << std::endl;
	for(const auto & i : built_in_rules)
		std::cout << "	   " << i.first << std::endl;
	std::cout << std::endl;


	std::cout 	<< "Report hardcash bugs to whackashoe@gmail.com" << std::endl
				<< "homepage: <https://lalala>" << std::endl;

}

void display_version()
{
	std::cout << "something v" << VERSION << std::endl;
}
