#ifndef _RICPP_TOOLS_STRINGLIST_H
#define _RICPP_TOOLS_STRINGLIST_H

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

/** @file stringlist.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declaration of the string list class
 */
#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif // _RICPP_TOOLS_INLINETOOLS_H

#include <list>
#include <functional>
#include <algorithm>

namespace RiCPP {

/** @brief Class used to store strings, used like a stack with a constant iterator.
 *
 *  Strings can be pushed or obtained from a string
 *  list by 'exploding' a string like a search path for executables.
 *
 *  For substitution of variables see RiCPP::varSubst()
 */
class CStringList {
public:
	/** @brief Const iterator for the strings.
	 */
	typedef std::list<std::string>::const_iterator const_iterator;
	/** @brief Size type for the number of stored strings: CStringList::size()
	 */
	typedef std::list<std::string>::size_type size_type;

private:
	/** @brief The strings are stored in a std::list.
	 */
	std::list<std::string> m_stringList;

	/** @brief Converts all ':' to '|'
	 *
	 * Used in path lists to mask the ':' which is a path separator
	 * and a scheme and drive letter separator.
	 *
	 * @param str String where the '|' are converted
	 */
	inline void maskColon(std::string &str)
	{
		std::for_each(str.begin(), str.end(), std::ptr_fun(RiCPP::maskColon));
	}
	
	/** @brief Converts all '|' to ':'
	 * @param str String where the '|' are converted
	 */
	inline void unmaskColon(std::string &str)
	{
		std::for_each(str.begin(), str.end(), std::ptr_fun(RiCPP::unmaskColon));
	}

public:
	/** @brief Standard constructor
	 *
	 * No search callback.
	 */
	inline CStringList() {  }
	inline CStringList(char separator, const char *str,	bool isPathList)
	{
		explode(separator, str, isPathList);
	}

	/** @brief Separates a string.
	 *
	 *  Separates a string @a str using the separator character @a separator. The separated
	 *  strings are stored internally.
	 *  
	 *  @param separator Character that separates the strings, usually ':' (the RiCPP
	 *         separator for path lists) or ' ' (to separate parameters like in name of
	 *         TRiCPP::begin(RtString name)). If the separator is NUL, no separation
	 *         is done, only the variables are substituted before inserting.
	 *  @param str The string that will be separated.
	 *  @param isPathList true, to handle pathlists. For WIN32 a bit messy because rib uses ':'
	 *         as path separator, but windows uses them as drive letter separators as well.
	 *         For under Win32 the String is scanned from left to right to find pathes
	 *         like C:\path or c/path, ... so C:/test:. will explode to be C:/test and .
	 *         Disadvantage: Directorynames A, B, C, .... at the beginning of a path are not possible.
	 *  @return The number of strings inserted.
	 */
	size_type explode(char separator, const char *str, bool isPathList);

	/** @brief Concatenates stringlist
	 *
	 * The stringlist is concatenated to single string using @a separator.
	 *
	 * @param separator Character that is used as separator for the new @a str
	 * @retval str Container for the new concatenated string
	 * @param isPathList true, for handling pathlists.
	 */
	void implode(
		char separator,
		std::string &str,
		bool isPathList
	);

	/** @brief Pushes a string to the stored strings.
	 *
	 *  @param str A copy of the NUL terminated string is stored.
	 */
	inline void push(const char *str)
	{
		m_stringList.push_back(noNullStr(str));
	}

	/** @brief Removes the last inserted string.
	 */
	inline void pop_back()
	{
		m_stringList.pop_back();
	}

	/** @brief Use top() to access the last inserted string.
	 *  @return The last inserted string.
	 */
	inline std::string &back()
	{
		return m_stringList.back();
	}

	/** @brief Constant iterator to access the strings (beginning).
	 *  @return Iterator with the first inserted string as current element.
	 */
	inline const_iterator begin() const
	{
		return m_stringList.begin();
	}

	/** @brief Constant iterator to access the strings (behind the last element).
	 *  @return Iterator to query the end of the iteration
	 *          (like sthe std iterators does not refer a valid element).
	 */
	inline const_iterator end() const
	{
		return m_stringList.end();
	}

	/** @brief Gets the size of the string list
	 * @return The number of stored strings.
	 */
	inline size_type size() const
	{
		return m_stringList.size();
	}

	/** @brief Removes all strings.
	 */
	inline void clear()
	{
		m_stringList.clear();
	}

	/** @brief Is the stringlist empty?
	 *  @return true, if there are no stored strings (also size() == 0).
	 */
	inline bool empty() const
	{
		return m_stringList.empty();
	}

	/** @brief Checks if a string is in the string list.
	 * @param str String to search for
	 * @return true, if string was found in the string list
	 */
	inline bool isMember(const char *str) const
	{
		return std::find(
			m_stringList.begin(),
			m_stringList.end(),
			std::string(noNullStr(str))
			) != m_stringList.end();
	}
}; // CStringList

}
#endif // _RICPP_TOOLS_STRINGLIST_H
