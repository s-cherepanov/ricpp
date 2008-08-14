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
#define _TRACE
// #define _TRACE_CONE
// #define _TRACE_CYLINDER
// #define _TRACE_DISK
// #define _TRACE_PARABOLOID
// #define _TRACE_SPHERE
// #define _TRACE_TORUS
#endif
#include "ricpp/tools/trace.h"

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

void CBasePolygonTriangulator::triangleStrip(std::vector<IndexType> &strip, IndexType nVerts, IndexType offs) const
{
	strip.clear();
	if ( nVerts == 0 )
		return;

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

void CBasePolygonTriangulator::insertParams(CFace &f, IndexType faceIdx, const CParameterList &plist, const std::vector<RtInt> &verts, IndexType nverts, IndexType vertsOffs)
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
			{
				assert(verts.size() >= nverts+vertsOffs);
				f.insertVarying(p, nverts, &verts[vertsOffs]);
			}
			break;
				
			case CLASS_FACEVARYING:
				// NO break;
			case CLASS_FACEVERTEX:
			{
				f.insertFaceVarying(p, nverts, vertsOffs);
			}
			break;
				
			default:
				break;
		}
	}
}

// =============================================================================

CSurface *CPolygonTriangulator::triangulate()
{
	RtInt nvertices = m_obj.nVertices();
	if ( nvertices == 0 )
		return 0;
	
	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	
	CFace &f = surf->newFace();
	f.faceType(FACETYPE_TRIANGLESTRIPS);
	
	std::vector<IndexType> &strip = f.indices();
	triangleStrip(strip, nvertices, 0);
	
	for ( CParameterList::const_iterator piter = m_obj.parameters().begin(); piter != m_obj.parameters().end(); piter++ ) {
		f.insertConst(*piter);
	}
	
	f.sizes().resize(1);
	f.sizes()[0] = static_cast<IndexType>(strip.size());
	
	if ( !f.floats(RI_N) ) {
		// Add normals
	}

	return surf;
}

// =============================================================================

CSurface *CPointsPolygonsTriangulator::triangulate()
{
	RtInt npolys = m_obj.nPolys();
	if ( npolys == 0 ) {
		return  0;
	}
	
	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	
	RtInt vertsOffs = 0;

	for ( RtInt faceIdx = 0; faceIdx < npolys; ++faceIdx ) {
		if ( m_obj.nVerts()[faceIdx] == 0 )
			continue;

		CFace &f = surf->newFace();
		f.faceType(FACETYPE_TRIANGLESTRIPS);
		
		RtInt nverts = m_obj.nVerts()[faceIdx];
		std::vector<IndexType> &strip = f.indices();

		triangleStrip(strip, nverts, vertsOffs);
		
		insertParams(f, faceIdx, m_obj.parameters(), m_obj.verts(), nverts, vertsOffs);
		f.sizes().resize(1);
		f.sizes()[0] = static_cast<IndexType>(strip.size());
		
		if ( !f.floats(RI_N) ) {
			// Add normals
		}

		vertsOffs += m_obj.nVerts()[faceIdx];
	}
	
	return surf;
}

// =============================================================================

CSurface *CGeneralPolygonTriangulator::triangulate()
{
	//  RtInt nloops, RtInt nverts[], const CParameterList &params

	if ( !m_tpPtr || m_tpPtr->triangles().size() <= 0 ) {
		return 0;
	}
	
	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	
	CFace &f = surf->newFace();
	f.faceType(FACETYPE_TRIANGLES);
	
	std::vector<IndexType> &strip = f.indices();
	strip = m_tpPtr->triangles();
	
	for ( CParameterList::const_iterator piter = m_obj.parameters().begin(); piter != m_obj.parameters().end(); piter++ ) {
		f.insertConst(*piter);
	}
	
	f.sizes().resize(1);
	f.sizes()[0] = static_cast<IndexType>(strip.size());
	
	if ( !f.floats(RI_N) ) {
		// Add normals
	}

	return surf;
}

// =============================================================================

