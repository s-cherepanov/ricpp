#ifndef _RICPP_RICPP_SUBDIVFUNC_H
#define _RICPP_RICPP_SUBDIVFUNC_H

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

/** @file subdivfunc.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The standard subdivision functions and free function for IRi::procedural()
 *         not yet implemented
 */

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

namespace RiCPP {

/** @brief Implements the DelayedReadArchive procedural.
 */
class CProcDelayedReadArchive : public ISubdivFunc {
public:
	static class CProcDelayedReadArchive func;
	inline static RtToken myName() {return RI_DELAYED_READ_ARCHIVE; }

	/** @brief Clone the instance.
	 *  @return A new cloned instance of the function.
	 */
	inline virtual ISubdivFunc *duplicate() const { return new CProcDelayedReadArchive(*this); }

	/** @brief The name of the function
	 *  @return The name of the function as used in RIB
	 */
	inline virtual RtToken name() const {return myName();}

	/** @brief Delayed read of a RIB archive
	 * @param ri Interface to be used
	 * @param data Array of one string, the name of the archive as null terminated string
	 * @param detail level of detail of the bounding box of the procedural or RI_INFINITY
	 */
	virtual RtVoid operator()(IRi &ri, RtPointer data, RtFloat detail) const;

	inline virtual ISubdivFunc &singleton()  const { return func; }
};

/** @brief Implements the RunProgram procedural.
 */
class CProcRunProgram : public ISubdivFunc {
public:
	static class CProcRunProgram func;
	inline static RtToken myName() {return RI_RUN_PROGRAM; }

	/** @brief Clone the instance.
	 *  @return A new cloned instance of the function.
	 */
	inline virtual ISubdivFunc *duplicate() const { return new CProcRunProgram(*this); }

	/** @brief The name of the function
	 *  @return The name of the function as used in RIB
	 */
	inline virtual RtToken name() const {return myName();}

	/** @brief Run a helper program and capture output as RIB for ri
	 * @param ri Interface to be used
	 * @param data Array of two strings, the program name and its command line arguments
	 * @param detail level of detail of the bounding box of the procedural or RI_INFINITY
	 */
	virtual RtVoid operator()(IRi &ri, RtPointer data, RtFloat detail) const;

	inline virtual ISubdivFunc &singleton()  const { return func; }
};

/** @brief Implements the DynamicLoad procedural.
 */
class CProcDynamicLoad : public ISubdivFunc {
public:
	static class CProcDynamicLoad func;
	inline static RtToken myName() {return RI_DYNAMIC_LOAD; }

	/** @brief Clone the instance.
	 *  @return A new cloned instance of the function.
	 */
	inline virtual ISubdivFunc *duplicate() const { return new CProcDynamicLoad(*this); }

	/** @brief The name of the function
	 *  @return The name of the function as used in RIB
	 */
	inline virtual RtToken name() const {return myName();}

	/** @brief Calls a dynamic library implementing: RtPointer ConvertParameters(IRi &ri, char *initial data), 
	 * void Subdivide(IRi &ri, RtPointer blinddata, RtFloat detailsize), void Free(IRi &ri, RtPointer blinddata)
	 * @param ri Interface to be used
	 * @param data Array of two strings, the library name and its parameters (converted to blinddata by ConvertParameters)
	 * @param detail level of detail of the bounding box of the procedural or RI_INFINITY
	 */
	virtual RtVoid operator()(IRi &ri, RtPointer data, RtFloat detail) const;

	inline virtual ISubdivFunc &singleton()  const { return func; }
};

/** @brief Implements the free function that can be called by the interface to free the 'data'
 *  of own procedurals.
 */
class CProcFree : public IFreeFunc {
public:
	static class CProcFree func;
	inline static RtToken myName() {return RI_FREE; }

	/** @brief Clone the instance.
	 *  @return A new cloned instance of the function.
	 */
	inline virtual IFreeFunc *duplicate() const { return new CProcFree(*this); }

	/** @brief The name of the function
	 *  @return The name of the function (no RIB binding)
	 */
	inline virtual RtToken name() const {return myName();}

	/** @brief The implementation of the free function
	 *
	 * Just call free() for the data.
	 * @param ri Interface to be used
	 * @param data
	 */
	virtual RtVoid operator()(IRi &ri, RtPointer data) const;

	inline virtual IFreeFunc &singleton()  const { return func; }
};


/** @brief Factory for subdivision functions.
 */
class CSubdivFuncFactory {
public:
	inline virtual ~CSubdivFuncFactory() {}

	virtual ISubdivFunc *newFunc(RtToken name);
	virtual void deleteFunc(ISubdivFunc *f);

	virtual ISubdivFunc *singleton(RtToken name) const;
};

/** @brief Factory for free functions.
 */
class CFreeFuncFactory {
public:
	inline virtual ~CFreeFuncFactory() {}

	virtual IFreeFunc *newFunc(RtToken name);
	virtual void deleteFunc(IFreeFunc *f);

	virtual IFreeFunc *singleton(RtToken name) const;
};

}

#endif // _RICPP_RICPP_SUBDIVFUNC_H
