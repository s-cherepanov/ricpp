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

#ifndef _RICPP_TOOLS_ENV_H
#include "ricpp/tools/env.h"
#endif _RICPP_TOOLS_ENV_H

#include <list>
#include <map>
#include <algorithm>

namespace RiCPP {

/** @brief Class used to store strings, used like a stack with a constant iterator.
 *
 *  The Strings can contain variables ($VARIABLE), which are substituted
 *  before inserted. Strings can be pushed or obtained from a string list by
 *  'exploding' a string like a search path for executables. Can also
 *  substitute environment variables like $HOME or $PROGDIR, see env.h.
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
	/** @brief A map to substitute environment variables, stores name as key and the substitute.
	 */
	std::map<std::string, std::string> m_substMap;

	/** @brief Searches a variable varName in \a m_substMap (and environment) and returns the found
	 *  value in \a varName.
	 *  @param varName (input/output) variable name to search for, also used return
	 *         the found value. Is cleared if not empty.
	 *  @param useEnv If true tries to get \a varEnv as environment variable if it is not
	 *         found in \a m_substMap.
	 */
	void getVar(std::string &varName, bool useEnv);

public:
	/** @brief Seperates a string.
	 *
	 *  Seperates a string \a str using the seperator character \a seperator. The seperated
	 *  strings are stored internally. Does substitution of $variable. If useEnv is
	 *  true, the variables are search among the environment variables as well. Special
	 *  chars like '$' can be mask by '\' to loose their special meaning. You can use double
	 *  quoted parts of the string to enclose a string that contains a seperator or
	 *  single quoted parts to supress also the variable processing.
	 *  
	 *  @param seperator Character that seperates the strings, usually ';' (the RiCPP
	 *         seperator for path lists) or ' ' (to seperate parameters like in name of
	 *         TRiCPP::begin(RtString name)). If the seperator is NUL, no seperation
	 *         is done, only the variables are substituted before inserting.
	 *  @param str The string that is seperated.
	 *  @param useEnv true if the environment variables should be also searched for $variables.
	 *  @return The number of strings inserted.
	 */
	size_type explode(
		char seperator,
		const char *str,
		bool useEnv
		);

	/** @brief Pushes a string to the stored strings. Variables are substituted.
	 *  @param str A copy of the NUL terminated string is stored.
	 *  @param useEnv true if the environment variables of the system should
	 *         also be used a variables
	 */
	inline void push(const char *str, bool useEnv)
	{
		explode(0, str, useEnv);
	}

	/** @brief Pushes a string to the stored strings.
	 *  @param v A copy of the string is stored.
	 *  @param useEnv true if the environment variables of the system should
	 *         also be used a variables
	 */
	inline void push(const std::string &v, bool useEnv)
	{
		explode(0, v.c_str(), useEnv);
	}

	/** @brief Removes the last inserted string.
	 */
	inline void pop()
	{
		m_stringList.pop_back();
	}

	/** @brief Use top() to access the last inserted string.
	 *  @return The last inserted string.
	 */
	inline std::string &top()
	{
		return m_stringList.back();
	}

	/** @brief Constant iterator to access the strings (beginning).
	 *  @return Iterator with the first inserted string as current element.
	 */
	inline const_iterator begin()
	{
		return m_stringList.begin();
	}

	/** @brief Constant iterator to access the strings (behind the last element).
	 *  @return Iterator to query the end of the iteration
	 *          (like sthe std iterators does not refer a valid element).
	 */
	inline const_iterator end()
	{
		return m_stringList.end();
	}

	/** @brief Gets the size of the string list
	 * @return The number of stored strings.
	 */
	inline size_type size()
	{
		return m_stringList.size();
	}

	/** @brief Adds a substitute for a variable.
	 * 
	 *  A variable \a strvar that can occur inside the strings 
	 *  will be replaced with \a strsubst before stored.
	 *
	 * @param strvar The name of the variable. E.g. $MYVAR is the variable, \a strvar is "MYVAR"
	 * @param strsubst The string that is used as substitute for the variable.
	 */
	inline void addSubst(const char *strvar, const char *strsubst)
	{
		std::string var(strvar ? strvar : "");
		std::string subst(strsubst ? strsubst : "");
		m_substMap[var] = subst;
	}

	/** @brief Removes a substitute for a variable.
	 *
	 * @param strvar The name of the variable.
	 */
	inline void removeSubst(const char *strvar)
	{
		std::string var(strvar ? strvar : "");
		if ( m_substMap.find(var) != m_substMap.end() ) {
			m_substMap.erase(var);
		}
	}

	/** @brief Removes all variables.
	 */
	inline void clearSubst()
	{
		m_substMap.clear();
	}

	/** @brief Removes all strings.
	 */
	inline void clear()
	{
		m_stringList.clear();
	}

	/** @brief Find out if no strings are stored
	 *  @return true, if there are no stored strings (also size() == 0).
	 */
	inline bool empty()
	{
		return m_stringList.empty();
	}
}; // CStringList

}
#endif // _RICPP_TOOLS_STRINGLIST_H