CSurface *CPointsGeneralPolygonsTriangulator::triangulate()
{
	if ( !m_tpPtr || m_tpPtr->size() == 0 ) {
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
		 std::vector<CTriangulatedPolygon>::const_iterator iter = m_tpPtr->begin();
		 iter != m_tpPtr->end();
		 iter++, faceIdx++, vertsOffs+=nverts )
	{
		nloops = m_obj.nLoops()[faceIdx];
		nverts = 0;
		for ( RtInt loopi = 0; loopi < nloops; ++loopi ) {
			nverts += m_obj.nVerts()[nvertsOffs++];
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
		
		insertParams(f, faceIdx, m_obj.parameters(), m_obj.verts(), nverts, vertsOffs);

		f.sizes().resize(1);
		f.sizes()[0] = static_cast<IndexType>(strip.size());
		
		if ( !f.floats(RI_N) ) {
			// Add normals
		}

		// Next polygon (faceIdx)
		offs += static_cast<IndexType>(strip.size());
	}
	
	return surf;
}

// =============================================================================

struct SParametricVars {
	IndexType nVars, tessU, tessV;
	RtFloat deltaU, deltaV;
	RtFloat flipNormal;
	std::vector<RtFloat> *positions, *normals;

	SParametricVars(RtInt aTessU, RtInt aTessV, bool theEqualOrientations, CFace &aFace)
	{
		initVars(aTessU, aTessV, theEqualOrientations, aFace);
	}

	SParametricVars(
				  const CDeclaration &posDecl, const CDeclaration &normDecl,
				  RtInt aTessU, RtInt aTessV,
				  bool theEqualOrientations, CFace &aFace)
	{
		initVars(posDecl, normDecl, aTessU, aTessV, theEqualOrientations, aFace);
	}
	
	void initVars(RtInt aTessU, RtInt aTessV, bool theEqualOrientations, CFace &aFace)
	{
		tessU = (IndexType)aTessU;
		tessV = (IndexType)aTessV;
		
		deltaU = 0;
		deltaV = 0;
		
		flipNormal = theEqualOrientations ? (RtFloat)1.0 : (RtFloat)-1.0;
		
		for ( int i = 0; i < 2; ++i ) {
			if ( tessU + 1 < tessU )
				tessU -= 1;
			
			if ( tessV + 1 < tessV )
				tessV -= 1;
			
			deltaU = (RtFloat)(1.0 / (RtFloat)tessU);
			deltaV = (RtFloat)(1.0 / (RtFloat)tessV);
			
			if ( (1.0 + deltaU) == 1.0 ) {
				deltaU = eps<RtFloat>();
				tessU = (IndexType)(1.0/deltaU);
			}
			if ( (1.0 + deltaV) == 1.0 ) {
				deltaV = eps<RtFloat>();
				tessV = (IndexType)(1.0/deltaV);
			}
		}
		
		nVars = ((IndexType)tessU+1)*((IndexType)tessV+1);
	}

	void initVars(
						 const CDeclaration &posDecl, const CDeclaration &normDecl,
						 RtInt aTessU, RtInt aTessV,
						 bool theEqualOrientations, CFace &aFace)
	{
		initVars(aTessU, aTessV, theEqualOrientations, aFace);
		
		std::vector<RtFloat> &p = aFace.insertFloatVar(posDecl, nVars).values();
		std::vector<RtFloat> &n = aFace.insertFloatVar(normDecl, nVars).values();
		
		positions = &p;
		normals = &n;
	}
};

// =============================================================================

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


// =============================================================================

static void buildConePN(RtFloat height, RtFloat radius, RtFloat thetamax, RtFloat displacement, const CDeclaration &posDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f, const SParametricVars &var)
{
	std::vector<RtFloat> unitcircle;
	getUnitCircle(unitcircle, var.tessU, deg2rad(thetamax));
	
	RtFloat dz = getDelta(0, height, static_cast<RtFloat>(var.tessV));
	
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
	IndexType vverts = var.tessV+1;
	
	for ( z = displacement, v = 0.0; vverts > 0; z += dz, --vverts, v += var.deltaV ) {
		if ( v > 1.0 )
			v = 1.0;
		r = radius * ((RtFloat)1.0-v);
		if ( vverts == var.tessV+1 ) {
			r = radius;
		}
		if ( vverts == 1 ) {
			r = 0.0;
			v = 1.0;
			z = height+displacement;
		}
		
		uverts = var.tessU + 1;
		
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
			
			// normalize3(n);
			
			(*var.normals)[nidx++] = n[0] * var.flipNormal;
			(*var.normals)[nidx++] = n[1] * var.flipNormal;
			(*var.normals)[nidx++] = n[2] * var.flipNormal;
		}
	}
}


static void buildHyperboloidPN(RtPoint point1, RtPoint point2, RtFloat thetamax, const CDeclaration &posDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f, const SParametricVars &var)
{
	std::vector<RtFloat> unitcircle;
	getUnitCircle(unitcircle, var.tessU, deg2rad(thetamax));

	RtFloat u, v, costheta, sintheta;
	RtFloat p[3], p0[3], n[3];
	
	IndexType vverts;
	IndexType uverts;
	
	IndexType puidx=0;
	IndexType pidx=0;
	
	RtFloat xl = point2[0] - point1[0];
	RtFloat yl = point2[1] - point1[1];
	RtFloat zl = point2[2] - point1[2];
	
	RtFloat deltax = xl * var.deltaV;
	RtFloat deltay = yl * var.deltaV;
	RtFloat deltaz = zl * var.deltaV;
	
	p0[0] = point1[0];
	p0[1] = point1[1];
	p0[2] = point1[2];
	
	vverts = var.tessV+1;
	for ( v=0.0; vverts > 0; p0[0] += deltax, p0[1] += deltay, p0[2] += deltaz, --vverts, v += var.deltaV ) {
		if ( v > 1 || vverts == 1 ) {
			v = 1.0;
			p0[0] = point2[0];
			p0[1] = point2[1];
			p0[2] = point2[2];
		}
		uverts = var.tessU+1;
		
		for ( u = 0.0, puidx=0; uverts > 0; --uverts, u += var.tessU ) {
			if ( u > 1 || uverts == 1 ) {
				u = 1.0;
			}
			
			costheta = unitcircle[puidx];
			sintheta = unitcircle[++puidx];
			++puidx;
			
			p[0] = p0[0]*costheta - p0[1]*sintheta;
			p[1] = p0[0]*sintheta + p0[1]*costheta;
			p[2] = p0[2];
			
			n[0] = p[0];
			n[1] = p[1];
			n[2] = ((xl*costheta + yl*sintheta) * (p0[0]*costheta - p0[1]*sintheta) +
					(xl*sintheta - yl*costheta) * (p0[0]*sintheta + p0[1]*costheta));
			n[2] /= zl;
			
			// normalize3(n);
			
			(*var.positions)[pidx]   = p[0];
			(*var.positions)[pidx+1] = p[1];
			(*var.positions)[pidx+2] = p[2];
			
			(*var.normals)[pidx]   = n[0] * var.flipNormal;
			(*var.normals)[pidx+1] = n[1] * var.flipNormal;
			(*var.normals)[pidx+2] = n[2] * var.flipNormal;
			
			pidx += 3;
		}
	}
}

