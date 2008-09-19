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
 *  @todo Special handling of homogeneous points: hpoint
 */

#include "ricpp/ricontext/triangulation.h"

#ifndef _RICPP_TOOLS_TEMPLATEFUNCS_H
#include "ricpp/tools/templatefuncs.h"
#endif // _RICPP_TOOLS_TEMPLATEFUNCS_H

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

#ifdef _DEBUG
#define _TRACE
// #define _TRACE_POLY
// #define _TRACE_CONE
// #define _TRACE_CYLINDER
// #define _TRACE_DISK
// #define _TRACE_PARABOLOID
// #define _TRACE_SPHERE
// #define _TRACE_TORUS
// #define _TRACE_NUPATCH
#endif

#ifndef _RICPP_TOOLS_TRACE_H
#include "ricpp/tools/trace.h"
#endif

using namespace RiCPP;


// =============================================================================


CTesselator::~CTesselator()
{
	detachPtr();
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
	for ( std::list<CSurface *>::iterator iter = m_surfaces.begin();
		 iter != m_surfaces.end();
		 iter++)
	{
		if ( !(*iter) )
			continue;
		if ( (*iter)->tessU() == tessU() && (*iter)->tessV() == tessV() ) {
			return (*iter);
		}
	}
	
	CSurface *s = new CSurface(tessU(), tessV());
	if ( !s )
		return 0;
	m_surfaces.push_back(s);
	return s;
}

void CTesselator::detachPtr()
{
	CVarParamRManInterfaceCall *aPtr = m_objPtr;
	m_objPtr = 0;
	if ( aPtr ) {
		aPtr->detach();
	}
}

void CTesselator::detach()
{
	detachPtr();
	delete this;
}

void CTesselator::attach(CVarParamRManInterfaceCall *anObjPtr)
{
	if ( anObjPtr == m_objPtr )
		return;
	detachPtr();
	m_objPtr = anObjPtr;
	if ( anObjPtr )
		anObjPtr->attach(this);
}

// =============================================================================

void CBasePolygonTesselator::triangles(IndexType nVerts, IndexType offs, std::vector<IndexType> &stripIdx) const
{
	IndexType tip, back, front, cnt;
	
	assert(nVerts >= 3 && stripIdx.size() >= 3);
	if ( nVerts < 3 )
		return;
	
	cnt = 0;
	tip = 0;
	front = 1;
	back = nVerts-1;

#ifdef _TRACE_POLY
	std::cout << stripIdx.size() << " " << nVerts << std::endl;
#endif

	while ( front < back ) {
		
#ifdef _TRACE_POLY
		std::cout << cnt << ": " << back << " " << tip << " " << front << std::endl;
#endif
		
		assert(cnt <= stripIdx.size()-3);
		if ( frontFaceCW() ) {
			stripIdx[cnt++] = offs + back;
			stripIdx[cnt++] = offs + tip;
			stripIdx[cnt++] = offs + front;
		} else {
			stripIdx[cnt++] = offs + front;
			stripIdx[cnt++] = offs + tip;
			stripIdx[cnt++] = offs + back;
		}

		tip = back--;
		if ( front >= back ) {
			assert(cnt == stripIdx.size());
			return;
		}
		assert(cnt <= stripIdx.size()-3);

#ifdef _TRACE_POLY
		std::cout << cnt << ": " << back << " "<<  tip << " " << front << std::endl;
#endif

		if ( frontFaceCW() ) {
			stripIdx[cnt++] = offs + back;
			stripIdx[cnt++] = offs + tip;
			stripIdx[cnt++] = offs + front;
		} else {
			stripIdx[cnt++] = offs + front;
			stripIdx[cnt++] = offs + tip;
			stripIdx[cnt++] = offs + back;
		}

		tip = front++;
	}
	assert(cnt == stripIdx.size());
	
}

void CBasePolygonTesselator::strip(IndexType nVerts, IndexType offs, std::vector<IndexType> &stripIdx) const
{
	IndexType i, j;
	stripIdx[0] = offs;
	if ( frontFaceCW() ) {
		for ( i=1, j=1; i < nVerts; ++j ) {
			stripIdx[i++] = offs+j;
			if ( i < nVerts ) {
				stripIdx[i++] = offs+nVerts-j;
			}
		}
	} else {
		for ( i=1, j=1; i < nVerts; ++j ) {
			stripIdx[i++] = offs+nVerts-j;
			if ( i < nVerts ) {
				stripIdx[i++] = offs+j;
			}
		}
	}
}

void CBasePolygonTesselator::buildIndices(IndexType nVerts, IndexType offs, CFace &f)
{	
	f.indices().clear();
	if ( nVerts == 0 ) {
		return;
	}
	
	if ( useStrips() ) {
		f.faceType(FACETYPE_TRIANGLESTRIPS);
		f.indices().resize(tmax((IndexType)3, nVerts));
	} else {
		f.faceType(FACETYPE_TRIANGLES);
		f.indices().resize((nVerts > 3) ? 3*(nVerts-2) : 3);
	}
	

	// Special cases

	if ( nVerts == 1 ) {
		f.indices()[0] = offs;
		f.indices()[1] = offs;
		f.indices()[2] = offs;
		return;
	}
	
	if ( nVerts == 2 ) {
		f.indices()[0] = offs;
		f.indices()[1] = offs+1;
		f.indices()[2] = offs;
		return;
	}
	
	// nVerts >= 3
	if ( useStrips() )
		strip(nVerts, offs, f.indices());
	else
		triangles(nVerts, offs, f.indices());
}

void CBasePolygonTesselator::insertParams(IndexType faceIdx, const CParameterList &plist, const std::vector<RtInt> &verts, IndexType nverts, IndexType vertsOffs, CFace &f)
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


bool CBasePolygonTesselator::addNormals(const CDeclaration &normDecl, CFace &f)
{
	const TemplPrimVar<RtFloat> *pp = f.floats(RI_P);
	if ( !pp )
		return false;

	if ( !pp->declaration().isFloat3Decl() ) {
		return false;
	}
	if ( !normDecl.isFloat3Decl() ) {
		return false;
	}


	const std::vector<RtFloat> &pos = pp->values();

	if ( pos.size() < 9 )
		return false;

//	std::vector<RtFloat> &n = f.insertFloatVar(normDecl, (IndexType)pos.size()/3).values();
	RtFloat aNormal[3];

	IndexType prev = (IndexType)pos.size()-3, cur = 0, next = 3;

	do {
		if ( planeLH(&aNormal[0], &pos[next], &pos[cur], &pos[prev]) ) {
			addNormalsToPos(normDecl, aNormal, pos, f);
			return true;
		}
		prev -= 3;
		next += 3;
	} while ( next < prev );


	return false;
}


void CBasePolygonTesselator::addNormalsToPos(const CDeclaration &normDecl, const RtNormal &aNormal, const std::vector<RtFloat> &pos, CFace &f)
{
	std::vector<RtFloat> &n = f.insertFloatVar(normDecl, (IndexType)pos.size()/3).values();
	
	IndexType cur = 0;
	assert(n.size() >= 3);
	
	while ( cur < n.size()-2 ) {
		if ( flipNormals() ) {
			n[cur++] = -aNormal[0];
			n[cur++] = -aNormal[1];
			n[cur++] = -aNormal[2];
		} else {
			n[cur++] = aNormal[0];
			n[cur++] = aNormal[1];
			n[cur++] = aNormal[2];
		}
	}
}

bool CBasePolygonTesselator::addNormals(const CDeclaration &normDecl, const RtNormal &aNormal, CFace &f)
{
	const TemplPrimVar<RtFloat> *pp = f.floats(RI_P);
	if ( !pp )
		return false;
	
	if ( !pp->declaration().isFloat3Decl() ) {
		return false;
	}
	if ( !normDecl.isFloat3Decl() ) {
		return false;
	}
		
	const std::vector<RtFloat> &pos = pp->values();

	addNormalsToPos(normDecl, aNormal, pos, f);
	return true;
}

