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
#include <vector>
#include <map>
#include <algorithm>

namespace RiCPP {

template<typename CharType = char, typename ValueType = std::vector<CharType> >
class TStringList {
private:
	std::list<ValueType> m_stringList;
	std::map<ValueType, ValueType> m_substMap;

public:
	typedef typename ValueType value_type;
	typedef typename std::list<ValueType>::const_iterator const_iterator;
	typedef typename std::list<ValueType>::size_type size_type;

private:
	inline static bool isTrue(CharType c) { return true; }
	inline static void copyinto(const CharType *str, value_type &v, bool terminateStr=true) {
		v.clear();
		if ( !str )
			return;
		v.reserve(32);
		for ( ; *str; str++ )
			v.push_back(*str);
		if ( terminateStr )
			v.push_back(0);
	}

	void insertVar(value_type &varName) {
		varName.push_back(0);
		if ( m_substMap.find(varName) != m_substMap.end() ) {
			value_type &subst = m_substMap[varName];
			varName = subst;
		} else {
			std::string var;
			std::string strname;
			copy(varName.begin(), varName.end(), back_inserter(strname));
			CEnv::find(var, strname.c_str());
			if ( !var.empty() ) {
				varName.clear();
				copy(var.begin(), var.end(), back_inserter(varName));
			} else {
				varName.clear();
			}
		}
	}
public:
	inline TStringList() {
	}
	inline bool explode(
		const CharType *str,
		CharType seperator = (CharType)';',
		CharType maskChar = (CharType)'\\',
		CharType varChar = (CharType)'$',
		CharType singleQuote = (CharType)'\'',
		CharType doubleQuote = (CharType)'\"'
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
			maskinquote,
			varinquote,
		};
		EState state = normal;

		value_type strval;
		copyinto(str, strval);

		value_type v;
		v.clear();
		v.reserve(32);
		value_type varName;
		varName.clear();
		varName.reserve(32);

		unsigned long idx = 0;
		value_type::iterator savptr;
		value_type::iterator ptr = strval.begin();
		bool ptrinc = true;
		for ( ; *ptr != 0; ptrinc ? ++ptr : ptr ) {
			ptrinc = true;
			switch ( state ) {
				case normal:
					if ( *ptr == seperator ) {
						v.push_back(0);
						push(v);
						v.clear();
						continue;
					} else if ( *ptr == maskChar ) {
						state = maskchar;
					} else if ( *ptr == singleQuote ) {
						state = singlequote;
					} else if ( *ptr == doubleQuote ) {
						state = doublequote;
					} else if ( *ptr == varChar ) {
						savptr = ptr;
						state = varchar;
					} else {
						v.push_back(*ptr);
					}
					break;
				case maskchar:
					v.push_back(*ptr);
					state = normal;
					break;
				case varchar:
					if ( *ptr == varChar ) {
						state = normal;
						value_type::difference_type d = distance(strval.begin(), savptr);
						ptr++;
						savptr = strval.erase(savptr, ptr);
						insertVar(varName);
						strval.insert(savptr, varName.begin(), varName.end());
						varName.clear();
						ptr = strval.begin();
						advance(ptr, d);
						ptrinc = false;
					} else {
						varName.push_back(*ptr);
					}
					break;
				case singlequote:
					if ( *ptr == singleQuote ) {
						state = normal;
					} else {
						v.push_back(*ptr);
					}
					break;
				case doublequote:
					if ( *ptr == maskChar ) {
						state = maskinquote;
					} else if ( *ptr == varChar ) {
						state = varinquote;
						savptr = ptr;
					} else if ( *ptr == doubleQuote ) {
						state = normal;
					} else {
						v.push_back(*ptr);
					}
					break;
				case maskinquote:
					v.push_back(*ptr);
					state = doublequote;
					break;
				case varinquote:
					if ( *ptr == varChar ) {
						state = doublequote;
						varName.push_back(0);
						value_type::difference_type d = distance(strval.begin(), savptr);
						ptr++;
						savptr = strval.erase(savptr, ptr);
						insertVar(varName);
						strval.insert(savptr, varName.begin(), varName.end());
						varName.clear();
						ptr = strval.begin();
						advance(ptr, d);
						ptrinc = false;
					} else {
						varName.push_back(*ptr);
					}
					break;
			}
		}
		if ( v.size() > 0 ) {
			v.push_back(0);
			push(v);
		}
		return true;
	}
	inline void push(const CharType *str)
	{
		if ( !str )
			return;
		value_type v;
		copyinto(str, v);
		m_stringList.push_back(v);
		return;
	}
	inline void push(const value_type &v)
	{
		m_stringList.push_back(v);
		return;
	}
	inline void pop()
	{
		m_stringList.pop_back();
	}
	inline value_type &top()
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
	inline void addSubst(const CharType *strvar, const CharType *strsubst)
	{
		value_type var;
		value_type subst;
		copyinto(strvar, var);
		copyinto(strsubst, subst, false);
		m_substMap[var] = subst;
		return;
	}
	inline void removeSubst(const CharType *strvar)
	{
		value_type var;
		copyinto(strvar, var);
		m_substMap[var] = "";
	}
	inline void clearSubst()
	{
		m_substMap.clear()
	}
	inline void clear()
	{
		m_stringList.clear()
	}
	inline bool empty()
	{
		return m_stringList.empty();
	}
}; // TStringList

}
#endif // _RICPP_TOOLS_STRINGLIST_H