// =============================================================================

void CParametricTriangulator::insertParamsBilinear(IndexType faceIndex,
												   const IndexType (&cornerIdx)[4], const IndexType (&faceCornerIdx)[4],
												   RtInt tessU, RtInt tessV,
												   CFace &f)
{
	CParameterList::const_iterator iter = obj().parameters().begin();
	for ( ; iter != obj().parameters().end(); iter++ ) {
		if ( (*iter).var() == RI_P || (*iter).var() == RI_N ) {
			// Points and normals are not used because the are build in buildPN
			continue;
		}
		switch ( (*iter).declaration().storageClass() ) {
			case CLASS_CONSTANT:
				f.insertConst(*iter);
				break;
				
			case CLASS_UNIFORM:
				f.insertUniform(*iter, faceIndex);
				break;
				
			case CLASS_VARYING:
			case CLASS_VERTEX: {
					if ( (*iter).declaration().basicType() == BASICTYPE_FLOAT ) {
						f.bilinearBlend(*iter, cornerIdx, tessU, tessV);
					}
			}
			break;
				
			case CLASS_FACEVERTEX:
			case CLASS_FACEVARYING: {
				if ( (*iter).declaration().basicType() == BASICTYPE_FLOAT ) {
					f.bilinearBlend(*iter, faceCornerIdx, tessU, tessV);
				}
			}
			break;
				
			default:
			break;
		}
		
	}
}

void CParametricTriangulator::insertParamsBicubic(IndexType faceIndex,
												  const IndexType (&cornerIdx)[4], const IndexType (&faceCornerIdx)[4],
												  const IndexType (&controlIdx)[16], const IndexType (&faceControlIdx)[16],
												  RtInt tessU, RtInt tessV,
												  CFace &f)
{
	CParameterList::const_iterator iter = obj().parameters().begin();
	for ( ; iter != obj().parameters().end(); iter++ ) {
		if ( (*iter).var() == RI_P || (*iter).var() == RI_N ) {
			// Points and normals are not used because the are build in buildPN
			continue;
		}
		switch ( (*iter).declaration().storageClass() ) {
			case CLASS_CONSTANT:
				f.insertConst(*iter);
				break;
			case CLASS_UNIFORM:
				f.insertUniform(*iter, faceIndex);
				break;
			case CLASS_VARYING: {
				if ( (*iter).declaration().basicType() == BASICTYPE_FLOAT ) {
					f.bilinearBlend(*iter, cornerIdx, tessU, tessV);
				}
			}
			break;
				
			case CLASS_VERTEX: {
			}
			break;
				
			case CLASS_FACEVERTEX: {
			}
			break;
				
			case CLASS_FACEVARYING: {
				if ( (*iter).declaration().basicType() == BASICTYPE_FLOAT ) {
					f.bilinearBlend(*iter, cornerIdx, tessU, tessV);
				}
			}
			break;
				
			default:
			break;
		}
		
	}
}

// =============================================================================

void CQuadricTriangulator::insertParams(RtInt tessU, RtInt tessV, CFace &f)
{
	static const IndexType cornerIdx[4] = {0, 1, 2, 3};
	insertParamsBilinear(0, cornerIdx, cornerIdx, tessU, tessV, f);
}

CSurface *CQuadricTriangulator::triangulate(const CDeclaration &posDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, bool useStrips)
{
	if ( tessU < 0 || tessV < 0 || !posDecl.isFloat3Decl() || !normDecl.isFloat3Decl() )
		return 0;
	
	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	
	CFace &f = surf->newFace();
	
	buildPN(posDecl, normDecl, tessU, tessV, equalOrientations, f);
	insertParams(tessU, tessV, f);
		
	if ( useStrips )
		f.buildStripIndices(tessU, tessV, true);
	else
		f.buildTriangleIndices(tessU, tessV, true);

	return surf;
}

// =============================================================================

