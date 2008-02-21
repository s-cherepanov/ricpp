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

bool CStringList::getVar(std::string &varName, bool useEnv)
{
	if ( m_callback ) {
		if ( (*m_callback)(varName) )
			return true;
	}

	if ( varName.size() <= 0 )
		return false;

	if ( varName.size() == 1 ) {
		if ( varName[0] == '@' || varName[0] == '&' )
			return false;
	}

	if ( m_substMap.find(varName) != m_substMap.end() ) {
		std::string &subst = m_substMap[varName];
		varName = subst;
		return true;
	}
	
	if ( useEnv ) {
		std::string var;
		std::string strname;
		CEnv::find(var, varName.c_str());
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
	char seperator,
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
	const char maskChar = '\\';
	const char singleQuote = '\'';
	const char doubleQuote = '\"';
	const char varBegin = '{';
	const char varEnd = '}';

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
	bool iterinc = true;

	if ( doSubstitute ) {
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
					} else if ( isPathList && (*iter == '@' || *iter == '&') ) {
						saviter = iter;
						varName = *iter;
						++iter;

						// replace variable
						getVar(varName, useEnv);

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
				case maskchar:
					state = normal;
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
							getVar(varName, useEnv);
							if ( isPathList ) {
								CFilepathConverter::convertToInternal(varName);
							}

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
						getVar(varName, useEnv);
						if ( isPathList ) {
							CFilepathConverter::convertToInternal(varName);
						}

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
				case singlequote:
					if ( *iter == singleQuote ) {
						state = normal;
					}
					break;
			}
		}
		if ( state == varchar || state == varcharpar ) {
			// copy from varchar replace variable
			getVar(varName, useEnv);
			if ( isPathList ) {
				CFilepathConverter::convertToInternal(varName);
			}

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

#ifdef WIN32
	if ( isPathList && seperator == ':' ) {
		for ( iter = strval.begin();  iter != strval.end(); iterinc ? ++iter : iter ) {
			if ( *iter == ';'  ) {
				*iter = ':';
			}
		}
	}
#endif

#if 0
	// The messy part...
	// In RIB files the seperator for pathes is unfortunatly ':'.
	// ':' can have a double meaning as seperator for drive letters
	// and seperator for pathes.
	// ':' is converted to '|', if "<letter>:/" is the prefix of a path
	// i.e. a windows path letter. Since there are no drive letters
	// in other OSes as WIN32 this is done only for WIN32 binaries
	// If RIB files have to be exchanged across OSe, drive letters
	// won't work anyway.
	// Also in windows the path seperator can be a ';', which
	// is converted to ':'
	// Also schemes can have a : as seperator, e.g. "http:/", it is
	// assumend that these schems are already given as "http|/", since
	// URLs instead of filenames are not standard RIB
	if ( isPathList && seperator == ':' ) {
		state = normal;

		bool startpath = true;
		bool firstletter = false;

		for ( iter = strval.begin();  iter != strval.end(); iterinc ? ++iter : iter ) {
			iterinc = true;

			if ( firstletter && *iter == ':' ) {
				saviter = iter;
				++iter;
				iterinc = false;
				if ( *iter == '/' ) {
					*saviter = '|';
				}
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
				// also convert a seperator ';' to ':'
				*iter = ':';
				continue;
			}
		}
		// CFilepathConverter::convertToInternal(strval);
	}
#endif

	state = normal;

	for ( iter = strval.begin();  iter != strval.end(); iterinc ? ++iter : iter ) {
		iterinc = true;
		switch ( state ) {
			case normal:
				if ( *iter == seperator ) {
					++count;
					// Not only Win32, unmasks also scheme (e.g. 'http|' -> 'http:')
					if ( isPathList && seperator == ':' )
						unmaskColon(v);
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
				// if ( *iter != singleQuote && *iter != doubleQuote && *iter != maskChar && *iter != seperator )
				//	v.push_back(maskChar);
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
				// if ( *iter != singleQuote && *iter != doubleQuote && *iter != maskChar && *iter != seperator )
				//	v.push_back(maskChar);
				v.push_back(*iter);
				state = doublequote;
				break;
		}
	}
	
	if ( v.size() > 0 ) {
		++count;
		// Not only Win32, unmasks also scheme (e.g. 'http|' -> 'http:')
		if ( isPathList && seperator == ':' )
			unmaskColon(v);
		m_stringList.push_back(v);
	}

	return count;
}

void CStringList::implode(char seperator, std::string &str, bool isPathList)
{
	const_iterator i = begin();
	std::string mask;

	str = "";
	while ( i != end() ) {
		if ( isPathList && seperator == ':' ) {
			// Not only Win32, masks also scheme (e.g. 'http:' -> 'http|')
			mask = (*i);
			maskColon(mask);
			str += mask;
		} else {
			str += (*i);
		}
		i++;
		if ( i != end() ) {
			str += seperator;
		}
	}
}
