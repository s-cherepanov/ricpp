#ifndef _RICPP_TOOLS_STRINGLIST_H
#define _RICPP_TOOLS_STRINGLIST_H

// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 200,, 2005 Pixar
//                 All rights Reservered
//
// Copyright � of RiCPP 2007, Andreas Pidde
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

#ifndef _RICPP_TOOLS_ENV_H
#include "tools/env.h"
#endif _RICPP_TOOLS_ENV_H

#include <list>
#include <map>
#include <string>
#include <algorithm>

namespace RiCPP {

// typedef std::list<std::string>::const_iterator CStringList_const_iterator;
// typedef std::list<std::string>::size_type CStringList_size_type;

class CStringList {
public:
	typedef std::list<std::string>::const_iterator const_iterator;
	typedef std::list<std::string>::size_type size_type;

private:
	std::list<std::string> m_stringList;
	std::map<std::string, std::string> m_substMap;

	void insertVar(std::string &varName, bool useEnv) {
		if ( m_substMap.find(varName) != m_substMap.end() ) {
			std::string &subst = m_substMap[varName];
			varName = subst;
		}
		
		if ( useEnv ) {
			std::string var;
			std::string strname;
			CEnv::find(var, varName.c_str());
			if ( !var.empty() ) {
				varName = var;
			} else {
				varName.clear();
			}
		} else {
			varName.clear();
		}
	}

public:
	inline CStringList() {
	}
	inline bool explode(
		const char *str,
		char seperator = ';',
		char maskChar = '\\',
		char varChar = '$',
		char singleQuote = '\'',
		char doubleQuote = '\"',
		bool useEnv = true
		)
	{
		if ( !str )
			return false;

		enum EState {
			normal,
			maskchar,
			varchar,
			singlequote,
			doublequote,
			maskinquote
		};
		EState state = normal;

		std::string strval(str ? str : "");

		std::string v("");
		std::string varName("");

		std::string::iterator saviter;
		std::string::iterator iter;
		bool iterinc;

		for ( iter = strval.begin(); iter != strval.end(); iterinc ? ++iter : iter ) {
			iterinc = true;
			switch ( state ) {
				case normal:
					if ( *iter == maskChar ) {
						state = maskchar;
						continue;
					} else if ( *iter == singleQuote ) {
						state = singlequote;
					} else if ( *iter == varChar ) {
						saviter = iter;
						state = varchar;
					}
					break;
				case maskchar:
					state = normal;
					break;
				case varchar:
					if ( *iter == varChar ) {
						state = normal;
						std::string::difference_type d = distance(strval.begin(), saviter);
						iter++;
						saviter = strval.erase(saviter, iter);
						insertVar(varName, useEnv);
						d += varName.size();
						strval.insert(saviter, varName.begin(), varName.end());
						varName.clear();
						iter = strval.begin();
						advance(iter, d);
						iterinc = false;
					} else {
						varName.push_back(*iter);
					}
					break;
				case singlequote:
					if ( *iter == singleQuote ) {
						state = normal;
					}
					break;
			}
		}

		for ( iter = strval.begin();  iter != strval.end(); iterinc ? ++iter : iter ) {
			iterinc = true;
			switch ( state ) {
				case normal:
					if ( *iter == seperator ) {
						push(v);
						v.clear();
						continue;
					} else if ( *iter == maskChar ) {
						state = maskchar;
					} else if ( *iter == singleQuote ) {
						state = singlequote;
					} else if ( *iter == doubleQuote ) {
						state = doublequote;
					} else {
						v.push_back(*iter);
					}
					break;
				case maskchar:
					v.push_back(*iter);
					state = normal;
					break;
				case singlequote:
					if ( *iter == singleQuote ) {
						state = normal;
					} else {
						v.push_back(*iter);
					}
					break;
				case doublequote:
					if ( *iter == maskChar ) {
						state = maskinquote;
					} else if ( *iter == doubleQuote ) {
						state = normal;
					} else {
						v.push_back(*iter);
					}
					break;
				case maskinquote:
					v.push_back(*iter);
					state = doublequote;
					break;
			}
		}
		
		if ( v.size() > 0 ) {
			push(v);
		}
		return true;
	}
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