// =============================================================================

CSurface *CPolygonTesselator::tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl)
{
	RtInt nvertices = m_obj->nVertices();
	if ( nvertices == 0 ) {
		return 0;
	}
	
	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	if ( !surf->empty() ) {
		return surf;
	}
	
	CFace &f = surf->newFace();
	buildIndices(nvertices, 0, f);
	
	for ( CParameterList::const_iterator piter = m_obj->parameters().begin(); piter != m_obj->parameters().end(); piter++ ) {
		f.insertConst(*piter);
	}
	
	f.sizes().resize(1);
	f.sizes()[0] = static_cast<IndexType>(f.indices().size());
	
	if ( !f.floats(RI_N) ) {
		addNormals(normDecl, f);
	}

	return surf;
}

// =============================================================================

CSurface *CPointsPolygonsTesselator::tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl)
{
	size_t npolys =  m_obj->nVerts().size();
	if ( npolys == 0 ) {
		return  0;
	}
	
	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	if ( !surf->empty() ) {
		return surf;
	}
	
	RtInt vertsOffs = 0;

	for ( IndexType faceIdx = 0; faceIdx < npolys; ++faceIdx ) {
		RtInt nverts = m_obj->nVerts()[faceIdx];
		if ( nverts == 0 )
			continue;

		CFace &f = surf->newFace();

		buildIndices(nverts, 0, f);
	
		insertParams(faceIdx, m_obj->parameters(), m_obj->verts(), nverts, vertsOffs, f);
		f.sizes().resize(1);
		f.sizes()[0] = static_cast<IndexType>(f.indices().size());
		
		if ( !f.floats(RI_N) ) {
			addNormals(normDecl, f);
		}

		vertsOffs += nverts;
	}
	return surf;
}

// =============================================================================

CSurface *CGeneralPolygonTesselator::tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl)
{
	//  RtInt nloops, RtInt nverts[], const CParameterList &params

	if ( !m_tpPtr || m_tpPtr->triangles().size() <= 0 ) {
		return 0;
	}
	
	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	if ( !surf->empty() ) {
		return surf;
	}
	
	CFace &f = surf->newFace();
	f.faceType(FACETYPE_TRIANGLES);
	
	std::vector<IndexType> &triangles = f.indices();
	triangles = m_tpPtr->triangles();
	
	for ( CParameterList::const_iterator piter = m_obj->parameters().begin(); piter != m_obj->parameters().end(); piter++ ) {
		f.insertConst(*piter);
	}
	
	f.sizes().resize(1);
	f.sizes()[0] = static_cast<IndexType>(triangles.size());
	
	if ( !f.floats(RI_N) ) {
		addNormals(normDecl, m_tpPtr->normal(), f);
	}

	return surf;
}

// =============================================================================

CSurface *CPointsGeneralPolygonsTesselator::tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl)
{
	if ( !m_tpPtr || m_tpPtr->size() == 0 ) {
		return 0;
	}
	
	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	if ( !surf->empty() ) {
		return surf;
	}

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
		nloops = m_obj->nLoops()[faceIdx];
		nverts = 0;
		for ( RtInt loopi = 0; loopi < nloops; ++loopi ) {
			nverts += m_obj->nVerts()[nvertsOffs++];
		}
		
		if ( nverts <= 0 || (*iter).triangles().size() <= 0 )
			continue;

		CFace &f = surf->newFace();
		f.faceType(FACETYPE_TRIANGLES);
		
		std::vector<IndexType> &triangles = f.indices();
		triangles = (*iter).triangles();
		// (*iter).drefTriangles(m_obj->vertsPtr(), triangles);

		try {
			insertParams(faceIdx, m_obj->parameters(), m_obj->verts(), nverts, vertsOffs, f);
		} catch ( std::exception &e1 ) {
			throw ExceptRiCPPError(RIE_BUG, RIE_ERROR, __LINE__, __FILE__, "Error in 'CPointsGeneralPolygonsTesselator::tesselate()' at 'insertParams()': %s", e1.what());
		} catch ( ... ) {
			throw ExceptRiCPPError(RIE_BUG, RIE_ERROR, __LINE__, __FILE__, "Error in 'CPointsGeneralPolygonsTesselator::tesselate()' at 'insertParams()': %s", RI_UNKNOWN);
		}
		
		f.sizes().resize(1);
		f.sizes()[0] = static_cast<IndexType>(triangles.size());
		
		if ( !f.floats(RI_N) ) {
			try {
				addNormals(normDecl, (*iter).normal(), f);
			} catch ( std::exception &e1 ) {
				throw ExceptRiCPPError(RIE_BUG, RIE_ERROR, __LINE__, __FILE__, "Error in 'CPointsGeneralPolygonsTesselator::tesselate()' at 'addNormals()': %s", e1.what());
			} catch ( ... ) {
				throw ExceptRiCPPError(RIE_BUG, RIE_ERROR, __LINE__, __FILE__, "Error in 'CPointsGeneralPolygonsTesselator::tesselate()' at 'addNormals()': %s", RI_UNKNOWN);
			}
		}
	}
	
	return surf;
}

// =============================================================================

void CUnitCircle::calc()
{
	if ( m_tess < 1 ) m_tess = 1;
	validateMinMax(m_thetaMin, m_thetaMax);
	
	RtFloat theta = m_thetaMin;
	
	RtFloat deltaVal = delta<RtFloat>(theta, m_thetaMax, static_cast<RtFloat>(m_tess));
	
	IndexType nverts = m_tess+1;
		
	m_circleData.clear();
	m_circleData.resize(nverts*2);

	IndexType i;
	IndexType endIdx = (nverts-1)*2;
	for ( i = 0; i < endIdx; theta += deltaVal ) {
		if ( theta > m_thetaMax )
			theta = m_thetaMax;
		m_circleData[i++] = (RtFloat)cos(theta);
		assert(i<endIdx);
		m_circleData[i++] = (RtFloat)sin(theta);
	}

	assert(i < nverts*2);
	m_circleData[i++] = (RtFloat)cos(m_thetaMax);
	assert(i < nverts*2);
	m_circleData[i++] = (RtFloat)sin(m_thetaMax);
}


// =============================================================================

void CParametricTesselator::SParametricVars::initVars(IndexType aTessU, IndexType aTessV, bool aFlipNormal, CFace &aFace)
{
	IndexType tessU = aTessU;
	IndexType tessV = aTessV;
	
	deltaU = 0;
	deltaV = 0;
	
	flipNormal = aFlipNormal ? (RtFloat)-1.0 : (RtFloat)1.0;
	
	for ( int i = 0; i < 2; ++i ) {
		if ( tessU + 1 < tessU )
			tessU -= 1;
		
		if ( tessV + 1 < tessV )
			tessV -= 1;
		
		deltaU = deltaNotZero<RtFloat>(0, 1, static_cast<RtFloat>(tessU));
		deltaV = deltaNotZero<RtFloat>(0, 1, static_cast<RtFloat>(tessV));
	}
	
	nVars = (tessU+1)*(tessV+1);
}

void CParametricTesselator::SParametricVars::initVars(const CDeclaration &posDecl, const CDeclaration &normDecl,
													  IndexType aTessU, IndexType aTessV, bool aFlipNormal, CFace &aFace)
{
	initVars(aTessU, aTessV, aFlipNormal, aFace);
	
	std::vector<RtFloat> &p = aFace.insertFloatVar(posDecl, nVars).values();
	std::vector<RtFloat> &n = aFace.insertFloatVar(normDecl, nVars).values();
	
	positions = &p;
	normals = &n;
}

