#ifndef _ABLOADER_TOOLS_TRACE
#define _ABLOADER_TOOLS_TRACE

#include <iostream>
#include <string>

#ifdef _TRACE
inline void trace(const std::string &message) { std::cout << message << std::endl; }
#else
// inline void trace(const std::string &message) {}
#define trace(arg)
#endif // _TRACE

#endif // _ABLOADER_TOOLS_TRACE
