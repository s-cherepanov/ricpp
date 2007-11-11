#ifndef _RICPP_TOOLS_STRINGPATTERN_H
#define _RICPP_TOOLS_STRINGPATTERN_H

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

/** @file stringpattern.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Header for a glob-style matching for strings (as used in conditions of RI if blocks).
 */

#include <string>

namespace RiCPP {

/** @brief Glob style pattern matcher.
 *
 *  Simple backtracking glob style Pattern matcher with '*' and '?' wildcards.
 *  Escapes: "\*" matches "*", "\?" matches "?", "\\" matches "\", "\a" etc. matches "\a".
 */
class CStringPattern {
private:
	std::string m_pattern; //!< The pattern used for matching
	
	/** @brief Recursive matching.
	 *
	 * @param patternPos Position in pattern (m_pattern).
	 * @param aString Address of the string to match.
	 */
	bool doMatch(const char *patternPos, const char **aString) const;
public:
	/** @brief Default constructor.
	 *
	 *  @param aPattern Pattern used for matching.
	 */
	inline CStringPattern(const char *aPattern = 0)
	{
		pattern(aPattern);
	}

	/** @brief Gets the current pattern.
	 *
	 *  @return Current pattern  
	 */
	inline const char *pattern() const
	{
		return m_pattern.c_str();
	}

	/** @brief Sets the current pattern.
	 *
	 *  @param aPattern Pattern to use.  
	 */
	void pattern(const char *aPattern);

	/** @brief Matches the pattern stored in this object to a string.
	 *
	 *  @param aString String to be matched.  
	 */
	bool matches(const char *aString) const;
}; // CStringPattern

}

#endif // _RICPP_TOOLS_STRINGPATTERN_H