void CParametricTesselator::insertBilinearParams(IndexType faceIndex,
												 const IndexType (&cornerIdx)[4],
												 const IndexType (&faceCornerIdx)[4],
												 CFace &f)
{
	CParameterList::const_iterator iter = obj().parameters().begin();
	for ( ; iter != obj().parameters().end(); iter++ ) {
		if ( (*iter).var() == RI_P || (*iter).var() == RI_N ) {
			// Points and normals are not used because they are build in buildPN
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
						f.bilinearBlend(*iter, cornerIdx);
					}
			}
			break;
				
			case CLASS_FACEVERTEX:
			case CLASS_FACEVARYING: {
				if ( (*iter).declaration().basicType() == BASICTYPE_FLOAT ) {
					f.bilinearBlend(*iter, faceCornerIdx);
				}
			}
			break;
				
			default:
			break;
		}
		
	}
}

void CParametricTesselator::getStdCornerIdx(IndexType offset, IndexType (&idx)[4]) const
{
	// 0, 1, (LH)
	// 2, 3
	
	// Winding will be done by indices
//	if ( frontFaceCW() ) {
	
		idx[0] = offset;
		idx[1] = offset+1;
		idx[2] = offset+2;
		idx[3] = offset+3;
	
//	} else {
	
	/*
		idx[0] = offset+2;
		idx[1] = offset+3;
		idx[2] = offset;
		idx[3] = offset+1;
	 */
	
//	}
}

/*
void CParametricTesselator::getCornerIdx(IndexType upatch, IndexType vpatch, IndexType nu, IndexType nv, IndexType (&idx)[4]) const
{
	idx[0] = (vpatch % nv)       * nu + (upatch % nu);
	idx[1] = (vpatch % nv)       * nu + ((upatch + 1) % nu);
	idx[2] = ((vpatch + 1) % nv) * nu + (upatch % nu);
	idx[3] = ((vpatch + 1) % nv) * nu + ((upatch + 1) % nu);
}
*/

void CParametricTesselator::getCornerIdx(IndexType upatch, IndexType vpatch, IndexType nuPatches, IndexType nvPatches, IndexType (&idx)[4]) const
{
	IndexType nu = nuPatches+1;
	IndexType nv = nvPatches+1;
	idx[0] = (vpatch % nv)       * nu + (upatch % nu);
	idx[1] = (vpatch % nv)       * nu + ((upatch + 1) % nu);
	idx[2] = ((vpatch + 1) % nv) * nu + (upatch % nu);
	idx[3] = ((vpatch + 1) % nv) * nu + ((upatch + 1) % nu);
}

void CParametricTesselator::getFaceCornerIdx(IndexType faceIndex, IndexType (&idx)[4]) const
{
	getStdCornerIdx(faceIndex * 4, idx); // 4 is the pathchsize (2x2)
}

void CParametricTesselator::getFaceCornerIdx(IndexType upatch, IndexType vpatch, IndexType nu, IndexType nv, IndexType (&idx)[4]) const
{
	IndexType faceIndex = (vpatch * nu + upatch);
	getFaceCornerIdx(faceIndex, idx);
}

void CParametricTesselator::buildIndices(CFace &f)
{
	//	if ( useTriangles() ) {
	if ( useStrips() ) {
		f.faceType(FACETYPE_TRIANGLESTRIPS);
		f.buildStripIndices(frontFaceCW());
	} else {
		f.faceType(FACETYPE_TRIANGLES);
		f.buildTriangleIndices(frontFaceCW());
	}
	//	} else {
	/** @todo quads and quad strips
	 */
	//	}
}

// =============================================================================

void CQuadricTesselator::insertParams(CFace &f)
{
	IndexType cornerIdx[4];
	getStdCornerIdx(0, cornerIdx);
	insertBilinearParams(0, cornerIdx, cornerIdx, f);
}

