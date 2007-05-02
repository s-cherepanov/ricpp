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
#include <stdlib.h> //!< Included for getenv()

using namespace RiCPP;

void CEnv::genpath(std::string &temp) {
	std::string::iterator i = temp.begin();
	for ( ; i != temp.end(); i++ ) {
		if ( (*i) == ':' )
			(*i) = ';';
	}
}

void CEnv::get(std::string &var, const char *varName) {
	if ( !varName )
		return;

	const char *p = getenv(varName);
	var = p ? p : "";
	genpath(var);
}

void CEnv::getTmp(std::string &tmp) {
	tmp = "";
	get(tmp, "TMP");
	if ( tmp[tmp.size()-1] != '/' )
		tmp += "/";
}

void CEnv::getHome(std::string &home) {
	home = "";
	get(home, "HOME");
	if ( home[home.size()-1] != '/' )
		home += "/";
}

void CEnv::getPath(std::string &path) {
	path = "";
	get(path, "PATH");
}

void CEnv::getProgDir(std::string &prog) {
	prog = "";
}

void CEnv::find(std::string &var, const char *varName) {
	if ( !varName )
		return;

	if ( !strcasecmp(varName, "HOME") )
		return getHome(var);
	if ( !strcasecmp(varName, "TMP") )
		return getTmp(var);
	if ( !strcasecmp(varName, "PATH") )
		return getPath(var);
	if ( !strcasecmp(varName, "PROG") )
		return getProgDir(var);

	return get(var, varName);
}