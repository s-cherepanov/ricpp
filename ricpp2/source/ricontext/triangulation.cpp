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
		const CParameter &p = (*piter);
		f.insertConst(p);
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

	for ( RtInt face = 0; face < npolys; ++face ) {
		if ( obj.nVerts()[face] == 0 )
			continue;

		CFace &f = surf->newFace();
		f.faceType(FACETYPE_TRIANGLESTRIPS);
		
		RtInt nverts = obj.nVerts()[face];

		std::vector<IndexType> &strip = f.indices();
		triangleStrip(strip, nverts, vertsOffs);

		for ( CParameterList::const_iterator piter = obj.parameters().begin(); piter != obj.parameters().end(); piter++ ) {
			const CParameter &p = (*piter);

			const CDeclaration *decl = p.declarationPtr();
			if ( !decl )
				continue;

			switch ( decl->storageClass() ) {
					
				case CLASS_CONSTANT:
					f.insertConst(p);
					break;
					
				case CLASS_UNIFORM:
					f.insertUniform(p, face);
					break;
					
				case CLASS_VARYING:
					// NO break;
				case CLASS_VERTEX:
					f.insertVarying(p, nverts, &obj.verts()[vertsOffs]);
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
		
		f.sizes().resize(1);
		f.sizes()[0] = strip.size();

		if ( !f.floats(RI_N) ) {
			// Add normals
		}

		vertsOffs += obj.nVerts()[face];
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
	
	std::vector<IndexType> &idx = f.indices();
	idx = tp->triangles();
	
	for ( CParameterList::const_iterator piter = obj.parameters().begin(); piter != obj.parameters().end(); piter++ ) {
		const CParameter &p = (*piter);
		f.insertConst(p);
	}
	
	f.sizes().resize(1);
	f.sizes()[0] = idx.size();
	
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
	RtInt face = 0;
	RtInt nloops = 0;
	RtInt nverts = 0;
	RtInt nvertsOffs = 0;
	RtInt vertsOffs = 0;
	for (
		 std::vector<CTriangulatedPolygon>::const_iterator iter = tp.begin();
		 iter != tp.end();
		 iter++, face++, vertsOffs+=nverts )
	{
		nloops = obj.nLoops()[face];
		nverts = 0;
		for ( RtInt loopi = 0; loopi < nloops; ++loopi ) {
			nverts += obj.nVerts()[nvertsOffs++];
		}
		
		if ( nverts <= 0 || (*iter).triangles().size() <= 0 )
			continue;

		CFace &f = surf->newFace();
		f.faceType(FACETYPE_TRIANGLES);
		
		std::vector<IndexType> &idx = f.indices();
		idx = (*iter).triangles();

		for ( std::vector<IndexType>::iterator idxIter = idx.begin(); idxIter != idx.end(); idxIter++ ) {
			(*idxIter) -= offs;
		}
		
		for ( CParameterList::const_iterator piter = obj.parameters().begin(); piter != obj.parameters().end(); piter++ ) {
			const CParameter &p = (*piter);
			const CDeclaration *decl = p.declarationPtr();
			if ( !decl )
				continue;
			switch ( decl->storageClass() ) {

				case CLASS_CONSTANT: {
					f.insertConst(p);
					break;
				}

				case CLASS_UNIFORM: {
					f.insertUniform(p, face);
					break;
				}
					
				case CLASS_VARYING:
					// NO break;
				case CLASS_VERTEX:
					f.insertVarying(p, nverts, &obj.verts()[vertsOffs]);
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
		
		
		f.sizes().resize(1);
		f.sizes()[0] = idx.size();
		
		if ( !f.floats(RI_N) ) {
			// Add normals
		}

		// Next polygon (face)
		offs += idx.size();
	}
	
	return surf;
}
