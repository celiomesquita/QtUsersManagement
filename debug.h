#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

#ifdef DEBUG
#define DEBUG_LOG(x) std::cout << x << std::endl
#else
#define DEBUG_LOG(x)
#endif

#endif // DEBUG_H
