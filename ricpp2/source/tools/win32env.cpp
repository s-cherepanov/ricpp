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

/** @file win32env.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Win32 implementation of parts of CEnv, the adapter for environment variables.
 */

#if defined _WIN32
#include "ricpp/tools/env.h"

#include <windows.h>
#include <cstdlib>

using namespace RiCPP;

/** @brief Win32 implementation to get an environment variable.
 *
 * Uses the Win32 function getenv_s() to access the environment variables,
 * can return an empty string. Gnu-C does not declare getenv_s().
 */
std::string &CEnv::get(std::string &var, const char *varName, bool convertPath)
{
	var = "";
	if ( !varName )
		return var;

#if defined _MSC_VER
	{
	char p[MAX_PATH+1] = { 0 };
	char *ptr = &p[0];

	size_t requiredSize = 0;
	size_t size = 0;

	errno_t err = 0;

	getenv_s( &requiredSize, NULL, 0, varName);

	if ( !requiredSize ) {
		// varName not found
		return var;
	}

	if ( requiredSize >= sizeof(p) ) {
		ptr = new char[requiredSize+1];
	}

	ptr[0] = 0;
	err = getenv_s(&size, ptr, requiredSize, varName);
	ptr[requiredSize] = 0;

	if ( !err )
		var = ptr;

	if ( &p[0] != ptr )
		delete[] ptr;
	}
#else
	{
	const char *p = getenv(varName);
	var = p ? p : "";
	}
#endif

	if ( convertPath )
		return CFilepathConverter::convertToInternal(var);

	return var;
}

/** @brief Win32 implementation to get the path of a temporary directory.
 *
 *  First TMP is considered (because TMP is the variable for RiCPP), than
 *  TEMP. If nothing is set the home directory @see getHome() is returned.
 *  The path can be empty.
 */
std::string &CEnv::getTmp(std::string &tmp, bool convertPath)
{
	tmp = "";
	get(tmp, "TMP", convertPath); // first look TMP - can be set set by the program itself
	if ( tmp.size() < 1 ) {
		get(tmp, "TEMP", convertPath);
	}

	if ( tmp.size() < 1 ) {
		// if there is not temp take the home as temp
		return getHome(tmp, convertPath);
	}

	return tmp;
}


/** @brief Win32 implementation to get the home path of the user.
 *
 *  IF HOME is not set, the user variables HOMEDRIVE and HOMEPATH
 *  (Windows home path) are examined, if they are not set the profile path
 *  will be used if existent. The path can be empty.
 */
std::string &CEnv::getHome(std::string &home, bool convertPath)
{
	home = "";

	get(home, "HOME", convertPath);
	if ( home.size() > 0 ) {
		return home;
	}

	get(home, "HOMEDRIVE", false);
	if ( home.size() > 0 ) {
		std::string homepath = "";
		get(homepath, "HOMEPATH", false);
		home += homepath;
	}

	if ( home.size() < 2 ) {
		// No home, take the Userprofile (To do: "My Files" ?)
		get(home, "USERPROFILE", convertPath);
		return home;
	}

	return convertPath ? CFilepathConverter::convertToInternal(home) : home;
}

/** @brief Win32 implementation to get the search path.
 *
 *  The variable PATH is examined. The search path can be empty.
 */
std::string &CEnv::getPath(std::string &path, bool convertPath)
{
	path = "";
	get(path, "PATH", false);

	return convertPath ? CFilepathConverter::convertListToInternal(path) : path;
}

/** @brief Win32 implementation to get the absolute path to the running executable.
 *
 *  Uses GetModuleFileNameA(NULL, ...) to get the path of the running executable,
 *  without the filename of the executable (only called once).
 *  Uses still ANSI and cannot resolve junction points (file links). Can return
 *  an empty string. Uses a singleton to store the path.
 */
std::string &CEnv::getProgDir(std::string &prog, bool convertPath)
{
	static std::string path = "";
	static std::string internalPath = "";
	static bool isset = false;
	
	if ( !isset ) {
		isset = true;
		
		// In Win32 docs it is said, that the terminating NUL is counted for MAX_PATH
		char modulepath[MAX_PATH] = { 0 };
		
		DWORD fsize = GetModuleFileNameA(NULL,
			modulepath, sizeof(modulepath)-1);
		modulepath[sizeof(modulepath)-1] = (char)0;

		char *ptr = modulepath;

		// avoid unused variable warning
		fsize = fsize+0;

		/* GetModuleFileNameA and GetFullPathNameA will not resolve links
		 * (junction points) created with mklink.
		 */
		/*
		char pathbuf[MAX_PATH] = { 0 };
		if ( GetFullPathNameA(modulepath,
			sizeof(pathbuf), pathbuf, &ptr) != 0
		) {
			pathbuf[sizeof(pathbuf)-1] = 0;
			ptr = pathbuf;
		}
		*/
		
		size_t len = strlen(ptr);
		while ( len > 0 ) {
			--len;
			if ( ptr[len] == '\\' ) {
				ptr[len] = 0;
				break;
			}
		}

		path = ptr;
		internalPath = path;
		CFilepathConverter::convertToInternal(internalPath);
	}
	
	prog = convertPath ? internalPath : path;
	return prog;
}
#endif // _WIN32
