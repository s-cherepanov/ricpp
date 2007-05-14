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

class CMacLibFunc : public ILibFunc {
	void *m_funcPtr;
public:
	CMacLibFunc(void *aFuncPtr);
	virtual ~CMacLibFunc();
	virtual bool valid();
	void *funcPtr() const;
}; // CMacLibFunc

class CMacDynLib : public CDynLib {
	void *m_libHandle;
protected:
	virtual bool doUnload();
	virtual bool doLoad();
	virtual const char *findLib();
public:
	CMacDynLib(const char *libname, const char *searchpath, long int version=-1);
	virtual ~CMacDynLib();
	virtual bool isLoaded() const;
	virtual bool valid() const;
	virtual ILibFunc *getFunc(const char *name) const;
}; // CMacDynLib

}

#endif // _RICPP_MACDYNLIB_WIN32DYNLIB_H
