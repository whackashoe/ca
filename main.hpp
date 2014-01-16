#ifndef MAIN_H_
#define MAIN_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <map>
#include <memory>
#include <gmp.h>
#include <gmpxx.h>

#include "neighborrule.hpp"
#include "inputparser.hpp"

template <class T>
void printcontainer(T c)
{
	for(auto &i : c) std::cout << i;
	std::cout << std::endl;
}

std::vector<uint32_t> generate_rule(const uint64_t states,
									const uint64_t dimension, 
									const mpz_class ruleset);

void iterate(	const InputParser & input,
				std::map<std::string, NeighborRule> & built_in_rules,
				const mpz_class iterations);

std::vector<uint32_t> iterate_integer_ruleset(	const std::vector<uint32_t> & source,
												const std::vector<uint32_t> & rules,
												const uint32_t grid_size,
												const uint32_t states,
												const uint32_t dimension);

std::vector<uint32_t> iterate_builtin_ruleset(	const std::vector<uint32_t> & source,
												const NeighborRule & rules,
												const uint32_t grid_size,
												const uint32_t states,
												const uint32_t dimension);

unsigned int compare_grid(const std::vector<int> & target, const std::vector<uint32_t> & src);

std::vector<int> get_moore_neighborhood(const std::vector<uint32_t> & grid, 
										const uint32_t grid_size, 
										const uint32_t dimension,
										const uint32_t position);

/*
void get_moore_neighborhood(const std::vector<uint32_t> & grid, 
							std::vector<uint32_t> & cells,
							const std::vector<std::vector<int32_t>> & offsets,   
							const std::vector<uint64_t> & grid_dimension_sizes,
							const uint32_t position);
*/
std::vector<uint64_t> generate_grid_dimension_sizes(const uint32_t dimension, const uint32_t grid_size);
std::vector<std::vector<int32_t>> generate_moore_offsets(	const uint32_t dimension,
															const uint32_t grid_size,
															const std::vector<uint64_t> & grid_dimension_sizes);
uint64_t calc_rule_position_from_adjacent_cells(std::vector<int32_t> input,
												const uint32_t states);

void initialize_built_in_rules(std::map<std::string, NeighborRule> & built_in_rules);

void display_help(const std::map<std::string, NeighborRule> & built_in_rules);
void display_version();

#endif
