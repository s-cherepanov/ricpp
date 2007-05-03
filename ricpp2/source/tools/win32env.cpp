// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 200,, 2005 Pixar
//                 All rights Reservered
//
// Copyright © of RiCPP 2007, Andreas Pidde
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
	get(tmp, "TEMP");
	if ( tmp.size() < 1 ) {
		get(tmp, "TMP");
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
		// No home, take the Userprofile (To do: Better "My Files")
		get(home, "USERPROFILE");
	}

	return CFilepathConverter::convertToInternal(home);
}

std::string &CEnv::getPath(std::string &path) {
	path = "";
	return CFilepathConverter::convertToInternal(get(path, "PATH"));
}

std::string &CEnv::getProgDir(std::string &prog) {
	// may be better: modulepath as singleton and dynamically allocated
	char modulepath[MAX_PATH];
	modulepath[0] = 0;
	DWORD fsize = GetModuleFileNameA(NULL, modulepath, sizeof(modulepath)-1);
	modulepath[sizeof(modulepath)-1] = (char)0;
	size_t len = strlen(modulepath);
	while ( len > 0 ) {
		--len;
		if ( modulepath[len] == '\\' ) {
			modulepath[len] = 0;
			break;
		}
	}

	prog = modulepath;
	return CFilepathConverter::convertToInternal(prog);
	// may be test if path really exist, because it can be truncated or
	// directly use unicode for native pathes but the later leads to trouble
	// because the ri (internal) strings are not unicode...
}
