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
#include "tools/env.h"
#endif _RICPP_TOOLS_ENV_H

#include <list>
#include <map>
#include <algorithm>

namespace RiCPP {

class CStringList {
public:
	typedef std::list<std::string>::const_iterator const_iterator;
	typedef std::list<std::string>::size_type size_type;

private:
	std::list<std::string> m_stringList;
	std::map<std::string, std::string> m_substMap;

	void insertVar(std::string &varName, bool useEnv);

public:
	inline CStringList() { }
	bool explode(
		const char *str,
		char seperator = ';',
		char varChar = '$',
		char maskChar = '\\',
		char singleQuote = '\'',
		char doubleQuote = '\"',
		bool useEnv = true
		);

	inline void push(const char *str)
	{
		if ( !str )
			return;
		std::string v(str);
		m_stringList.push_back(v);
		return;
	}

	inline void push(const std::string &v)
	{
		m_stringList.push_back(v);
		return;
	}

	inline void pop()
	{
		m_stringList.pop_back();
	}

	inline std::string &top()
	{
		return m_stringList.back();
	}

	inline const_iterator begin()
	{
		return m_stringList.begin();
	}

	inline const_iterator end()
	{
		return m_stringList.end();
	}

	inline size_type size()
	{
		return m_stringList.size();
	}

	inline void addSubst(const char *strvar, const char *strsubst)
	{
		std::string var(strvar ? strvar : "");
		std::string subst(strsubst ? strsubst : "");
		m_substMap[var] = subst;
	}

	inline void removeSubst(const char *strvar)
	{
		std::string var(strvar ? strvar : "");
		if ( m_substMap.find(var) != m_substMap.end() )
			m_substMap.erase(var);
	}

	inline void clearSubst()
	{
		m_substMap.clear();
	}

	inline void clear()
	{
		m_stringList.clear();
	}

	inline bool empty()
	{
		return m_stringList.empty();
	}
}; // CStringList

}
#endif // _RICPP_TOOLS_STRINGLIST_H
