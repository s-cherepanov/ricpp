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

/** @file stringlist.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the string list class
 */

#include "ricpp/tools/stringlist.h"

using namespace RiCPP;

CStringList::size_type CStringList::explode(
	char separator,
	const char *str,
	bool isPathList
)
{
	size_type count = 0;
	if ( !str )
		return count;

	std::string strval(str ? str : "");

	std::string::iterator iter;
	bool iterinc;

#if defined(WIN32)
	if ( isPathList  && separator == ':' ) {
		// Special case for windows.
		//
		// Win32 scans here for C/path -> C|/path and  C:/path -> C|/path
		// In RIB files the separator for pathes is unfortunatly ':'.
		// ':' can have a double meaning as separator for drive letters
		// and separator for pathes.
		// ':' is converted to '|', if "<letter>:/" is the prefix of a path
		// i.e. a windows path letter. Since there are no drive letters
		// in other OSes as WIN32 this is done only for WIN32 binaries
		// If RIB files have to be exchanged across OSe, drive letters
		// won't work anyway.
		// The pipe symbol '|' is usually not part of a search path.
		// Also in windows the path separator can be a ';', which
		// is converted to ':'
		//
		// Remark: If URIs should be used for pathnames, eventually.
		// Also URI schemes have a ':' as separator, e.g. "http:", it is
		// assumend that these schemes are already given as "http|", since
		// URIs instead of filenames are not standard RIB.
		//
		// Note, the CUri has a method to convert pathes to 'file:' URIs.

		std::string::iterator saviter;

		bool startpath = true;
		bool firstletter = false;

		iterinc = true;
		for ( iter = strval.begin();  iter != strval.end(); iterinc ? ++iter : iter ) {
			iterinc = true;

			if ( firstletter && *iter == ':' ) {
				// <letter>:/path -> <letter>|/path
				saviter = iter;
				++iter;
				iterinc = false;
				if ( *iter == '/' || *iter == '\\' ) {
					*saviter = '|';
				}
				firstletter = false;
				continue;
			}

			if ( firstletter && (*iter == '/' || *iter == '\\') ) {
				// <letter>/path -> <letter>|/path
				std::string::difference_type d = distance(strval.begin(), iter);
				d += 1;
				if ( separator == ':' )
					strval.insert(iter, 1, '|');
				else
					strval.insert(iter, 1, ':');
				iter = strval.begin();
				advance(iter, d);
				firstletter = false;
				continue;
			}

			firstletter = false;
			if ( startpath ) {
				if ( (*iter >= 'a' && *iter <= 'z') || (*iter >= 'A' && *iter <= 'Z') ) {
					firstletter = true;
					startpath = false;
					continue;
				}
			}

			startpath = false;
			if ( *iter == ':' || *iter ==';' ) {
				startpath = true;
				// Also converts a separator ';' to ':'
				*iter = ':';
				continue;
			}
		}
	}
#endif

	iterinc = true;
	std::string v;
	for ( iter = strval.begin();  iter != strval.end(); iterinc ? ++iter : iter ) {
		iterinc = true;
		if ( *iter == separator ) {
			++count;
			// Not only Win32, unmasks also scheme (e.g. 'http:' could be written as 'http|' )
			// The pipe symbol is usually not part of a search path
			if ( isPathList && separator == ':' )
				unmaskColon(v);
			m_stringList.push_back(v);
			v.clear();
		} else {
			v.push_back(*iter);
		}
	}
	
	if ( v.size() > 0 ) {
		++count;
		// Not only Win32, unmasks also scheme (e.g. 'http:' could be written as 'http|' )
		// The pipe symbol is usually not part of a search path
		if ( isPathList && separator == ':' )
			unmaskColon(v);
		m_stringList.push_back(v);
	}

	// Win32: In m_stringList the pathes are again <letter>:/path

	return count;
}

void CStringList::implode(char separator, std::string &str, bool isPathList)
{
	const_iterator i = begin();
	std::string mask;

	str = "";
	while ( i != end() ) {
		if ( isPathList && separator == ':' ) {
			// Not only Win32, masks also scheme (e.g. 'http:' -> 'http|')
			mask = (*i);
			maskColon(mask);
			str += mask;
		} else {
			str += (*i);
		}
		i++;
		if ( i != end() ) {
			str += separator;
		}
	}
}
