#ifndef _RICPP_TOOLS_PLATFORM_H
#define _RICPP_TOOLS_PLATFORM_H
#include <string.h>
#ifdef WIN32
inline int strcasecmp(const char *s1, const char *s2) { return _stricmp(s1, s2); }
#endif
#endif
