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
 *  \brief Pixel filters for RiCPP, see RISPEC3.2 Appendix E
 */

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

namespace RiCPP {

/** @brief The gaussian pixel filter.
 */
class CGaussianFilter : public IFilterFunc {
	inline virtual IFilterFunc *duplicate() const { return new CGaussianFilter(*this); }
	inline virtual RtToken name() const { return RI_GAUSIAN_FILTER; }
	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const;
};

/** @brief The box pixel filter.
 */
class CBoxFilter : public IFilterFunc {
	inline virtual IFilterFunc *duplicate() const { return new CBoxFilter(*this); }
	inline virtual RtToken name() const { return RI_BOX_FILTER; }
	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const;
};

/** @brief The triangle pixel filter.
 */
class CTriangleFilter : public IFilterFunc {
	inline virtual IFilterFunc *duplicate() const { return new CTriangleFilter(*this); }
	inline virtual RtToken name() const { return RI_TRIANGLE_FILTER; }
	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const;
};

/** @brief The Catmull-Rom pixel filter.
 */
class CCatmullRomFilter : public IFilterFunc {
	inline virtual IFilterFunc *duplicate() const { return new CCatmullRomFilter(*this); }
	inline virtual RtToken name() const { return RI_CATMULL_ROM_FILTER; }
	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const;
};

/** @brief The sinc pixel filter.
 */
class CSincFilter : public IFilterFunc {
	inline virtual IFilterFunc *duplicate() const { return new CSincFilter(*this); }
	inline virtual RtToken name() const { return RI_SINC_FILTER; }
	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const;
};
} // namespace RiCPP

#endif // _RICPP_RICPP_FILTERS_H
