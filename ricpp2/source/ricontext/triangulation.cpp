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

#include "ricpp/ricontext/triangulation.h"
#include "ricpp/tools/templatefuncs.h"

/*
#ifdef _DEBUG
#include <iostream>
#endif
*/

using namespace RiCPP;


// =============================================================================


CTesselator::CTesselator()
{
}

CTesselator::~CTesselator()
{
	std::list<CSurface *>::iterator iter = m_surfaces.begin();
	for ( ; iter != m_surfaces.end(); iter++ ) {
		delete (*iter);
	}
}

bool CTesselator::releaseSurface(CSurface *surf)
{
	if ( !surf )
		return false;
	
	std::list<CSurface *>::iterator iter = std::find(m_surfaces.begin(), m_surfaces.end(), surf);
	if ( iter == m_surfaces.end() )
		return false;

	m_surfaces.erase(iter);
	delete surf;
	
	return true;
}

CSurface *CTesselator::createSurface()
{
	CSurface *s = new CSurface;
	if ( !s )
		return 0;
	m_surfaces.push_back(s);
	return s;
}

void CTesselator::getDelta(unsigned long &tess, RtFloat &delta) const
{
}

void CTesselator::getDeltas(unsigned long &tessU, unsigned long &tessV, RtFloat &deltaU, RtFloat &deltaV) const
{
	getDelta(tessU, deltaU);
	getDelta(tessV, deltaV);
}

// =============================================================================

void CPolygonTriangulator::triangleStrip(std::vector<IndexType> &strip, IndexType nVerts, IndexType offs) const
{
	strip.clear();

	strip.resize(tmax((IndexType)3, nVerts));

	strip[0] = 0;
	
	if ( nVerts == 1 ) {
		strip[1] = 0;
		strip[2] = 0;
	} else if ( nVerts == 2 ) {
		strip[1] = 1;
		strip[2] = 0;
	} else {
		IndexType startIdx = 0;
		IndexType endIdx = nVerts;
		IndexType i, j;
		for ( i=1, j=1; i < nVerts; ++j ) {
			strip[i] = offs+startIdx+j;
			++i;
			if ( i < nVerts ) {
				strip[i] = offs+endIdx-j;
				++i;
			}
		}
	}
}

void CPolygonTriangulator::insertParameters(CFace &f, IndexType faceIdx, const CParameterList &plist, const std::vector<RtInt> &verts, IndexType nverts, IndexType vertsOffs) const
{
	for ( CParameterList::const_iterator piter = plist.begin(); piter != plist.end(); piter++ ) {
		const CParameter &p = (*piter);
		
		const CDeclaration *decl = p.declarationPtr();
		if ( !decl )
			continue;
		
		switch ( decl->storageClass() ) {
				
			case CLASS_CONSTANT:
				f.insertConst(p);
				break;
				
			case CLASS_UNIFORM:
				f.insertUniform(p, faceIdx);
				break;
				
			case CLASS_VARYING:
				// NO break;
			case CLASS_VERTEX:
				f.insertVarying(p, nverts, &verts[vertsOffs]);
				break;
				
			case CLASS_FACEVARYING:
				// NO break;
			case CLASS_FACEVERTEX:
				f.insertFaceVarying(p, nverts, vertsOffs);
				break;
				
			default:
				break;
		}
	}
}

// =============================================================================

CSurface *CConvexPolygonTriangulator::triangulate(CRiPolygon &obj)
{
	RtInt nvertices = obj.nVertices();
	
	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	
	CFace &f = surf->newFace();
	f.faceType(FACETYPE_TRIANGLESTRIPS);
	
	std::vector<IndexType> &strip = f.indices();
	triangleStrip(strip, nvertices, 0);
	
	for ( CParameterList::const_iterator piter = obj.parameters().begin(); piter != obj.parameters().end(); piter++ ) {
		f.insertConst(*piter);
	}
	
	f.sizes().resize(1);
	f.sizes()[0] = strip.size();
	
	if ( !f.floats(RI_N) ) {
		// Add normals
	}

	return surf;
}

