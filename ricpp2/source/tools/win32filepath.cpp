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

/** @file win32filepath.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Win32 implementation of the file path classes.
 */

#include "tools/filepath.h"
#include <windows.h>
#include <direct.h>

using namespace RiCPP;

/** @brief The native path seperator in windows is the backslash '\'
 */
char CFilepathConverter::nativePathSeperator() { return '\\'; }

/** @brief Different pathes are seperated by ':' like in %PATH%
 */
char CFilepathConverter::nativePathlistSeperator() { return ';'; }

/** @brief path conversion is done by replacing all '\' by '/'.
 */
std::string &CFilepathConverter::convertToInternal(std::string &var)
{
	std::string::iterator i = var.begin();
	for ( ; i != var.end(); i++ ) {
		if ( (*i) == '\\' )
			(*i) = '/';
	}

	return var;
}

/** @brief path conversion is done by replacing all '/' by '\'.
 */
std::string &CFilepathConverter::convertToNative(std::string &var)
{
	std::string::iterator i = var.begin();
	for ( ; i != var.end(); i++ ) {
		if ( (*i) == '/' )
			(*i) = '\\';
	}

	return var;
}

/** @brief See description of CFilepath::convertToNative() in header file filepath.h
 */
void CFilepath::convertToNative()
{
	char pathbuf[MAX_PATH];

	if ( m_filepath.empty() ) {
		if ( _getcwd(pathbuf, sizeof(pathbuf)) ) {
			pathbuf[sizeof(pathbuf)-1] = 0;
			m_filepath = pathbuf;
		}
	}

	m_nativepath = m_filepath;
	CFilepathConverter::convertToNative(m_nativepath);

	m_fullpath = "";
	char *ptr;
	if ( GetFullPathNameA(m_nativepath.c_str(), sizeof(pathbuf), pathbuf, &ptr) != 0 ) {
		pathbuf[sizeof(pathbuf)-1] = 0;
		m_fullpath = pathbuf;
	}
}

/** @brief In Win32 the path can be absolute by a trailing '\' (or two if it is a UNC path) or if the second charakter in the path is a ':' (the first is a drive letter)
 */
bool CFilepath::isAbsolute() const
{
	return (m_nativepath.size() > 0 && m_nativepath[0] == '\\') || (m_nativepath.size() > 1 && m_nativepath[1] == ':');
}
