#ifndef _RICPP_RICPP_ERRORHANDLERS_H
#define _RICPP_RICPP_ERRORHANDLERS_H

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

class CAbortErrorHandler : public IErrorHandler {
public:
	inline virtual const char *name() const {return "abortErrorHandler";}
	virtual RtVoid operator()(RtInt code, RtInt severity, RtString msg) const;
};

class CPrintErrorHandler : public IErrorHandler  {
public:
	inline virtual const char *name() const {return "printErrorHandler";}
	virtual RtVoid operator()(RtInt code, RtInt severity, RtString msg) const;
};

class CIgnoreErrorHandler : public IErrorHandler  {
public:
	inline virtual const char *name() const {return "ignoreErrorHandler";}
	virtual RtVoid operator()(RtInt code, RtInt severity, RtString msg) const;
};

} // namespace RiCPP

#endif // _RICPP_RICPP_ERRORHANDLERS_H
