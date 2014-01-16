#ifndef NEIGHBORRULE_H_
#define NEIGHBORRULE_H_

#include <vector>
#include <functional>
#include <cstdint>


class NeighborRule {
private:
	std::vector < std::function < int32_t (	const uint32_t,
											const uint32_t, 
											const uint32_t, 
											const uint32_t) > > rules;

public:
	NeighborRule();
	
	void add_rule(std::function < int32_t (	const uint32_t, 
											const uint32_t,
											const uint32_t,
											const uint32_t) > fn);

	uint32_t apply(	const uint32_t val,
					const uint32_t neighbors,
					const uint32_t states,
					const uint32_t dimension) const;
};

#endif