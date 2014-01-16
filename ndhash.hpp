#ifndef NDHASH_H_
#define NDHASH_H_

#include <functional>
#include <limits>
#include "ndtree.hpp"

class NDTree;

class NDHash
{
public:
    std::size_t operator()(const std::unique_ptr<NDTree> & tree) const;
};

#endif