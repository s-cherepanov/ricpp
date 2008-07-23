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

/** @file triangulation.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implements triangulation of the surface primitives.
 */

#ifndef _RICPP_RICONTEXT_SURFACE_H
#include "ricpp/ricontext/surface.h"
#endif _RICPP_RICONTEXT_SURFACE_H

#ifndef _RICPP_RICONTEXT_RIMACROPRIMS_H
#include "ricpp/ricontext/rimacroprims.h"
#endif _RICPP_RICONTEXT_RIMACROPRIMS_H

using namespace RiCPP;


class CTriangulator {
	friend class CSurfaceContainer;
protected:
	virtual void releaseSurface(CSurface *surf);
public:
	inline virtual CTriangulator() {}
};

class CPolygonTriangulator : CTriangulator {
public:
	CSurface *triangulate(CRiPolygon &poly, RtInt nvertices, const CParameterList &params);
};


// =============================================================================

void CTriangulator::releaseSurface(CSurface *surf)
{
	if ( surf )
		delete surf;
}


CSurface *CPolygonTriangulator::triangulate(CRiPolygon &poly, RtInt nvertices, const CParameterList &params)
{
	CSurface *surf = new CSurface;
	if ( !surf ) 
		return 0;
	
	CFace &f = surf->newFace();

	std::vector<CFace::TypeIndex> &strip = f.indices();
	
	return surf;
}
