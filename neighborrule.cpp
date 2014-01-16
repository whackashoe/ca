#include <vector>
#include <functional>
#include <cstdint>
#include "neighborrule.hpp"

//encapsulation class for custom neighborly rule functions
NeighborRule::NeighborRule()
{

}

//rule functions should return -1 if not applicable as soon as possible
void NeighborRule::add_rule(std::function < int32_t (const uint32_t,
													const uint32_t,
													const uint32_t,
													const uint32_t) > fn)
{
	rules.push_back(fn);
}

uint32_t NeighborRule::apply(	const uint32_t val,
								const uint32_t neighbors,
								const uint32_t states,
								const uint32_t dimension) const
{
	for(auto & i : rules) {
		const auto m = i(val, neighbors, states, dimension);
		if(m >= 0) return m;
	}

	return 0;
}