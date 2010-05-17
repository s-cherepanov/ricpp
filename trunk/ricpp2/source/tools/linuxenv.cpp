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

/** @file linuxenv.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Linux 10 implementation of parts of CEnv,
 *         the adapter for environment variables.
 */

#include "ricpp/tools/env.h"

#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif // _RICPP_TOOLS_INLINETOOLS_H

#include <sys/errno.h>
#include <sys/types.h>
#include <sys/param.h>

#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

using namespace RiCPP;

/** @brief Unix implementation to get the absolute path of the running executable.
 */
std::string &CEnv::getProgDir(std::string &prog, bool convertPath)
{
	static std::string path = "";
	static std::string internalPath = "";
	static bool isset = false;

	prog = "";

	if ( !isset ) {
		isset = true; // only try one time, path can be empty

		char *buf = new char[PATH_MAX+1];

		pid_t pid;
		char aString[128];
		ssize_t res;

		pid = getpid();

		snprintf(aString, sizeof(aString), "/proc/%d/exe", pid);

		res = readlink(aString, buf, PATH_MAX);
		if ( res == (ssize_t)-1 ) {
			// BSD
			snprintf(aString, sizeof(aString), "/proc/%d/file", pid);
			res = readlink(aString, buf, PATH_MAX);
			if ( res == (ssize_t)-1 ) {
				// Solaris
				snprintf(aString, sizeof(aString), "/proc/%d/path/a.out", pid);
				res = readlink(aString, buf, PATH_MAX);
			}
			if ( res == (ssize_t)-1 ) {
				// Error
				return prog;
			}
		}

		if ( res > PATH_MAX ) {
			delete buf;
			buf = new char[res+1];
			ssize_t newres = readlink(aString, buf, res);
			if ( newres == (ssize_t)-1 || newres > res ) {
				// error
				return prog;
			}
			res = newres;
		}
		buf[res]='\0';

		while ( res > 0 ) {
			--res;
			if ( buf[res] == '/' ) {
				buf[res] = 0;
				break;
			}
		}

		path = buf;
		internalPath = path;
		CFilepathConverter::convertToInternal(internalPath);

		delete buf;
	}

	prog = convertPath ? internalPath : path;

	// std::cerr << "************** prog" << prog << std::endl;
	return prog;
}

