// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 200,, 2005 Pixar
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

#include "tools/env.h"
#include "tools/filepath.h"

#include <mach-o/dyld.h>

#include <sys/errno.h>
#include <unistd.h>
#include <stdlib.h> //!< Included for getenv()

#include <iostream>

using namespace RiCPP;

static char *_ricpp_cutfilename(char *buf) {
	if ( !buf )
		return "";
		
	uint32_t len = strlen(buf);
	while ( len != 0 && buf[len-1] != '/' )
		--len;
	if ( len )
		buf[len-1] = 0;
	else
		buf[0] = 0; // was root path
		
	return buf;
}

std::string &CEnv::get(std::string &var, const char *varName) {
	var ="";
	if ( !varName )
		return var;

	const char *p = getenv(varName);
	return var = p ? p : "";
}

std::string &CEnv::getTmp(std::string &tmp) {
	tmp = "";
	if ( get(tmp, "TMP").empty() ) {
		// To Do : Test existence of $HOME/tmp

		// else
		get(tmp, "HOME");
	}
	return CFilepathConverter::convertToInternal(tmp);
}

std::string &CEnv::getHome(std::string &home) {
	home = "";
	return CFilepathConverter::convertToInternal(get(home, "HOME"));
}

std::string &CEnv::getPath(std::string &path) {
	path = "";
	get(path, "PATH");

	std::string::iterator i = path.begin();
	for ( ; i != path.end(); i++ ) {
		if ( (*i) == ':' )
			(*i) = ';';
	}
	return CFilepathConverter::convertToInternal(path);
}

std::string &CEnv::getProgDir(std::string &prog) {
	/* See Man page NSModule:
       extern int _NSGetExecutablePath(
            char *buf,
            unsigned long *bufsize);
		and realpath() (instead of readlink)
	*/
	
	static std::string path = "";
	static bool isset = false;

	if ( !isset ) {
		uint32_t buffsize = 0;
		char *buf = 0; 
		char symbuf[PATH_MAX+1] = { 0 };
		
		isset = true; // only try one time, path can be empty, if executable at root path
		
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
					_ricpp_cutfilename(symbuf);
					path = symbuf;
				} else {
					path = "";
				}
			}
		}
		CFilepathConverter::convertToInternal(path);
	}
	
	prog = path;
	return prog;
}