void CConeTriangulator::buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f)
{
#ifdef _TRACE_CONE
	std::cout << "-> CConeTriangulator::buildPN()" << std::endl;
#endif
	RtFloat height = m_obj.height();
	if ( nearlyZero(height) )
		return;
	
	RtFloat radius = m_obj.radius();
	if ( nearlyZero(radius) )
		return;
	
	RtFloat thetamax = m_obj.thetaMax();
	thetamax = clamp<RtFloat>(thetamax, -360, 360);
	if ( nearlyZero(thetamax) )
		return;
	
	SParametricVars var(posDecl, normDecl, tessU, tessV, equalOrientations, f);
	buildConePN(height, radius, thetamax, 0, posDecl, normDecl, tessU, tessV, equalOrientations, f, var);
		
#ifdef _TRACE_CONE
	std::cout << "<- CConeTriangulator::buildPN()" << std::endl;
#endif
}

void CCylinderTriangulator::buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f)
{
#ifdef _TRACE_CYLINDER
	std::cout << "-> CCylinderTriangulator::buildPN()" << std::endl;
#endif
	RtFloat zmin = m_obj.zMin();
	RtFloat zmax = m_obj.zMax();
	testMinMax(zmin, zmax);
	if ( nearlyZero(zmax-zmin) )
		return;
	
	RtFloat radius = m_obj.radius();
	if ( nearlyZero(radius) )
		return;
	
	RtFloat thetamax = m_obj.thetaMax();
	thetamax = clamp<RtFloat>(thetamax, -360, 360);
	if ( nearlyZero(thetamax) )
		return;
	
	SParametricVars var(posDecl, normDecl, tessU, tessV, equalOrientations, f);
	
	std::vector<RtFloat> unitcircle;
	getUnitCircle(unitcircle, var.tessU, deg2rad(thetamax));

	RtFloat u, v, z;
	RtFloat pTempZ;
	
	IndexType vverts;
	IndexType uverts;
	
	IndexType puidx=0;
	IndexType pidx=0;
	
	RtFloat dz = getDeltaNotZero(zmin, zmax, static_cast<RtFloat>(var.tessV));
	
	for ( vverts = var.tessV+1, v = 0.0, z = zmin;
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
		for (	uverts = var.tessU+1, u = 0.0;
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

void CDiskTriangulator::buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f)
{
#ifdef _TRACE_DISK
	std::cout << "-> CDiskTriangulator::buildPN()" << std::endl;
#endif
	RtFloat displacement = m_obj.height();

	RtFloat radius = m_obj.radius();
	if ( nearlyZero(radius) )
		return;
	
	RtFloat thetamax = m_obj.thetaMax();
	thetamax = clamp<RtFloat>(thetamax, -360, 360);
	if ( nearlyZero(thetamax) )
		return;
		   
	SParametricVars vars(posDecl, normDecl, tessU, tessV, equalOrientations, f);
	buildConePN(0, radius, thetamax, displacement, posDecl, normDecl, tessU, tessV, equalOrientations, f, vars);
	
#ifdef _TRACE_DISK
	std::cout << "<- CDiskTriangulator::buildPN()" << std::endl;
#endif
}

void CHyperboloidTriangulator::buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f)
{
#ifdef _TRACE_HYPERBOLOID
	std::cout << "-> CHyperboloidTriangulator::buildPN()" << std::endl;
#endif

	RtPoint point1, point2;
	
	memcpy(point1, m_obj.point1(), sizeof(RtPoint));
	memcpy(point2, m_obj.point2(), sizeof(RtPoint));

	if ( eqVect3(point1, point2) )
		return;
	
	RtFloat thetamax = m_obj.thetaMax();
	thetamax = clamp<RtFloat>(thetamax, -360, 360);
	if( nearlyZero(thetamax) )

		return;
	
	SParametricVars var(posDecl, normDecl, tessU, tessV, equalOrientations, f);
	buildHyperboloidPN(point1, point2, thetamax, posDecl, normDecl, tessU, tessV, equalOrientations, f, var);
	
#ifdef _TRACE_HYPERBOLOID
	std::cout << "<- CHyperboloidTriangulator::buildPN()" << std::endl;
#endif
}

void CParaboloidTriangulator::buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f)
{
#ifdef _TRACE_PARABOLOID
	std::cout << "-> CParaboloidTriangulator::buildPN()" << std::endl;
#endif
	RtFloat zmin = m_obj.zMin();
	RtFloat zmax = m_obj.zMax();
	if ( nearlyZero(zmax-zmin) )
		return;
	
	RtFloat rmax = m_obj.rMax();
	if ( nearlyZero(rmax) )
		return;
	
	RtFloat thetamax = m_obj.thetaMax();
	thetamax = clamp<RtFloat>(thetamax, -360, 360);
	if ( nearlyZero(thetamax) )
		return;

	SParametricVars var(posDecl, normDecl, tessU, tessV, equalOrientations, f);
	
	std::vector<RtFloat> unitcircle;
	getUnitCircle(unitcircle, var.tessU, deg2rad(thetamax));
	
	RtFloat u, v, r, nn, z;
	
	IndexType nidx  = 0;
	IndexType pidx  = 0;
	IndexType puidx  = 0;
	
	RtFloat ntemp[3];
	RtFloat m = zmax/(rmax*rmax); // 2D: f(x) = mx**2; F(x)=2mx
	
	RtFloat dz = getDeltaNotZero(zmin, zmax, static_cast<RtFloat>(var.tessV));
	
	IndexType uverts;
	IndexType vverts = var.tessV+1;
	
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
		uverts = var.tessU + 1;
		
		for ( u = 0.0, puidx=0; uverts > 0; --uverts, u+=var.deltaU ) {
			if ( u > 1.0 || uverts == 1 )
				u = 1.0;
			
			ntemp[0] = nn * unitcircle[puidx];
			ntemp[1] = nn * unitcircle[puidx+1];
			ntemp[2] = -1;

			(*var.positions)[pidx++] = r * unitcircle[puidx++];
			(*var.positions)[pidx++] = r * unitcircle[puidx++];
			(*var.positions)[pidx++] = z;
			
			// normalize3(ntemp);
			
			(*var.normals)[nidx++] = var.flipNormal*ntemp[0];
			(*var.normals)[nidx++] = var.flipNormal*ntemp[1];
			(*var.normals)[nidx++] = var.flipNormal*ntemp[2];
		}
	}

