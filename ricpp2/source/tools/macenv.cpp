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

/** @file macenv.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief MacOs 10 implementation of parts of CEnv,
 *         the adapter for environment variables.
 */

#include "ricpp/tools/env.h"

#ifndef _RICPP_TOOLS_FILEPATH_H
#include "ricpp/tools/filepath.h"
#endif // _RICPP_TOOLS_FILEPATH_H

#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif // _RICPP_TOOLS_INLINETOOLS_H

#include <mach-o/dyld.h>

#include <sys/errno.h>
#include <sys/types.h>

#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>

using namespace RiCPP;

/** @brief Mac implementation to get an environment variable.
 *
 * Uses the standard function getenv() to acces the environment variables,
 * can return an empty string.
 */
std::string &CEnv::get(std::string &var, const char *varName)
{
	var.empty();
	if ( !varName )
		return var;

	const char *p = getenv(varName);
	return var = p ? p : "";
}


/** @brief Mac implementation to get the directory for temporary data.
 *
 * Get the value of the variable TMP if set, otherwise $home/tmp 
 * or if this also do not exist, returns $home. If there is no
 * home directory the empty string is returned.
 */
std::string &CEnv::getTmp(std::string &tmp)
{
	tmp = "";
	if ( get(tmp, "TMP").empty() ) {
		get(tmp, "HOME");
		
		std::string path = tmp;
		path += "/tmp";
		
		DIR *d = opendir(path.c_str());
		if ( d ) {
			tmp = path;
			closedir(d);
		} 
	}
	// return CFilepathConverter::convertToInternal(tmp);
	return tmp;
}

/** @brief Mac implementation to get the user's home directory.
 *
 * Examines the variable HOME.
 */
std::string &CEnv::getHome(std::string &home)
{
	home = "";
	get(home, "HOME");
	// return CFilepathConverter::convertToInternal(home);
	return home;
}

/** @brief Mac implementation to get the executable search path list.
 *
 * Examines the variable PATH.
 */
std::string &CEnv::getPath(std::string &path)
{
	path = "";
	get(path, "PATH");

	// return CFilepathConverter::convertToInternal(path);
	return path;
}

/** @brief Mac implementation to get the absolute path of the running executable.
 *
 * Use the system function _NSGetExecutablePath() and realpath() to replace
 * symlinks. The filename itself is deleted. Can return
 * an empty string. Uses a singleton to store the path.
 */
std::string &CEnv::getProgDir(std::string &prog)
{
	/* See Man page NSModule:
       extern int _NSGetExecutablePath(
            char *buf,
            unsigned long *bufsize);
		and realpath() (used instead of readlink())
	*/
	
	static std::string path = "";
	static bool isset = false;

	if ( !isset ) {
		isset = true; // only try one time, path can be empty

		uint32_t buffsize = 0;
		char *buf = 0; 
		char symbuf[PATH_MAX+1] = { 0 };
			
		_NSGetExecutablePath(0, &buffsize);
		
		if ( buffsize > 0 ) {
			uint32_t realbuffsize = buffsize+1;
			buf = new char[realbuffsize];
			if ( buf ) {
				buf[0] = 0;
				_NSGetExecutablePath(buf, &buffsize);
				buf[realbuffsize-1] = 0;
				symbuf[0] = 0;
				if ( realpath(buf, symbuf) ) {
					symbuf[sizeof(symbuf)-1] = 0;
					cutfilename(symbuf);
					path = symbuf;
				} else {
					path = "";
				}
				delete buf;
			}
		}
		// CFilepathConverter::convertToInternal(path);
	}
	
	prog = path;
	return prog;
}
