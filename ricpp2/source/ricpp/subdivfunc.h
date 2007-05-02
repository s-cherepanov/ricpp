#ifndef _RICPP_RICPP_SUBDIVFUNC_H
#define _RICPP_RICPP_SUBDIVFUNC_H

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

/** @file subdivfunc.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The standard subdivision functions und free function for IRi::procedural()
 *         not yet implemented
 *
 *     RenderMan(R) is a registered trademark of Pixar
 * The RenderMan(R) Interface Procedures and Protocol are:
 *         Copyright 1988, 1989, 2000, 2005 Pixar
 *                 All rights Reservered
 */

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

namespace RiCPP {

/** Implements the DelayedReadArchive procedural
 */
class CProcDelayedReadArchive : public ISubdivFunc {
public:
	/** @return The name of the function as used in RIB
	 */
	inline virtual const char *name() const {return "DelayedReadArchive";}
	/** Delayed read of a RIB archive
	 * @param ri Interface to be used
	 * @param data Array of one string, the name of the archive as null terminated string
	 * @param detail level of detail of the bounding box of the procedural or RI_INFINITY
	 */
	virtual RtVoid operator()(IRi &ri, RtPointer data, RtFloat detail) const;
};

/** Implements the RunProgram procedural
 */
class CProcRunProgram : public ISubdivFunc {
public:
	/** @return The name of the function as used in RIB
	 */
	inline virtual const char *name() const {return "RunProgram";}
	/** Run a helper program and capture output as RIB for ri
	 * @param ri Interface to be used
	 * @param data Array of two strings, the program name and its command line arguments
	 * @param detail level of detail of the bounding box of the procedural or RI_INFINITY
	 */
	virtual RtVoid operator()(IRi &ri, RtPointer data, RtFloat detail) const;
};

/** Implements the DynamicLoad procedural
 */
class CProcDynamicLoad : public ISubdivFunc {
public:
	/** @return The name of the function as used in RIB
	 */
	inline virtual const char *name() const {return "DynamicLoad";}
	/** Calls a dynamic library implementing: RtPointer ConvertParameters(IRi &ri, char *initial data), 
	 * void Subdivide(IRi &ri, RtPointer blinddata, RtFloat detailsize), void Free(IRi &ri, RtPointer blinddata)
	 * @param ri Interface to be used
	 * @param data Array of two strings, the library name and its parameters (converted to blinddata by ConvertParameters)
	 * @param detail level of detail of the bounding box of the procedural or RI_INFINITY
	 */
	virtual RtVoid operator()(IRi &ri, RtPointer data, RtFloat detail) const;
};

/** Implements the free function that can be called by the interface to free the 'data'
 *  of own procedurals
 */
class CProcFree : public IFreeFunc {
public:
	/** @return The name of the function (no RIB binding)
	 */
	inline virtual const char *name() const {return "Free";}
	/**
	 * @param ri Interface to be used
	 * @param data
	 */
	virtual RtVoid operator()(IRi &, RtPointer data) const;
};
}

#endif // _RICPP_RICPP_SUBDIVFUNC_H
