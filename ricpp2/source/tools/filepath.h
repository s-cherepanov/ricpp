#ifndef _RICPP_TOOLS_FILEPATH_H
#define _RICPP_TOOLS_FILEPATH_H

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

/** @file filepath.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief OS independent handling of (still ANSI) file pathes
 */

#include <string>

namespace RiCPP {

	/** Helper class to convert file pathes to the internal representation and
	 *  back to the native representation. Contains static members only.
	 */
	class CFilepathConverter {
	public:
		/** The character to seperate parts of a directory, internally used. The /
		 * (like in Unix) is used because \ must be masked in strings.
		 * @return Internally used path seperator
		 */
		inline static char internalPathSeperator() {return '/';}

		/** The internal character to seperate multiple pathes (e.g. search path), the
		 * ; (like in Windows) is used, because : can be part of regular windows file names (e.g. C:\autoexec.bat)
		 * @return Internally used seperator for multiple pathes
		 */
		inline static char internalPathlistSeperator() {return ';';}

		/** The character to seperate parts of a native directory.
		 * @return Native path seperator
		 */
		static char nativePathSeperator();

		/** The character to seperate native directory in directory lists.
		 * @return Native pathlist seperator
		 */
		static char nativePathlistSeperator();

		/** Converts a native path (or list) into the internal representation
		 * @param var (input/output) with the path string, gets converted to internal representation
		 * @param return Reference to var
		 */
		static std::string &convertToInternal(std::string &var);

		/** Converts an internally used path (or list) into the native representation
		 * @param var (input/output) with the path string, gets converted to native representation
		 * @param return Reference to var
		 */
		static std::string &convertToNative(std::string &var);
	}; // CFilepathConverter


	/** Class to temporally store the name of a file path and convert from
	 *  internal representation to native.
	 *  Get the native and native expanded representation of an internal path
	 */
	class CFilepath {
		std::string m_filepath; //< Internal path
		std::string m_nativepath; //< Native path
		std::string m_fullpath; //< Native expanded path
		void convertToNative(); //< converts m_filepath to the native representations
	public:
		/** Init with the current working directory
		 */
		inline CFilepath():m_filepath("") { convertToNative(); }

		/** Init with an internal path representation
		 */
		inline CFilepath(const char *aFilepath):m_filepath(aFilepath ? aFilepath : "") { convertToNative(); }

		/** Returns the original internal representation of the filepath
		 *  @return Internal representation
		 */
		inline const char *filepath() const { return m_filepath.c_str(); }

		/** Returns the native representation of the filepath
		 *  @return Native representation
		 */
		inline const char *nativepath() const { return m_nativepath.c_str(); }

		/** Returns the expanded native representation of the filepath
		 *  @return Expanded native representation (full path)
		 */
		inline const char *fullpath() const { return m_fullpath.c_str(); }

		/** Returns true if the original path was absolute
		 *  @return Path was absolute
		 */
		bool isAbsolute() const;

		/** Only for convinience, returns true if the original path was
		 *  relative (not absolute)
		 *  @return Path was relative (not absolute)
		 */
		inline bool isRelative() const { return !isAbsolute(); }
	}; // CFilepath
}; // namespace RiCPP

#endif // _RICPP_TOOLS_FILEPATH_H
