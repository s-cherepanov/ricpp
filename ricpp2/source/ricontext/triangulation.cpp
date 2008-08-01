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

#ifdef _DEBUG
#include <iostream>
// #define _TRACE_PARABOLOID
#endif

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

CQuadricTriangulator::getUnitCircle(std::vector<RtFloat> &circledata, IndexType tess, RtFloat thetamax, RtFloat thetamin)
{
	if ( thetamax < thetamin )
		std::swap(thetamax, thetamin);
	
	const RtFloat thetamaxrad = deg2rad(thetamax);
	RtFloat thetaminrad = deg2rad(thetamin);
	
	if ( tess < 1 ) tess = 1;

	RtFloat delta = (thetamaxrad-thetaminrad)/(RtFloat)tess;
	if ( nearlyZero(delta) )
		delta = eps<RtFloat>();
	
	IndexType nverts = tess+1;
		
	circledata.clear();
	circledata.resize(nverts*2);

	IndexType i;
	IndexType endIdx = (nverts-1)*2;
	for ( i=0; i<endIdx; thetaminrad+=delta) {
		if ( thetaminrad > thetamaxrad )
			thetaminrad = thetamaxrad;
		circledata[i++] = (RtFloat)cos(thetaminrad);
		assert(i<endIdx);
		circledata[i++] = (RtFloat)sin(thetaminrad);
	}
	circledata[i++] = (RtFloat)cos(thetamaxrad);
	circledata[i++] = (RtFloat)sin(thetamaxrad);
}

// =============================================================================
void CParaboloidTriangulator::buildPN(const CRiParaboloid &obj, const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, RtToken primitiveOrientation, CFace &f)
{
#ifdef _TRACE_PARABOLOID
	std::cout << "-> buildPN()" << std::endl;
#endif
	
	const RtFloat zmin = obj.zMin();
	const RtFloat zmax = obj.zMax();
	const RtFloat rmax = obj.rMax();
	const RtFloat thetamax = obj.thetaMax();
	
	IndexType realTessU = (IndexType)tessU;
	IndexType realTessV = (IndexType)tessV;

	RtFloat deltau = 0;
	RtFloat deltav = 0;
	
	RtFloat flipNormal = primitiveOrientation == RI_RH ? -1.0 : 1.0;
	// flipNormal = 1.0;

	for ( int i = 0; i < 2; ++i ) {
		if ( realTessU + 1 < realTessU )
			realTessU -= 1;
		
		if ( realTessV + 1 < realTessV )
			realTessV -= 1;
		
		RtFloat deltau = (RtFloat)(1.0 / (RtFloat)realTessU);
		RtFloat deltav = (RtFloat)(1.0 / (RtFloat)realTessV);
		
		if ( (1.0 + deltau) == 1.0 ) {
			deltau = eps<RtFloat>();
			realTessU = (IndexType)(1.0/deltau);
		}
		if ( (1.0 + deltav) == 1.0 ) {
			deltav = eps<RtFloat>();
			realTessV = (IndexType)(1.0/deltau);
		}
	}
	
	const IndexType nVar = ((IndexType)realTessU+1)*((IndexType)realTessV+1);

	
	std::vector<RtFloat> &p = f.insertFloatVar(pointDecl, nVar).value();
	std::vector<RtFloat> &n = f.insertFloatVar(normDecl, nVar).value();

	
	std::vector<RtFloat> unitcircle;
	getUnitCircle(unitcircle, realTessU, thetamax);
	
	
	RtFloat u, v, r, nn, z;
	
	IndexType nidx  = 0;
	IndexType pidx  = 0;
	IndexType puidx  = 0;
	
	RtFloat len;
	RtFloat ntemp[3];
	RtFloat m = zmax/(rmax*rmax); // 2D: f(x) = mx**2; F(x)=2mx
	
	RtFloat rangez = zmax - zmin;
	RtFloat dz = rangez / (RtFloat)realTessV;
	if ( nearlyZero(dz) )
		dz = eps<RtFloat>();
	
	IndexType uverts;
	IndexType vverts = realTessV+1;
	
	for ( z = zmin, v = 0.0; vverts > 0; z += dz, --vverts, v+=deltav ) {
		if ( z > zmax )
			z = zmax;
		
		r = rmax * (RtFloat)sqrt(z / zmax);
		if ( vverts == 1 ) {
			r = rmax;
			v = 1.0;
			z = zmax;
		}
		
		nn = (RtFloat)(2.0*m*r);
		len = 1.0;
		uverts = realTessU + 1;
		
		for ( u = 0.0, puidx=0; uverts > 0; --uverts, u+=deltau ) {
			if ( uverts == 1 )
				u = 1.0;
			
			ntemp[0] = nn * unitcircle[puidx];
			ntemp[1] = nn * unitcircle[puidx+1];
			ntemp[2] = -1;

			p[pidx++] = r * unitcircle[puidx++];
			p[pidx++] = r * unitcircle[puidx++];
			p[pidx++] = z;
			
			len = vlen3(ntemp);

			ntemp[0] /= len;
			ntemp[1] /= len;
			ntemp[2] /= len;
			
			n[nidx++] = flipNormal*ntemp[0];
			n[nidx++] = flipNormal*ntemp[1];
			n[nidx++] = flipNormal*ntemp[2];
		}
	}

#ifdef _TRACE_PARABOLOID
	std::cout << "<- buildPN()" << std::endl;
#endif
}


CSurface *CParaboloidTriangulator::triangulate(CRiParaboloid &obj, const CDeclaration &posDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, RtToken primitiveOrientation)
{
	if ( tessU < 0 || tessV < 0 || !posDecl.isFloat3Decl() || !normDecl.isFloat3Decl() )
		return 0;

	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}

	CFace &f = surf->newFace();

	buildPN(obj, posDecl, normDecl, tessU, tessV, primitiveOrientation, f);

	f.buildStripIndices(tessU, tessV);
	return surf;
}
