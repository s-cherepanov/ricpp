#ifndef _RICPP_TOOLS_FILEPATH_H
#define _RICPP_TOOLS_FILEPATH_H

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

/** @file filepath.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief OS independent handling of (still ANSI) file pathes.
  * 
  * The class is implemented in machfilepath.cpp and win32filepath.cpp.
 */

#include <string>

namespace RiCPP {

	/** @brief Helper class to convert file pathes to the internal representation and
	 *  back to the native representation. Contains static members only.
	 */
	class CFilepathConverter {
	public:
		/** @brief The character to seperate parts of a directory, internally used.
		 *
		 * The '/' (like in Unix) is used internally because '\' must be masked in strings.
		 *
		 * @return Internally used path seperator
		 */
		inline static char internalPathSeperator() {return '/';}

		/** @brief The internal character to seperate multiple pathes (e.g. search path)
		 *
		 * ';' (like in Windows) is used, because ':' can be part of regular windows file names (e.g. C:\\autoexec.bat)
		 *
		 * @return Internally used seperator for multiple pathes
		 */
		inline static char internalPathlistSeperator() {return ';';}

		/** @brief The character to seperate parts of a native directory.
		 * @return Native path seperator.
		 */
		static char nativePathSeperator();

		/** @brief The character to seperate native directory in directory lists.
		 * @return Native pathlist seperator.
		 */
		static char nativePathlistSeperator();

		/** @brief Converts a native path (not a list) into the internal representation.
		 * @param var (input/output) with the path string, will be converted into internal representation.
		 * @return Reference to var.
		 */
		static std::string &convertToInternal(std::string &var);

		/** @brief Converts an internally used path (or list) into the native representation
		 * @param var (input/output) with the path string, will be converted into native representation.
		 * @return Reference to var.
		 */
		static std::string &convertToNative(std::string &var);
	}; // CFilepathConverter


	/** @brief Class to temporally store a file path and convert from internal
	 *  to native representation.
	 *
	 *  There are member functions to get the native and native
	 *  expanded representation of an internal path.
	 */
	class CFilepath {
		std::string m_filepath; //< Internal path.
		std::string m_nativepath; //< Native path.
		std::string m_fullpath; //< Native expanded path (also called full path, real path).
		void convertToNative(); //< converts /a m_filepath to the native representations /a m_nativepath and /a m_fullpath.
	public:
		/** @brief Init with the current working directory
		 */
		inline CFilepath()
			:m_filepath("")
		{
			convertToNative();
		}

		/** @brief Init with an internal path representation.
		 * @param aFilepath Internal representation of a filepath, you can use CFilepathConverter::convertToInternal()
		 * to convert a native into an internal representation.
		 */
		inline CFilepath(const char *aFilepath)
			: m_filepath(aFilepath ? aFilepath : "")
		{
			convertToNative();
		}

		/** @brief Gets the original internal representation of the filepath.
		 *  @return Internal representation
		 */
		inline const char *filepath() const { return m_filepath.c_str(); }

		/** @brief Gets the native representation of the path.
		 *  @return Native representation of the path.
		 */
		inline const char *nativepath() const { return m_nativepath.c_str(); }

		/** @brief Gets the expanded native representation of the path (real path or full path).
		 *  @return Expanded native representation of the path.
		 */
		inline const char *fullpath() const { return m_fullpath.c_str(); }

		/** @brief Checks if a path (the current object) is absolute.
		 *  @return Path is absolute.
		 */
		bool isAbsolute() const;

		/** @brief Only for convinience, Checks if a path (the current object) is relative (not absolute).
		 *  @return Path is relative (not absolute).
		 */
		inline bool isRelative() const { return !isAbsolute(); }
	}; // CFilepath
}; // namespace RiCPP

#endif // _RICPP_TOOLS_FILEPATH_H
