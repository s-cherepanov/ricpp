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

void CStringList::getVar(std::string &varName, bool useEnv)
{
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

CStringList::size_type CStringList::explode(
	char seperator,
	const char *str,
	bool useEnv)
{
	size_type count = 0;
	if ( !str )
		return count;

	const char varChar = '$';
	const char maskChar = '\\';
	const char singleQuote = '\'';
	const char doubleQuote = '\"';

	enum EState {
		normal,
		maskchar,
		varchar,
		varcharpar,
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
				if ( *iter == '(' ) {
					state = varcharpar;
				} else if (
					(*iter >= 'a' && *iter <= 'z') ||
					(*iter >= 'A' && *iter <= 'Z') ||
					(*iter >= '0' && *iter <= '9') ||
					*iter == '_'
				) {
					varName.push_back(*iter);
				} else {
					state = normal;

					// replace variable
					std::string::difference_type d = distance(strval.begin(), saviter);
					saviter = strval.erase(saviter, iter);
					getVar(varName, useEnv);
					d += varName.size();
					strval.insert(saviter, varName.begin(), varName.end());
					varName.clear();
					iter = strval.begin();
					advance(iter, d);
					iterinc = false;
				}
				break;
			case varcharpar:
				if ( *iter == ')' ) {
					state = normal;
					iter++;

					// copy from varchar replace variable
					std::string::difference_type d = distance(strval.begin(), saviter);
					saviter = strval.erase(saviter, iter);
					getVar(varName, useEnv);
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
	if ( state == varchar || state == varcharpar ) {
		// copy from varchar replace variable
		std::string::difference_type d = distance(strval.begin(), saviter);
		saviter = strval.erase(saviter, iter);
		getVar(varName, useEnv);
		d += varName.size();
		strval.insert(saviter, varName.begin(), varName.end());
		varName.clear();
		// iter = strval.begin();
		// advance(iter, d);
		// iterinc = false;
	}

	state = normal;

	for ( iter = strval.begin();  iter != strval.end(); iterinc ? ++iter : iter ) {
		iterinc = true;
		switch ( state ) {
			case normal:
				if ( *iter == seperator ) {
					++count;
					m_stringList.push_back(v);
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
		++count;
		m_stringList.push_back(v);
	}

	return count;
}
