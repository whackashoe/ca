#include <sstream>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <unistd.h>

#include "main.hpp"

int main(int argc, char ** argv)
{
	bool state_flag 	= false;
	bool dimension_flag = false;
	bool grid_size_flag = false;


	uint32_t states;
	uint32_t dimension;
	uint32_t grid_size;

	int c;
	while((c = getopt (argc, argv, "s:d:g:-:")) != -1) {
		std::string tmp_s;

		switch(c) {
			case 's':
				state_flag 		= true;
				tmp_s = optarg;
				std::istringstream(tmp_s) >> states;
				break;
			case 'd':
				dimension_flag 	= true;
				tmp_s = optarg;
				std::istringstream(tmp_s) >> dimension;
				break;
			case 'g':
				grid_size_flag 	= true;
				tmp_s = optarg;
				std::istringstream(tmp_s) >> grid_size;
				break;
			case '-':
				if(strcmp(optarg, "help") == 0) {
					displayHelp();
					exit(0);
				}
				if(strcmp(optarg, "version") == 0) {
					displayVersion();
					exit(0);
				}
				break;
			case '?':
				if(optopt == 's')
					 std::cerr << "Option -" << optopt << "requires an argument.\n" << std::endl;
				else if(isprint (optopt))
					std::cerr << "Unknown option -" << optopt << std::endl;
				else
					std::cerr << "Unknown option character -" << optopt << std::endl;

				std::cerr << "Try " << (*argv) << " --help" << std::endl;
				exit(1);
			default:
				exit(1);
		}
	}

	std::function<void (const std::string &)> error_exit = [](const std::string & message) {
		std::cerr << message << std::endl;
		exit(1);
	};

	if(!state_flag)			error_exit("State must be set");
	if(!dimension_flag)		error_exit("Dimension must be set");
	if(!grid_size_flag)		error_exit("Grid Size must be set");
	if(states 		== 0)	error_exit("State either invalid or 0 (must be >0)");
	if(dimension 	== 0)	error_exit("Dimension either invalid or 0 (must be >0)");
	if(grid_size 	== 0) 	error_exit("Grid Size either invalid or 0 (must be >0)");
	

	std::string input;
	while(std::cin >> input) {
		std::cout << input;
	}
}

void displayHelp()
{

}

void displayVersion()
{

}