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

/** @file stringpattern.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implements a glob-style matching for strings (as used in conditions of RI if blocks).
 */

#include "ricpp/tools/stringpattern.h"

#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif // _RICPP_TOOLS_INLINETOOLS_H

using namespace RiCPP;

bool CStringPattern::doMatch(const char *patternPos, const char **aString) const
{
	for ( ; *patternPos && (*aString)[0]; ++patternPos, ++(*aString) ) {
		char p = patternPos[0];
		char c = (*aString)[0];

		if ( p == '*' ) {
			// '*...*' -> '*'
			while ( patternPos[0] == '*' ) {
				++patternPos;
				if ( !patternPos[0] ) {
					// * at the end matches everything
					return true;
				}
			}
			// This can lead to heavy backtracking for patterns with many asterisks.
			const char *sav = *aString;
			while ( !doMatch(patternPos, aString) ) {
				*aString = sav;
				++(*aString);
				sav = *aString;
				if ( !(*aString)[0] )
					return false;
			}
			return true;
		}

		if ( p == '?' ) {
			// Matches everything (but not '\0')
			continue;
		}

		if ( p == '\\' ) {
			// Escaped operator?
			if ( (patternPos)[1]=='*' || (patternPos)[1]=='?' || (patternPos)[1]=='\\' ) {
				++patternPos;
				p = *patternPos;
			}
		}

		if ( p != c ) {
			// Characters don't match
			return false;
		}
	}

	// Both have to be at end '\0'
	return *patternPos == *aString[0];
}

void CStringPattern::pattern(const char *aPattern)
{
	m_pattern = noNullStr(aPattern);
}

bool CStringPattern::matches(const char *aString) const
{
	aString = noNullStr(aString);
	return doMatch(m_pattern.c_str(), &aString);
}
