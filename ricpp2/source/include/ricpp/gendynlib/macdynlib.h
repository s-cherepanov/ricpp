#ifndef _RICPP_GENDYNLIB_MACDYNLIB_H
#define _RICPP_GENDYNLIB_MACDYNLIB_H

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

/** @file macdynlib.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declaration of a class to handle dynamic libraries in MacOS.
 */

#ifndef _RICPP_GENDYNLIB_DYNLIB_H
#include "ricpp/gendynlib/dynlib.h"
#endif // _RICPP_GENDYNLIB_DYNLIB_H

namespace RiCPP {

/** @brief The Mac implementation to handle functions of dynamic libraries.
 */
class CMacLibFunc : public ILibFunc {
	void *m_funcPtr; ///< @brief The pointer to a function of a dynamic library.
public:
	/** @brief Initialize with a function pointer.
	 *
	 * If the pointer is not valid valid() will return false. This can be
	 * used for error handling.
	 *
	 * @param aFuncPtr Pointer to a loaded library function.
	 */
	CMacLibFunc(void *aFuncPtr);
	/** @brief Destructor, nothing to do.
	 */
	virtual ~CMacLibFunc();
	/** @brief Test if function is valid.
	 * @return true, if the function can be used (the function pointer is valid).
	 */
	virtual bool valid();
	/** @brief The function pointer to call the function.
	 * @return The function pointer, used by the framework to call the library function.
	 */
	void *funcPtr() const;
}; // CMacLibFunc

/** @brief The Mac implemenatation to handle dynamic libraries.
 */
class CMacDynLib : public CDynLib {
	void *m_libHandle; ///< @brief The handle of the loaded library
protected:
	/** @brief Removes the library from memory.
	 * @return true, library could be removed.
	 */
	virtual bool doUnload();
	/** @brief Loads the library into memory.
	 * @return true, library could be loaded.
	 */
	virtual bool doLoad();
	/** @brief Searches the library in the search path.
	 * @return The full pathname of the library.
	 */
	virtual const char *findLib();
	/** @brief Query if the library is loaded.
	 * @return True, if the library is loaded into memory.
	 */
	virtual bool isLoaded() const;
public:
	/** @brief Query if the library is loaded.
	 * @param libname The name of the library as used in begin() without any suffixes
	 * @param searchpath The semicolon seperated searchpath (internal representation) for the library
	 * @param version The version of the library to load
	 */
	CMacDynLib(const char *libname, const char *searchpath, long int version=-1);
	/** @brief Destructor, removes the library from memory.
	 */
	virtual ~CMacDynLib();
	/** @brief Test if library is loaded
	 *
	 * @return True, library could be loaded into memory.
	 */
	virtual bool valid() const;
	/** @brief Gets a function out of the library
	 *
	 * @return A pointer to the function, is not neccesarily valid and can be 0.
	 */
	virtual ILibFunc *getFunc(const char *name) const;
}; // CMacDynLib

}

#endif // _RICPP_MACDYNLIB_WIN32DYNLIB_H
