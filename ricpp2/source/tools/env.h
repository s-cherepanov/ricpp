#ifndef _RICPP_TOOLS_ENV_H
#define _RICPP_TOOLS_ENV_H

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
#include <string>

namespace RiCPP {

	class CEnv {
		static void genpath(std::string &temp);
		static void get(std::string &temp, const char *varName);
	public:
		static void getTemp(std::string &temp);
		static void getHome(std::string &home);
		static void getPath(std::string &path);
		static void getProgDir(std::string &prog);
		static void find(std::string &temp, const char *varName);
	}; // CEnv
}; // namespace RiCPP

#endif // _RICPP_TOOLS_ENV_H
