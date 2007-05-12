#ifndef _RICPP_GENDYNLIB_WIN32DYNLIB_H
#define _RICPP_GENDYNLIB_WIN32DYNLIB_H

#ifdef WIN32

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

#ifndef _RICPP_GENDYNLIB_DYNLIB_H
#include "ricpp/gendynlib/dynlib.h"
#endif // _RICPP_GENDYNLIB_DYNLIB_H

#ifndef _WINDOWS_
#include <windows.h>
#endif // _WINDOWS_

namespace RiCPP {

class CWin32LibFunc : public CLibFunc {
	FARPROC m_funcPtr;
public:
	CWin32LibFunc(FARPROC ptr);
	virtual ~CWin32LibFunc();
	virtual bool valid();
	FARPROC funcPtr() const;
}; // CWin32LibFunc

class CWin32DynLib : public CDynLib {
	static const HMODULE invalidLibHandle;
	HMODULE m_libHandle;
protected:
	virtual bool doUnload();
	virtual bool doLoad();
	virtual const char *findLib();
	virtual bool isLoaded() const;
public:
	CWin32DynLib(const char *libname, const char *searchpath, long int version = -1);
	virtual ~CWin32DynLib();
	virtual bool valid() const;
	virtual CLibFunc *getFunc(const char *name) const;
}; // CWin32DynLib

}

#endif // WIN32

#endif // _RICPP_GENDYNLIB_WIN32DYNLIB_H
