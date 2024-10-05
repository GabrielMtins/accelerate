#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cmath>

#define MAX_ENTITIES 4096
#define MAX_COMPONENTS 256

namespace acc {

/**
 * The main type used in this engine. It's just an id.
 */
using Entity = size_t;

};

#endif
