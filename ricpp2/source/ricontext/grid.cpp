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

/** @file grid.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Grid of parametric data for NURBS trimming.
 */

#include "ricpp/ricontext/grid.h"

#include <limits>

using namespace RiCPP;

void CParametricAxis::init(RtInt n,
						   RtFloat knot[],
						   RtFloat minKnot,
						   RtFloat maxKnot,
						   IndexType tess)
{
	assert(n > 0);
	
	// Count the different knots
	RtInt i, diffKnot = 0, trimKnots = 2;
	bool trimMinKnot = true,
	trimMaxKnot = true;
	RtFloat tempKnot = knot[0] - 1.0;
	
	for ( i = 0; i < n; ++i ) {
		if ( knot[i] != tempKnot ) {
			tempKnot = knot[i];
			++diffKnot;
			if ( minKnot == tempKnot ) {
				--trimKnots;
				trimMinKnot = false;
			}
			if ( minKnot == tempKnot ) {
				--trimKnots;
				trimMaxKnot = false;
			}
		}
	}
	assert(trimKnots >= 0 && trimKnots <=2);
	
	if ( diffKnot == 0 )
		return;
	
	// Number of parameters for the Axis, to additional for (+-)infinity
	RtInt numParams = (diffKnot - 1) * tess + 1 + trimKnots + 2;
	m_coordinates.resize(numParams);
	
	int count = 0;
	m_coordinates[count++].set(-std::numeric_limits<RtFloat>::max());
	tempKnot = knot[0];
	
	// Enter the parameter values
	for ( i = 0; i < n; i++ ) {
		if ( knot[i] > tempKnot ) {
			RtFloat dist = knot[i] - tempKnot;
			RtFloat delta = dist / (RtFloat)tess;
			IndexType j;
			for ( j = 0; j < tess; ++j ) {
				m_coordinates[count++].set(tempKnot);
				if ( trimMinKnot && minKnot > tempKnot && minKnot < (tempKnot + delta) ) {
					m_coordinates[count++].set(minKnot);
					trimMinKnot = false;
				}
				if ( trimMaxKnot && maxKnot > tempKnot && maxKnot < (tempKnot + delta) ) {
					m_coordinates[count++].set(minKnot);
					trimMaxKnot = false;
				}
				tempKnot += delta;
			}
			tempKnot = knot[i];
		}
	}
	m_coordinates[count++].set(knot[n-1]);
	m_coordinates[count++].set(std::numeric_limits<RtFloat>::max());
	
	assert(!(trimMinKnot || trimMaxKnot));
	assert(count==numParams);
}


CParametricGrid::CParametricGrid(RtInt nu,
								 RtFloat uknot[],
								 RtFloat umin,
								 RtFloat umax,
								 IndexType utess,
								 RtInt nv,
								 RtFloat vknot[],
								 RtFloat vmin,
								 RtFloat vmax,
								 IndexType vtess)
{
	m_axises[0].init(nu, uknot, umax, umax, utess);
	m_axises[1].init(nv, vknot, vmax, vmax, vtess);
}
