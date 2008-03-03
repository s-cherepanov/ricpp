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

#ifndef _RICPP_TOOLS_FILEPATH_H
#include "ricpp/tools/filepath.h"
#endif // _RICPP_TOOLS_FILEPATH_H

using namespace RiCPP;


bool CPathReplace::operator()(std::string &varName)
{
	if ( varName == "&" ) {
		varName = m_path;
		return true;
	} else if ( varName == "@" ) {
		varName = m_standardpath;
		return true;
	}
	
	return false;
}

bool CStringList::getVar(std::string &varName, char separator, bool useEnv)
{
	if ( m_callback ) {
		if ( (*m_callback)(varName) ) {
			return true;
		}
	}

	if ( varName.size() <= 0 )
		return false;

	if ( varName.size() == 1 ) {
		if ( varName[0] == '@' || varName[0] == '&' ) {
			varName.clear();
			return false;
		}
	}

	if ( m_substMap.find(varName) != m_substMap.end() ) {
		std::string &subst = m_substMap[varName];
		varName = subst;
		return true;
	}
	
	if ( useEnv ) {
		std::string var;
		std::string strname;
		CEnv::find(var, varName.c_str(), false);
		if ( !var.empty() ) {
			varName = var;
			return true;
		} else {
			varName.clear();
		}
	} else {
		varName.clear();
	}
	return false;
}

CStringList::size_type CStringList::explode(
	char separator,
	const char *str,
	bool doSubstitute,
	bool useEnv,
	bool isPathList
)
{
	size_type count = 0;
	if ( !str )
		return count;

	const char varChar = '$';
	const char varBegin = '{';
	const char varEnd = '}';

	enum EState {
		normal,
		varchar,
		varcharpar
	};
	EState state = normal;

	std::string strval(str ? str : "");

	std::string v("");
	std::string varName("");

	std::string::iterator saviter;
	std::string::iterator iter;
	bool iterinc = true;

	/// @todo Substitution part of the renderstate maybe give frontend a state for default options and declarations
	if ( doSubstitute ) {
		for ( iter = strval.begin(); iter != strval.end(); iterinc ? ++iter : iter ) {
			iterinc = true;
			switch ( state ) {
				case normal:
					if ( *iter == varChar ) {
						saviter = iter;
						state = varchar;
					} else if ( isPathList && (*iter == '@' || *iter == '&') ) {
						saviter = iter;
						varName = *iter;
						++iter;

						// replace variable @ or &
						getVar(varName, separator, useEnv);

						std::string::difference_type d = distance(strval.begin(), saviter);
						saviter = strval.erase(saviter, iter);
						d += varName.size();
						strval.insert(saviter, varName.begin(), varName.end());
						iter = strval.begin();
						advance(iter, d);

						varName.clear();
						iterinc = false;
					}
					break;
				case varchar:
					if ( *iter == varBegin ) {
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

						if ( !varName.empty() ) {
							// replace variable
							getVar(varName, separator, useEnv);

							std::string::difference_type d = distance(strval.begin(), saviter);
							saviter = strval.erase(saviter, iter);
							d += varName.size();
							strval.insert(saviter, varName.begin(), varName.end());
							iter = strval.begin();
							advance(iter, d);

							varName.clear();
							iterinc = false;
						}
					}
					break;
				case varcharpar:
					if ( *iter == varEnd ) {
						state = normal;
						iter++;

						// replace variable
						getVar(varName, separator, useEnv);

						std::string::difference_type d = distance(strval.begin(), saviter);
						saviter = strval.erase(saviter, iter);
						d += varName.size();
						strval.insert(saviter, varName.begin(), varName.end());
						iter = strval.begin();
						advance(iter, d);

						varName.clear();
						iterinc = false;
					} else {
						varName.push_back(*iter);
					}
					break;
			}
		}
		if ( state == varchar || state == varcharpar ) {
			// copy from varchar replace variable
			getVar(varName, separator, useEnv);

			std::string::difference_type d = distance(strval.begin(), saviter);
			saviter = strval.erase(saviter, iter);
			d += varName.size();
			strval.insert(saviter, varName.begin(), varName.end());
			// iter = strval.begin();
			// advance(iter, d);

			varName.clear();
			// iterinc = false;
		}
	}

	state = normal;
	
//if 0
#ifdef WIN32
	if ( isPathList && separator == ':' ) {
		// The messy part...
		// Win32 scans here for C/path -> C|/path and  C:/path -> C|/path
		// In RIB files the separator for pathes is unfortunatly ':'.
		// ':' can have a double meaning as separator for drive letters
		// and separator for pathes.
		// ':' is converted to '|', if "<letter>:/" is the prefix of a path
		// i.e. a windows path letter. Since there are no drive letters
		// in other OSes as WIN32 this is done only for WIN32 binaries
		// If RIB files have to be exchanged across OSe, drive letters
		// won't work anyway.
		// Also in windows the path separator can be a ';', which
		// is converted to ':'
		// Also URI schemes can have a : as separator, e.g. "http:/", it is
		// assumend that these schems are already given as "http|/", since
		// URIs instead of filenames are not standard RIB

		state = normal;

		bool startpath = true;
		bool firstletter = false;

		for ( iter = strval.begin();  iter != strval.end(); iterinc ? ++iter : iter ) {
			iterinc = true;

			if ( firstletter && *iter == ':' ) {
				// <letter>:/path -> <letter>|/path
				saviter = iter;
				++iter;
				iterinc = false;
				if ( *iter == '/' ) {
					*saviter = '|';
				}
				firstletter = false;
				continue;
			}

			if ( firstletter && *iter == '/' ) {
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
//endif

	for ( iter = strval.begin();  iter != strval.end(); iterinc ? ++iter : iter ) {
		iterinc = true;
		switch ( state ) {
			case normal:
				if ( *iter == separator ) {
					++count;
					// Not only Win32, unmasks also scheme (e.g. 'http|' -> 'http:')
					if ( isPathList && separator == ':' )
						unmaskColon(v);
					m_stringList.push_back(v);
					v.clear();
				} else {
					v.push_back(*iter);
				}
				break;
		}
	}
	
	if ( v.size() > 0 ) {
		++count;
		// Not only Win32, unmasks also scheme (e.g. 'http|' -> 'http:')
		if ( isPathList && separator == ':' )
			unmaskColon(v);
		m_stringList.push_back(v);
	}

	// Win32: In m_stringList the pathes are <letter>:/path

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

std::string &CStringList::expand(std::string &result, const char *str, bool isPath) {
	result = "";
	if ( emptyStr(str) )
		return result;
	std::string tempPath = str;
	if (isPath) {
		CFilepathConverter::convertToInternal(tempPath);
	}
	explode(0, tempPath.c_str(), true, true, isPath);
	result.clear();
	const_iterator first = begin();
	if ( first != end() ) {
		result = (*first).c_str();
	}
	return result;
}
