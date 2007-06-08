#ifndef _RICPP_TOOLS_ENV_H
#define _RICPP_TOOLS_ENV_H

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

/** @file env.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Adapter for environment variables.
 *
 *  In this file the class CEnv is declared to access environment variables.
 *  Since the implementation differs from OS to OS, parts of the object
 *  are implemented in different files: see win32env.cpp for windows and
 *  see macenv.cpp for MacOS 10.
 */

#ifndef _RICPP_TOOLS_PLATFORM_H
#include "ricpp/tools/platform.h"
#endif // _RICPP_TOOLS_PLATFORM_H

#include <string>

namespace RiCPP {

	/** @brief Class to access the environment variables.
	 *
	 *  There are different implementations for different OSes.
	 *  If variable names are used in strings of RiCPP, e.g. for name in
	 *  IRi::begin(RtString name), they are started with a $ sign,
	 *  like the four special vars $TMP, $HOME, $PATH, $PROGDIR.
	 *  Implementation can be found in win32env.cpp for Windows and macenv.cpp for MacOs.
	 *  The values variables are not converted as file pathes, since it cannot be
	 *  expected in general (i.e. get()) that a variable is a filename. Conversion
	 *  is done e.g. in CStringList
	 */
	class CEnv {
		/** @brief Find the value of a variable
		 *
		 *         Accesses the variable with the name \a varName
		 *         and stores the value found in \a var. If the variable
		 *         is not found, \a var is cleared.
		 *  @retval var String reference where the value of the environment variable
		 *         \a varName is stored
		 *  @param varName Pointer to the name of the environment variable
		 *         (without the leading $ sign)
		 *  @return A reference to \a var
		 */
		static std::string &get(std::string &var, const char *varName);

	public:
		/** @brief Gets the string "TMP", the name of the TMP-variable (see getTmp()).
		 *  @return "TMP"
		 */
		inline static char *tmpName() {return "TMP";}

		/** @brief Gets the string "HOME", the name of the HOME-variable (see getHome()).
		 *  @return "HOME"
		 */
		inline static char *homeName() {return "HOME";}

		/** @brief Gets the string "PATH", the name of the PATH-variable (see getPath()).
		 *  @return "PATH"
		 */
		inline static char *pathName() {return "PATH";}

		/** @brief Gets the string "PROGDIR", the name of the PROGDIR-variable (see getProgDir()).
		 *  @return "PROGDIR"
		 */
		inline static char *progDirName() {return "PROGDIR";}

		/** @brief Gets the value of the special variable TMP (so called in RiCPP).
		 *
		 *  A path to a directory, where data can be stored temporarily.
		 *  The native name of this variable can be different.
		 *
		 *  @param tmp String reference where the value of the special environment variable TMP is stored.
		 *  @return A reference to \a tmp.
		 */
		static std::string &getTmp(std::string &tmp);

		/** @brief Gets the value of the special variable HOME (so called in RiCPP).
		 *
		 *  A path to the
		 *  home directory of the current user.
		 *  The native name of this variable can be different.
		 *
		 *  @param home String reference where the value of the special environment variable
		 *         HOME is stored.
		 *  @return A reference to \a home.
		 */
		static std::string &getHome(std::string &home);

		/** @brief Gets the value of the special variable PATH (so called in RiCPP).
		 *
		 *  A pathlist with the search pathes for executable programs.
		 *  The native name of this variable can be different.
		 *
		 *  @param path String reference where the value of the special environment
		 *         variable PATH is stored.
		 *  @return A reference to \a path.
		 */
		static std::string &getPath(std::string &path);

		/** @brief Gets the value of the special variable PROGDIR (so called in RiCPP).
		 *
		 *  The absolute path to the directory of the running program.
		 *  The native name of this variable can be different (is not existent).
		 *
		 *  @param prog String reference where the value of the special environment variable
		 *         PROG is stored
		 *  @return A reference to \a prog.
		 */
		static std::string &getProgDir(std::string &prog);

		/** @brief Finds the value for the environment variable.
		 *
		 *  Returns the value of the environment variable with the name \a varName or
		 *  the special values for the variables TMP, HOME, PATH, PROGDIR in \a var
		 *  (these four names are not case sensitive, that's maybe not the case for
		 *  other variables or different for different OSes).
		 *  If no variable \a varName is found, var is cleared.
		 *
		 *  @retVal var Value of the environment variable is stored here
		 *  @param varName Pointer to the name of the environment variable
		 *         (without the leading $ sign).
		 *  @return A reference to \a var, the value of the variable.
		 */
		inline static std::string &find(std::string &var, const char *varName)
		{
			// Clear the variable
			var = "";

			// No variable name
			if ( !varName )
				return var;

			// Special variables HOME, TMP, PATH, PROGDIR
			if ( !strcasecmp(varName, homeName()) )
				return getHome(var);
			if ( !strcasecmp(varName, tmpName()) )
				return getTmp(var);
			if ( !strcasecmp(varName, pathName()) )
				return getPath(var);
			if ( !strcasecmp(varName, progDirName()) )
				return getProgDir(var);

			// Get an environment variable
			return get(var, varName);
		}
	}; // CEnv
}; // namespace RiCPP

#endif // _RICPP_TOOLS_ENV_H