#ifdef _TRACE_PARABOLOID
	std::cout << "<- CParaboloidTriangulator::buildPN()" << std::endl;
#endif
}

void CSphereTriangulator::buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f)
{
#ifdef _TRACE_SPHERE
	std::cout << "-> CSphereTriangulator::buildPN()" << std::endl;
#endif

	RtFloat radius = m_obj.radius();
	if ( nearlyZero(radius) )
		return;

	RtFloat zmin = m_obj.zMin();
	clamp<RtFloat>(zmin, -radius, radius);
	RtFloat zmax = m_obj.zMax();
	clamp<RtFloat>(zmax, -radius, radius);

	if ( nearlyZero(zmax-zmin) )
		return;

	
	RtFloat thetamax = m_obj.thetaMax();
	thetamax = clamp<RtFloat>(thetamax, -360, 360);

	if ( nearlyZero(thetamax) )
		return;
	
	RtFloat phimin;
	phimin = (RtFloat)asin(zmin/radius);
	
	RtFloat phimax;
	phimax = (RtFloat)asin(zmax/radius);

	SParametricVars var(posDecl, normDecl, tessU, tessV, equalOrientations, f);
	
	std::vector<RtFloat> unitcircleU;
	getUnitCircle(unitcircleU, var.tessU, deg2rad(thetamax));

	std::vector<RtFloat> unitcircleV;
	if ( var.tessU == var.tessV && nearlyZero(phimin) && nearlyEqual(thetamax, phimax) )
		unitcircleV = unitcircleU;
	else
		getUnitCircle(unitcircleV, var.tessV, phimax, phimin);
	
	
	RtFloat u, v, cosphi;
	RtFloat ptempZ, ntemp[3];
	
	IndexType vverts;
	IndexType uverts;
	
	IndexType pvidx=0;
	IndexType puidx=0;
	IndexType pidx=0;
	
	for (vverts = var.tessV+1, v = 0.0;
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
		for (uverts = var.tessU+1, u = 0.0;
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
			
			// normalize3(&(*var.normals)[pidx]);

			pidx+=3;
		}
	}
	
#ifdef _TRACE_SPHERE
	std::cout << "<- CSphereTriangulator::buildPN()" << std::endl;
#endif
}

void CTorusTriangulator::buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f)
{
#ifdef _TRACE_TORUS
	std::cout << "-> CTorusTriangulator::buildPN()" << std::endl;
#endif
	
	RtFloat majorrad = m_obj.majorRad();
	RtFloat minorrad = m_obj.minorRad();
	if ( nearlyZero(minorrad) )
		return;
	
	RtFloat phimin = m_obj.phiMin();
	RtFloat phimax = m_obj.phiMax();
	if ( nearlyEqual(phimin, phimax) )
		return;

	RtFloat thetamax = m_obj.thetaMax();
	thetamax = clamp<RtFloat>(thetamax, -360, 360);
	if ( nearlyZero(thetamax) )
		return;

	SParametricVars var(posDecl, normDecl, tessU, tessV, equalOrientations, f);
	
	std::vector<RtFloat> unitcircleU;
	getUnitCircle(unitcircleU, var.tessU, deg2rad(thetamax));

	std::vector<RtFloat> unitcircleV;
	getUnitCircle(unitcircleV, var.tessV, deg2rad(phimax), deg2rad(phimin));

	RtFloat u, v, r, cosphi, sinphi, sintheta, costheta;
	RtFloat pTempZ;
	
	IndexType vverts;
	IndexType uverts;
	
	IndexType puidx=0;
	IndexType pvidx=0;
	IndexType pidx=0;
	
	vverts = var.tessV+1;
	
	for ( v = 0.0; vverts > 0; --vverts, v += var.deltaV )
	{
		if ( v > 1 || vverts == 1 ) {
			v = 1.0;
		}
		cosphi = unitcircleV[pvidx++];
		sinphi = unitcircleV[pvidx++]; // Z of the normals stays constant (Sin phi) for the same v
		
		r      = minorrad * cosphi;
		pTempZ = minorrad * sinphi;
		
		uverts = var.tessU + 1;

		for ( puidx = 0, u = 0.0; uverts > 0; --uverts, u += var.deltaU ) {
			if ( u > 1 || uverts == 1 ) {
				u = 1.0;
			}
			
			costheta = unitcircleU[puidx++];
			sintheta = unitcircleU[puidx++];
			
			(*var.positions)[pidx]   = (majorrad+r) * costheta;
			(*var.positions)[pidx+1] = (majorrad+r) * sintheta;
			(*var.positions)[pidx+2] = pTempZ;

			(*var.normals)[pidx]     = var.flipNormal * (cosphi * costheta);
			(*var.normals)[pidx+1]   = var.flipNormal * (cosphi * sintheta);
			(*var.normals)[pidx+2]   = var.flipNormal * sinphi;
			
			// normalize3(&(*var.normals)[pidx]);
			
			pidx += 3;
		}
	}

#ifdef _TRACE_TORUS
	std::cout << "<- CTorusTriangulator::buildPN()" << std::endl;
#endif
}


