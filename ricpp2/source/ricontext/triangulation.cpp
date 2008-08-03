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
// #define _TRACE_CONE
// #define _TRACE_CYLINDER
// #define _TRACE_PARABOLOID
// #define _TRACE_SPHERE
// #define _TRACE_DISK
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

struct SQuadricVars {
	IndexType nVars, realTessU, realTessV;
	RtFloat deltaU, deltaV;
	RtFloat flipNormal;
	std::vector<RtFloat> *positions, *normals;
};

static void getUnitCircle(std::vector<RtFloat> &circledata, IndexType tess, RtFloat thetamax, RtFloat thetamin=0)
{
	if ( thetamax < thetamin )
		std::swap(thetamax, thetamin);
	
	const RtFloat thetamaxrad = thetamax;
	RtFloat thetaminrad = thetamin;
	
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

static void initVars(
	const CDeclaration &pointDecl, const CDeclaration &normDecl,
	RtInt tessU, RtInt tessV,
	bool equalOrientations, CFace &f, SQuadricVars &retVals)
{
	retVals.realTessU = (IndexType)tessU;
	retVals.realTessV = (IndexType)tessV;
	
	retVals.deltaU = 0;
	retVals.deltaV = 0;
	
	retVals.flipNormal = equalOrientations ? 1.0 : -1.0;
	// flipNormal = 1.0;
	
	for ( int i = 0; i < 2; ++i ) {
		if ( retVals.realTessU + 1 < retVals.realTessU )
			retVals.realTessU -= 1;
		
		if ( retVals.realTessV + 1 < retVals.realTessV )
			retVals.realTessV -= 1;
		
		retVals.deltaU = (RtFloat)(1.0 / (RtFloat)retVals.realTessU);
		retVals.deltaV = (RtFloat)(1.0 / (RtFloat)retVals.realTessV);
		
		if ( (1.0 + retVals.deltaU) == 1.0 ) {
			retVals.deltaU = eps<RtFloat>();
			retVals.realTessU = (IndexType)(1.0/retVals.deltaU);
		}
		if ( (1.0 + retVals.deltaV) == 1.0 ) {
			retVals.deltaV = eps<RtFloat>();
			retVals.realTessV = (IndexType)(1.0/retVals.deltaV);
		}
	}
	
	retVals.nVars = ((IndexType)retVals.realTessU+1)*((IndexType)retVals.realTessV+1);
	
	std::vector<RtFloat> &p = f.insertFloatVar(pointDecl, retVals.nVars).value();
	std::vector<RtFloat> &n = f.insertFloatVar(normDecl, retVals.nVars).value();
	
	retVals.positions = &p;
	retVals.normals = &n;
}

static void buildConePN(RtFloat height, RtFloat radius, RtFloat thetamax, RtFloat displacement, const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f, const SQuadricVars &var)
{
	std::vector<RtFloat> unitcircle;
	getUnitCircle(unitcircle, var.realTessU, deg2rad(thetamax));
	
	RtFloat dz = getDelta(0, height, var.realTessV);
	
	RtFloat u, v, r, z;
	RtFloat n[3];
	
	IndexType nidx  = 0;
	IndexType pidx  = 0;
	IndexType puidx  = 0;
	
	RtFloat len = (RtFloat)sqrt(radius*radius+height*height);
	assert(len != 0);
	// assert len != 0 (because radius > 0)
	
	RtFloat mx = 0;
	RtFloat mz = 0;
	
	if ( len > 0 ) {
		mx = height/len;
		mz = radius/len; // (mx 0 mz) is the surface normal at theta = 0, mz is constant
	}
	
	IndexType uverts;
	IndexType vverts = var.realTessV+1;
	
	for ( z = displacement, v = 0.0; vverts > 0; z += dz, --vverts, v += var.deltaV ) {
		if ( v > 1.0 )
			v = 1.0;
		r = radius * ((RtFloat)1.0-v);
		if ( vverts == var.realTessV+1 ) {
			r = radius;
		}
		if ( vverts == 1 ) {
			r = 0.0;
			v = 1.0;
			z = height+displacement;
		}
		
		uverts = var.realTessU + 1;
		
		for ( u = 0.0, puidx=0; uverts > 0; --uverts, u+=var.deltaU ) {
			if ( u > 1.0 || uverts == 1 )
				u = 1.0;
			
			n[0] = unitcircle[puidx++];
			n[1] = unitcircle[puidx++];
			n[2] = mz;
			
			(*var.positions)[pidx++] = r * n[0];
			(*var.positions)[pidx++] = r * n[1];
			(*var.positions)[pidx++] = z;
			
			n[0] *= mx;
			n[1] *= mx;
			
			normalize3(n);
			
			(*var.normals)[nidx++] = n[0] * var.flipNormal;
			(*var.normals)[nidx++] = n[1] * var.flipNormal;
			(*var.normals)[nidx++] = n[2] * var.flipNormal;
		}
	}
}

// =============================================================================

CSurface *CQuadricTriangulator::triangulate(const CDeclaration &posDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations)
{
	if ( tessU < 0 || tessV < 0 || !posDecl.isFloat3Decl() || !normDecl.isFloat3Decl() )
		return 0;
	
	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	
	CFace &f = surf->newFace();
	
	buildPN(posDecl, normDecl, tessU, tessV, equalOrientations, f);
	
	f.buildStripIndices(tessU, tessV, true);
	return surf;
}

// =============================================================================

void CConeTriangulator::buildPN(const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f)
{
#ifdef _TRACE_CONE
	std::cout << "-> CConeTriangulator::buildPN()" << std::endl;
#endif
	assert(m_obj != 0);
	const RtFloat height = m_obj->height();
	const RtFloat radius = m_obj->radius();
	const RtFloat thetamax = m_obj->thetaMax();
	
	SQuadricVars var;
	initVars(pointDecl, normDecl, tessU, tessV, equalOrientations, f, var);
	buildConePN(height, radius, thetamax, 0, pointDecl, normDecl, tessU, tessV, equalOrientations, f, var);
		
#ifdef _TRACE_CONE
	std::cout << "<- CConeTriangulator::buildPN()" << std::endl;
#endif
}

void CCylinderTriangulator::buildPN(const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f)
{
#ifdef _TRACE_CYLINDER
	std::cout << "-> CCylinderTriangulator::buildPN()" << std::endl;
#endif
	assert(m_obj != 0);
	const RtFloat zmin = m_obj->zMin();
	const RtFloat zmax = m_obj->zMax();
	const RtFloat radius = m_obj->radius();
	const RtFloat thetamax = m_obj->thetaMax();
		
	SQuadricVars var;
	initVars(pointDecl, normDecl, tessU, tessV, equalOrientations, f, var);
	
	std::vector<RtFloat> unitcircle;
	getUnitCircle(unitcircle, var.realTessU, deg2rad(thetamax));

	RtFloat u, v, z;
	RtFloat pTempZ;
	
	IndexType vverts;
	IndexType uverts;
	
	IndexType puidx=0;
	IndexType pidx=0;
	
	RtFloat dz = getDeltaNotZero(zmin, zmax, var.realTessV);
	
	for ( vverts = var.realTessV+1, v = 0.0, z = zmin;
		  vverts > 0;
		  v += var.deltaV, --vverts, z += dz )
	{
		if ( v > 1.0 )
			v = 1.0;
		if ( vverts == 1 ) {
			v = 1.0;
			z = zmax;
		}
		if ( z > zmax )
			z = zmax;
		pTempZ   = z;
		
		puidx = 0;
		for (	uverts = var.realTessU+1, u = 0.0;
			 uverts > 0;
			 u += var.deltaU, --uverts)
		{
			if ( u > 1.0 || uverts == 1 ) {
				u = 1.0;
			}

			(*var.normals)[pidx] = var.flipNormal * unitcircle[puidx];
			(*var.positions)[pidx] = radius * unitcircle[puidx];
			
			++pidx;
			++puidx;
			
			(*var.normals)[pidx] = var.flipNormal * unitcircle[puidx];
			(*var.positions)[pidx] = radius * unitcircle[puidx];
			
			++pidx;
			++puidx;
			
			(*var.normals)[pidx] = 0.0;
			(*var.positions)[pidx] = pTempZ;
			
			++pidx;
		}
	}
	
#ifdef _TRACE_CYLINDER
	std::cout << "<- CCylinderTriangulator::buildPN()" << std::endl;
#endif
}

void CDiskTriangulator::buildPN(const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f)
{
#ifdef _TRACE_DISK
	std::cout << "-> CDiskTriangulator::buildPN()" << std::endl;
#endif
	assert(m_obj != 0);
	const RtFloat displacement = m_obj->height();
	const RtFloat radius = m_obj->radius();
	const RtFloat thetamax = m_obj->thetaMax();
	
	SQuadricVars var;
	initVars(pointDecl, normDecl, tessU, tessV, equalOrientations, f, var);
	buildConePN(0, radius, thetamax, displacement, pointDecl, normDecl, tessU, tessV, equalOrientations, f, var);
	
#ifdef _TRACE_DISK
	std::cout << "<- CDiskTriangulator::buildPN()" << std::endl;
#endif
}

void CParaboloidTriangulator::buildPN(const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f)
{
#ifdef _TRACE_PARABOLOID
	std::cout << "-> CParaboloidTriangulator::buildPN()" << std::endl;
#endif
	assert(m_obj != 0);
	const RtFloat zmin = m_obj->zMin();
	const RtFloat zmax = m_obj->zMax();
	const RtFloat rmax = m_obj->rMax();
	const RtFloat thetamax = m_obj->thetaMax();
	
	SQuadricVars var;
	initVars(pointDecl, normDecl, tessU, tessV, equalOrientations, f, var);
	
	std::vector<RtFloat> unitcircle;
	getUnitCircle(unitcircle, var.realTessU, deg2rad(thetamax));
	
	RtFloat u, v, r, nn, z;
	
	IndexType nidx  = 0;
	IndexType pidx  = 0;
	IndexType puidx  = 0;
	
	RtFloat ntemp[3];
	RtFloat m = zmax/(rmax*rmax); // 2D: f(x) = mx**2; F(x)=2mx
	
	RtFloat dz = getDeltaNotZero(zmin, zmax, var.realTessV);
	
	IndexType uverts;
	IndexType vverts = var.realTessV+1;
	
	for ( z = zmin, v = 0.0; vverts > 0; z += dz, --vverts, v += var.deltaV ) {
		if ( v > 1.0 )
			v = 1.0;
		if ( z > zmax )
			z = zmax;
		
		r = rmax * (RtFloat)sqrt(z / zmax);
		if ( vverts == 1 ) {
			r = rmax;
			v = 1.0;
			z = zmax;
		}
		
		nn = (RtFloat)(2.0*m*r);
		uverts = var.realTessU + 1;
		
		for ( u = 0.0, puidx=0; uverts > 0; --uverts, u+=var.deltaU ) {
			if ( u > 1.0 || uverts == 1 )
				u = 1.0;
			
			ntemp[0] = nn * unitcircle[puidx];
			ntemp[1] = nn * unitcircle[puidx+1];
			ntemp[2] = -1;

			(*var.positions)[pidx++] = r * unitcircle[puidx++];
			(*var.positions)[pidx++] = r * unitcircle[puidx++];
			(*var.positions)[pidx++] = z;
			
			normalize3(ntemp);
			
			(*var.normals)[nidx++] = var.flipNormal*ntemp[0];
			(*var.normals)[nidx++] = var.flipNormal*ntemp[1];
			(*var.normals)[nidx++] = var.flipNormal*ntemp[2];
		}
	}

#ifdef _TRACE_PARABOLOID
	std::cout << "<- CParaboloidTriangulator::buildPN()" << std::endl;
#endif
}


void CSphereTriangulator::buildPN(const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f)
{
#ifdef _TRACE_SPHERE
	std::cout << "-> CSphereTriangulator::buildPN()" << std::endl;
#endif
	assert(m_obj != 0);
	const RtFloat zmin = m_obj->zMin();
	const RtFloat zmax = m_obj->zMax();
	const RtFloat radius = m_obj->radius();
	const RtFloat thetamax = m_obj->thetaMax();
	
	RtFloat phimin;
	if ( zmin > -radius )
		phimin = (RtFloat)asin(zmin/radius);
	else
		phimin = -pi_2<RtFloat>();
	
	RtFloat phimax;
	if ( zmax < radius )
		phimax = (RtFloat)asin(zmax/radius);
	else
		phimax = pi_2<RtFloat>();

	SQuadricVars var;
	initVars(pointDecl, normDecl, tessU, tessV, equalOrientations, f, var);
	
	std::vector<RtFloat> unitcircleU;
	getUnitCircle(unitcircleU, var.realTessU, deg2rad(thetamax));

	std::vector<RtFloat> unitcircleV;
	getUnitCircle(unitcircleV, var.realTessV, phimax, phimin);
	
	
	RtFloat u, v, cosphi;
	RtFloat ptempZ, ntempZ, ntemp[3];
	
	IndexType vverts;
	IndexType uverts;
	
	IndexType pvidx=0;
	IndexType puidx=0;
	IndexType pidx=0;
	
	for (vverts = var.realTessV+1, v = 0.0;
		 vverts > 0;
		 v += var.deltaV, --vverts )
	{
		if ( v > 1.0 || vverts == 1 ) {
			v = 1.0;
		}
		
		cosphi = unitcircleV[pvidx++];
		ntemp[2] = unitcircleV[pvidx++];
		ptempZ   = radius * ntemp[2];
		
		puidx = 0;
		for (uverts = var.realTessU+1, u = 0.0;
			 uverts > 0;
			 u += var.deltaU, --uverts)
		{
			if ( u > 0 || uverts == 1 ) {
				u = 1.0;
			}
			
			ntemp[0] = cosphi * unitcircleU[puidx++];
			ntemp[1] = cosphi * unitcircleU[puidx++];

			(*var.positions)[pidx]   = radius * ntemp[0];;
			(*var.positions)[pidx+1] = radius * ntemp[1];
			(*var.positions)[pidx+2] = ptempZ;
			
			(*var.normals)[pidx]   = var.flipNormal * ntemp[0];
			(*var.normals)[pidx+1] = var.flipNormal * ntemp[1];
			(*var.normals)[pidx+2] = var.flipNormal * ntemp[2];
			
			normalize3(&(*var.normals)[pidx]);

			pidx+=3;
		}
	}
	
#ifdef _TRACE_SPHERE
	std::cout << "<- CSphereTriangulator::buildPN()" << std::endl;
#endif
}
