#ifndef _RICPP_TOOLS_ENV_H
#define _RICPP_TOOLS_ENV_H

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

/** @file env.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Adapter for environment variables
 */

#ifndef _RICPP_TOOLS_PLATFORM_H
#include "tools/platform.h"
#endif // _RICPP_TOOLS_PLATFORM_H

#include <string>

namespace RiCPP {

	/** @brief Access of the environment variables. There are
	 *  different implementations for different OSes. If variable names are used in
	 *  strings of RiPP, e.g. in name in IRi::begin(RtString name), they are
	 *  started with a $ sign, like the four special vars $TMP, $HOME, $PATH, $PROGDIR.
	 */
	class CEnv {
		/** @brief Accesses the variable with the name varName
		 *  and stores the found value in temp. If the variable
		 *  is not found, temp is cleared
		 *  @param var String reference where the value of the environment variable (varName) is stored
		 *  @param varName pointer to the name of the environment variable (without the $ sign)
		 *  @return A reference to var
		 */
		static std::string &get(std::string &var, const char *varName);

	public:
		/** @brief Gets the string "TMP", the name of the TMP-variable (see getTmp)
		 *  @return "TMP"
		 */
		inline static char *tmpName() {return "TMP";}

		/** @brief Gets the string "HOME", the name of the HOME-variable (see getHome)
		 *  @return "HOME"
		 */
		inline static char *homeName() {return "HOME";}

		/** @brief Gets the string "PATH", the name of the PATH-variable (see getPath)
		 *  @return "PATH"
		 */
		inline static char *pathName() {return "PATH";}

		/** @brief Gets the string "PROGDIR", the name of the PROGDIR-variable (see getProgDir)
		 *  @return "PROGDIR"
		 */
		inline static char *progDirName() {return "PROGDIR";}

		/** @brief Gets the value of the special variable TMP (so named in RiCPP), a path to a directory
		 *  where data can be stored temporarily. The path is encoded using the internal RiCPP format.
		 *  The native name of this variable can be different.
		 *  @param tmp String reference where the value of the special environment variable TMP is stored
		 *  @return A reference to tmp
		 */
		static std::string &getTmp(std::string &tmp);

		/** @brief Gets the value of the special variable HOME (so named in RiCPP), a path to the
		 *  home directory of the current user. The path is encoded using the internal RiCPP format.
		 *  The native name of this variable can be different.
		 *  @param home String reference where the value of the special environment variable HOME is stored
		 *  @return A reference to home
		 */
		static std::string &getHome(std::string &home);

		/** @brief Gets the value of the special variable PATH (so named in RiCPP), a pathlist with
		 *  search pathes for executable programs. The searchpath is encoded using the internal RiCPP format.
		 *  The native name of this variable can be different.
		 *  @param path String reference where the value of the special environment variable HOME is stored
		 *  @return A reference to path
		 */
		static std::string &getPath(std::string &path);

		/** @brief Gets the value of the special variable PROGDIR (so named in RiCPP), the absolute path
		 *  to the directory of the running program. The path is encoded using the internal RiCPP format.
		 *  The native name of this variable can be different.
		 *  @param path String reference where the value of the special environment variable PROG is stored
		 *  @return A reference to prog
		 */
		static std::string &getProgDir(std::string &prog);

		/** @brief Finds the value for the environment variable with the name (varName), returns
		 *  the special values for TMP, HOME, PATH, PROGDIR (these four names are not case sensitive,
		 *  thats maybe not the case for other variables or different on different OSes)
		 *  @param varName pointer to the name of the environment variable (without the $ sign)
		 *  @return A reference to var
		 */
		inline static std::string &find(std::string &var, const char *varName) {
			var = "";
			if ( !varName )
				return var;

			if ( !strcasecmp(varName, homeName()) )
				return getHome(var);
			if ( !strcasecmp(varName, tmpName()) )
				return getTmp(var);
			if ( !strcasecmp(varName, pathName()) )
				return getPath(var);
			if ( !strcasecmp(varName, progDirName()) )
				return getProgDir(var);

			return get(var, varName);
		}
	}; // CEnv
}; // namespace RiCPP

#endif // _RICPP_TOOLS_ENV_H