static bool extractRiPz(const CParameter &p, IndexType sizeU, IndexType sizeV, IndexType faceIndex, const IndexType cornerIdx[], std::vector<RtFloat> &retval)
{
	assert((sizeU == 2 && sizeV == 2)||(sizeU == 4 && sizeV == 4));

	if ( !p.declaration().isFloat1Decl()) {
		return false;
	}
	
	const std::vector<RtFloat> &val = p.floats();
	const RtFloat *t = 0;

	if ( sizeU == 2 && sizeV == 2 ) {
		static const RtFloat txy2[2] = {
			static_cast<RtFloat>(0.0),
			static_cast<RtFloat>(1.0)			
		};
		t = &txy2[0];
		
	} else if ( sizeU == 4 && sizeV == 4 ) {
		static const RtFloat txy4[4] = {
			static_cast<RtFloat>(0.0),
			static_cast<RtFloat>(1.0/3.0),
			static_cast<RtFloat>(2.0/3.0),
			static_cast<RtFloat>(1.0)			
		};
		t = &txy4[0];

	} else {
		return false;
	}

	if ( p.declaration().isConstant() ) {
		if ( val.size() < 1 )
			return false;
		for ( IndexType v = 0, i = 0; v < sizeV; ++v ) {
			for ( IndexType u = 0; u < sizeU; ++u ) {
				retval[i++] = t[u];
				retval[i++] = t[v];
				retval[i++] = val[0];
			}
		}
		return true;
	}
	if ( p.declaration().isUniform() ) {
		if ( (faceIndex+1) > val.size() )
			return false;
		for ( IndexType v = 0, i = 0; v < sizeV; ++v ) {
			for ( IndexType u = 0; u < sizeU; ++u ) {
				retval[i++] = t[u];
				retval[i++] = t[v];
				retval[i++] = val[faceIndex];
			}
		}
		return true;
	}
	
	for ( IndexType v = 0, i = 0; v < sizeV; ++v ) {
		for ( IndexType u = 0; u < sizeU; ++u ) {
			if ( cornerIdx[v*sizeU+u]+1 > val.size() )
				return false;
			retval[i++] = t[u];
			retval[i++] = t[v];
			retval[i++] = val[cornerIdx[v*sizeU+u]];
		}
	}
	return true;
}

static bool extractRiPw(const CParameter &p, IndexType sizeU, IndexType sizeV, IndexType faceIndex, const IndexType cornerIdx[], std::vector<RtFloat> &retval)
{
	IndexType size = sizeU * sizeV;
	
	if ( !p.declaration().isFloat4Decl()) {
		return false;
	}

	const std::vector<RtFloat> &val = p.floats();
	
	if ( p.declaration().isConstant() ) {
		if ( val.size() < 4 )
			return false;
		for ( IndexType i = 0; i < size; ++i ) {
			retval[i*4  ] = val[0];
			retval[i*4+1] = val[1];
			retval[i*4+2] = val[2];
			retval[i*4+3] = val[3];
		}
		return true;
	}
	if ( p.declaration().isUniform() ) {
		if ( (faceIndex+1)*3 > val.size() )
			return false;
		for ( IndexType i = 0; i < size; ++i ) {
			retval[i*4  ] = val[faceIndex*4];
			retval[i*4+1] = val[faceIndex*4+1];
			retval[i*4+2] = val[faceIndex*4+2];
			retval[i*4+2] = val[faceIndex*4+2];
		}
		return true;
	}

	for ( IndexType i = 0; i < size; ++i ) {
		if ( (cornerIdx[i]+1)*4 > val.size() )
			return false;
		retval[i*4  ] = val[cornerIdx[i]*4];
		retval[i*4+1] = val[cornerIdx[i]*4+1];
		retval[i*4+2] = val[cornerIdx[i]*4+2];
		retval[i*4+3] = val[cornerIdx[i]*4+2];
	}
	return true;
}

