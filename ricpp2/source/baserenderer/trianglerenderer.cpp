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

/** @file trianglerenderer.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Base class for renderers that use triangle primitives.
 */

#include "ricpp/baserenderer/trianglerenderer.h"

#ifndef _RICPP_RICONTEXT_TRIANGULATION_H
#include "ricpp/ricontext/triangulation.h"
#endif // _RICPP_RICONTEXT_TRIANGULATION_H

using namespace RiCPP;

RtVoid CTriangleRenderer::triangulate(CRiPolygon &obj)
{
	CConvexPolygonTriangulator t;
	hide(t.triangulate(obj));
}

RtVoid CTriangleRenderer::triangulate(CRiPointsPolygons &obj)
{
	CConvexPointsPolygonsTriangulator t;
	hide(t.triangulate(obj));
}

RtVoid CTriangleRenderer::triangulate(CRiGeneralPolygon &obj)
{
	CGeneralPolygonTriangulator t;
	hide(t.triangulate(obj, polygonTriangulationStrategy()));
}

RtVoid CTriangleRenderer::triangulate(CRiPointsGeneralPolygons &obj)
{
	CPointsGeneralPolygonsTriangulator t;
	hide(t.triangulate(obj, polygonTriangulationStrategy()));
}

RtVoid CTriangleRenderer::triangulate(CRiParaboloid &obj)
{
	CParaboloidTriangulator t;
	CDeclaration *pdecl = renderState()->declFind(RI_P);
	if ( !pdecl )
		return;
	CDeclaration *ndecl = renderState()->declFind(RI_N);
	if ( !ndecl )
		return;
	hide(t.triangulate(obj, *pdecl, *ndecl, 63, 63));
}

