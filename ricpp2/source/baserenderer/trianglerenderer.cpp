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

using namespace RiCPP;

#define TESSELATION 32

CTriangleRenderer::CTriangleRenderer()
{
	m_tessX = TESSELATION;
	m_tessY = TESSELATION;
}

RtVoid CTriangleRenderer::triangulate(CRiPolygon &obj)
{
	CPolygonTriangulator t(obj);
	hide(t.triangulate());
}

RtVoid CTriangleRenderer::triangulate(CRiPointsPolygons &obj)
{
	CPointsPolygonsTriangulator t(obj);
	hide(t.triangulate());
}

RtVoid CTriangleRenderer::triangulate(CRiGeneralPolygon &obj)
{
	CGeneralPolygonTriangulator t(obj, polygonTriangulationStrategy());
	hide(t.triangulate());
}

RtVoid CTriangleRenderer::triangulate(CRiPointsGeneralPolygons &obj)
{
	CPointsGeneralPolygonsTriangulator t(obj, polygonTriangulationStrategy());
	hide(t.triangulate());
}

RtVoid CTriangleRenderer::triangulate(CQuadricTriangulator &triObj)
{
	const CDeclaration *pdecl = renderState()->declFind(RI_P);
	if ( !pdecl )
		return;
	const CDeclaration *ndecl = renderState()->declFind(RI_N);
	if ( !ndecl )
		return;
	hide(triObj.triangulate(*pdecl, *ndecl, m_tessX, m_tessY, attributes().primitiveOrientation()==attributes().coordSysOrientation()));
}

RtVoid CTriangleRenderer::triangulate(CRiCone &obj)
{
	CConeTriangulator t(obj);
	triangulate(t);
}

RtVoid CTriangleRenderer::triangulate(CRiCylinder &obj)
{
	CCylinderTriangulator t(obj);
	triangulate(t);
}

RtVoid CTriangleRenderer::triangulate(CRiDisk &obj)
{
	CDiskTriangulator t(obj);
	triangulate(t);
}

RtVoid CTriangleRenderer::triangulate(CRiHyperboloid &obj)
{
	CHyperboloidTriangulator t(obj);
	triangulate(t);
}

RtVoid CTriangleRenderer::triangulate(CRiParaboloid &obj)
{
	CParaboloidTriangulator t(obj);
	triangulate(t);
}

RtVoid CTriangleRenderer::triangulate(CRiSphere &obj)
{
	CSphereTriangulator t(obj);
	triangulate(t);
}

RtVoid CTriangleRenderer::triangulate(CRiTorus &obj)
{
	CTorusTriangulator t(obj);
	triangulate(t);
}
