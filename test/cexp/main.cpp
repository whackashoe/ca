#include <iostream>
#include <array>
#include "cexp.hpp"
#include "prettyprint.hpp"

namespace typedef foo {std::string max = "max";} foob;
namespace foo{std::string max = "max";}

int main() {
	std::cout << foob::max << " " << foo::max;
}