void CQuadricTesselator::buildConePN(RtFloat height, RtFloat radius, RtFloat thetamax, RtFloat displacement, const CDeclaration &posDecl, const CDeclaration &normDecl, const SParametricVars &var, CFace &f)
{
	CUnitCircle unitcircle(tessU(), deg2rad(thetamax), 0);
	
	RtFloat dz = delta<RtFloat>(0, height, static_cast<RtFloat>(tessV()));
	
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
	IndexType vverts = tessV()+1;
	
	for ( z = displacement, v = 0.0; vverts > 0; z += dz, --vverts, v += var.deltaV ) {
		if ( v > 1.0 )
			v = 1.0;
		r = radius * ((RtFloat)1.0-v);
		if ( vverts == (IndexType)tessV()+1 ) {
			r = radius;
		}
		if ( vverts == 1 ) {
			r = 0.0;
			v = 1.0;
			z = height+displacement;
		}
		
		uverts = tessU() + 1;
		
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


void CQuadricTesselator::buildHyperboloidPN(RtPoint point1, RtPoint point2, RtFloat thetamax, const CDeclaration &posDecl, const CDeclaration &normDecl, const SParametricVars &var, CFace &f)
{
	CUnitCircle unitcircle(tessU(), deg2rad(thetamax), 0);
	
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
	
	vverts = tessV()+1;
	for ( v=0.0; vverts > 0; p0[0] += deltax, p0[1] += deltay, p0[2] += deltaz, --vverts, v += var.deltaV ) {
		if ( v > 1 || vverts == 1 ) {
			v = 1.0;
			p0[0] = point2[0];
			p0[1] = point2[1];
			p0[2] = point2[2];
		}
		uverts = tessU()+1;
		
		for ( u = 0.0, puidx=0; uverts > 0; --uverts, u += tessU() ) {
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

CSurface *CQuadricTesselator::tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl)
{
	assert(posDecl.isFloat3Decl());
	assert(normDecl.isFloat3Decl());
	
	if ( !posDecl.isFloat3Decl() || !normDecl.isFloat3Decl() )
		return 0;
	
	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	if ( !surf->empty() ) {
		return surf;
	}
	
	CFace &f = surf->newFace(tessU(), tessV());
	
	buildPN(posDecl, normDecl, f);
	insertParams(f);
	buildIndices(f);
	
	return surf;
}

// =============================================================================

void CConeTesselator::buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, CFace &f)
{
#ifdef _TRACE_CONE
	std::cout << "-> CConeTesselator::buildPN()" << std::endl;
#endif
	RtFloat height = m_obj->height();
	if ( nearlyZero(height) )
		return;
	
	RtFloat radius = m_obj->radius();
	if ( nearlyZero(radius) )
		return;
	
	RtFloat thetamax = m_obj->thetaMax();
	thetamax = clamp<RtFloat>(thetamax, -360, 360);
	if ( nearlyZero(thetamax) )
		return;
	
	SParametricVars var(posDecl, normDecl, tessU(), tessV(), flipNormals(), f);
	buildConePN(height, radius, thetamax, 0, posDecl, normDecl, var, f);
		
#ifdef _TRACE_CONE
	std::cout << "<- CConeTesselator::buildPN()" << std::endl;
#endif
}

void CCylinderTesselator::buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, CFace &f)
{
#ifdef _TRACE_CYLINDER
	std::cout << "-> CCylinderTesselator::buildPN()" << std::endl;
#endif
	RtFloat zmin = m_obj->zMin();
	RtFloat zmax = m_obj->zMax();
	validateMinMax(zmin, zmax);
	if ( nearlyZero(zmax-zmin) )
		return;
	
	RtFloat radius = m_obj->radius();
	if ( nearlyZero(radius) )
		return;
	
	RtFloat thetamax = m_obj->thetaMax();
	thetamax = clamp<RtFloat>(thetamax, -360, 360);
	if ( nearlyZero(thetamax) )
		return;
	
	SParametricVars var(posDecl, normDecl, tessU(), tessV(), flipNormals(), f);
	
	CUnitCircle unitcircle(tessU(), deg2rad(thetamax), 0);
	RtFloat u, v, z;
	RtFloat pTempZ;
	
	IndexType vverts;
	IndexType uverts;
	
	IndexType puidx=0;
	IndexType pidx=0;
	
	RtFloat dz = deltaNotZero<RtFloat>(zmin, zmax, static_cast<RtFloat>(tessV()));
	
	for ( vverts = tessV()+1, v = 0.0, z = zmin;
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
		for (	uverts = tessU()+1, u = 0.0;
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
	std::cout << "<- CCylinderTesselator::buildPN()" << std::endl;
#endif
}

void CDiskTesselator::buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, CFace &f)
{
#ifdef _TRACE_DISK
	std::cout << "-> CDiskTesselator::buildPN()" << std::endl;
#endif
	RtFloat displacement = m_obj->height();

	RtFloat radius = m_obj->radius();
	if ( nearlyZero(radius) )
		return;
	
	RtFloat thetamax = m_obj->thetaMax();
	thetamax = clamp<RtFloat>(thetamax, -360, 360);
	if ( nearlyZero(thetamax) )
		return;
		   
	SParametricVars vars(posDecl, normDecl, tessU(), tessV(), flipNormals(), f);
	buildConePN(0, radius, thetamax, displacement, posDecl, normDecl, vars, f);
	
#ifdef _TRACE_DISK
	std::cout << "<- CDiskTesselator::buildPN()" << std::endl;
#endif
}

void CHyperboloidTesselator::buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, CFace &f)
{
#ifdef _TRACE_HYPERBOLOID
	std::cout << "-> CHyperboloidTesselator::buildPN()" << std::endl;
#endif

	RtPoint point1, point2;
	
	memcpy(point1, m_obj->point1(), sizeof(RtPoint));
	memcpy(point2, m_obj->point2(), sizeof(RtPoint));

	if ( eqVect3(point1, point2) )
		return;
	
	RtFloat thetamax = m_obj->thetaMax();
	thetamax = clamp<RtFloat>(thetamax, -360, 360);
	if( nearlyZero(thetamax) )

		return;
	
	SParametricVars var(posDecl, normDecl, tessU(), tessV(), flipNormals(), f);
	buildHyperboloidPN(point1, point2, thetamax, posDecl, normDecl, var, f);
	
#ifdef _TRACE_HYPERBOLOID
	std::cout << "<- CHyperboloidTesselator::buildPN()" << std::endl;
#endif
}

void CParaboloidTesselator::buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, CFace &f)
{
#ifdef _TRACE_PARABOLOID
	std::cout << "-> CParaboloidTesselator::buildPN()" << std::endl;
#endif
	RtFloat zmin = m_obj->zMin();
	RtFloat zmax = m_obj->zMax();
	if ( nearlyEqual(zmin, zmax) )
		return;
	
	RtFloat rmax = m_obj->rMax();
	if ( nearlyZero(rmax) )
		return;
	
	RtFloat thetamax = m_obj->thetaMax();
	thetamax = clamp<RtFloat>(thetamax, -360, 360);
	if ( nearlyZero(thetamax) )
		return;

	SParametricVars var(posDecl, normDecl, tessU(), tessV(), flipNormals(), f);
	
	CUnitCircle unitcircle(tessU(), deg2rad(thetamax), 0);
	
	RtFloat u, v, r, nn, z;
	
	IndexType nidx  = 0;
	IndexType pidx  = 0;
	IndexType puidx  = 0;
	
	RtFloat ntemp[3];
	RtFloat m = zmax/(rmax*rmax); // 2D: f(x) = mx**2; F(x)=2mx
	
	RtFloat dz = deltaNotZero<RtFloat>(zmin, zmax, static_cast<RtFloat>(tessV()));
	
	IndexType uverts;
	IndexType vverts = tessV()+1;
	
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
		uverts = tessU() + 1;
		
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
	std::cout << "<- CParaboloidTesselator::buildPN()" << std::endl;
#endif
}

void CSphereTesselator::buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, CFace &f)
{
#ifdef _TRACE_SPHERE
	std::cout << "-> CSphereTesselator::buildPN()" << std::endl;
#endif

	RtFloat radius = m_obj->radius();
	if ( nearlyZero(radius) )
		return;

	RtFloat zmin = m_obj->zMin();
	clamp<RtFloat>(zmin, -radius, radius);
	RtFloat zmax = m_obj->zMax();
	clamp<RtFloat>(zmax, -radius, radius);

	if ( nearlyZero(zmax-zmin) )
		return;

	
	RtFloat thetamax = m_obj->thetaMax();
	thetamax = clamp<RtFloat>(thetamax, -360, 360);

	if ( nearlyZero(thetamax) )
		return;
	
	RtFloat phimin;
	phimin = (RtFloat)asin(zmin/radius);
	
	RtFloat phimax;
	phimax = (RtFloat)asin(zmax/radius);

	SParametricVars var(posDecl, normDecl, tessU(), tessV(), flipNormals(), f);
	
	CUnitCircle unitcircleU(tessU(), deg2rad(thetamax), 0);
	CUnitCircle unitcircleV(tessU(), phimax, phimin);
		
	RtFloat u, v, cosphi;
	RtFloat ptempZ, ntemp[3];
	
	IndexType vverts;
	IndexType uverts;
	
	IndexType pvidx=0;
	IndexType puidx=0;
	IndexType pidx=0;
	
	for (vverts = tessV()+1, v = 0.0;
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
		for (uverts = tessU()+1, u = 0.0;
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
	std::cout << "<- CSphereTesselator::buildPN()" << std::endl;
#endif
}

void CTorusTesselator::buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, CFace &f)
{
#ifdef _TRACE_TORUS
	std::cout << "-> CTorusTesselator::buildPN()" << std::endl;
#endif
	
	RtFloat majorrad = m_obj->majorRad();
	RtFloat minorrad = m_obj->minorRad();
	if ( nearlyZero(minorrad) )
		return;
	
	RtFloat phimin = m_obj->phiMin();
	RtFloat phimax = m_obj->phiMax();
	if ( nearlyEqual(phimin, phimax) )
		return;

	RtFloat thetamax = m_obj->thetaMax();
	thetamax = clamp<RtFloat>(thetamax, -360, 360);
	if ( nearlyZero(thetamax) )
		return;

	SParametricVars var(posDecl, normDecl, tessU(), tessV(), flipNormals(), f);
	
	CUnitCircle unitcircleU(tessU(), deg2rad(thetamax), 0);
	CUnitCircle unitcircleV(tessU(), deg2rad(phimax), deg2rad(phimin));

	RtFloat u, v, r, cosphi, sinphi, sintheta, costheta;
	RtFloat pTempZ;
	
	IndexType vverts;
	IndexType uverts;
	
	IndexType puidx=0;
	IndexType pvidx=0;
	IndexType pidx=0;
	
	vverts = tessV()+1;
	
	for ( v = 0.0; vverts > 0; --vverts, v += var.deltaV )
	{
		if ( v > 1 || vverts == 1 ) {
			v = 1.0;
		}
		cosphi = unitcircleV[pvidx++];
		sinphi = unitcircleV[pvidx++]; // Z of the normals stays constant (Sin phi) for the same v
		
		r      = minorrad * cosphi;
		pTempZ = minorrad * sinphi;
		
		uverts = tessU() + 1;

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
	std::cout << "<- CTorusTesselator::buildPN()" << std::endl;
#endif
}

// =============================================================================

static void pw2p(const std::vector<RtFloat> &pw, std::vector<RtFloat> &p)
{
	IndexType size = (IndexType)pw.size()/4;
	if ( p.size() != size ) {
		p.clear();
		p.resize(3*size);
	}
	for ( IndexType i = 0; i < size; ++i ) {
		p[i*3  ] = pw[i*4  ]/pw[i*4+3];
		p[i*3+1] = pw[i*4+1]/pw[i*4+3];
		p[i*3+2] = pw[i*4+2]/pw[i*4+3];
	}
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

static bool extractRi3F(const CParameter &p, IndexType sizeU, IndexType sizeV, IndexType faceIndex, const IndexType cornerIdx[], std::vector<RtFloat> &retval)
{
	// Can be used for RI_P and RI_N
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

// =============================================================================

void CRootPatchTesselator::getStdControlIdx(IndexType offset, IndexType (&idx)[16]) const
{
	//  0,  1,  2,  3, (LH)
	//  4,  5,  6,  7,
	//  8,  9, 10, 11,
	// 12, 13, 14, 15
	
	// Winding will be done by indices
	//	if ( frontFaceCW() ) {
	
	for ( IndexType i = 0; i < 16; ++i ) {
		idx[i] = offset+i;
	}
	
	// } else {
	/*
	 IndexType i = 0;
	 for ( IndexType v = 4; v > 0; ) {
	 for ( IndexType u = 0; u < 4; ++u, ++i ) {
	 --v;
	 idx[i] = offset+v*4+u;
	 }
	 }
	 */
	//	}
}

void CRootPatchTesselator::getControlIdx(IndexType upatch, IndexType vpatch, IndexType nu, IndexType nv, IndexType ustep, IndexType vstep, IndexType (&idx)[16]) const
{
	for ( int v = 0; v < 4; ++v ) {
		for ( int u = 0; u < 4; ++u ) {
			idx[v*4+u] = ((vpatch*vstep+v)%nv)*nu + (upatch*ustep+u)%nu;
		}
	}
}

void CRootPatchTesselator::getFaceControlIdx(IndexType upatch, IndexType vpatch, IndexType nu, IndexType nv, IndexType (&idx)[16]) const
{
	IndexType offset = (vpatch * nu + upatch) * 16; // 16 is the pathchsize (4x4)
	getStdControlIdx(offset, idx);
}


void CRootPatchTesselator::insertBicubicParams(IndexType faceIndex,
											   const IndexType (&cornerIdx)[4], const IndexType (&faceCornerIdx)[4],
											   const IndexType (&controlIdx)[16], const IndexType (&faceControlIdx)[16],
											   CFace &f)
{
	CParameterList::const_iterator iter = obj().parameters().begin();
	for ( ; iter != obj().parameters().end(); iter++ ) {
		if ( (*iter).var() == RI_P || (*iter).var() == RI_N ) {
			// Points and normals are not used because they are build in buildPN
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
					f.bilinearBlend(*iter, cornerIdx);
				}
			}
				break;
				
			case CLASS_VERTEX: {
				if ( (*iter).declaration().basicType() == BASICTYPE_FLOAT ) {
					f.bicubicBlend(*iter, controlIdx, basisVectors());
				}
			}
				break;
				
			case CLASS_FACEVERTEX: {
				if ( (*iter).declaration().basicType() == BASICTYPE_FLOAT ) {
					f.bicubicBlend(*iter, faceControlIdx, basisVectors());
				}
			}
				break;
				
			case CLASS_FACEVARYING: {
				if ( (*iter).declaration().basicType() == BASICTYPE_FLOAT ) {
					f.bilinearBlend(*iter, faceCornerIdx);
				}
			}
				break;
				
			default:
				break;
		}
		
	}
}

void CRootPatchTesselator::buildBilinearPN(const CDeclaration &posDecl,
											 const CDeclaration &normDecl,
											 IndexType faceIndex,
											 const IndexType (&cornerIdx)[4], const IndexType (&faceCornerIdx)[4],
											 CFace &f)
{
	IndexType posCornerIdx[4]; // {0, 1, 2, 3};
	getStdCornerIdx(0, posCornerIdx);

	const CParameter *p = obj().parameters().get(RI_P);
	const CParameter *pz = 0;
	const CParameter *pw = 0;
	
	std::vector<RtFloat> pos(4*4);
	
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
		if ( !extractRi3F(*p, 2, 2, faceIndex, p->declaration().isFace() ? faceCornerIdx : cornerIdx, pos) ) {
			return;
		}
	}
		
	SParametricVars vars(posDecl, normDecl, tessU(), tessV(), flipNormals(), f);
	
	CBilinearBlend bilinBlend(tessU(), tessV());
	
	if ( !pw ) {
		bilinBlend.bilinearBlend(3, posCornerIdx, pos, *vars.positions);
	} else {
		std::vector<RtFloat> blendPw;
		bilinBlend.bilinearBlend(4, posCornerIdx, pos, blendPw);
		pw2p(blendPw, *vars.positions);
	}
	
	const CParameter *n = obj().parameters().get(RI_N);
	std::vector<RtFloat> nrm(4*3);
	
	if ( !n || !extractRi3F(*p, 2, 2, faceIndex, p->declaration().isFace() ? faceCornerIdx : cornerIdx, nrm) ) {
		IndexType nc[4] = {0, (IndexType)tessU()*3, (IndexType)(tessV()*(tessU()+1))*3, (IndexType)vars.positions->size()-3};
		if ( flipNormals() ) {
			// Get the normals, edges can be degenerated
			if ( !planeRH<RtFloat>(&nrm[0], &(*vars.positions)[nc[1]], &(*vars.positions)[nc[0]], &(*vars.positions)[nc[2]]) ) {
				if ( !planeRH<RtFloat>(&nrm[0], &(*vars.positions)[nc[1]], &(*vars.positions)[nc[0]], &(*vars.positions)[nc[3]]) ) {
					planeRH<RtFloat>(&nrm[0], &(*vars.positions)[nc[3]], &(*vars.positions)[nc[0]], &(*vars.positions)[nc[2]]);
				}
			}
				
			if ( !planeRH<RtFloat>(&nrm[3], &(*vars.positions)[nc[3]], &(*vars.positions)[nc[1]], &(*vars.positions)[nc[0]]) ) {
				if ( !planeRH<RtFloat>(&nrm[3], &(*vars.positions)[nc[3]], &(*vars.positions)[nc[1]], &(*vars.positions)[nc[2]]) ) {
					planeRH<RtFloat>(&nrm[3], &(*vars.positions)[nc[2]], &(*vars.positions)[nc[1]], &(*vars.positions)[nc[0]]);
				}
			}

			if ( !planeRH<RtFloat>(&nrm[6], &(*vars.positions)[nc[0]], &(*vars.positions)[nc[2]], &(*vars.positions)[nc[3]]) ) {
				if ( !planeRH<RtFloat>(&nrm[6], &(*vars.positions)[nc[0]], &(*vars.positions)[nc[2]], &(*vars.positions)[nc[1]]) ) {
					planeRH<RtFloat>(&nrm[6], &(*vars.positions)[nc[1]], &(*vars.positions)[nc[2]], &(*vars.positions)[nc[3]]);
				}
			}

			
			if ( !planeRH<RtFloat>(&nrm[9], &(*vars.positions)[nc[2]], &(*vars.positions)[nc[3]], &(*vars.positions)[nc[1]]) ) {
				if ( planeRH<RtFloat>(&nrm[9], &(*vars.positions)[nc[2]], &(*vars.positions)[nc[3]], &(*vars.positions)[nc[0]]) ) {
					planeRH<RtFloat>(&nrm[9], &(*vars.positions)[nc[0]], &(*vars.positions)[nc[3]], &(*vars.positions)[nc[1]]);
				}
			}
		} else {
			// Same for planeLH
			// Get the normals, edges can be degenerated
			if ( !planeLH<RtFloat>(&nrm[0], &(*vars.positions)[nc[1]], &(*vars.positions)[nc[0]], &(*vars.positions)[nc[2]]) ) {
				if ( !planeLH<RtFloat>(&nrm[0], &(*vars.positions)[nc[1]], &(*vars.positions)[nc[0]], &(*vars.positions)[nc[3]]) ) {
					planeLH<RtFloat>(&nrm[0], &(*vars.positions)[nc[3]], &(*vars.positions)[nc[0]], &(*vars.positions)[nc[2]]);
				}
			}
			
			if ( !planeLH<RtFloat>(&nrm[3], &(*vars.positions)[nc[3]], &(*vars.positions)[nc[1]], &(*vars.positions)[nc[0]]) ) {
				if ( !planeLH<RtFloat>(&nrm[3], &(*vars.positions)[nc[3]], &(*vars.positions)[nc[1]], &(*vars.positions)[nc[2]]) ) {
					planeLH<RtFloat>(&nrm[3], &(*vars.positions)[nc[2]], &(*vars.positions)[nc[1]], &(*vars.positions)[nc[0]]);
				}
			}
			
			if ( !planeLH<RtFloat>(&nrm[6], &(*vars.positions)[nc[0]], &(*vars.positions)[nc[2]], &(*vars.positions)[nc[3]]) ) {
				if ( !planeLH<RtFloat>(&nrm[6], &(*vars.positions)[nc[0]], &(*vars.positions)[nc[2]], &(*vars.positions)[nc[1]]) ) {
					planeLH<RtFloat>(&nrm[6], &(*vars.positions)[nc[1]], &(*vars.positions)[nc[2]], &(*vars.positions)[nc[3]]);
				}
			}
			
			
			if ( !planeLH<RtFloat>(&nrm[9], &(*vars.positions)[nc[2]], &(*vars.positions)[nc[3]], &(*vars.positions)[nc[1]]) ) {
				if ( planeLH<RtFloat>(&nrm[9], &(*vars.positions)[nc[2]], &(*vars.positions)[nc[3]], &(*vars.positions)[nc[0]]) ) {
					planeLH<RtFloat>(&nrm[9], &(*vars.positions)[nc[0]], &(*vars.positions)[nc[3]], &(*vars.positions)[nc[1]]);
				}
			}
		}
	}

	bilinBlend.bilinearBlend(3, posCornerIdx, nrm, *vars.normals);
}


void CRootPatchTesselator::buildBicubicPN(const CDeclaration &posDecl,
											const CDeclaration &normDecl,
											IndexType faceIndex,
											const IndexType (&cornerIdx)[4], const IndexType (&faceCornerIdx)[4],
											const IndexType (&controlIdx)[16], const IndexType (&faceControlIdx)[16],
											CFace &f)
{
	// IndexType posCornerIdx[4] = {0, 1, 2, 3};
	// Not getStdCornerIdx(0, posCornerIdx), because indices are selected already

	IndexType posControlIdx[16] = {
		  0,  1,  2,  3,
		  4,  5,  6,  7,
		  8,  9, 10, 11,
		 12, 13, 14, 15
	};
	// Not getStdControlIdx(0, posControlIdx), because indices are selected already
	
	// const CParameter *n = obj().parameters().get(RI_N);
	const CParameter *p = obj().parameters().get(RI_P);
	const CParameter *pz = 0;
	const CParameter *pw = 0;
	
	std::vector<RtFloat> pos(16*4);
	
	if ( !p ) {
		pz = obj().parameters().get(RI_PZ);
		if ( !pz ) {
			pw = obj().parameters().get(RI_PW);
			if ( !pw ) {
				// No position information
				return;
			}
			if ( !extractRiPw(*pw, 4, 4, faceIndex, pw->declaration().isFace() ? faceControlIdx : controlIdx, pos) ) {
				return;
			}			
		} else {
			if ( !extractRiPz(*pz, 4, 4, faceIndex, pz->declaration().isFace() ? faceControlIdx : controlIdx, pos) ) {
				return;
			}
		}
	} else {
		if ( !extractRi3F(*p, 4, 4, faceIndex, p->declaration().isFace() ? faceControlIdx : controlIdx, pos) ) {
			return;
		}
	}

	/*
	if ( n ) {
		std::vector<RtFloat> nrm(16*3);
	}
	 */

	SParametricVars vars(posDecl, normDecl, tessU(), tessV(), flipNormals(), f);
	basisVectors().reset(tessU(), tessV(), basis().uBasis(), basis().vBasis());

	if ( !pw ) {
		// basisVectors().bicubicBlend(3, posControlIdx, pos, *vars.positions); vars.normals->clear();
		basisVectors().bicubicBlendWithNormals(3, posControlIdx, pos, flipNormals(), *vars.positions, *vars.normals);
	} else {
		std::vector<RtFloat> blendPw;
		// basisVectors().bicubicBlend(4, posControlIdx, pos, blendPw); vars.normals->clear();
		basisVectors().bicubicBlendWithNormals(4, posControlIdx, pos, flipNormals(), blendPw, *vars.normals);
		pw2p(blendPw, *vars.positions);
	}
}

// =============================================================================

CSurface *CPatchTesselator::tesselate(const CDeclaration &posDecl,
										  const CDeclaration &normDecl)
{	
	RtToken type = m_obj->type();

	assert(type == RI_BILINEAR || type == RI_BICUBIC);
	if ( type != RI_BILINEAR && type != RI_BICUBIC ) {
		/// @todo Errorhandling unknown Patch type
		return 0;
	}

	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	if ( !surf->empty() ) {
		return surf;
	}
	
	IndexType cornerIdx[4];
	getStdCornerIdx(0, cornerIdx);
	
	IndexType controlIdx[16];
	getStdControlIdx(0, controlIdx);

	if ( !newBasisVectors() )
		return 0;

	CFace &f = surf->newFace(tessU(), tessV());

	if ( type == RI_BICUBIC ) {
		buildBicubicPN(posDecl, normDecl, 0, cornerIdx, cornerIdx, controlIdx, controlIdx, f);
		insertBicubicParams(0, cornerIdx, cornerIdx, controlIdx, controlIdx, f);
	} else {
		// type == RI_BILINEAR
		buildBilinearPN(posDecl, normDecl, 0, cornerIdx, cornerIdx, f);
		insertBilinearParams(0, cornerIdx, cornerIdx, f);
	}
	
	buildIndices(f);
	deleteBasisVectors();

	return surf;
}

CSurface *CPatchMeshTesselator::tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl)
{
	RtToken type = m_obj->type();

	assert(type == RI_BILINEAR || type == RI_BICUBIC);
	if ( type != RI_BILINEAR && type != RI_BICUBIC ) {
		/// @todo Errorhandling unknown Patch type
		return 0;
	}
	
	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	if ( !surf->empty() ) {
		return surf;
	}

	IndexType cornerIdx[4], faceCornerIdx[4];
	IndexType controlIdx[16], faceControlIdx[16];

	if ( !newBasisVectors() )
		return 0;

	for ( RtInt v = 0, i = 0; v < m_obj->nvPatches(); ++v ) {
		for ( RtInt u = 0; u < m_obj->nuPatches(); ++u, ++i ) {
			CFace &f = surf->newFace(tessU(), tessV());
			/*
			getCornerIdx(u, v, m_obj->nu(), m_obj->nv(), cornerIdx); // nuPatches, nvPatches ???
			getFaceCornerIdx(u, v, m_obj->nu(), m_obj->nv(), faceCornerIdx); // nuPatches, nvPatches ???
			*/

			getCornerIdx(u, v, m_obj->nuPatches(), m_obj->nvPatches(), cornerIdx);
			getFaceCornerIdx(u, v, m_obj->nuPatches(), m_obj->nvPatches(), faceCornerIdx);

			if ( type == RI_BICUBIC ) {
				getControlIdx(u, v, m_obj->nu(), m_obj->nv(), basis().uStep(), basis().vStep(), controlIdx);
				getFaceControlIdx(u, v, m_obj->nu(), m_obj->nv(), faceControlIdx);
				buildBicubicPN(posDecl, normDecl, i, cornerIdx, faceCornerIdx, controlIdx, faceControlIdx, f);
				insertBicubicParams(i, cornerIdx, faceCornerIdx, controlIdx, faceControlIdx, f);
			} else {
				// type == RI_BILINEAR
				buildBilinearPN(posDecl, normDecl, i, cornerIdx, faceCornerIdx, f);
				insertBilinearParams(i, cornerIdx, faceCornerIdx, f);
			}
			
			buildIndices(f);
		}
	}

	deleteBasisVectors();
	return surf;
}


