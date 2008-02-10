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

using namespace RiCPP;

//
// CGaussianFilter
//
RtToken CGaussianFilter::myName() {return RI_GAUSSIAN; }
RtFloat	CGaussianFilter::operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const
{
	x *= (RtFloat)(2.0 / xwidth);
	y *= (RtFloat)(2.0 / ywidth);
	return (RtFloat)exp(-2.0 * (x*x + y*y));
}

CGaussianFilter CGaussianFilter::func;

//
// CBoxFilter
//
RtToken CBoxFilter::myName() {return RI_BOX; }
RtFloat	CBoxFilter::operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const
{
	x = x;
	y = y;
	xwidth = xwidth;
	ywidth = ywidth;

	return (RtFloat)1.0;
}

CBoxFilter CBoxFilter::func;


//
// CTriangleFilter
//
RtToken CTriangleFilter::myName() {return RI_TRIANGLE; }
RtFloat	CTriangleFilter::operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const
{
	return (RtFloat)(((1.0 - fabs(x)) / (xwidth * 0.5)) *
		   ((1.0 - fabs(y)) / (ywidth * 0.5)));
}

CTriangleFilter CTriangleFilter::func;


//
// CCatmullRomFilter
//
RtToken CCatmullRomFilter::myName() {return RI_CATMULL_ROM; }
RtFloat	CCatmullRomFilter::operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const
{
	RtFloat r2 = (x*x + y*y);
	RtFloat r = (RtFloat)sqrt(r2);

	xwidth = xwidth;
	ywidth = ywidth;

	return (RtFloat)((r > 2.0) ? 0.0 :
	                 (r < 1.0) ? (3.0*r*r2 - 5.0*r2 + 2.0) :
		                         (-r*r2 + 5.0*r2 - 8.0*r + 4.0));
}

CCatmullRomFilter CCatmullRomFilter::func;

//
// CSincFilter
//
RtToken CSincFilter::myName() {return RI_SINC; }
RtFloat	CSincFilter::operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const
{
	RtFloat s, t;

	xwidth = xwidth;
	ywidth = ywidth;

	if ( x > -0.001 && x < 0.001)
		s = 1.0;
	else
		s = (RtFloat)(sin(x)/x);

	if ( y > -0.001 && y < 0.001 )
		t = 1.0;
	else
		t = (RtFloat)(sin(y)/y);

	return s*t;
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

IFilterFunc *CFilterFuncFactory::singleton(RtToken name) const
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