static bool extractRiP(const CParameter &p, IndexType sizeU, IndexType sizeV, IndexType faceIndex, const IndexType cornerIdx[], std::vector<RtFloat> &retval)
{
	IndexType size = sizeU * sizeV;
	assert(retval.size() >= size);

	if ( !p.declaration().isFloat3Decl()) {
		return false;
	}

	const std::vector<RtFloat> &val = p.floats();
	if ( p.declaration().isConstant() ) {
		if ( val.size() < 3 )
			return false;
		for ( IndexType i = 0; i < size; ++i ) {
			retval[i*3  ] = val[0];
			retval[i*3+1] = val[1];
			retval[i*3+2] = val[2];
		}
		return true;
	}
	if ( p.declaration().isUniform() ) {
		if ( (faceIndex+1)*3 > val.size() )
			return false;
		for ( IndexType i = 0; i < size; ++i ) {
			retval[i*3  ] = val[faceIndex*3];
			retval[i*3+1] = val[faceIndex*3+1];
			retval[i*3+2] = val[faceIndex*3+2];
		}
		return true;
	}
	for ( IndexType i = 0; i < size; ++i ) {
		if ( (cornerIdx[i]+1)*3 > val.size() )
			return false;
		retval[i*3  ] = val[cornerIdx[i]*3];
		retval[i*3+1] = val[cornerIdx[i]*3+1];
		retval[i*3+2] = val[cornerIdx[i]*3+2];
	}	
	return true;
}

static void pw2p(const std::vector<RtFloat> &pw, std::vector<RtFloat> &p)
{
	IndexType size = pw.size()/4;
	if ( p.size() < size )
		p.clear();
	p.resize(3*size);
	for ( IndexType i = 0; i < size; ++i ) {
		p[i*3  ] = pw[i*4  ]/pw[i*4+3];
		p[i*3+1] = pw[i*4+1]/pw[i*4+3];
		p[i*3+2] = pw[i*4+2]/pw[i*4+3];
	}
}

static void bilinearBlend(RtInt tessU, RtInt tessV, IndexType elemSize, const IndexType (&cornerIdx)[4], const std::vector<RtFloat> &vals, std::vector<RtFloat> &retvals)
{
	assert(tessU > 0);
	assert(tessV > 0);
	assert(elemSize > 0);
	
	retvals.clear();
	retvals.resize((tessU+1) * (tessV+1) * elemSize);
	
	RtFloat deltau = (RtFloat)(1.0/(RtFloat)(tessU));
	RtFloat deltav = (RtFloat)(1.0/(RtFloat)(tessV));
	
	RtFloat u, v;
	IndexType ui, vi, ei, idx;
	IndexType startPos, endPos;
	
	for ( v = (RtFloat)0.0, vi = 0; vi < (IndexType)tessV+1; ++vi, v += deltav ) {
		if ( v > 1.0 || vi == (IndexType)tessV ) {
			v = 1.0;
		}
		startPos = vi * ((IndexType)tessU + 1) * elemSize;
		endPos   = startPos + (IndexType)tessU * elemSize;
		assert(startPos != endPos);
		for ( ei = 0; ei < elemSize; ++ei ) {
			retvals[startPos+ei] = lerp(v, vals[cornerIdx[0]*elemSize+ei], vals[cornerIdx[2]*elemSize+ei]);
		}
		for ( ei = 0; ei < elemSize; ++ei ) {
			retvals[endPos+ei]   = lerp(v, vals[cornerIdx[1]*elemSize+ei], vals[cornerIdx[3]*elemSize+ei]);
		}
		idx = startPos+elemSize;
		for ( u = deltau, ui = 1; ui < (IndexType)tessU; ++ui, u += deltau ) {
			if ( u > 1.0 ) {
				u = 1.0;
			}
			for ( ei = 0; ei < elemSize; ++ei, ++idx ) {
				retvals[idx] = lerp(u, retvals[startPos+ei], retvals[endPos+ei]);
			}
		}
	}
}

// =============================================================================