// =============================================================================

void CNuPatchTesselator::insertNuParams(CFace &f)
{
	CParameterList::const_iterator iter = obj().parameters().begin();
	for ( ; iter != obj().parameters().end(); iter++ ) {
		if ( (*iter).var() == RI_P || (*iter).var() == RI_N ) {
			// Points and normals are not used because they are build in buildPN
			continue;
		}
		switch ( (*iter).declaration().storageClass() ) {
			case CLASS_CONSTANT:
				f.insertConst(*iter);
				break;
			case CLASS_UNIFORM:
				f.insertUniform(*iter, faceIndex());
				break;
				
			case CLASS_VARYING: {
				/// @todo clipping if tmin, tmax are used to clip parametric space
				if ( (*iter).declaration().basicType() == BASICTYPE_FLOAT ) {
					f.bilinearBlend(*iter, m_cornerIdx);
				}
			}
				break;
				
			case CLASS_VERTEX: {
				if ( (*iter).declaration().basicType() == BASICTYPE_FLOAT ) {
					f.nuBlend(*iter, m_vertexIdx, m_useg, m_vseg, basis());
				}
			}
				break;
				
			case CLASS_FACEVERTEX: {
				if ( (*iter).declaration().basicType() == BASICTYPE_FLOAT ) {
					f.nuBlend(*iter, m_faceVertexIdx, m_useg, m_vseg, basis());
				}
			}
				break;
				
			case CLASS_FACEVARYING: {
				/// @todo clipping if tmin, tmax are used to clip parametric space
				if ( (*iter).declaration().basicType() == BASICTYPE_FLOAT ) {
					f.bilinearBlend(*iter, m_faceCornerIdx);
				}
			}
				break;
				
			default:
				break;
		}
		
	}
}

