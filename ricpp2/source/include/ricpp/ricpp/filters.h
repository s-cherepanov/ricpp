#ifndef _RICPP_RICPP_FILTERS_H
#define _RICPP_RICPP_FILTERS_H

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

/** \file filters.h
 *  \author Andreas Pidde (andreas@pidde.de)
 *  \brief Pixel filters for RiCPP, see [RISPEC3.2] Appendix E
 */

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

namespace RiCPP {

class CGaussianFilter : public IFilterFunc {
	inline virtual const char *name() const { return "gausianFilter"; }
	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const;
};

class CBoxFilter : public IFilterFunc {
	inline virtual const char *name() const { return "boxFilter"; }
	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const;
};

class CTriangleFilter : public IFilterFunc {
	inline virtual const char *name() const { return "triangleFilter"; }
	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const;
};

class CCatmullRomFilter : public IFilterFunc {
	inline virtual const char *name() const { return "catmullRomFilter"; }
	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const;
};

class CSincFilter : public IFilterFunc {
	inline virtual const char *name() const { return "sincFilter"; }
	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const;
};
} // namespace RiCPP

#endif // _RICPP_RICPP_FILTERS_H