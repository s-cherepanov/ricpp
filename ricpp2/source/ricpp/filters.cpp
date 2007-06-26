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

/** \file filters.cpp
 *  \author Andreas Pidde (andreas@pidde.de)
 *  \brief Pixel filters for RiCPP, see [RISPEC3.2] Appendix E
 */

#include "ricpp/ricpp/filters.h"
#include <cmath>

using namespace RiCPP;

//
// CGaussianFilter
//
RtFloat	CGaussianFilter::operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const
{
	x *= (RtFloat)(2.0 / xwidth);
	y *= (RtFloat)(2.0 / ywidth);
	return (RtFloat)exp(-2.0 * (x*x + y*y));
}


//
// CBoxFilter
//
RtFloat	CBoxFilter::operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const
{
	x = x;
	y = y;
	xwidth = xwidth;
	ywidth = ywidth;

	return (RtFloat)1.0;
}


//
// CTriangleFilter
//
RtFloat	CTriangleFilter::operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const
{
	return (RtFloat)(((1.0 - fabs(x)) / (xwidth * 0.5)) *
		   ((1.0 - fabs(y)) / (ywidth * 0.5)));
}


//
// CCatmullRomFilter
//
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


//
// CSincFilter
//
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