bool CNuPatchTesselator::extractPFromPz(const CParameter *pz, std::vector<RtFloat> &results) const
{
	RtFloat knotVal;
	if ( !pz || !pz->declaration().isFloat1Decl() || pz->floats().size() <= 0 ) {
		results.clear();
		return false;
	}
	
	std::vector<RtFloat> bilinZ;
	if ( pz->declaration().isVarying() ) {
		CBilinearBlend bilinBlend(uBasis().order(), vBasis().order());
		if ( pz->declaration().isFace() ) {
			bilinBlend.bilinearBlend(1, m_faceCornerIdx, pz->floats(), bilinZ);
		} else {
			bilinBlend.bilinearBlend(1, m_cornerIdx, pz->floats(), bilinZ);
		}
	}
	
	if ( results.size() != basis().numFaceVertices()*3 ) {
		results.clear();
		results.resize(basis().numFaceVertices()*3);
	}

	RtInt ridx = 0, pzidx = 0;
	for ( RtInt vo = 0; vo < vBasis().order(); ++vo ) {
		knotVal = vBasis().knots()[m_vseg+vBasis().order()+vo-1];
		for ( RtInt uo = 0; uo < uBasis().order(); ++uo ) {
			results[ridx++] = uBasis().knots()[m_useg+uBasis().order()+uo-1];
			results[ridx++] = knotVal;
			switch ( pz->declaration().storageClass() ) {
				case CLASS_CONSTANT:
					results[ridx++] = pz->floats()[0];
					break;
				case CLASS_UNIFORM:
					results[ridx++] = pz->floats()[faceIndex()];
					break;
				case CLASS_VARYING: /* no break */
				case CLASS_FACEVARYING:
					results[ridx++] = bilinZ[pzidx++];
					break;
				case CLASS_VERTEX:
					results[ridx++] = pz->floats()[m_vertexIdx[pzidx++]];
					break;
				case CLASS_FACEVERTEX:
					results[ridx++] = pz->floats()[m_faceVertexIdx[pzidx++]];
					break;
				default:
					break;
			}
		}
	}
	
	return true;
}

