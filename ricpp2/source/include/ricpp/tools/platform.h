#ifndef _RICPP_TOOLS_PLATFORM_H
#define _RICPP_TOOLS_PLATFORM_H

/*
// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 2000, 2005 Pixar
//                 All rights Reservered
//
// Copyright (c) of RiCPP 2007, Andreas Pidde
// Contact: andreas@pidde.de
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/** @file platform.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief (Small) adapters for system dependent functions.
 */

#include <cstring>
#include <ios>

#if defined(WIN32)
#if !defined(_WINDOWS_)
#include <windows.h>
#endif // _WINDOWS_
#endif

namespace RiCPP {

#if defined(WIN32)

/** @brief Case insensitive strcmp() - Compare s1 with s2, under Mac and Linux/Unix there is no _stricmp(), it is named strcasecmp().
 *  @param s1 first NUL terminated string
 *  @param s2 second NUL terminated string
 *  @return 1, if s1 > s2, 0 if the strings are equal and -1 if s2 > s1
 */
inline int strcasecmp(const char *s1, const char *s2) { return _stricmp(s1, s2); }

/** @brief Export declaration for the included functions
 */
#define EXPORT __declspec ( dllexport )

/** @brief Open mode type for streams.
 */
typedef std::ios_base::openmode TypeOpenMode;

#else

// __GNUC__

/** @brief Generic pointer to a function.
 *
 * FARPROC Pointers are needed for functions loaded from dynamic libraries.
 * The return value need not to be integer and the functions might have parameters.
 */
typedef int (*FARPROC)();

/** @brief CDECL is not used for MAC
 */
#define CDECL

/** @brief Symbolic name for visibility("default") attribute.
 */
#define EXPORT __attribute__((visibility("default")))

/** @brief Open mode type for streams.
 */
typedef std::_Ios_Openmode TypeOpenMode;
#endif
}

#endif
