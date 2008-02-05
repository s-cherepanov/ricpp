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
public:
	static class CGaussianFilter func;
	static RtToken myName();
	inline virtual IFilterFunc *duplicate() const { return new CGaussianFilter(*this); }
	inline virtual RtToken name() const { return myName(); }
	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const;
	inline virtual IFilterFunc &singleton() const { return func; }
};

/** @brief The box pixel filter.
 */
class CBoxFilter : public IFilterFunc {
public:
	static class CBoxFilter func;
	static RtToken myName();
	inline virtual IFilterFunc *duplicate() const { return new CBoxFilter(*this); }
	inline virtual RtToken name() const { return myName(); }
	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const;
	inline virtual IFilterFunc &singleton() const { return func; }
};

/** @brief The triangle pixel filter.
 */
class CTriangleFilter : public IFilterFunc {
public:
	static class CTriangleFilter func;
	static RtToken myName();
	inline virtual IFilterFunc *duplicate() const { return new CTriangleFilter(*this); }
	inline virtual RtToken name() const { return myName(); }
	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const;
	inline virtual IFilterFunc &singleton() const { return func; }
};

/** @brief The Catmull-Rom pixel filter.
 */
class CCatmullRomFilter : public IFilterFunc {
public:
	static class CCatmullRomFilter func;
	static RtToken myName();
	inline virtual IFilterFunc *duplicate() const { return new CCatmullRomFilter(*this); }
	inline virtual RtToken name() const { return myName(); }
	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const;
	inline virtual IFilterFunc &singleton() const { return func; }
};

/** @brief The sinc pixel filter.
 */
class CSincFilter : public IFilterFunc {
public:
	static class CSincFilter func;
	static RtToken myName();
	inline virtual IFilterFunc *duplicate() const { return new CSincFilter(*this); }
	inline virtual RtToken name() const { return myName(); }
	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const;
	inline virtual IFilterFunc &singleton() const { return func; }
};


/** @brief Factory for pixel filter functions.
 */
class CFilterFuncFactory {
public:
	inline virtual ~CFilterFuncFactory() {}

	virtual IFilterFunc *newFunc(RtToken name);
	virtual void deleteFunc(IFilterFunc *f);

	virtual IFilterFunc *singleton(RtToken name) const;
};

} // namespace RiCPP

#endif // _RICPP_RICPP_FILTERS_H