void CNuPatchTesselator::buildNuPN(const CDeclaration &posDecl, const CDeclaration &normDecl, CFace &f)
{
	RtInt uspan = uBasis().span(m_useg);
	RtInt vspan = vBasis().span(m_vseg);
	
	RtInt uVals = uBasis().valCnts()[uspan];
	RtInt vVals = vBasis().valCnts()[vspan];
	
	if ( uVals == 0 || vVals == 0 ) {
		return;
	}

	const CParameter *pw = obj().parameters().get(RI_PW);
	const CParameter *p = 0;
	const CParameter *pz = 0;
	

	std::vector<RtFloat> pos;
	const std::vector<RtFloat> *posPtr = 0;
	const std::vector<IndexType> *idxPtr = 0;

	IndexType elemSize = 0;
	
	if ( !pw ) {
		p = obj().parameters().get(RI_P);
		if ( !p ) {
			pz = obj().parameters().get(RI_PZ);
			if ( !pz ) {
				return;
			} else {
				if ( !extractPFromPz(pz, pos) )
					return;
				posPtr = &pos;
				elemSize = 3;
				idxPtr = &m_idIdx;
			}
		} else {
			if ( !p->declaration().isFloat3Decl() )
				return;
			posPtr = &p->floats();
			elemSize = 3;
			idxPtr = &m_vertexIdx;
			if ( p->declaration().isFace() )
				idxPtr = &m_faceVertexIdx;
		}
	} else {
		if ( !pw->declaration().isFloat4Decl() )
			return;
		posPtr = &pw->floats();
		elemSize = 4;
		idxPtr = &m_vertexIdx;
		if ( pw->declaration().isFace() )
			idxPtr = &m_faceVertexIdx;
	}
	
	if ( !posPtr )
		return;
	

	SParametricVars vars(posDecl, normDecl, uVals, vVals, flipNormals(), f);

	if ( !pw ) {
		basis().nuBlendWithNormals(elemSize, *posPtr, m_useg, m_vseg, *idxPtr, flipNormals(), *vars.positions, *vars.normals);
	} else {
		std::vector<RtFloat> blendPw;
		basis().nuBlendWithNormals(elemSize, *posPtr, m_useg, m_vseg, *idxPtr, flipNormals(), blendPw, *vars.normals);
		pw2p(blendPw, *vars.positions);
	}
	
}

void CNuPatchTesselator::fillIdx(RtInt usegment, RtInt vsegment)
{
	m_useg = usegment;
	m_vseg = vsegment;
	
	IndexType cornerIdxArray[4], faceCornerIdxArray[4];
	m_cornerIdx.resize(4);
	m_faceCornerIdx.resize(4);
	
	getCornerIdx(m_useg, m_vseg, uBasis().numSegments(), vBasis().numSegments(), cornerIdxArray); // numSegments() vs. nCPts ???
	m_cornerIdx.assign(&cornerIdxArray[0], &cornerIdxArray[0] + 4);
	getFaceCornerIdx(faceIndex(), faceCornerIdxArray);
	m_faceCornerIdx.assign(&faceCornerIdxArray[0], &faceCornerIdxArray[0] + 4);

	
	m_vertexIdx.clear();
	m_vertexIdx.resize(uBasis().order()*vBasis().order());
	m_faceVertexIdx.clear();
	m_faceVertexIdx.resize(uBasis().order()*vBasis().order());
	m_idIdx.clear();
	m_idIdx.resize(uBasis().order()*vBasis().order());

	RtInt idx, uo, vo;
	
	idx = 0;
	for ( vo = 0; vo < vBasis().order(); ++vo ) {
		for ( uo = 0; uo < uBasis().order(); ++uo, ++idx ) {
			m_vertexIdx[idx]     = (m_vseg + vo) * uBasis().nCPts() + m_useg + uo;
			m_faceVertexIdx[idx] = faceIndex() * basis().numFaces() + idx;
			m_idIdx[idx]         = idx;
		}
	}
}

