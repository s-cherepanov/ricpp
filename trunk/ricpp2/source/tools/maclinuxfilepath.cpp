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
 *  @brief MAC and Linux implementation of the file path classes
 */

#include "ricpp/tools/filepath.h"

#ifndef _RICPP_TOOLS_STRINGPATTERN_H
#include "ricpp/tools/stringpattern.h"
#endif // _RICPP_TOOLS_STRINGPATTERN_H

#include <cstdlib>
#include <cerrno>
#include <iostream>

#include <sys/param.h>
#include <unistd.h>

#include <dirent.h>

using namespace RiCPP;

/** @brief The '/' is the path separator at MacOS
 */
char CFilepathConverter::nativePathSeparator() { return '/'; }

/** @brief The ':' separates different pathes in MacOS
 */
char CFilepathConverter::nativePathlistSeparator() { return ':'; }

/** @brief Dynamic libraries have the prefix "lib" in mac systems
 */
const char *CFilepathConverter::nativeDynlibPrefix() { return "lib"; }

/** @brief No operation, the internal representation and the MAC representation are equal.
 */
std::string &CFilepathConverter::convertToInternal(std::string &var) {
	return var;
}

/** @brief No operation, the internal representation and the MAC representation are equal.
 */
std::string &CFilepathConverter::convertListToInternal(std::string &var, char internalListSeparator) {
	return var;
}

/** @brief No operation, the internal representation and the MAC representation are equal.
 */
std::string &CFilepathConverter::convertToNative(std::string &var) {
	return var;
}

std::string &CFilepathConverter::convertListToNative(std::string &var, char internalListSeparator) {
	return var;
}

/** @brief See description of CFilepath::convertToNative() in header file filepath.h.
 */
void CFilepath::convertToNative() {
	char pathbuf[PATH_MAX + 1];
	pathbuf[0] = 0;

	// std::cerr << "convertToNative - m_filepath:" << m_filepath<< std::endl;
	if ( m_filepath.empty() ) {
		if ( getcwd(pathbuf, sizeof(pathbuf)) ) {
			pathbuf[sizeof(pathbuf)-1] = 0;
			m_filepath = pathbuf;
			CFilepathConverter::convertToInternal(m_filepath);
		}
	}

	m_nativepath = m_filepath;
	CFilepathConverter::convertToNative(m_nativepath);
	m_fullpath = m_nativepath;

	if ( realpath(m_nativepath.c_str(), pathbuf) ) {
		pathbuf[sizeof(pathbuf)-1] = 0;
		m_fullpath = pathbuf;
	}
	// std::cerr << "***** convertToNative - m_nativepath:" << m_nativepath << " m_fullpath " << m_fullpath << " PATH_MAX  " << PATH_MAX << std::endl;
}

/** @brief In MacOs a path is absolut if it has a path separator '/' as  its first character.
 */
bool CFilepath::isAbsolute() const {
	return (m_nativepath.size() > 0 && m_nativepath[0] == '/');
}

/* Replaced by CStringPattern
static bool filenamecmp(const char *direntry, const char *pattern)
{
	int ret;
	const char *patpos = pattern ? strchr(pattern, '*') : 0;
	direntry = direntry ? direntry : "";

	if ( patpos ) {
		ret = strncmp(direntry, pattern, (int)(patpos - pattern));
		if ( ret != 0 ) {
			return false;
		}
		++patpos;
		int len = strlen(direntry)-strlen(patpos);
		if ( len > 0 ) {
			return !strcmp(direntry+len, patpos);
		}
	} else if (!strcmp(direntry, pattern)) {
		return true;
	}

	return false;
}
*/

bool CDirectory::readDirectory(const char *pattern) {
	struct dirent *dir_entry_p;
	DIR *dir_p;

	m_dirList.clear();
	std::string direntry(m_directory.fullpath());
	direntry += "/";

	if ( !(pattern && *pattern) ) {
		pattern = "*";
	}

	CStringPattern strPat(pattern);

	dir_p = opendir(direntry.c_str());
	if ( dir_p == 0 )  {
		return false;
	}
	
	while ( (dir_entry_p = readdir(dir_p)) != 0 ) {
		direntry = dir_entry_p->d_name ? dir_entry_p->d_name : "";
		if ( direntry != "." && direntry != ".." && strPat.matches(direntry.c_str()) ) {
			m_dirList.push_back(CFilepath(direntry));
		}
	}

	closedir(dir_p);
	return errno == 0;
}
