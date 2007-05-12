#ifndef _RICPP_GENDYNLIB_DYNLIB_H
#define _RICPP_GENDYNLIB_DYNLIB_H

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

/** @file dynlib.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declaration of a class to handle dynamic libraries in general.
 */

#ifndef _RICPP_TOOLS_STRINGLIST_H
#include "ricpp/tools/stringlist.h"
#endif // _RICPP_TOOLS_STRINGLIST_H

namespace RiCPP {

	/** Base class to represent the exported functions of libraries.
	 *  There are child classes containing the system depending parts
	 *  of the handling of these functions: CMacLibFunc, CWin32LibFunc.
	 *  These classes contain pointers to the functions. CDynLib
	 *  handles the creation of CLibFunc instances. Used for
	 *  example by CRendererLoader::CRendererLib to represent a
	 *  library of renderer creators (used to generate concrete renderer contexts).
	 */
	class CLibFunc {
	public:
		/** Virtual destructor
		 */
		inline virtual ~CLibFunc() { }
		/** Overload this member functions to test if the function is valid.
		 */
		virtual bool valid()=0;
	}; // CLibFunc

	/** Base class to represent dynamic libraries in general. There are child classes
	 *  to handle the system dependend parts (CMacDynLib and CWin32DynLib)
	 */
	class CDynLib {
		/** Counts how often a library is loaded, load() and unload() handles the
		 *  incrementation and decrementation of this member. load() only loads
		 *  the library for the first time called, and only if unload() encounters
		 *  that m_useCount is 0, it unloads the library by calling the
		 *  appropriate system function.
		 */
		unsigned long m_useCount;

	protected:
		/** The major version of the library that should be loaded.
		 *  Libraries bear the major version in their name libname.version.suffix
		 */
		long int m_version;
		
		/** The name of the library as given by the framework, without suffixes
		 */
		std::string m_libname;
		
		/** The full directory path of the library, inserted by findLib()
		 */
		std::string m_libpath;

		/** The search path for the library, if empty, the system dependent
		 *  searchpath is used.
		 */
		CStringList m_searchpath;

		/** Overload this member function with a method to find the
		 *  library m_libname of version m_version in the m_searchpath.
		 *  If the search path is empty the library shall be searched in a system
		 *  dependent manner. The function puts the full directory path
		 *  (with the filename) where the library was found in m_libpath
		 *  and returns a pointer to the C-String, should return 0
		 *  if library was not found.
		 */
		virtual const char *findLib() = 0;
		
		/** Overload to load the library (called by load() if m_useCount increments to 1)
		 */
		virtual bool doLoad() = 0;

		/** Overload to unload the library (called by unload() if m_useCount
		 *  decrements to 0 or by the destructor of the child class)
		 */
		virtual bool doUnload() = 0;

		/** @return Used by load() to check if the library is loaded, independend of the m_useCount
		 */
		virtual bool isLoaded() const = 0;
	public:
		/** Object creation (done by CDynLibFactory) initialises the members
		 *  with the parameters
		 */
		CDynLib(const char *libname, const char *searchpath, long int version=-1);

		/** Destructor, doUnload() must be called by the destructors of the
		 *  system dependend child classes
		 */
		inline virtual ~CDynLib() { }

		/** @return The C-String of the library name m_libname
		 */
		virtual const char *libname() const;

		/** @brief get the filepath of a library.
		 *
		 * Can call findLib(), if m_libpath is not already set.
		 *		
		 * @return The C-String of the complete library path m_libpath
		 */
		virtual const char *libpath();

		/** Handles the loading of the library, calls doload() if called the first time
		 */
		bool load();

		/** Handles the unloading of the library, calls dounload()
		 *  if called as much times as load(), use m_useCount for this.
		 */
		bool unload();

		/** @return m_useCount is returned
		 */
		unsigned long useCount() const;

		/** @return true if useCount() > 0
		 */
		virtual bool valid() const;

		/** @brief Gets a system dependend object to call an exported library function with the functionname \a name
		 *
		 * @param name Functionname of the exported function
		 * @return System dependend function object, used to call the function name the
		 *         Signature must be known by the object that called getFunc(), eg.
		 *         CRendererLoader::CRendererLib
		 */
		virtual CLibFunc *getFunc(const char *name) const = 0;

		/** @brief Deletes a function object created with getFunc()
		 *
		 * @param f A function object previously created with getFunc() of the same CDynLib
		 */
		virtual void deleteFunc(CLibFunc *f) const;
	}; // CDynLib

	/** @brief factory object to create system dependend CDynLib objects
	 */
	class CDynLibFactory {
		public:
			/** @brief Gets a new instance of a dynamic library should
			 *
			 * Normally only used once per library, can be used to examine the
			 * library path.
			 */
			static CDynLib *newDynLib(const char *libname, const char *searchpath, long int version=-1);

			/** @brief Gets a new instance of a dynamic library should
			 *
			 * Normally only used once per library, can be used to examine the
			 * library path.
			 * @param lib CDynLib created by a previous call of newDynLib()
			 */
			static void deleteDynLib(CDynLib *lib);
	}; // CDynLibFactory

} // namespace RiCPP

#endif // _RICPP_GENDYNLIB_DYNLIB_H
