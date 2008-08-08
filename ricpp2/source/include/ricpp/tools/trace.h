#ifndef _RICPP_TOOLS_TRACE_H
#define _RICPP_TOOLS_TRACE_H

#include <iostream>
#include <string>

namespace RiCPP {

#ifdef _TRACE
inline void trace(const std::string &message) { std::cout << message << std::endl; }
#else
inline void trace(const std::string &message) {}
#endif // _TRACE

}

#endif // _RICPP_TOOLS_TRACE_H
