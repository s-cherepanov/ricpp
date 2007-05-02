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
#include <windows.h> //!< Needed GetModulePath()
#include <stdlib.h> //!< Included for getenv()

using namespace RiCPP;

void CEnv::genpath(std::string &temp) {
	std::string::iterator i = temp.begin();
	for ( ; i != temp.end(); i++ ) {
		if ( (*i) == '\\' )
			(*i) = '/';
	}
}

void CEnv::get(std::string &var, const char *varName) {
	if ( !varName )
		return;

	char p[256];
	size_t requiredSize;
	size_t size;
	errno_t err;
	char *ptr = &p[0];

	getenv_s( &requiredSize, NULL, 0, varName);
	if ( requiredSize < 1 ) {
		var = "";
		return;
	}

	if ( requiredSize >= sizeof(p)-1 ) {
		ptr = new char[requiredSize+1];
	}

	ptr[0] = 0;
	err = getenv_s(&size, ptr, requiredSize, varName);
	ptr[requiredSize-1] = 0;

	var = ptr;
	genpath(var);

	if ( &p[0] != ptr )
		delete[] ptr;
}

void CEnv::getTmp(std::string &tmp) {
	tmp = "";
	get(tmp, "TEMP");
	if ( tmp.size() < 1 ) {
		get(tmp, "TMP");
	}
	if ( tmp.size() < 1 ) {
		get(tmp, "USERPROFILE");
	}
	if ( tmp[tmp.size()-1] != '/' )
		tmp += "/";
}

void CEnv::getHome(std::string &home) {
	home = "";

	get(home, "HOMEDRIVE");
	if ( home.size() > 0 ) {
		std::string homepath = "";
		get(homepath, "HOMEPATH");
		home += homepath;
	}

	if ( home.size() < 1 ) {
		get(home, "USERPROFILE");
	}
	if ( home[home.size()-1] != '/' )
		home += "/";
}

void CEnv::getPath(std::string &path) {
	path = "";
	get(path, "PATH");
}

void CEnv::getProgDir(std::string &prog) {
	char modulepath[MAX_PATH];
	modulepath[0] = (char)0;
	DWORD fsize = GetModuleFileNameA(NULL, modulepath, sizeof(modulepath));
	modulepath[sizeof(modulepath)-1] = (char)0;
	size_t len = strlen(modulepath);
	while ( len > 0 ) {
		--len;
		if ( modulepath[len] == '\\' ) {
			modulepath[len+1] = 0;
			break;
		}
	}

	prog = modulepath;
	genpath(prog);
	if ( prog[prog.size()-1] != '/' )
		prog += "/";
}

void CEnv::find(std::string &var, const char *varName) {
	if ( !varName )
		return;

	if ( !_stricmp(varName, "HOME") )
		return getHome(var);
	if ( !_stricmp(varName, "TMP") )
		return getTmp(var);
	if ( !_stricmp(varName, "PATH") )
		return getPath(var);
	if ( !_stricmp(varName, "PROG") )
		return getProgDir(var);

	return get(var, varName);
}