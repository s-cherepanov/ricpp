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

/** @file filters.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Pixel filters for RiCPP, see [RISPEC3.2] Appendix E
 */
#include "ricpp/ricpp/filters.h"

#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif // _RICPP_TOOLS_INLINETOOLS_H

#ifndef _RICPP_RI_RI_H
#include "ricpp/ri/ri.h"
#endif // _RICPP_TOOLS_INLINETOOLS_H

using namespace RiCPP;

// Various filters

//
// CGaussianFilter
//
RtToken CGaussianFilter::myName() {return RI_GAUSSIAN; }
RtFloat	CGaussianFilter::operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const
{
	return RiGaussianFilter(x, y, xwidth, ywidth);
}

CGaussianFilter CGaussianFilter::func;

//
// CBoxFilter
//
RtToken CBoxFilter::myName() {return RI_BOX; }
RtFloat	CBoxFilter::operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const
{
	return RiBoxFilter(x, y, xwidth, ywidth);
}

CBoxFilter CBoxFilter::func;


//
// CTriangleFilter
//
RtToken CTriangleFilter::myName() {return RI_TRIANGLE; }
RtFloat	CTriangleFilter::operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const
{
	return RiTriangleFilter(x, y, xwidth, ywidth);
}

CTriangleFilter CTriangleFilter::func;


//
// CCatmullRomFilter
//
RtToken CCatmullRomFilter::myName() {return RI_CATMULL_ROM; }
RtFloat	CCatmullRomFilter::operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const
{
	return RiCatmullRomFilter(x, y, xwidth, ywidth);
}

CCatmullRomFilter CCatmullRomFilter::func;

//
// CSincFilter
//
RtToken CSincFilter::myName() {return RI_SINC; }
RtFloat	CSincFilter::operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const
{
	return RiSincFilter(x, y, xwidth, ywidth);
}

CSincFilter CSincFilter::func;

//
// CFilterFuncFactory
//

IFilterFunc *CFilterFuncFactory::newFunc(RtToken name)
{
	if ( emptyStr(name) )
		return 0;

	if ( CGaussianFilter::myName() == name )
		return new CGaussianFilter;

	if ( CBoxFilter::myName() == name )
		return new CBoxFilter;

	if ( CTriangleFilter::myName() == name )
		return new CTriangleFilter;

	if ( CCatmullRomFilter::myName() == name || name == RI_CATMULLROM )
		return new CCatmullRomFilter;

	if ( CSincFilter::myName() == name )
		return new CSincFilter;

	return 0;
}

void CFilterFuncFactory::deleteFunc(IFilterFunc *f)
{
	if ( !f )
		return;
	delete f;
}

const IFilterFunc *CFilterFuncFactory::singleton(RtToken name) const
{
	if ( emptyStr(name) )
		return 0;

	if ( CGaussianFilter::myName() == name )
		return &CGaussianFilter::func;

	if ( CBoxFilter::myName() == name )
		return &CBoxFilter::func;

	if ( CTriangleFilter::myName() == name )
		return &CTriangleFilter::func;

	if ( CCatmullRomFilter::myName() == name || name == RI_CATMULLROM )
		return &CCatmullRomFilter::func;

	if ( CSincFilter::myName() == name )
		return &CSincFilter::func;

	return 0;
}
