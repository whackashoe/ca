#include <functional>
#include <limits>
#include "ndhash.hpp"
#include "ndtree.hpp"


std::size_t NDHash::operator()(const std::unique_ptr<NDTree> & tree) const
{
	std::size_t ret = std::hash<uint32_t>()(tree->state);

	std::function<void (const std::unique_ptr<NDTree> &)> iterate = [&](const std::unique_ptr<NDTree> & part) {
		if(!part->leaf) {
			ret ^= (std::hash<uint32_t>()(std::numeric_limits<uint32_t>::max()) << 1);
			for(const auto & i : part->nodes)
				iterate(i);
		} else {
	    	ret ^= (std::hash<uint32_t>()(part->state) << 1);
		}
	};

	iterate(tree);
	return ret;
}