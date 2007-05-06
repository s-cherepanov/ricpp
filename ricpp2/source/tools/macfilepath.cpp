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


/** @file macfilepath.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief MAC implementation of the file path classes
 */

#include "tools/filepath.h"

#include <sys/param.h>
#include <unistd.h>
#include <stdlib.h>

using namespace RiCPP;

/** @brief The '/' is the path seperator at MacOS
 */
char CFilepathConverter::nativePathSeperator() { return '/'; }

/** @brief The ':' seperates different pathes in MacOS
 */
char CFilepathConverter::nativePathlistSeperator() { return ':'; }

/** @brief No operation, the internal representation and the MAC representation are equal.
 */
std::string &CFilepathConverter::convertToInternal(std::string &var) {
	return var;
}

/** @brief No operation, the internal representation and the MAC representation are equal.
 */
std::string &CFilepathConverter::convertToNative(std::string &var) {
	return var;
}

/** @brief See description of CFilepath::convertToNative() in header file filepath.h.
 */
void CFilepath::convertToNative() {
	char pathbuf[PATH_MAX+1];

	if ( m_filepath.empty() ) {
		if ( getcwd(pathbuf, sizeof(pathbuf)) ) {
			pathbuf[sizeof(pathbuf)-1] = 0;
			m_filepath = pathbuf;
		}
	}

	m_nativepath = m_filepath;
	CFilepathConverter::convertToNative(m_nativepath);

	if ( realpath(m_nativepath.c_str(), pathbuf) ) {
		pathbuf[sizeof(pathbuf)-1] = 0;
		m_fullpath = pathbuf;
	}
}

/** @brief In MacOs a path is absolut if it has a path seperator '/' as  its first charakter.
 */
bool CFilepath::isAbsolute() const {
	return (m_nativepath.size() > 0 && m_nativepath[0] == '/');
}
