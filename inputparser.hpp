#ifndef INPUTPARSER_H_
#define INPUTPARSER_H_

#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include "neighborrule.hpp"


class InputParser {
public:
	bool state_f 				= false;
	bool dimension_f 			= false;
	bool grid_size_f 			= false;
	bool rule_f 				= false;
	bool input_f				= false;
	bool use_builtin_rule_f 	= false;
	bool use_ui_f 				= false;
	bool print_f 				= false;

	uint32_t states 	= 2;
	uint32_t dimension 	= 1;
	uint32_t grid_size 	= 10;

	std::vector<uint32_t> rule;
	std::vector<uint32_t> source;
	std::string selected_neighbor_rule;


	void parse(int argc, char ** argv, const std::map<std::string, NeighborRule> & built_in_rules);
};

#endif