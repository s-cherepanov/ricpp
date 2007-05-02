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
#include <stdlib.h> //!< Included for getenv()

using namespace RiCPP;

std::string &CEnv::get(std::string &var, const char *varName) {
	var ="";
	if ( !varName )
		return var;

	const char *p = getenv(varName);
	return var = p ? p : "";
}

void CEnv::getTmp(std::string &tmp) {
	tmp = "";
	if ( get(tmp, "TMP").empty() )
		get(tmp, "HOME");
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
	prog = "";
	// ToDo find prog
	return CFilepathConverter::convertToInternal(prog);
}
