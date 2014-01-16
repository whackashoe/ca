#include <unistd.h>
#include <iostream>
#include <sstream>
#include "main.hpp"
#include "inputparser.hpp"

extern std::mt19937 rng;

void InputParser::parse(int argc, char ** argv, const std::map<std::string, NeighborRule> & built_in_rules)
{
	opterr = 0;
	bool errflag = false;
	bool printrule = false;
	int c;
	while((c = getopt (argc, argv, "s:d:g:r:iup-:")) != -1) {
		std::string tmp_s;

		switch(c) {
			case 's':
				state_f 		= true;
				tmp_s = optarg;
				std::istringstream(tmp_s) >> states;
				break;
			case 'd':
				dimension_f 	= true;
				tmp_s = optarg;
				std::istringstream(tmp_s) >> dimension;
				break;
			case 'g':
				grid_size_f 	= true;
				tmp_s = optarg;
				std::istringstream(tmp_s) >> grid_size;
				break;
			case 'r':
				rule_f			= true;
				tmp_s = optarg;

				if(built_in_rules.find(tmp_s) != built_in_rules.end()) {
					use_builtin_rule_f = true;
					selected_neighbor_rule = tmp_s;
				} else {
					rule = generate_rule(states, dimension, mpz_class(optarg));
				}
				break;
			case 'i':
				input_f 		= true;
				break;
			case 'u':
				use_ui_f 		= true;
				break;
			case 'p':
				print_f 		= true;
				break;
			case '-':
				if(strcmp(optarg, "help") == 0) {
					display_help(built_in_rules);
					exit(0);
				} else if(strcmp(optarg, "version") == 0) {
					display_version();
					exit(0);
				} else if(strcmp(optarg, "printrule") == 0) {
					printrule = true;
				} else {
					std::cerr << (*argv) << ": unknown option '--" << optarg << "'" << std::endl;
					errflag = true;
				}
				break;
			case '?':
				if(optopt == 's')
					std::cerr << (*argv) << ": option  '-" << static_cast<char>(optopt) << "' requires an argument." << std::endl;
				else if(isprint (optopt))
					std::cerr << (*argv) << ": unknown option '-" << static_cast<char>(optopt) << "'" << std::endl;
				else
					std::cerr << (*argv) << ": unknown option character '-" << static_cast<char>(optopt) << "'" << std::endl;

				errflag = true;
			default:
				exit(1);
		}
	}

	if(states < 2) {
		std::cerr << (*argv) << ": State either invalid or less than 2" << std::endl;
		errflag = true;
	}

	if(dimension < 1) {
		std::cerr << (*argv) << ": Dimension either invalid or less than 1" << std::endl;
		errflag = true;
	}

	if(grid_size < 1) {
		std::cerr << (*argv) << ": Grid Size either invalid or less than 1" << std::endl;
		errflag = true;
	}

	if(!use_builtin_rule_f && rule.empty()) {
		std::cerr << (*argv) << ": Rule not set" << std::endl;
		errflag = true;
	}

	if(input_f) {
		uint32_t supposed_size = static_cast<uint32_t>(std::pow(grid_size, dimension));

		std::string input;
		while(std::cin >> input);

		if(input.size() > supposed_size) {
			std::cerr 	<< (*argv) << ": Input data must be smaller than pow(grid_size, dimension)" << std::endl
						<< "e.g. " << input.size() << "/" << supposed_size << std::endl;
			errflag = true;
		} else {
			if(input.size() < supposed_size)
				while(input.size() < supposed_size)
					input += "0";
		}
		
		for(uint32_t i=0; i<input.size(); i++)
			source.push_back(input[i] - '0');

	} else if(!errflag) {
		for(uint32_t i=0; i<static_cast<uint32_t>(std::pow(grid_size, dimension)); i++)
			source.push_back(i % states);

		shuffle(source.begin(), source.end(), rng);
	}

	if(printrule) {
		if(use_builtin_rule_f)
			std::cout << selected_neighbor_rule << std::endl;
		else
			printcontainer(rule);
	}

	if(errflag) {
		std::cerr << "Try " << (*argv) << " --help" << std::endl;
		exit(1);
	}
}