void CRootPatchTriangulator::buildBilinearPN(const CDeclaration &posDecl,
											 const CDeclaration &normDecl,
											 RtInt tessU,
											 RtInt tessV,
											 bool equalOrientations,
											 IndexType faceIndex,
											 const IndexType (&cornerIdx)[4], const IndexType (&faceCornerIdx)[4],
											 CFace &f)
{
	static IndexType posCornerIdx[4] = {0, 1, 2, 3};

	const CParameter *p = obj().parameters().get(RI_P);
	const CParameter *pz = 0;
	const CParameter *pw = 0;
	
	std::vector<RtFloat> pos(16);
	
	if ( !p ) {
		pz = obj().parameters().get(RI_PZ);
		if ( !pz ) {
			pw = obj().parameters().get(RI_PW);
			if ( !pw ) {
				// No position information
				return;
			}
			if ( !extractRiPw(*pw, 2, 2, faceIndex, pw->declaration().isFace() ? faceCornerIdx : cornerIdx, pos) ) {
				return;
			}			
		} else {
			if ( !extractRiPz(*pz, 2, 2, faceIndex, pz->declaration().isFace() ? faceCornerIdx : cornerIdx, pos) ) {
				return;
			}
		}
	} else {
		if ( !extractRiP(*p, 2, 2, faceIndex, p->declaration().isFace() ? faceCornerIdx : cornerIdx, pos) ) {
			return;
		}
	}
		
	SParametricVars vars(posDecl, normDecl, tessU, tessV, equalOrientations, f);
	if ( !pw ) {
		bilinearBlend(tessU, tessV, 3, posCornerIdx, pos, *vars.positions);
	} else {
		std::vector<RtFloat> blendPw;
		bilinearBlend(tessU, tessV, 4, posCornerIdx, pos, blendPw);
		pw2p(blendPw, *vars.positions);
	}
	
	const CParameter *n = obj().parameters().get(RI_N);
	std::vector<RtFloat> nrm(12);
	if ( n ) {
		if ( !extractRiP(*p, 2, 2, faceIndex, p->declaration().isFace() ? faceCornerIdx : cornerIdx, nrm) ) {
			return;
		}
	} else {
		IndexType nc[4] = {0, (IndexType)tessU*3, (IndexType)(tessV*(tessU+1))*3, vars.positions->size()-3};
		if ( vars.flipNormal < 0.0 ) {
			plane<RtFloat>(&nrm[0], &(*vars.positions)[nc[2]], &(*vars.positions)[nc[0]], &(*vars.positions)[nc[1]]);
			plane<RtFloat>(&nrm[3], &(*vars.positions)[nc[0]], &(*vars.positions)[nc[1]], &(*vars.positions)[nc[3]]);
			plane<RtFloat>(&nrm[6], &(*vars.positions)[nc[3]], &(*vars.positions)[nc[2]], &(*vars.positions)[nc[0]]);
			plane<RtFloat>(&nrm[9], &(*vars.positions)[nc[1]], &(*vars.positions)[nc[3]], &(*vars.positions)[nc[2]]);
		} else {
			planeLH<RtFloat>(&nrm[0], &(*vars.positions)[nc[2]], &(*vars.positions)[nc[0]], &(*vars.positions)[nc[1]]);
			planeLH<RtFloat>(&nrm[3], &(*vars.positions)[nc[0]], &(*vars.positions)[nc[1]], &(*vars.positions)[nc[3]]);
			planeLH<RtFloat>(&nrm[6], &(*vars.positions)[nc[3]], &(*vars.positions)[nc[2]], &(*vars.positions)[nc[0]]);
			planeLH<RtFloat>(&nrm[9], &(*vars.positions)[nc[1]], &(*vars.positions)[nc[3]], &(*vars.positions)[nc[2]]);
		}
	}
	bilinearBlend(tessU, tessV, 3, posCornerIdx, nrm, *vars.normals);
}


void CRootPatchTriangulator::buildBicubicPN(const CDeclaration &posDecl,
											const CDeclaration &normDecl,
											RtInt tessU,
											RtInt tessV,
											bool equalOrientations,
											IndexType faceIndex,
											const IndexType (&cornerIdx)[4], const IndexType (&faceCornerIdx)[4],
											const IndexType (&controlIdx)[16], const IndexType (&faceControlIdx)[16],
											CFace &f)
{
	SParametricVars vars(posDecl, normDecl, tessU, tessV, equalOrientations, f);
}

// =============================================================================

CSurface *CPatchTriangulator::triangulate(const CDeclaration &posDecl,
										  const CDeclaration &normDecl,
										  RtInt tessU,
										  RtInt tessV,
										  bool equalOrientations,
										  bool useStrips)
{
	static IndexType cornerIdx[4] = {0, 1, 2, 3};
	static IndexType controlIdx[16] = {
		 0,  1,  2,  3,
		 4,  5,  6,  7,
		 8,  9, 10, 11,
		12, 13, 14, 15
	};
	
	RtToken type = m_obj.type();

	if ( type != RI_BILINEAR && type != RI_BICUBIC ) {
		/// @todo Errorhandling unknown Patch type
		return 0;
	}

	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	
	CFace &f = surf->newFace();

	if ( type == RI_BICUBIC ) {
		buildBicubicPN(posDecl, normDecl, tessU, tessV, equalOrientations, 0, cornerIdx, cornerIdx, controlIdx, controlIdx, f);
		insertParamsBicubic(0, cornerIdx, cornerIdx, controlIdx, controlIdx, tessU, tessV, f);
	} else {
		// type == RI_BILINEAR
		buildBilinearPN(posDecl, normDecl, tessU, tessV, equalOrientations, 0, cornerIdx, cornerIdx, f);
		insertParamsBilinear(0, cornerIdx, cornerIdx, tessU, tessV, f);
	}
	
	if ( useStrips )
		f.buildStripIndices(tessU, tessV, true);
	else
		f.buildTriangleIndices(tessU, tessV, true);

	return surf;
}

CSurface *CPatchMeshTriangulator::triangulate(const CDeclaration &posDecl,
											  const CDeclaration &normDecl,
											  RtInt tessU,
											  RtInt tessV,
											  bool equalOrientations,
											  bool useStrips)
{
	RtToken type = m_obj.type();

	if ( type != RI_BILINEAR && type != RI_BICUBIC ) {
		/// @todo Errorhandling unknown Patch type
		return 0;
	}
	
	if ( type == RI_BICUBIC ) {
	} else {
		// type == RI_BILINEAR
	}

	return 0;
}

