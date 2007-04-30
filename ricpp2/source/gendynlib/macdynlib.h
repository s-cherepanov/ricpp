#ifndef _RICPP_GENDYNLIB_MACDYNLIB_H
#define _RICPP_GENDYNLIB_MACDYNLIB_H

// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 200,, 2005 Pixar
//                 All rights Reservered
//
// Copyright � of RiCPP 2007, Andreas Pidde
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
#include "gendynlib/dynlib.h"
#endif // _RICPP_GENDYNLIB_DYNLIB_H

namespace RiCPP {

class CMacLibFunc : public CLibFunc {
	void *m_funcPtr;
public:
	CMacLibFunc(void *aFuncPtr);
	virtual ~CMacLibFunc();
	virtual bool valid();
	void *funcPtr() const;
}; // CMacLibFunc

class CMacDynLib : public CDynLib {
	std::string m_libPath;
	void *m_libHandle;
protected:
	virtual bool doUnload();
	virtual bool doLoad();
	virtual const char *findLib();
public:
	CMacDynLib(const char *libname, const char *searchpath);
	virtual ~CMacDynLib();
	static const char *staticClassName();
	virtual const char *className() const;
	virtual bool isLoaded() const;
	virtual bool valid() const;
	virtual CLibFunc *getFunc(const char *name) const;
}; // CMacDynLib

}

#endif // _RICPP_MACDYNLIB_WIN32DYNLIB_H