// =============================================================================

CSurface *CConvexPointsPolygonsTriangulator::triangulate(CRiPointsPolygons &obj)
{
	RtInt npolys = obj.nPolys();
	
	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	
	RtInt vertsOffs = 0;

	for ( RtInt faceIdx = 0; faceIdx < npolys; ++faceIdx ) {
		if ( obj.nVerts()[faceIdx] == 0 )
			continue;

		CFace &f = surf->newFace();
		f.faceType(FACETYPE_TRIANGLESTRIPS);
		
		RtInt nverts = obj.nVerts()[faceIdx];

		std::vector<IndexType> &strip = f.indices();
		triangleStrip(strip, nverts, vertsOffs);
		insertParameters(f, faceIdx, obj.parameters(), obj.verts(), nverts, vertsOffs);
		f.sizes().resize(1);
		f.sizes()[0] = strip.size();	
		
		if ( !f.floats(RI_N) ) {
			// Add normals
		}

		vertsOffs += obj.nVerts()[faceIdx];
	}
	
	return surf;
}

// =============================================================================

CSurface *CGeneralPolygonTriangulator::triangulate(CRiGeneralPolygon &obj, const IPolygonTriangulationStrategy &strategy)
{
	//  RtInt nloops, RtInt nverts[], const CParameterList &params
	
	const CTriangulatedPolygon *tp = obj.triangulate(strategy);
	if ( !tp || tp->triangles().size() <= 0 ) {
		return 0;
	}
	
	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	
	CFace &f = surf->newFace();
	f.faceType(FACETYPE_TRIANGLES);
	
	std::vector<IndexType> &strip = f.indices();
	strip = tp->triangles();
	
	for ( CParameterList::const_iterator piter = obj.parameters().begin(); piter != obj.parameters().end(); piter++ ) {
		f.insertConst(*piter);
	}
	
	f.sizes().resize(1);
	f.sizes()[0] = strip.size();
	
	if ( !f.floats(RI_N) ) {
		// Add normals
	}

	return surf;
}

// =============================================================================

CSurface *CPointsGeneralPolygonsTriangulator::triangulate(CRiPointsGeneralPolygons &obj, const IPolygonTriangulationStrategy &strategy)
{
	const std::vector<CTriangulatedPolygon> &tp = obj.triangulate(strategy);

	if ( tp.size() == 0 ) {
		return 0;
	}
	
	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}

	IndexType offs = 0;
	RtInt faceIdx = 0;
	RtInt nloops = 0;
	RtInt nverts = 0;
	RtInt nvertsOffs = 0;
	RtInt vertsOffs = 0;
	for (
		 std::vector<CTriangulatedPolygon>::const_iterator iter = tp.begin();
		 iter != tp.end();
		 iter++, faceIdx++, vertsOffs+=nverts )
	{
		nloops = obj.nLoops()[faceIdx];
		nverts = 0;
		for ( RtInt loopi = 0; loopi < nloops; ++loopi ) {
			nverts += obj.nVerts()[nvertsOffs++];
		}
		
		if ( nverts <= 0 || (*iter).triangles().size() <= 0 )
			continue;

		CFace &f = surf->newFace();
		f.faceType(FACETYPE_TRIANGLES);
		
		std::vector<IndexType> &strip = f.indices();
		strip = (*iter).triangles();

		for ( std::vector<IndexType>::iterator stripIter = strip.begin(); stripIter != strip.end(); stripIter++ ) {
			(*stripIter) -= offs;
		}
		
		insertParameters(f, faceIdx, obj.parameters(), obj.verts(), nverts, vertsOffs);

		f.sizes().resize(1);
		f.sizes()[0] = strip.size();
		
		if ( !f.floats(RI_N) ) {
			// Add normals
		}

		// Next polygon (faceIdx)
		offs += strip.size();
	}
	
	return surf;
}

// =============================================================================

CSurface *CParaboloidTriangulator::triangulate(CRiParaboloid &obj, unsigned long tessU, unsigned long tessV)
{
	return 0;
}
