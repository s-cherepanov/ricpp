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

#include "tools/env.h"
#include "tools/filepath.h"
#include <windows.h> //!< Needed GetModuleFileNameA()
#include <stdlib.h> //!< Included for getenv()

using namespace RiCPP;

std::string &CEnv::get(std::string &var, const char *varName) {
	var = "";
	if ( !varName )
		return var;

	char p[256];
	size_t requiredSize;
	size_t size;
	errno_t err;
	char *ptr = &p[0];

	getenv_s( &requiredSize, NULL, 0, varName);
	if ( requiredSize < 1 ) {
		return var;
	}

	if ( requiredSize >= sizeof(p) ) {
		ptr = new char[requiredSize+1];
	}

	ptr[0] = 0;
	err = getenv_s(&size, ptr, requiredSize, varName);
	ptr[requiredSize] = 0;

	var = ptr;

	if ( &p[0] != ptr )
		delete[] ptr;

	return var;
}

std::string &CEnv::getTmp(std::string &tmp) {
	tmp = "";
	get(tmp, "TMP"); // first look TMP - evtl. set by the program itself
	if ( tmp.size() < 1 ) {
		get(tmp, "TEMP");
	}

	if ( tmp.size() < 1 ) {
		// if there is not temp take the home as temp
		return getHome(tmp);
	}

	return CFilepathConverter::convertToInternal(tmp);
}

std::string &CEnv::getHome(std::string &home) {
	home = "";

	get(home, "HOMEDRIVE");
	if ( home.size() > 0 ) {
		std::string homepath = "";
		get(homepath, "HOMEPATH");
		home += homepath;
	}

	if ( home.size() < 1 ) {
		// No home, take the Userprofile (To do: "My Files" ?)
		get(home, "USERPROFILE");
	}

	return CFilepathConverter::convertToInternal(home);
}

std::string &CEnv::getPath(std::string &path) {
	path = "";
	return CFilepathConverter::convertToInternal(get(path, "PATH"));
}

std::string &CEnv::getProgDir(std::string &prog) {
	static std::string path = "";
	static bool isset = false;
	
	if ( !isset ) {
		isset = true;
		
		// In Win32 docs it is said, that the terminating NUL is counted for MAX_PATH
		char modulepath[MAX_PATH] = { 0 };
		char pathbuf[MAX_PATH] = { 0 };
		
		DWORD fsize = GetModuleFileNameA(NULL, modulepath, sizeof(modulepath)-1);
		modulepath[sizeof(modulepath)-1] = (char)0;

		char *ptr = modulepath;

		/* Will GetModuleFileNameA resolve hard lins created with mklink?
		 */
		if ( GetFullPathNameA(modulepath, sizeof(pathbuf), pathbuf, &ptr) != 0 ) {
			pathbuf[sizeof(pathbuf)-1] = 0;
			ptr = pathbuf;
		}

		
		size_t len = strlen(ptr);
		while ( len > 0 ) {
			--len;
			if ( ptr[len] == '\\' ) {
				ptr[len] = 0;
				break;
			}
		}

		path = ptr;
		CFilepathConverter::convertToInternal(path);
	}
	
	prog = path;
	return prog;
}
