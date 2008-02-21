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
  * The class is implemented in macfilepath.cpp and win32filepath.cpp.
 */

#include <string>
#include <list>

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
		 * In rib ':' is used as seperator. That's a bit unlucky, because ':' is used to
		 * seperate drive letters from the rest of the path. However CStringList::explode()
		 * tries to cope with this double meaning of ':'. I've considered to
		 * use ';' as path seperator, but this would be incompatible to others renderers
		 * and the RIB files.
		 *
		 * @return Internally used seperator for multiple pathes
		 * @see CStringList::explode()
		 */
		inline static char internalPathlistSeperator() {return ':';}

		/** @brief The character to seperate parts of a native directory.
		 * @return Native path seperator.
		 */
		static char nativePathSeperator();

		/** @brief The native prefix for dynamic libraries.
		 * @return Native prefix for dynamic libraries.
		 */
		static const char *nativeDynlibPrefix();

		/** @brief The native suffix for dynamic libraries.
		 * @return Native suffix for dynamic libraries.
		 */
		static const char *nativeDynlibSuffix();

		/** @brief The character to seperate native directory in directory lists.
		 * @return Native pathlist seperator.
		 */
		static char nativePathlistSeperator();

		/** @brief Converts a native path (not a list) into the internal URI compatible representation.
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
		std::string m_filepath; ///< Internal path.
		std::string m_nativepath; ///< Native path.
		std::string m_fullpath; ///< Native expanded path (also called full path, real path).
		void convertToNative(); ///< converts /a m_filepath to the native representations /a m_nativepath and /a m_fullpath.
	public:
		/** @brief Init with an internal path representation.
		 * @param aFilepath Internal representation of a filepath, you can use
		 *        CFilepathConverter::convertToInternal()
		 *        to convert a native into an internal representation.
		 *        If the path is empty the current working directory is used as path
		 */
		inline CFilepath(const char *aFilepath = 0)
			: m_filepath(aFilepath ? aFilepath : "")
		{
			convertToNative();
		}

		/** @brief Copy constructor
		 *  @param aFilepath Path to copy
		 */
		inline CFilepath(const std::string &aFilepath)
			: m_filepath(aFilepath)
		{
			convertToNative();
		}

		/** @brief Gets the original internal representation of the filepath.
		 *  @return Internal representation
		 */
		inline const char *filepath() const { return m_filepath.c_str(); }

		/** @brief Gets the basename of the path
		 *  @param suffix Suffix to cut away
		 *	@retval aBasename Basename is stored here
		 *  @return C-String pointer of @a aBasename
		 */
		inline const char *basename(const char *suffix, std::string &aBasename) const
		{
			const char *path = filepath();
			const char *left = strrchr(path, CFilepathConverter::internalPathSeperator());
			aBasename = "";
			if ( !(left && *left) ) {
				left = path;
			} else {
				++left;
			}

			if ( suffix && strlen(left) >= strlen(suffix) ) {
				const char *right = left+(strlen(left)-strlen(suffix));
				if ( !strcmp(right, suffix) ) {
					aBasename.assign(left, right);
					return aBasename.c_str();
				}
			}

			aBasename = left;
			return aBasename.c_str();
		}

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


	/** @brief Stores the contents of a directory.
	 */
	class CDirectory {
	public:
		/** @brief Const iterator for the strings.
		 */
		typedef std::list<CFilepath>::const_iterator const_iterator;
		/** @brief Size type for the number of stored strings: CStringList::size()
		 */
		typedef std::list<CFilepath>::size_type size_type;

	private:
		std::list<CFilepath> m_dirList; ///< Content of the directory
		CFilepath m_directory;          ///< Path of the directory
		
		/** @brief Reads a directory by using filepattern @a pattern
		 *  @param pattern Simple, glob style pattern.
		 *  @return true, directory could be read.
		 *  @todo Use glob style patterns of stringpattern.h
		 */
		bool readDirectory(const char *pattern = 0);

	public:
		/** @brief Init with an internal path representation.
		 * @param pattern Simple search pattern (like abc*.txt).
		 * @param aFilepath Internal representation of a filepath, you can use
		 *        CFilepathConverter::convertToInternal()
		 *        to convert a native into an internal representation.
		 *        If the path is empty the current working directory is used as path
		 */
		inline CDirectory(const char *pattern = 0, const char *aFilepath = 0)
			: m_directory(aFilepath)
		{
			readDirectory(pattern);
		}

		/** @brief Init with an internal path representation.
		 * @param pattern Simple search pattern (like abc*.txt).
		 * @param aFilepath Internal representation of a filepath, you can use
		 *        CFilepathConverter::convertToInternal()
		 *        to convert a native into an internal representation.
		 *        If the path is empty the current working directory is used as path
		 */
		inline CDirectory(const char *pattern, const std::string &aFilepath)
			: m_directory(aFilepath)
		{
			readDirectory(pattern);
		}

		/** @brief Constant iterator to access the strings (beginning).
		 *  @return Iterator with the first inserted string as current element.
		 */
		inline const_iterator begin() const
		{
			return m_dirList.begin();
		}

		/** @brief Constant iterator to access the strings (behind the last element).
		 *  @return Iterator to query the end of the iteration
		 *          (like sthe std iterators does not refer a valid element).
		 */
		inline const_iterator end() const
		{
			return m_dirList.end();
		}

		/** @brief Gets the size of the string list
		 * @return The number of stored strings.
		 */
		inline size_type size() const
		{
			return m_dirList.size();
		}

	};
} // namespace RiCPP

#endif // _RICPP_TOOLS_FILEPATH_H
