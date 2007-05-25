#ifndef _RICPP_TOOLS_INLINETOOLS_H
#define _RICPP_TOOLS_INLINETOOLS_H

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

/** @file inlinetools.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Frequent used inline tools
 */

namespace RiCPP {

/** @brief Gets "<empty>" for a 0 pointer or an empty "" string
 * @param str String to test
 * @return str itself or "<empty>" if the string is empty
 */
inline const char *markemptystr(const char *str)
{
	return  str && *str ? str : "<empty>";
}

/** @brief Gets "" for a 0 pointer
 * @param str String to test
 * @return str itself or "" if the string points to 0
 */
inline const char *nonullstr(const char *str)
{
	return  str ? str : "";
}

/** @brief Tests if string is empty, either pointer is 0 or ""
 * @param str String to test
 * @return true, if str is 0 or points to ""
 */
inline bool emptyStr(const char *str)
{
	return !str || !*str;
}

/** @brief Tests if string is not empty
 * @param str String to test
 * @return true, if str is not 0 or nor ""
 */
inline bool notEmptyStr(const char *str)
{
	return str && *str;
}
} // namespace RiCPP
#endif // _RICPP_TOOLS_INLINETOOLS_H
