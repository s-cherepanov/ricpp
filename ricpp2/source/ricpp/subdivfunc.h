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

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

namespace RiCPP {

class CProcDelayedReadArchive : public ISubdivFunc {
public:
	inline virtual const char *name() const {return "procDelayedReadArchive";}
	virtual RtVoid operator()(IRi &, RtPointer, RtFloat) const;
};

class CProcRunProgram : public ISubdivFunc {
public:
	inline virtual const char *name() const {return "procRunProgram";}
	virtual RtVoid operator()(IRi &, RtPointer, RtFloat) const;
};

class CProcDynamicLoad : public ISubdivFunc {
public:
	inline virtual const char *name() const {return "procDynamicLoad";}
	virtual RtVoid operator()(IRi &, RtPointer, RtFloat) const;
};

class CProcFreeFunc : public IFreeFunc {
public:
	inline virtual const char *name() const {return "procFreeFunc";}
	virtual RtVoid operator()(IRi &, RtPointer) const;
};
}

#endif // _RICPP_RICPP_SUBDIVFUNC_H