CSurface *CNuPatchTesselator::tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl)
{
	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	if ( !surf->empty() ) {
		return surf;
	}

	if ( !m_basis )
		m_basis = new CUVBSplineBasis(*m_obj, tessU(), tessV());

	if ( !m_basis )
		return 0;
	
	for ( RtInt vs = 0; vs < vBasis().numSegments(); ++vs ) {
		for ( RtInt  us = 0; us < uBasis().numSegments(); ++us ) {
			if ( uBasis().numParameters(us) <= 0 || vBasis().numParameters(vs) <= 0)
				continue;
			CFace &f = surf->newFace(uBasis().numParameters(us)-1, vBasis().numParameters(vs)-1);
			fillIdx(us, vs);			
			buildNuPN(posDecl, normDecl, f);
			insertNuParams(f);
			buildIndices(f);
		}
	}

	delete m_basis;
	m_basis = 0;

	return surf;
}

// ----------------------------------------------------------------------------

void CSubdivisionHierarchieTesselator::subdivide(const CSubdivisionStrategy &strategy, IndexType depth)
{
	if ( m_indices.size() == 0 ) {
		m_indices.push_back(CSubdivisionIndices());
		CSubdivisionIndices &root = m_indices.back();
		root.initialize(m_obj);
	}
	
	if ( depth >= m_indices.size() ) {
		// Not already subdivided, do further subdivision
		CSubdivisionIndices *cur = &m_indices.back();
		IndexType depthCnt = 1 + depth - static_cast<long>(m_indices.size());
		while ( depthCnt && !cur->illTopology() ) {
			m_indices.push_back(CSubdivisionIndices());
			CSubdivisionIndices &child = m_indices.back();
			child.subdivide(*cur, strategy, m_obj);
			cur = &child;
			depthCnt--;
		}
	}
}

void CSubdivisionHierarchieTesselator::insertParams(const CSubdivisionStrategy &strategy, const std::list<CSubdivisionIndices>::const_iterator &curIndices, CFace &aFace)
{
	IndexType nVertices = static_cast<IndexType>((*curIndices).vertices().size());
	
	CParameterList::const_iterator iter = obj().parameters().begin();
	for ( ; iter != obj().parameters().end(); iter++ ) {
		switch ( (*iter).declaration().storageClass() ) {
			case CLASS_VARYING: {
				if ( (*iter).declaration().basicType() == BASICTYPE_FLOAT ) {
					TemplPrimVar<RtFloat> &floats = aFace.reserveFloats((*iter).declaration());
					floats.values().resize(nVertices * (*iter).declaration().elemSize());
					assert(floats.values().size() >= (*iter).floats().size());
					std::copy((*iter).floats().begin(), (*iter).floats().end(), floats.values().begin());
					strategy.insertVaryingValues(m_indices.begin(), curIndices, floats.declaration(), floats.values());
				}
			} break;
				
			case CLASS_VERTEX: {
				if ( (*iter).declaration().basicType() == BASICTYPE_FLOAT ) {
					TemplPrimVar<RtFloat> &floats = aFace.reserveFloats((*iter).declaration());
					floats.values().resize(nVertices * (*iter).declaration().elemSize());
					assert(floats.values().size() >= (*iter).floats().size());
					std::copy((*iter).floats().begin(), (*iter).floats().end(), floats.values().begin());
					strategy.insertVertexValues(m_indices.begin(), curIndices, floats.declaration(), floats.values());
				}
			} break;
				
			default:
				break;
		}		
	}
}


void CSubdivisionHierarchieTesselator::extractFaces(const CSubdivisionStrategy &strategy, const std::list<CSubdivisionIndices>::iterator &curIndices, long faceIdx, const CFace &varyingData, std::vector<IndexType> &origIndices, CFace &f)
{
	(*curIndices).prepareFace(m_indices.begin(), curIndices, faceIdx, f.indices(), origIndices);

	for ( CParameterList::const_iterator iter = obj().parameters().begin();
		  iter != obj().parameters().end();
		 iter++ )
	{
		switch ( (*iter).declaration().storageClass() ) {

			case CLASS_CONSTANT:
				f.insertConst(*iter);
				break;
				
			case CLASS_UNIFORM:
				f.insertUniform(*iter, faceIdx);
				break;
				
			case CLASS_VARYING: {
				// in varyingData
			} break;
				
			case CLASS_VERTEX: {
				// in varyingData
			} break;
				
			case CLASS_FACEVARYING: {
				if ( (*iter).declaration().basicType() == BASICTYPE_FLOAT ) {
					TemplPrimVar<RtFloat> &floats = f.reserveFloats((*iter).declaration());
					strategy.insertFaceVaryingValues(m_indices.begin(), curIndices, origIndices, floats.declaration(), floats.values());
				}
			} break;
				
			case CLASS_FACEVERTEX: {
				// Not implemented
				/// @todo There is no class "face vertex". Just makes implementation more complex, remove it completely?
			} break;
				
			default:
				break;
		}		
	}
	
	// Copy varying (and vertex) data
	for ( CFace::TypeConstFloatIterator fiter = varyingData.floatsBegin(); fiter != varyingData.floatsEnd();  fiter++ ) {
		assert((*fiter).second.declaration().basicType() == BASICTYPE_FLOAT);
		if ( (*fiter).second.declaration().basicType() == BASICTYPE_FLOAT ) {
			TemplPrimVar<RtFloat> &floats = f.reserveFloats((*fiter).second.declaration());
			IndexType elemSize = floats.declaration().elemSize();
			floats.values().resize(origIndices.size()*elemSize);
			for ( IndexType i = 0; i < origIndices.size(); ++i ) {
				assert(origIndices[i] * elemSize + elemSize <= (*fiter).second.values().size());
				for ( IndexType elem = 0; elem < elemSize; ++elem ) {
					floats.values()[i * elemSize + elem] = (*fiter).second.values()[origIndices[i]*elemSize+elem];
				}
			}
		}
	}
	
	// InsertNormals
}

CSurface *CSubdivisionHierarchieTesselator::tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl)
{
	/// @todo testing
	return 0;
	

	
	
	IndexType maxTess = tmax(tessU(), tessV());
	
	IndexType depth = tmax<IndexType>(maxTess, 1);
	depth = static_cast<IndexType>(ceil(log2(static_cast<double>(depth))));
	
	const CSubdivisionStrategy *strategy = m_strategies.findObj(m_obj.scheme());
	if ( !strategy ) {
		strategy = m_strategies.findObj(RI_NULL);
		if ( !strategy )
			return 0;
	}
	
	tessU(maxTess);
	tessV(maxTess);
	
	CSurface *surf = createSurface();
	if ( !surf ) {
		return 0;
	}
	if ( !surf->empty() ) {
		return surf;
	}
	
	subdivide(*strategy, depth);
	
	std::list<CSubdivisionIndices>::iterator curIndices = m_indices.begin();
	for ( IndexType i = 0; i < depth && curIndices != m_indices.end(); ++i ) {
		curIndices++;
	}
	if ( curIndices == m_indices.end() || (*curIndices).illTopology() ) {
		return 0;
	}
	
	CFace *varyingData = new CFace();
	try {
		if ( varyingData ) {
			insertParams(*strategy, curIndices, *varyingData);
			std::vector<IndexType> origIndices;
			for ( RtInt faceIdx = 0; faceIdx < m_obj.nFaces(); ++faceIdx ) {
				extractFaces(*strategy, curIndices, faceIdx, *varyingData, origIndices, surf->newFace(tessU(), tessV(), FACETYPE_TRIANGLES));
			}
		}
	} catch ( ... ) {
		if ( varyingData )
			delete varyingData;
		throw;
	}	
	
	if ( varyingData )
		delete varyingData;

	return surf;
}
