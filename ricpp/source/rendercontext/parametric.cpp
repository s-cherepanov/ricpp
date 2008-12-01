// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2003, Andreas Pidde
//
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

/*! \file parametric.cpp
*  \brief Implementation of the classes for parametric meshes.
*         RenderMan is (R) by Pixar
*  \author Andreas Pidde (andreas@pidde.de)
*/

#include "stdafx.h"
#include "parametric.h"
#include "tools/logfile.h"

#include <math.h>

// Caches are static by now - must implement some other way to avoid clashes while using DLLs (?)
static TUVVectorCache s_uvVectorCache;
static TUnitCircleCache s_unitCircleCache;
static TUnitSphereCache s_unitSphereCache;


// ============================================================================
// TBSplineBasis


TBSplineBasis::TBSplineBasis(RtInt ncpts, RtInt order, const std::vector<RtFloat> &knots, RtFloat tmin, RtFloat tmax, RtInt tess) {
	m_ncpts = ncpts;
	m_order = order;
	m_knots = knots;
	m_tmin  = tmin;
	m_tmax  = tmax;
	m_tess = tess;
	m_segments = 0;
	calc();
}


void TBSplineBasis::bsplineBasisDerivate(RtFloat t, RtInt span,
	std::vector<RtFloat> &N, std::vector<RtFloat> &Nd)
{
	const RtInt size  = m_ncpts + m_order - 1;

	// Init N
	N.assign(size, 0.0);
	N[span] = 1.0;

	// Calculate
	RtInt i, j;
	RtFloat temp;

	for ( j=1; j < m_order; ++j ) {
		for ( i=0; i < size-j; ++i ) {

			// Calc the first Derivate
			if ( j == 1 ) {
				temp = m_knots[i+j] - m_knots[i];
				if ( temp >= riEpsilon ) {
					temp = N[i] / temp;
				}
				Nd[i] = temp;
				temp = m_knots[i+j+1] - m_knots[i+1];
				if ( temp >= riEpsilon ) {
					temp = N[i+1] / temp;
					Nd[i] -= temp;
				}
			} else {
				temp = m_knots[i+j] - m_knots[i];
				if ( temp >= riEpsilon ) {
					temp = (N[i] + (t-m_knots[i])*Nd[i]) / temp;
				}
				Nd[i] = temp;
				temp = m_knots[i+j+1] - m_knots[i+1];
				if ( temp >= riEpsilon ) {
					temp = (N[i+1] - (m_knots[i+j+1]-t)*Nd[i+1]) / temp;
					Nd[i] -= temp;
				}
			}

			// Calc the basis
			if ( N[i] >= riEpsilon ) {
				temp = m_knots[i+j]-m_knots[i];
				if ( temp >= riEpsilon ) {
					temp = N[i] * ((t - m_knots[i]) / temp);
				}
				N[i] = temp;
			}

			if ( N[i+1] >= riEpsilon ) {
				temp = m_knots[i+j+1]-m_knots[i+1];
				if ( temp >= riEpsilon ) {
					temp = N[i+1] * ((m_knots[i+j+1] - t) / temp);
					N[i] += temp;
				}
			}
		}
	}
}


bool TBSplineBasis::unvalidParams() {
	if ( m_tess <= 0 )
		m_tess = 1;

	if ( m_order > m_ncpts )
		m_order = m_ncpts;
	if ( m_ncpts == 0 || m_order == 0 )
		return true;

	if ( (RtInt)m_knots.size() < m_ncpts+m_order )
		return true;

	if ( (RtInt)m_knots.size() > m_ncpts+m_order )
		m_knots.resize(m_ncpts+m_order);

	RtInt i;
	for ( i = 1; i < m_ncpts+m_order; ++i ) {
		if ( m_knots[i] < m_knots[i-1] )
			m_knots[i-1] = m_knots[i];
	}

	if ( m_tmin > m_tmax ) {
		RtFloat t = m_tmax;
		m_tmax = m_tmin;
		m_tmin = t;
	}

	if ( m_tmin < m_knots[m_order-1] )
		m_tmin = m_knots[m_order-1];

	if ( m_tmax > m_knots[m_ncpts] )
		m_tmax = m_knots[m_ncpts];

	return false;
}


void TBSplineBasis::calc() {
	// Test parameter conditions
	m_segments = 0;
	if ( unvalidParams() )
		return;

	RtFloat ts, te;
	RtFloat t, dt;
	RtInt cnt, lastCnt;

	m_segments = 1 + m_ncpts - m_order;

	m_tVals.clear();
	m_tVals.reserve(m_segments*(m_tess+1));
	m_valCnts.clear();
	m_valCnts.resize(m_ncpts);
	m_valCnts.assign(m_ncpts, 0);
	m_valOffs.clear();
	m_valOffs.resize(m_ncpts);
	m_valOffs.assign(m_ncpts, 0);

	lastCnt = 0;
	for ( RtInt knotIdx = m_order-1; knotIdx < m_ncpts; ++knotIdx ) {
		if ( knotIdx > 0 ) {
			m_valOffs[knotIdx] = m_valOffs[knotIdx-1]+lastCnt;
			lastCnt = 0;
		}
		ts = m_knots[knotIdx];
		te = m_knots[knotIdx+1];
		if ( (te-ts) < riEpsilon ) continue;

		dt = (te-ts)/m_tess;

		if ( dt < riEpsilon )
			dt = riEpsilon;

		if ( m_tmax < ts )
			break;
		if ( m_tmin >= te )
			continue;

		// m_tmax >= ts && m_tmin < te

		if ( m_tmin > ts )
			ts = m_tmin;
		if ( m_tmax < te )
			te = m_tmax;

		m_valCnts[knotIdx] = (RtInt)m_tVals.size();
		m_tVals.push_back(ts);
		for ( t = m_knots[knotIdx], cnt = 0; t < te && cnt < m_tess; t += dt, ++cnt ) {
			if ( t <= ts )
				continue;
			m_tVals.push_back(t);
		}
		m_tess=cnt;
		m_tVals.push_back(te);
		lastCnt = m_valCnts[knotIdx] = (RtInt)m_tVals.size()-m_valCnts[knotIdx];
	}

	std::vector<RtFloat> Ntemp(m_ncpts+m_order-1);     // BASIS, result can be found in N[0..n-1]
	std::vector<RtFloat> Ndtemp(m_ncpts+m_order-1);    // BASIS derivative, result can be found in  Nd[0..n-1]

	m_basis.clear();
	m_basis.resize(m_tVals.size()*m_order);
	m_basisDeriv.clear();
	m_basisDeriv.resize(m_tVals.size()*m_order);

	RtInt GIdx = 0;
	RtInt i, j;
	std::vector<RtFloat>::iterator iter = m_tVals.begin();
	for ( RtInt span = m_order-1; span < m_ncpts; ++span ) {
		for ( j = m_valCnts[span]; j > 0 && iter != m_tVals.end(); iter++, --j ) {
			bsplineBasisDerivate(*iter, span, Ntemp, Ndtemp);
			for ( i = span-m_order+1; i <= span; ++i, ++GIdx ) {
				// Cache the results (basis and derivative) for parameter t
				m_basis[GIdx] = Ntemp[i];
				m_basisDeriv[GIdx] = Ndtemp[i];
			}
		}
	}
}


TBSplineBasis &TBSplineBasis::operator=(const TBSplineBasis &sp) {
	if ( &sp == this )
		return *this;
	m_ncpts = sp.m_ncpts;
	m_order = sp.m_order;
	m_tmin  = sp.m_tmin;
	m_tmax  = sp.m_tmax;
	m_knots = sp.m_knots;
	m_tess = sp.m_tess;
	m_segments = sp.m_segments;
	m_tVals = sp.m_tVals;
	m_valCnts = sp.m_valCnts;
	m_valOffs = sp.m_valOffs;

	m_basis = sp.m_basis;
	m_basisDeriv = sp.m_basisDeriv;
	return *this;
}


void TBSplineBasis::reset(RtInt ncpts, RtInt order, const std::vector<RtFloat> &knots, RtFloat tmin, RtFloat tmax, RtInt tess) {
	m_ncpts = ncpts;
	m_order = order;
	m_knots = knots;
	m_tmin  = tmin;
	m_tmax  = tmax;
	m_tess = tess;
	m_segments = 0;
	m_tVals.clear();
	m_valCnts.clear();
	m_valOffs.clear();
	m_basis.clear();
	m_basisDeriv.clear();
	calc();
}


void TBSplineBasis::reset(RtInt ncpts, RtInt order, const std::vector<RtFloat> &knots, RtInt knotOffs, RtFloat tmin, RtFloat tmax, RtInt tess) {
	m_ncpts = ncpts;
	m_order = order;
	m_knots.resize(m_ncpts+m_order);

	RtInt i, end=m_ncpts+m_order;
	RtFloat lastKnot = 0;
	if ( knotOffs + end > (RtInt)knots.size() ) {
		end = (RtInt)(knots.size() - knotOffs);
		if ( end < 0 )
			end = 0;
	}
	for (i = 0; i < end; ++i) {
		lastKnot = knots[knotOffs+i];
		m_knots[i] = lastKnot;
	}
	for (i = end; i < m_ncpts+m_order; ++i) {
		m_knots[i] = lastKnot;
	}

	m_tmin  = tmin;
	m_tmax  = tmax;
	m_tess = tess;
	m_segments = 0;
	m_tVals.clear();
	m_valCnts.clear();
	m_valOffs.clear();
	m_basis.clear();
	m_basisDeriv.clear();
	calc();
}

// ============================================================================
// TUVVector

// ----------------------------------------------------------------------------
void TUVVector::reset(IndexType tessu, IndexType tessv, const RtBasis uBasis, const RtBasis vBasis) {
	m_tessU = tessu;
	m_tessV = tessv;
	memcpy(m_uBasis, uBasis, sizeof(RtBasis));
	memcpy(m_vBasis, vBasis, sizeof(RtBasis));

	m_uVector.clear();
	m_vVector.clear();
	m_duVector.clear();
	m_dvVector.clear();

	m_uVector.resize((tessu+1)*4);
	m_vVector.resize((tessv+1)*4);
	m_duVector.resize((tessu+1)*4);
	m_dvVector.resize((tessv+1)*4);

	RtFloat t, t2, t3, s2, s3;
	IndexType i, k, u, v;

	RtFloat deltau = (RtFloat)(1.0 / (RtFloat)tessu);
	RtFloat deltav = (RtFloat)(1.0 / (RtFloat)tessv);

	for ( t = 0.0, k = 0, u = 0; u < tessu+1; ++u, t += deltau ) {
		if ( u == tessu ) {
			t = 1.0;
		}
		t2 = t*t;
		t3 = t*t2;
		s2 = (RtFloat)(t+t);
		s3 = (RtFloat)(t2+t2+t2);
		for ( i = 0; i < 4; ++i, ++k ) {
			m_uVector[k] =  t3 * m_uBasis[0][i]+
				t2 * m_uBasis[1][i]+
				t  * m_uBasis[2][i]+
				m_uBasis[3][i];
			m_duVector[k] = s3 * m_uBasis[0][i]+
				s2 * m_uBasis[1][i]+
				m_uBasis[2][i];
		}
	}

	for ( t = 0.0, k = 0, v = 0; v < tessv+1; ++v, t += deltav ) {
		if ( v == tessv ) {
			t = 1.0;
		}
		t2 = t*t;
		t3 = t*t2;
		s2 = (RtFloat)(t+t);
		s3 = (RtFloat)(t2+t2+t2);
		for ( i = 0; i < 4; ++i, ++k ) {
			m_vVector[k] =  t3 * m_vBasis[0][i]+
				t2 * m_vBasis[1][i]+
				t  * m_vBasis[2][i]+
				m_vBasis[3][i];
			m_dvVector[k] = s3 * m_vBasis[0][i]+
				s2 * m_vBasis[1][i]+
				m_vBasis[2][i];
		}
	}
}

// ============================================================================
// TUVVectorCache

// ----------------------------------------------------------------------------
TUVVector &TUVVectorCache::findUVVector(IndexType tessU, IndexType tessV, const RtBasis &uBasis, const RtBasis &vBasis) {
	bool isNew;
	return findBicubicUV(m_cache, tessU, tessV, uBasis, vBasis, m_lastIndex, isNew);
}


// ============================================================================
// TUVPatch

// ----------------------------------------------------------------------------
const RtFloat *TUVPatch::getTextureST(IndexType &size, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) {

	const RtFloat *pst = TPrimFace::getTextureST(size, s1, t1, s2, t2, s3, t3, s4, t4);
	if ( pst != NULL ) {
		m_transformS = false;
		m_transformT = false;
		return pst;
	}

	if (
		m_st && 

		((m_stRect[IDX_S1] == s1 &&
		  m_stRect[IDX_S2] == s2 &&
		  m_stRect[IDX_S3] == s3 &&
		  m_stRect[IDX_S4] == s4) ||
		  !m_transformS) &&

		((m_stRect[IDX_T1] == t1 &&
		  m_stRect[IDX_T2] == t2 &&
		  m_stRect[IDX_T3] == t3 &&
		  m_stRect[IDX_T4] == t4) ||
		  !m_transformT)
	) {
		size = m_st->size();
		return m_st->begin();
	}

	if ( !m_st ) {
		m_st = new TNamedVector<RtFloat>(TRi::RI_ST, 2, 1, (m_tessU+1)*(m_tessV+1));
		m_freeST = true;
		if ( !m_st )
			return NULL;

		m_transformS = true;
		const TNamedVector<RtFloat> *datas = getInterpolatedData(TRi::RI_S);
		if ( datas ) {
			if ( datas->size() != m_st->size()/2 ) {
				datas = NULL;
			} else {
				RtFloat *to = m_st->begin();
				RtFloat *from = datas->begin();
				for ( ; from != datas->end(); ++from ) {
					*to = *from;
					to += 2;
				}
				m_transformS = false;
			}
		}
		if ( !datas ) {
			if ( m_useUniformS ) {
				RtFloat *to = m_st->begin();
				for ( ; to < m_st->end(); to += 2 ) {
					*to = m_uniformS;
				}
				m_transformS = false;
			}
		}

		m_transformT = true;
		const TNamedVector<RtFloat> *datat = getInterpolatedData(TRi::RI_T);
		if ( datat ) {
			if ( datat->size() != m_st->size()/2 ) {
				datat = NULL;
			} else {
				RtFloat *to = m_st->begin();
				++to;
				RtFloat *from = datat->begin();
				for ( ; from != datat->end(); ++from ) {
					*to = *from;
					to += 2;
				}
				m_transformT = false;
			}
		}
		if ( !datat ) {
			if ( m_useUniformT ) {
				RtFloat *to = m_st->begin();
				++to;
				for ( ; to < m_st->end(); to += 2 ) {
					*to = m_uniformT;
				}
				m_transformT = false;
			}
		}
	}

	m_stRect[IDX_S1] = s1;
	m_stRect[IDX_T1] = t1;
	m_stRect[IDX_S2] = s2;
	m_stRect[IDX_T2] = t2;
	m_stRect[IDX_S3] = s3;
	m_stRect[IDX_T3] = t3;
	m_stRect[IDX_S4] = s4;
	m_stRect[IDX_T4] = t4;

	transformTextureST();

	size = m_st->size();
	return m_st->begin();
}


void TUVPatch::transformTextureST()
{
	assert(m_st != NULL);

	RtFloat slenTop    = m_stRect[IDX_S2] - m_stRect[IDX_S1];
	RtFloat slenBottom = m_stRect[IDX_S4] - m_stRect[IDX_S3];
	RtFloat tlenTop    = m_stRect[IDX_T3] - m_stRect[IDX_T1];
	RtFloat tlenBottom = m_stRect[IDX_T4] - m_stRect[IDX_T2];

	slenTop    /= m_numFacesU;
	slenBottom /= m_numFacesU;
	tlenTop    /= m_numFacesV;
	tlenBottom /= m_numFacesV;

	RtFloat st[8];
	st[IDX_S1] = m_stRect[IDX_S1] + m_faceIdxU * slenTop;
	st[IDX_S2] = st[IDX_S1] + slenTop;
	st[IDX_T1] = m_stRect[IDX_T1] + m_faceIdxV * tlenTop;
	st[IDX_T3] = st[IDX_T1] + tlenTop;
	st[IDX_S3] = m_stRect[IDX_S3] + m_faceIdxU * slenBottom;
	st[IDX_S4] = st[IDX_S3] + slenBottom;
	st[IDX_T2] = m_stRect[IDX_T2] + m_faceIdxV * tlenBottom;
	st[IDX_T4] = st[IDX_T2] + tlenBottom;

	IndexType idx[4];
	fillCornerIdx(idx);

	if ( m_transformS ) {
		bilinearBlend(m_tessU, m_tessV, idx, 2, st, *m_st, TRi::RI_ST, 1, 0, 1, false);
	}

	if ( m_transformT ) {
		bilinearBlend(m_tessU, m_tessV, idx, 2, &(st[1]), *m_st, TRi::RI_ST, 1, 1, 1, false);
	}
}

// ----------------------------------------------------------------------------
void TUVPatch::resizeVerticesBy(RtFloat r) {
	RtFloat *f;
	if ( r != 1.0 && m_vertexStrip ) {
		for ( f = m_vertexStrip->begin(); f != m_vertexStrip->end(); ++f ) {
			*f *= r;
		}
	}
}


// ----------------------------------------------------------------------------
TUVPatch &TUVPatch::operator=(const TUVPatch &uvPatch) {
	if ( &uvPatch == this )
		return *this;

	m_tessU = uvPatch.m_tessU;
	m_tessV = uvPatch.m_tessV;

	freePd();

	m_freePdu = uvPatch.m_freePdu;
	if ( m_freePdu && uvPatch.m_Pdu ) {
		m_Pdu = new TNamedVector<RtFloat>(*uvPatch.m_Pdu);
	} else {
		m_Pdu = uvPatch.m_Pdu;
	}

	m_freePdv = uvPatch.m_freePdv;
	if ( m_freePdv && uvPatch.m_Pdv ) {
		m_Pdv = new TNamedVector<RtFloat>(*uvPatch.m_Pdv);
	} else {
		m_Pdv = uvPatch.m_Pdv;
	}

	m_freeST = uvPatch.m_freeST;
	if ( m_freeST && uvPatch.m_st ) {
		m_st = new TNamedVector<RtFloat>(*uvPatch.m_st);
	} else {
		m_st = uvPatch.m_st;
	}

	m_transformS = uvPatch.m_transformS;
	m_transformT = uvPatch.m_transformT;

	memcpy(&m_stRect[0], &uvPatch.m_stRect[0], sizeof(m_stRect));

	TPrimFace::operator=(uvPatch);
	return *this;
}


// ----------------------------------------------------------------------------
void TUVPatch::insertVertexData(const TParameterMap &pm, RtInt nu, RtInt nv, RtInt upatch, RtInt vpatch) {
	IndexType cornerIdx[4];
	fillCornerIdx(cornerIdx, nu, nv, upatch, vpatch);
	IndexType faceCornerIdx[4] = {
		m_faceIdx*4+0, m_faceIdx*4+1,
		m_faceIdx*4+2, m_faceIdx*4+3,
	};
	IndexType size = (IndexType)pm.getSize();
	IndexType i;
	for ( i=0; i<size; ++i ) {
		const TParameter *p = pm.getParameterOf(i);
		if ( p ) {
			if ( p->getBasicType() == TYPE_FLOAT && p->getSize() >= 4 ) {
				// CLASS_VARYING and CLASS_VERTEX, handled in the same way (Quadrics, bilinear patches)
				if ( p->getClass() == CLASS_VARYING ||
					p->getClass() == CLASS_VERTEX )
				{
					m_vertexData.resize(m_vertexData.size()+1);
					bilinearBlend(m_tessU, m_tessV, cornerIdx, *p, m_vertexData.back());
				} else if ( p->getClass() == CLASS_FACEVARYING ||
					p->getClass() == CLASS_FACEVERTEX )
				{
					m_vertexData.resize(m_vertexData.size()+1);
					bilinearBlend(m_tessU, m_tessV, faceCornerIdx, *p, m_vertexData.back());
				}
			}
		}
	}
}


// ----------------------------------------------------------------------------
void TUVPatch::insertVertexIndexData() {
	// Indices for vertices in triangle strips
	IndexType uverts   = m_tessU + 1;
	IndexType nIndices = 2 * uverts;
	m_vertexIndices.clear();
	m_vertexIndices.resize(m_tessV);

	IndexType vidx;
	IndexType uidx, uend;
	IndexType offs = 0;
	IndexType idx = 0;

	for ( vidx = 0; vidx < m_tessV; ++vidx ) {
		m_vertexIndices[vidx].type(TGeometricIndices::strip);
		std::vector<TVertexIndexType> &v = m_vertexIndices[vidx].m_indices;
		v.clear();
		v.resize(nIndices);
		for ( idx = 0, uidx = offs, uend = offs+uverts; uidx < uend; ++uidx ) {
			v[idx]   = uidx;
			v[++idx] = uidx + uverts;
			++idx;
		}
		offs = uend;
	}
}


// ============================================================================
// TQuadric
// ----------------------------------------------------------------------------


// ============================================================================
// TSphereMesh

// ----------------------------------------------------------------------------
void TSphereMesh::createPN(IndexType  tessU, IndexType  tessV, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax) {
	m_tessU = tessU;
	m_tessV = tessV;
	IndexType  nverts = (m_tessU+1)*(m_tessV+1);

	RtFloat deltau = (RtFloat)(1.0 / (RtFloat)m_tessU);
	RtFloat deltav = (RtFloat)(1.0 / (RtFloat)m_tessV);

	newStrips();

	m_vertexStrip->initialize(TRi::RI_P, 3, 1, nverts);
	m_normalStrip->initialize(TRi::RI_N, 3, 1, nverts);

	RtFloat phimin;
	if ( zmin > -radius )
		phimin = (RtFloat)asin(zmin/radius);
	else
		phimin = -pi_2;

	RtFloat phimax;
	if ( zmax < radius )
		phimax = (RtFloat)asin(zmax/radius);
	else
		phimax = pi_2;

	std::vector<RtFloat> unitcircle;
	s_unitCircleCache.getUnitCircle(unitcircle, m_tessU, deg2rad(thetamax));

	std::vector<RtFloat> unitcirclez;
	s_unitCircleCache.getUnitCircle(unitcirclez, m_tessV, phimin, phimax);

	RtFloat changeOrientation = (RtFloat)(thetamax > 0 ? -1.0 : 1.0);

	RtFloat u, v, cosphi;
	RtFloat p[3], n[3];

	IndexType vverts;
	IndexType uverts;

	IndexType pvidx=0;
	IndexType puidx=0;
	IndexType pidx=0;

	for (	vverts = m_tessV+1, v = 0.0;
		vverts > 0;
		v += deltav, --vverts )
	{
		if ( vverts == 1 ) {
			v = 1.0;
		}
		cosphi = unitcirclez[pvidx];
		n[2]   = unitcirclez[++pvidx];
		p[2]   = radius * n[2];
		++pvidx;

		puidx = 0;
		for (	uverts = m_tessU+1, u = 0.0;
			uverts > 0;
			u += deltau, --uverts)
		{
			if ( uverts == 1 ) {
				u = 1.0;
			}
			n[0] = cosphi * unitcircle[puidx];
			p[0] = radius * n[0];
			(*m_normalStrip)[pidx] = n[0] * changeOrientation;
			(*m_vertexStrip)[pidx] = p[0];

			++puidx;
			++pidx;

			n[1] = cosphi * unitcircle[puidx];
			p[1] = radius * n[1];
			(*m_normalStrip)[pidx] = n[1] *  changeOrientation;
			(*m_vertexStrip)[pidx] = p[1];

			++puidx;
			++pidx;

			(*m_normalStrip)[pidx] = n[2] *  changeOrientation;
			(*m_vertexStrip)[pidx] = p[2];

			++pidx;
		}
	}
	insertVertexIndexData();
}


// ============================================================================
// TCylinderMesh

// ----------------------------------------------------------------------------
void TCylinderMesh::createPN(IndexType tessU, IndexType tessV, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax) {
	m_tessU = tessU;
	m_tessV = tessV;
	IndexType nverts = (m_tessU+1)*(m_tessV+1);

	RtFloat deltau = (RtFloat)(1.0 / (RtFloat)m_tessU);
	RtFloat deltav = (RtFloat)(1.0 / (RtFloat)m_tessV);
	RtFloat deltaz = (zmax-zmin)/ (RtFloat)m_tessV;

	newStrips();

	m_vertexStrip->initialize(TRi::RI_P, 3, 1, nverts);
	m_normalStrip->initialize(TRi::RI_N, 3, 1, nverts);

	std::vector<RtFloat> unitcircle;
	s_unitCircleCache.getUnitCircle(unitcircle, m_tessU, deg2rad(thetamax));

	RtFloat changeOrientation = (RtFloat)(thetamax > 0 ? -1.0 : 1.0);

	RtFloat u, v, z;
	RtFloat p[3], n[3];

	IndexType vverts;
	IndexType uverts;

	IndexType puidx=0;
	IndexType pidx=0;

	for (	vverts = m_tessV+1, v = 0.0, z = zmin;
		vverts > 0;
		v += deltav, --vverts, z += deltaz )
	{
		if ( vverts == 1 ) {
			v = 1.0;
			z = zmax;
		}
		n[2]   = 0.0;
		p[2]   = z;

		puidx = 0;
		for (	uverts = m_tessU+1, u = 0.0;
			uverts > 0;
			u += deltau, --uverts)
		{
			if ( uverts == 1 ) {
				u = 1.0;
			}
			n[0] = unitcircle[puidx];
			p[0] = radius * n[0];
			(*m_normalStrip)[pidx] = n[0] * changeOrientation;
			(*m_vertexStrip)[pidx] = p[0];

			++puidx;
			++pidx;

			n[1] = unitcircle[puidx];
			p[1] = radius * n[1];
			(*m_normalStrip)[pidx] = n[1] *  changeOrientation;
			(*m_vertexStrip)[pidx] = p[1];

			++puidx;
			++pidx;

			(*m_normalStrip)[pidx] = n[2] *  changeOrientation;
			(*m_vertexStrip)[pidx] = p[2];

			++pidx;
		}
	}
	insertVertexIndexData();
}


// ============================================================================
// TParaboloidMesh

// ----------------------------------------------------------------------------
void TParaboloidMesh::createPN(IndexType tessU, IndexType tessV, RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax) {
	m_tessU = tessU;
	m_tessV = tessV;

	IndexType nverts = (m_tessU+1)*(m_tessV+1);

	RtFloat deltau = (RtFloat)(1.0 / (RtFloat)m_tessU);
	RtFloat deltav = (RtFloat)(1.0 / (RtFloat)m_tessV);

	newStrips();

	m_vertexStrip->initialize(TRi::RI_P, 3, 1, nverts);
	m_normalStrip->initialize(TRi::RI_N, 3, 1, nverts);

	std::vector<RtFloat> unitcircle;
	s_unitCircleCache.getUnitCircle(unitcircle, m_tessU, deg2rad(thetamax));

	RtFloat changeOrientation = (RtFloat)(thetamax > 0 ? -1.0 : 1.0);

	RtFloat u, v, r, nn, z;
	RtFloat n[3];

	IndexType nidx  = 0;
	IndexType pidx  = 0;
	IndexType puidx  = 0;

	RtFloat len;
	RtFloat m = zmax/(rmax*rmax); // 2D: f(x) = mx**2; F(x)=2mx

	RtFloat dz = (zmax - zmin) / (RtFloat)m_tessV;
	IndexType uverts;
	IndexType vverts = m_tessV+1;

	for ( z = zmin, v = 0.0; vverts > 0; z += dz, --vverts, v+=deltav ) {
		r = rmax * (RtFloat)sqrt(z / zmax);
		if ( vverts == 1 ) {
			r = rmax;
			v = 1.0;
			z = zmax;
		}

		nn = (RtFloat)(2.0*m*r);
		len = 1.0;
		uverts = m_tessU + 1;

		for ( u = 0.0, puidx=0; uverts > 0; --uverts, u+=deltau ) {
			if ( uverts == 1 )
				u = 1.0;

			n[0] = nn*unitcircle[puidx];
			n[1] = nn*unitcircle[puidx+1];
			n[2] = -1;

			(*m_vertexStrip)[pidx]   = r * unitcircle[puidx];
			(*m_vertexStrip)[++pidx] = r * unitcircle[++puidx];
			(*m_vertexStrip)[++pidx] = z;
			++pidx;
			++puidx;

			if ( uverts == m_tessU + 1 )
				len = (RtFloat)sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);

			n[0] /= len;
			n[1] /= len;
			n[2] /= len;

			(*m_normalStrip)[nidx] = n[0] * changeOrientation;
			++nidx;

			(*m_normalStrip)[nidx] = n[1] * changeOrientation;
			++nidx;

			(*m_normalStrip)[nidx] = n[2] * changeOrientation;
			++nidx;
		}
	}
	insertVertexIndexData();
}


// ============================================================================
// TConeMesh

// ----------------------------------------------------------------------------
void TConeMesh::createPN(IndexType tessU, IndexType tessV, RtFloat height, RtFloat radius, RtFloat thetamax, RtFloat displacement) {
	m_tessU = tessU;
	m_tessV = tessV;

	IndexType nverts = (m_tessU+1)*(m_tessV+1);

	RtFloat deltau = (RtFloat)(1.0 / (RtFloat)m_tessU);
	RtFloat deltav = (RtFloat)(1.0 / (RtFloat)m_tessV);

	newStrips();

	m_vertexStrip->initialize(TRi::RI_P, 3, 1, nverts);
	m_normalStrip->initialize(TRi::RI_N, 3, 1, nverts);

	std::vector<RtFloat> unitcircle;
	s_unitCircleCache.getUnitCircle(unitcircle, m_tessU, deg2rad(thetamax));

	RtFloat changeOrientation = (RtFloat)(thetamax > 0 ? -1.0 : 1.0);

	RtFloat u, v, r, z;
	RtFloat n[3];

	IndexType nidx  = 0;
	IndexType pidx  = 0;
	IndexType puidx  = 0;

	RtFloat dz = height / (RtFloat)m_tessV;

	RtFloat len = (RtFloat)sqrt(radius*radius+height*height);
	// assert len != 0 (because radius > 0)

	RtFloat mx = 0.0;
	RtFloat mz = 0.0;

	if ( len > 0.0 ) {
		mx = height/len;
		mz = radius/len; // (mx 0 mz) is the surface normal at theta = 0, mz is constant
	}

	IndexType uverts;
	IndexType vverts = m_tessV+1;

	for ( z = displacement, v = 0.0; vverts > 0; z += dz, --vverts, v+=deltav ) {
		r = radius * ((RtFloat)1.0-v);
		if ( vverts == m_tessV+1 ) {
			r = radius;
		}
		if ( vverts == 1 ) {
			r = 0.0;
			v = 1.0;
			z = height+displacement;
		}

		uverts = m_tessU + 1;

		for ( u = 0.0, puidx=0; uverts > 0; --uverts, u+=deltau ) {
			if ( uverts == 1 )
				u = 1.0;

			n[0] = unitcircle[puidx];
			n[1] = unitcircle[++puidx];
			n[2] = mz;
			++puidx;

			(*m_vertexStrip)[pidx]   = r * n[0];
			(*m_vertexStrip)[++pidx] = r * n[1];
			(*m_vertexStrip)[++pidx] = z;
			++pidx;

			if ( uverts == m_tessU + 1 )
				len = (RtFloat)sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);

			n[0] *= mx;
			n[1] *= mx;

			(*m_normalStrip)[nidx] = n[0] * changeOrientation;
			++nidx;

			(*m_normalStrip)[nidx] = n[1] * changeOrientation;
			++nidx;

			(*m_normalStrip)[nidx] = n[2] * changeOrientation;
			++nidx;
		}
	}
	insertVertexIndexData();
}


// ============================================================================
// TTorusMesh

// ----------------------------------------------------------------------------
void TTorusMesh::createPN(IndexType tessU, IndexType tessV, RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax) {
	m_tessU = tessU;
	m_tessV = tessV;
	IndexType nverts = (m_tessU+1)*(m_tessV+1);

	RtFloat deltau   = (RtFloat)(1.0 / (RtFloat)m_tessU);
	RtFloat deltav   = (RtFloat)(1.0 / (RtFloat)m_tessV);

	newStrips();

	m_vertexStrip->initialize(TRi::RI_P, 3, 1, nverts);
	m_normalStrip->initialize(TRi::RI_N, 3, 1, nverts);

	std::vector<RtFloat> unitcircle;
	s_unitCircleCache.getUnitCircle(unitcircle, m_tessU, deg2rad(thetamax));

	std::vector<RtFloat> unitcirclez;
	s_unitCircleCache.getUnitCircle(unitcirclez, m_tessV, deg2rad(phimin), deg2rad(phimax));

	RtFloat changeOrientation = (RtFloat)(thetamax > 0 ? -1.0 : 1.0);

	RtFloat u, v, r, cosphi, sintheta, costheta;
	RtFloat p[3], n[3];

	IndexType vverts;
	IndexType uverts;

	IndexType puidx=0;
	IndexType pvidx=0;
	IndexType pidx=0;

	vverts = m_tessV+1;
	for ( v = 0.0;
		vverts > 0;
		--vverts, v+=deltav )
	{
		if ( vverts == 1 ) {
			v = 1.0;
		}
		cosphi = unitcirclez[pvidx];
		n[2]   = unitcirclez[++pvidx]; // Z of the normals stays constant (Sin phi) for the same v
		++pvidx;
		r = minorrad * cosphi;  // Cos phi
		p[2] = minorrad * n[2]; // Sin phi
		uverts = m_tessU+1;
		for ( puidx=0, u=0.0; uverts > 0; --uverts, u+=deltau ) {
			if ( uverts == 1 ) {
				u = 1.0;
			}

			costheta = unitcircle[puidx];
			sintheta = unitcircle[++puidx];
			++puidx;

			p[0] = (majorrad+r) * costheta;
			p[1] = (majorrad+r) * sintheta;
			n[0] = cosphi * costheta;
			n[1] = cosphi * sintheta;
			// constant n[2] for the same v
			(*m_vertexStrip)[pidx] = p[0];
			(*m_normalStrip)[pidx] = n[0] * changeOrientation;
			++pidx;

			(*m_vertexStrip)[pidx] = p[1];
			(*m_normalStrip)[pidx] = n[1] * changeOrientation;
			++pidx;

			(*m_vertexStrip)[pidx] = p[2];
			(*m_normalStrip)[pidx] = n[2] * changeOrientation;
			++pidx;
		}
	}

	insertVertexIndexData();
}


// ============================================================================
// THyperboloidMesh

// ----------------------------------------------------------------------------
void THyperboloidMesh::createPN(IndexType tessU, IndexType tessV, RtPoint point1, RtPoint point2, RtFloat thetamax) {
	m_tessU = tessU;
	m_tessV = tessV;
	IndexType nverts = (m_tessU+1)*(m_tessV+1);

	RtFloat deltau = (RtFloat)(1.0 / (RtFloat)m_tessU);
	RtFloat deltav = (RtFloat)(1.0 / (RtFloat)m_tessV);

	newStrips();

	m_vertexStrip->initialize(TRi::RI_P, 3, 1, nverts);
	m_normalStrip->initialize(TRi::RI_N, 3, 1, nverts);

	std::vector<RtFloat> unitcircle;
	s_unitCircleCache.getUnitCircle(unitcircle, m_tessU, deg2rad(thetamax));

	RtFloat changeOrientation = (RtFloat)(thetamax > 0 ? -1.0 : 1.0);

	RtFloat u, v, len, costheta, sintheta;
	RtFloat p[3], p0[3], n[3];

	IndexType vverts;
	IndexType uverts;

	IndexType puidx=0;
	IndexType pidx=0;

	RtFloat xl = point2[0] - point1[0];
	RtFloat yl = point2[1] - point1[1];
	RtFloat zl = point2[2] - point1[2];

	RtFloat deltax = xl * deltav;
	RtFloat deltay = yl * deltav;
	RtFloat deltaz = zl * deltav;

	p0[0] = point1[0];
	p0[1] = point1[1];
	p0[2] = point1[2];

	vverts = m_tessV+1;
	for ( v=0.0; vverts > 0; p0[0] += deltax, p0[1] += deltay, p0[2] += deltaz, --vverts, v += deltav ) {
		if ( vverts == 1 ) {
			v = 1.0;
			p0[0] = point2[0];
			p0[1] = point2[1];
			p0[2] = point2[2];
		}
		uverts = m_tessU+1;

		for ( u = 0.0, puidx=0; uverts > 0; --uverts, u += deltau ) {
			if ( uverts == 1 ) {
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

			len = (RtFloat)sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
			if ( len != 0.0 ) {
				n[0] /= len;
				n[1] /= len;
				n[2] /= len;
			} else {
				n[0] =  0.0;
				n[1] =  0.0;
				n[2] = -1.0;
			}

			(*m_vertexStrip)[pidx] = p[0];
			(*m_normalStrip)[pidx] = n[0] * changeOrientation;
			++pidx;

			(*m_vertexStrip)[pidx] = p[1];
			(*m_normalStrip)[pidx] = n[1] * changeOrientation;
			++pidx;

			(*m_vertexStrip)[pidx] = p[2];
			(*m_normalStrip)[pidx] = n[2] * changeOrientation;
			++pidx;
		}
	}

	insertVertexIndexData();
}


// ============================================================================
// TBilinearMesh

// ----------------------------------------------------------------------------
void TBilinearMesh::insertVertexData(const TParameterMap &pm, RtInt nu, RtInt nv, RtInt upatch, RtInt vpatch) {
	TUVPatch::insertVertexData(pm, nu, nv, upatch, vpatch);

	IndexType cornerIdx[4];
	fillCornerIdx(cornerIdx, nu, nv, upatch, vpatch);

	clearStrips();

	// Insert vertex data
	TNamedVector<RtFloat> *p = findElementVector(TRi::RI_P, m_vertexData);
	if ( p ) {
		m_freeVertexStrip = false;
		m_vertexStrip = p;
	} else {
		m_freeVertexStrip = true;
		p = findElementVector(TRi::RI_PW, m_vertexData);
		if ( p && p->components() == 4 && p->cardinality() == 1 ) {
			m_vertexStrip = new TNamedVector<RtFloat>;
			if ( !m_vertexStrip )
				throw TSystemException(__FILE__, __LINE__);
			m_vertexStrip->initialize(TRi::RI_P, 3, 1, p->numElems());
			RtFloat *src, *dest;
			for ( src = p->begin(), dest = m_vertexStrip->begin(); src+3 < p->end(); src += 4 ) {
				if ( src[3] != 0.0 ) {
					*(dest++) = src[0]/src[3];
					*(dest++) = src[1]/src[3];
					*(dest++) = src[2]/src[3];
				} else {
					*(dest++) = src[3];
					*(dest++) = src[3];
					*(dest++) = src[3];
				}
			}
		} else {
			p = findElementVector(TRi::RI_PZ, m_vertexData);
			if ( p && p->components() == 1 && p->cardinality() == 1 ) {
				m_vertexStrip = new TNamedVector<RtFloat>;
				if ( !m_vertexStrip )
					throw TSystemException(__FILE__, __LINE__);
				RtFloat uv[12] = {
					//  u    v    (z)
					0.0, 0.0, 0.0,
					1.0, 0.0, 0.0,
					0.0, 1.0, 0.0,
					1.0, 1.0, 0.0
				};
				IndexType idx[4] = {0, 1, 2, 3};
				bilinearBlend(m_tessU, m_tessV, idx, 3, uv, *m_vertexStrip, TRi::RI_P);
				assert(m_vertexStrip->numElems() == p->numElems());
				if ( m_vertexStrip->numElems() == p->numElems() ) {
					RtFloat *src, *dest;
					for ( src = p->begin(), dest = m_vertexStrip->begin(); src != p->end(); ++src ) {
						dest[2] = *src;
						dest += 3;
					}
				}
			}
		}
	}

	// Insert normal data
	p = findElementVector(TRi::RI_N, m_vertexData);
	if ( p ) {
		m_freeNormalStrip = false;
		m_normalStrip = p;
	} else {
		const TParameter *param = pm.getParameterOf(TRi::RI_N);
		IndexType idx = (IndexType)(nu*vpatch+upatch);
		if ( param && param->getBasicType() == TYPE_FLOAT &&
			param->getComponents() == 3 && param->getCardinality() == 1 &&
			( (param->getClass() == CLASS_UNIFORM && param->getSize() > idx) ||
			param->getClass() == CLASS_CONSTANT )
			) {
				m_useUniformNormal = true;
				RtFloat *ptr = (RtFloat *)param->getValue();
				if ( param->getClass() == CLASS_UNIFORM ) {
					m_uniformNormal[0] = ptr[idx*3+0];
					m_uniformNormal[1] = ptr[idx*3+1];
					m_uniformNormal[2] = ptr[idx*3+2];
				} else {
					m_uniformNormal[0] = ptr[0];
					m_uniformNormal[1] = ptr[1];
					m_uniformNormal[2] = ptr[2];
				}
		} else {
			m_freeNormalStrip = true;
			if ( m_vertexStrip ) {
				m_normalStrip = new TNamedVector<RtFloat>;
				if ( !m_normalStrip )
					throw TSystemException(__FILE__, __LINE__);
				// Calculate normals
				RtFloat *pt = m_vertexStrip->begin();
				RtFloat nm[12];
				IndexType pt0 =   0;
				IndexType pt1 =   m_tessU                 * 3;
				IndexType pt2 =  (m_tessV   *(m_tessU+1)) * 3;
				IndexType pt3 = ((m_tessV+1)*(m_tessU+1)) * 3 - 3;
				planeNorm(pt+pt2, pt+pt0, pt+pt1, &nm[0*3]); 
				planeNorm(pt+pt0, pt+pt1, pt+pt3, &nm[1*3]); 
				planeNorm(pt+pt3, pt+pt2, pt+pt0, &nm[2*3]); 
				planeNorm(pt+pt1, pt+pt3, pt+pt2, &nm[3*3]); 
				IndexType idx[4] = {0, 1, 2, 3};
				bilinearBlend(m_tessU, m_tessV, idx, 3, nm, *m_normalStrip, TRi::RI_N);
			}
		}
	}

	insertVertexIndexData();
}


// ============================================================================
// TBicubicMesh

// ----------------------------------------------------------------------------
// bicubicBlend
// Bicubic blending of a 4x4 patch, u major
// Indexes
//  0  1  2  3     u->
//  4  5  6  7
//  8  9 10 11
// 12 13 14 15
void TBicubicMesh::bicubicBlend(
		const IndexType *idx,
		const TParameter &source,
		TNamedVector<RtFloat> &dest,
		const TUVVector &uvVect,
		bool clearAndReserve)
{
	// Ignore if no floats
	if ( source.getBasicType() != TYPE_FLOAT )
		return;

	// For bicubic blending one needs 16 elems.
	IndexType sz = source.getSize();
	for ( int i=0; i< 16; ++i ) {
		if ( sz <= idx[i] )
			return;
	}

	bicubicBlend(
		idx,
		source.getParameterName(),
		source.getComponents(),
		source.getCardinality(),
		(RtFloat *)source.getValue(),
		dest,
		uvVect,
		clearAndReserve);
}

//! Blends RenderMan parameters (e.g. RI_P) by the basis values
void TBicubicMesh::bicubicBlend(
		const IndexType *idx,
		const char *name,
		IndexType components,
		IndexType cardinality,
		const RtFloat *source,
		TNamedVector<RtFloat> &dest,
		const TUVVector &uvvect,
		bool clearAndReserve)
{
	IndexType numComponents = components*cardinality;

	std::vector<RtFloat> temp;
	temp.resize(4*numComponents);
	std::vector<RtFloat> ds;
	ds.resize(numComponents);

	IndexType numElems = (m_tessU+1)*(m_tessV+1);

	IndexType i, j, k, u, v;

	if ( clearAndReserve ) {
		dest.clear();
		dest.initialize(name, components, cardinality, numElems);
	}

	RtFloat *ptr = dest.begin();

	for ( v = 0; v < m_tessV+1; ++v ) {
		for ( i = 0, k = 0; i < 4; ++i ) {
			for ( j = 0; j < numComponents; ++j, ++k ) {
				temp[k] =
					uvvect.m_vVector[v*4]   * source[idx[ i    ] * numComponents + j] +
					uvvect.m_vVector[v*4+1] * source[idx[(i+ 4)] * numComponents + j] +
					uvvect.m_vVector[v*4+2] * source[idx[(i+ 8)] * numComponents + j] +
					uvvect.m_vVector[v*4+3] * source[idx[(i+12)] * numComponents + j];
			}
		}
		for ( u = 0; u < m_tessU+1; ++u ) {
			for ( j = 0; j < numComponents; ++j ) {
				*(ptr++) =
					uvvect.m_uVector[u*4]   * temp[                j] +
					uvvect.m_uVector[u*4+1] * temp[  numComponents+j] +
					uvvect.m_uVector[u*4+2] * temp[2*numComponents+j] +
					uvvect.m_uVector[u*4+3] * temp[3*numComponents+j];
			}
		}
	}
}


// ----------------------------------------------------------------------------
void TBicubicMesh::bicubicBlendNormals(
		const IndexType *idx,
		const TParameter &source,
		TNamedVector<RtFloat> &dest,
		const TUVVector &uvVect,
		bool clearAndReserve)
{
	// Ignore if no floats
	if ( source.getBasicType() != TYPE_FLOAT )
		return;

	// For bicubic blending one needs 16 elems.
	IndexType sz = source.getSize();
	for ( int i=0; i< 16; ++i ) {
		if ( sz <= idx[i] )
			return;
	}

	bicubicBlendNormals(
		idx,
		source.getParameterName(),
		source.getComponents(),
		source.getCardinality(),
		(RtFloat *)source.getValue(),
		dest,
		uvVect,
		clearAndReserve);
}

// ----------------------------------------------------------------------------
void TBicubicMesh::bicubicBlendNormals(
	const IndexType *idx,
	const char *name,
	IndexType components,
	IndexType cardinality,
	const RtFloat *source,
	TNamedVector<RtFloat> &dest,
	const TUVVector &uvvect,
	bool clearAndReserve)
{
	if ( components != 3 && cardinality != 1 )
		return;

	const IndexType numComponents = components * cardinality;
	IndexType numElems = (m_tessU+1)*(m_tessV+1);

	std::vector<RtFloat> temp;
	temp.resize(4*numComponents);
	std::vector<RtFloat> ds;
	ds.resize(numComponents);

	freePd();
	if ( m_freeNormalStrip && m_normalStrip )
		delete m_normalStrip;
	m_normalStrip = NULL;
	m_freeNormalStrip = false;

	if ( clearAndReserve ) {
		dest.clear();
		dest.initialize(name, components, cardinality, numElems);
	}

	m_Pdu = new TNamedVector<RtFloat>(name, components, cardinality, numElems);
	if ( !m_Pdu )
		return;
	m_freePdu = true;

	m_Pdv = new TNamedVector<RtFloat>(name, components, cardinality, numElems);
	if ( !m_Pdv )
		return;
	m_freePdv = true;

	m_normalStrip = new TNamedVector<RtFloat>(TRi::RI_N, 3, 1, numElems);
	if ( !m_normalStrip )
		return;
	m_freeNormalStrip = true;

	RtFloat *ptr = dest.begin();

	RtFloat *ptru = m_Pdu->begin();
	if ( !ptru )
		return;

	RtFloat *ptrv = m_Pdv->begin();
	if ( !ptrv )
		return;

	RtFloat *nrm = m_normalStrip->begin();
	if ( !nrm )
		return;

	IndexType i, j, k, u, v;

	for ( v = 0; v < m_tessV+1; ++v ) {
		for ( i = 0, k = 0; i < 4; ++i ) {
			for ( j = 0; j < numComponents; ++j, ++k ) {
				temp[k] =
					uvvect.m_vVector[v*4]   * source[idx[ i    ]*numComponents + j] +
					uvvect.m_vVector[v*4+1] * source[idx[(i+ 4)]*numComponents + j] +
					uvvect.m_vVector[v*4+2] * source[idx[(i+ 8)]*numComponents + j] +
					uvvect.m_vVector[v*4+3] * source[idx[(i+12)]*numComponents + j];
			}
		}
		for ( u = 0; u < m_tessU+1; ++u ) {
			for ( j = 0; j < numComponents; ++j ) {
				*(ptru++) =
					uvvect.m_duVector[u*4]   * temp[                j] +
					uvvect.m_duVector[u*4+1] * temp[  numComponents+j] +
					uvvect.m_duVector[u*4+2] * temp[2*numComponents+j] +
					uvvect.m_duVector[u*4+3] * temp[3*numComponents+j];
				*(ptr++) =
					uvvect.m_uVector[u*4]   * temp[                j] +
					uvvect.m_uVector[u*4+1] * temp[  numComponents+j] +
					uvvect.m_uVector[u*4+2] * temp[2*numComponents+j] +
					uvvect.m_uVector[u*4+3] * temp[3*numComponents+j];
			}
		}
	}

	RtFloat *ptrvsav;
	for ( u = 0; u < m_tessU+1; ++u ) {
		for ( i = 0, k = 0; i < 4; ++i ) {
			for ( j = 0; j < numComponents; ++j, ++k ) {
				temp[k] =
					uvvect.m_uVector[u*4]   * source[idx[(i*4)  ]*numComponents+j] +
					uvvect.m_uVector[u*4+1] * source[idx[(i*4+1)]*numComponents+j] +
					uvvect.m_uVector[u*4+2] * source[idx[(i*4+2)]*numComponents+j] +
					uvvect.m_uVector[u*4+3] * source[idx[(i*4+3)]*numComponents+j];
			}
		}

		ptrvsav = ptrv + numComponents;
		for ( v = 0; v < m_tessV+1; ++v ) {
			for ( j = 0; j < numComponents; ++j ) {
				*(ptrv+j) =
					uvvect.m_dvVector[v*4]   * temp[                j] +
					uvvect.m_dvVector[v*4+1] * temp[  numComponents+j] +
					uvvect.m_dvVector[v*4+2] * temp[2*numComponents+j] +
					uvvect.m_dvVector[v*4+3] * temp[3*numComponents+j];
			}
			ptrv += (m_tessU+1) * numComponents;
		}
		ptrv = ptrvsav;
	}

	ptr  = dest.begin();
	ptru = m_Pdu->begin();
	ptrv = m_Pdv->begin();

	if ( components == 4 ) {
		RtFloat w2;
		for ( i = 0; i < numElems; ++i ) {
			w2 = ptr[3] * ptr[3];
			for ( j = 0; j < 3; ++j ) {
				ptru[j] = (ptr[3]*ptru[j] - ptr[j]*ptru[3])/w2;
				ptrv[j] = (ptr[3]*ptrv[j] - ptr[j]*ptrv[3])/w2;
			}
			ptr  += components;
			ptru += components;
			ptrv += components;
		}
		ptr  = dest.begin();
		ptru = m_Pdu->begin();
		ptrv = m_Pdv->begin();
	}

	for ( i = 0; i < numElems; ++i ) {
		planeNorm(ptrv, ptru, nrm);
		nrm  += 3;
		ptru += components;
		ptrv += components;
	}

}

// ----------------------------------------------------------------------------
void TBicubicMesh::insertVertexDataUV(
									  const TParameterMap &pm,
									  const TUVVector &uv,
									  RtInt nu, RtInt nv,
									  RtInt nu_bilin, RtInt nv_bilin,
									  RtInt upatch, RtInt vpatch,
									  RtInt ustep, RtInt vstep)
{
	IndexType cornerIdx[4];
	fillCornerIdx(cornerIdx, nu_bilin, nv_bilin, upatch, vpatch);

	IndexType faceCornerIdx[4] = {
		m_faceIdx*4+0, m_faceIdx*4+1,
		m_faceIdx*4+2, m_faceIdx*4+3,
	};

	IndexType cubicIdx[16] =
	{
		((vpatch*vstep+0)%nv)*nu + (upatch*ustep+0)%nu,  ((vpatch*vstep+0)%nv)*nu + (upatch*ustep+1)%nu,  ((vpatch*vstep+0)%nv)*nu + (upatch*ustep+2)%nu,  ((vpatch*vstep+0)%nv)*nu + (upatch*ustep+3)%nu,
		((vpatch*vstep+1)%nv)*nu + (upatch*ustep+0)%nu,  ((vpatch*vstep+1)%nv)*nu + (upatch*ustep+1)%nu,  ((vpatch*vstep+1)%nv)*nu + (upatch*ustep+2)%nu,  ((vpatch*vstep+1)%nv)*nu + (upatch*ustep+3)%nu,
		((vpatch*vstep+2)%nv)*nu + (upatch*ustep+0)%nu,  ((vpatch*vstep+2)%nv)*nu + (upatch*ustep+1)%nu,  ((vpatch*vstep+2)%nv)*nu + (upatch*ustep+2)%nu,  ((vpatch*vstep+2)%nv)*nu + (upatch*ustep+3)%nu,
		((vpatch*vstep+3)%nv)*nu + (upatch*ustep+0)%nu,  ((vpatch*vstep+3)%nv)*nu + (upatch*ustep+1)%nu,  ((vpatch*vstep+3)%nv)*nu + (upatch*ustep+2)%nu,  ((vpatch*vstep+3)%nv)*nu + (upatch*ustep+3)%nu
	};

	IndexType faceCubicIdx[16] =
	{
		m_faceIdx*16+ 0, m_faceIdx*16+ 1, m_faceIdx*16+ 2, m_faceIdx*16+ 3,
		m_faceIdx*16+ 4, m_faceIdx*16+ 5, m_faceIdx*16+ 6, m_faceIdx*16+ 7,
		m_faceIdx*16+ 8, m_faceIdx*16+ 9, m_faceIdx*16+10, m_faceIdx*16+11,
		m_faceIdx*16+12, m_faceIdx*16+13, m_faceIdx*16+14, m_faceIdx*16+15,
	};

	IndexType size = (IndexType)pm.getSize();
	IndexType i;
	for ( i=0; i<size; ++i ) {
		const TParameter *p = pm.getParameterOf(i);
		if ( p ) {
			if ( p->getBasicType() == TYPE_FLOAT ) {
				// Single bicubic patches using CLASS_VARYING and CLASS_VERTEX only
				if ( p->getClass() == CLASS_VARYING ) {
					m_vertexData.resize(m_vertexData.size()+1);
					bilinearBlend(m_tessU, m_tessV, cornerIdx, *p, m_vertexData.back());
				} else if ( p->getClass() == CLASS_FACEVARYING ) {
					m_vertexData.resize(m_vertexData.size()+1);
					bilinearBlend(m_tessU, m_tessV, faceCornerIdx, *p, m_vertexData.back());
				} else if ( p->getClass() == CLASS_VERTEX || p->getClass() == CLASS_FACEVERTEX ) {
					IndexType *idx = p->getClass() == CLASS_VERTEX ? &cubicIdx[0] : &faceCubicIdx[0];
					m_vertexData.resize(m_vertexData.size()+1);
					TNamedVector<RtFloat> &v = m_vertexData.back();
					if ( !strcmp(TRi::RI_P, p->getName()) ) {
						if ( !m_vertexStrip ) {
							bicubicBlendNormals(idx, *p, v, uv, true);
							m_freeVertexStrip = false;
							m_vertexStrip = &v;
						} else {
							bicubicBlend(idx, *p, v, uv, true);
						}
					} else if ( !strcmp(TRi::RI_PW, p->getName()) ) {
						if ( !m_vertexStrip ) {
							bicubicBlendNormals(idx, *p, v, uv, true);
							m_freeVertexStrip = true;
							m_vertexStrip = new TNamedVector<RtFloat>;
							if ( !m_vertexStrip )
								throw TSystemException(__FILE__, __LINE__);
							m_vertexStrip->initialize(TRi::RI_P, 3, 1, v.numElems());
							const RtFloat *src;
							RtFloat *dest;
							for ( src = v.begin(), dest = m_vertexStrip->begin(); src+3 < v.end(); src += 4 ) {
								if ( src[3] != 0.0 ) {
									*(dest++) = src[0]/src[3];
									*(dest++) = src[1]/src[3];
									*(dest++) = src[2]/src[3];
								} else {
									*(dest++) = src[0];
									*(dest++) = src[1];
									*(dest++) = src[2];
								}
							}
						} else {
							bicubicBlend(idx, *p, v, uv, true);
						}
					} else if ( !strcmp(TRi::RI_PZ, p->getName()) ) {
						if ( !m_vertexStrip ) {
							TNamedVector<RtFloat> ri_p("RI_P", 3, 1, 16);
							const RtFloat *src = (const RtFloat *)p->getValue();
							RtFloat *dest = ri_p.begin();
							IndexType stepsize = p->getComponents() * p->getCardinality();
							IndexType zidx = 0;
							IndexType uzidx;
							IndexType vzidx;
							RtFloat tval[4] = {(RtFloat)0.0, (RtFloat)(1.0/3.0), (RtFloat)(2.0/3.0), (RtFloat)1.0};
							for ( vzidx = 0; vzidx < 4; ++vzidx ) {
								for ( uzidx = 0; uzidx < 4; ++uzidx, ++zidx ) {
									*(dest++) = tval[uzidx];
									*(dest++) = tval[vzidx];
									*(dest++) = src[idx[zidx]];
								}
							}
							bicubicBlendNormals(idx, TRi::RI_PZ, 3, 1, ri_p.begin(), v, uv, true);
							m_freeVertexStrip = false;
							m_vertexStrip = &v;
						} else {
							bicubicBlend(idx, *p, v, uv, true);
						}
					} else {
						bicubicBlend(idx, *p, v, uv, true);
					}
				}
			}
		}
	}

	insertVertexIndexData();
}


// ============================================================================
// TNuMesh

void TNuMesh::nuBlend(
					  const TParameter &source,
					  TNamedVector<RtFloat> &dest,
					  RtInt useg,
					  RtInt vseg,
					  const TBSplineBasis &uBasis,
					  const TBSplineBasis &vBasis,
					  const std::vector<IndexType> &idx
					  ) const
{
	nuBlend(
		source.getParameterName(),
		source.getComponents(),
		source.getCardinality(),
		(const RtFloat *)source.getValue(),
		dest,
		useg, vseg,
		uBasis, vBasis,
		idx,
		true
		);
}

void TNuMesh::nuBlend(
					  const char *name,
					  IndexType components,
					  IndexType cardinality,
					  const RtFloat *source,
					  TNamedVector<RtFloat> &dest,
					  RtInt useg,
					  RtInt vseg,
					  const TBSplineBasis &uBasis,
					  const TBSplineBasis &vBasis,
					  const std::vector<IndexType> &idx,
					  bool clearAndReserve) const
{
	RtInt uspan = useg + uBasis.m_order - 1;
	RtInt vspan = vseg + vBasis.m_order - 1;

	IndexType pnu     = uBasis.m_valCnts[uspan];
	IndexType pnv     = vBasis.m_valCnts[vspan];
	IndexType uoffset = uBasis.m_valOffs[uspan];
	IndexType voffset = vBasis.m_valOffs[vspan];

	IndexType numElementsSource = components * cardinality;
	IndexType numElems = pnu*pnv;

	if ( clearAndReserve ) {
		dest.clear();
		if ( numElems > 0 )
			dest.initialize(
				name,
				components,
				cardinality,
				numElems
				);
	}

	const RtFloat *s = source;
	if ( !s )
		return;

	RtFloat *ptr = dest.begin();
	if ( !ptr )
		return;

	std::vector<RtFloat> temp;
	temp.resize(numElementsSource * uBasis.m_order);

	IndexType i, j, k, u, v;

	RtFloat vBaseElem, uBaseElem, comp;

	for ( v = 0; v < pnv; ++v ) {

		for ( i = 0; i < (IndexType)uBasis.m_order; ++i ) {
			for ( j = 0; j < numElementsSource; ++j ) {
				temp[i*numElementsSource + j] = 0;
			}
			for ( k = 0; k < (IndexType)vBasis.m_order; ++k ) {
				vBaseElem = vBasis.m_basis[(IndexType)((voffset+v)*vBasis.m_order+k)];
				for ( j = 0; j < numElementsSource; ++j ) {
					// comp = s[(vseg+k)*uBasis.m_ncpts*numElementsSource + (useg+i)*numElementsSource + j];
					comp = s[idx[k*uBasis.m_order+i]*numElementsSource+j];
					temp[i*numElementsSource + j] +=
						comp * vBaseElem;
				}
			}
		}

		for ( u = 0; u < pnu; ++u, ptr += numElementsSource ) {
			for ( j = 0; j < numElementsSource; ++j ) {
				*(ptr+j) = 0;
			}
			for ( k = 0; k < (IndexType)uBasis.m_order; ++k ) {
				uBaseElem = uBasis.m_basis[(IndexType)((uoffset+u)*uBasis.m_order+k)];
				for ( j = 0; j < numElementsSource; ++j ) {
					comp = temp[k*numElementsSource + j];
					*(ptr+j) += comp * uBaseElem;
				}
			}
		}
	}
}


void TNuMesh::nuBlendNormals(
					  const TParameter &source,
					  TNamedVector<RtFloat> &dest,
					  RtInt useg,
					  RtInt vseg,
					  const TBSplineBasis &uBasis,
					  const TBSplineBasis &vBasis,
					  const std::vector<IndexType> &idx
					  )
{
	nuBlendNormals(
		source.getParameterName(),
		source.getComponents(),
		source.getCardinality(),
		(const RtFloat *)source.getValue(),
		dest,
		useg, vseg,
		uBasis, vBasis,
		idx,
		true
		);
}


void TNuMesh::nuBlendNormals(
					  const char *name,
					  IndexType components,
					  IndexType cardinality,
					  const RtFloat *source,
					  TNamedVector<RtFloat> &dest,
					  RtInt useg,
					  RtInt vseg,
					  const TBSplineBasis &uBasis,
					  const TBSplineBasis &vBasis,
					  const std::vector<IndexType> &idx,
					  bool clearAndReserve)
{
	RtInt uspan = useg + uBasis.m_order - 1;
	RtInt vspan = vseg + vBasis.m_order - 1;

	IndexType pnu = uBasis.m_valCnts[uspan];
	IndexType pnv = vBasis.m_valCnts[vspan];
	IndexType uoffset = uBasis.m_valOffs[uspan];
	IndexType voffset = vBasis.m_valOffs[vspan];

	IndexType numElementsSource = components * cardinality;
	IndexType numElems = pnu*pnv;

	freePd();
	if ( m_freeNormalStrip && m_normalStrip )
		delete m_normalStrip;
	m_normalStrip = NULL;
	m_freeNormalStrip = false;

	if ( numElems <= 0 )
		return;

	if ( clearAndReserve ) {
		dest.clear();
		if ( numElems > 0 )
			dest.initialize(
				name,
				components,
				cardinality,
				numElems
				);
	}

	// calc the partial derivatives with respect to u and v and the normals
	// example for homogene system:
	// calculate in the first step: dx/du, dy/du, dz/du
	// then dX/du = (w*dx/du - x*dw/du)/w**2
	// dY/du, dZ/du analog
	// then the dv and at last the normals as crossproduct of the partial derivatives

	m_Pdu = new TNamedVector<RtFloat>(name, components, cardinality, numElems);
	if ( !m_Pdu )
		return;
	m_freePdu = true;
	m_Pdv = new TNamedVector<RtFloat>(name, components, cardinality, numElems);
	if ( !m_Pdv )
		return;
	m_freePdv = true;

	m_normalStrip = new TNamedVector<RtFloat>(TRi::RI_N, 3, 1, numElems);
	if ( !m_normalStrip )
		return;
	m_freeNormalStrip = true;

	const RtFloat *s = source;
	if ( !s )
		return;

	RtFloat *ptru = m_Pdu->begin();
	if ( !ptru )
		return;

	RtFloat *ptrv = m_Pdv->begin();
	if ( !ptrv )
		return;

	RtFloat *nrm = m_normalStrip->begin();
	if ( !nrm )
		return;

	RtFloat *ptr = dest.begin();
	if ( !ptr )
		return;

	std::vector<RtFloat> temp;
	temp.resize(numElementsSource * uBasis.m_order);

	IndexType i, j, k, u, v;
	RtFloat w2;

	RtFloat uBaseElem, vBaseElem, duBaseElem, dvBaseElem, comp;

	for ( v = 0; v < pnv; ++v ) {

		for ( i = 0; i < (IndexType)uBasis.m_order; ++i ) {
			for ( j = 0; j < numElementsSource; ++j ) {
				temp[i*numElementsSource + j] = 0;
			}
			for ( k = 0; k < (IndexType)vBasis.m_order; ++k ) {
				vBaseElem = vBasis.m_basis[(IndexType)((voffset+v)*vBasis.m_order+k)];
				for ( j = 0; j < numElementsSource; ++j ) {
					// comp = s[(vseg+k)*uBasis.m_ncpts*numElementsSource + (useg+i)*numElementsSource + j];
					comp = s[idx[k*uBasis.m_order+i]*numElementsSource+j];
					temp[i*numElementsSource + j] += comp * vBaseElem;
				}
			}
		}

		for ( u = 0; u < pnu; ++u, ptr += numElementsSource, ptru += numElementsSource ) {
			for ( j = 0; j < numElementsSource; ++j ) {
				*(ptr+j) = 0;
				*(ptru+j) = 0;
			}
			for ( k = 0; k < (IndexType)uBasis.m_order; ++k ) {
				uBaseElem  = uBasis.m_basis[(IndexType)((uoffset+u)*uBasis.m_order+k)];
				duBaseElem = uBasis.m_basisDeriv[(IndexType)((uoffset+u)*uBasis.m_order+k)];
				for ( j = 0; j < numElementsSource; ++j ) {
					comp = temp[k*numElementsSource + j];
					*(ptr+j)  += comp * uBaseElem;
					*(ptru+j) += comp * duBaseElem;
				}
			}
		}
	}

	temp.clear();
	temp.resize(numElementsSource * vBasis.m_order);
	RtFloat *ptrvsav;

	for ( u = 0; u < pnu; ++u ) {

		for ( i = 0; i < (IndexType)vBasis.m_order; ++i ) {
			for ( j = 0; j < numElementsSource; ++j ) {
				temp[i*numElementsSource + j] = 0;
			}
			for ( k = 0; k < (IndexType)uBasis.m_order; ++k ) {
				uBaseElem = uBasis.m_basis[(IndexType)((uoffset+u)*uBasis.m_order+k)];
				for ( j = 0; j < numElementsSource; ++j ) {
					// comp = s[(vseg+i)*uBasis.m_ncpts*numElementsSource + (useg+k)*numElementsSource + j];
					comp = s[idx[i*uBasis.m_order+k]*numElementsSource + j];
					temp[i*numElementsSource + j] += comp * uBaseElem;
				}
			}
		}

		ptrvsav = ptrv + numElementsSource;
		for ( v = 0; v < pnv; ++v, ptrv += (numElementsSource * pnu)  ) {
			for ( j = 0; j < numElementsSource; ++j ) {
				*(ptrv+j) = 0;
			}
			for ( k = 0; k < (IndexType)vBasis.m_order; ++k ) {
				dvBaseElem = vBasis.m_basisDeriv[(IndexType)((voffset+v)*vBasis.m_order+k)];
				for ( j = 0; j < numElementsSource; ++j ) {
					comp = temp[k*numElementsSource + j];
					*(ptrv+j) += comp * dvBaseElem;
				}
			}
		}
		ptrv = ptrvsav;
	}

	ptr  = dest.begin();
	ptru = m_Pdu->begin();
	ptrv = m_Pdv->begin();

	if ( numElementsSource == 4 ) {
		for ( i = 0; i < numElems; ++i ) {
			w2 = ptr[3] * ptr[3];
			for ( j = 0; j < 3; ++j ) {
				ptru[j] = (ptr[3]*ptru[j] - ptr[j]*ptru[3])/w2;
				ptrv[j] = (ptr[3]*ptrv[j] - ptr[j]*ptrv[3])/w2;
			}
			ptr  += numElementsSource;
			ptru += numElementsSource;
			ptrv += numElementsSource;
		}
		ptr  = dest.begin();
		ptru = m_Pdu->begin();
		ptrv = m_Pdv->begin();
	}

	for ( i = 0; i < numElems; ++i ) {
		planeNorm(ptrv, ptru, nrm);
		nrm  += 3;
		ptru += numElementsSource;
		ptrv += numElementsSource;
	}
}

void TNuMesh::insertVertexDataNU(
	const TParameterMap &pm,
	RtInt useg, RtInt vseg,
	const TBSplineBasis &uBasis, const TBSplineBasis &vBasis,
	TParameterGrid &paramGrid
	)
{
	RtInt uspan = useg + uBasis.m_order - 1;
	RtInt vspan = vseg + vBasis.m_order - 1;

	if ( uBasis.m_valCnts[uspan] == 0 || vBasis.m_valCnts[vspan] == 0 ) {
		tessU(0);
		tessV(0);
		return;
	}

	tessU(uBasis.m_valCnts[uspan]-1);
	tessV(vBasis.m_valCnts[vspan]-1);

	IndexType n = (IndexType)uBasis.m_ncpts * (IndexType)vBasis.m_ncpts;
	IndexType faceverts = (IndexType)(uBasis.m_ncpts+numFacesU()) * (IndexType)(vBasis.m_ncpts+numFacesV());
	IndexType size = (IndexType)pm.getSize();

	IndexType cornerIdx[4];
	IndexType cornerIdIdx[4] = {0, 1, 2, 3};
	fillCornerIdx(cornerIdx, uBasis.m_segments+1, vBasis.m_segments+1, useg, vseg); // m_cpts
	IndexType corners = (useg+1) * (vseg+1);
	IndexType faceCornerIdx[4] = {
		m_faceIdx*4+0, m_faceIdx*4+1,
		m_faceIdx*4+2, m_faceIdx*4+3,
	};
	IndexType facecorners     = m_numFacesU * m_numFacesV * 4;
	IndexType verticesPerFace = uBasis.m_order * vBasis.m_order;
	IndexType facevertices    = verticesPerFace * m_numFaces;

	IndexType i, j, k;
	std::vector<IndexType> vertexIdx(verticesPerFace);
	std::vector<IndexType> facevertexIdx(verticesPerFace);
	std::vector<IndexType> identityIdx(verticesPerFace);

	k = 0;
	for ( i = 0; i < (IndexType)vBasis.m_order; ++i ) {
		for ( j = 0; j < (IndexType)uBasis.m_order; ++j, ++k ) {
			vertexIdx[k]     = (vseg + i) * uBasis.m_ncpts + useg + j;
			facevertexIdx[k] = m_faceIdx * verticesPerFace + k;
			identityIdx[k] = k;
		}
	}

	RtFloat *texture_s_found = NULL;
	RtFloat *texture_t_found = NULL;
	RtFloat *texture_st_found = NULL;

	for ( i=0; i<size; ++i ) {
		const TParameter *p = pm.getParameterOf(i);
		if ( p ) {
			if ( p->getBasicType() == TYPE_FLOAT ) {
				if ( p->getClass() == CLASS_VARYING && p->getSize() >= corners ) {
					m_vertexData.resize(m_vertexData.size()+1);
					bilinearBlend(m_tessU, m_tessV, cornerIdx, *p, m_vertexData.back());
				} else if ( p->getClass() == CLASS_FACEVARYING && p->getSize() >= facecorners ) {
					m_vertexData.resize(m_vertexData.size()+1);
					bilinearBlend(m_tessU, m_tessV, faceCornerIdx, *p, m_vertexData.back());
				} else if (
					(p->getClass() == CLASS_VERTEX && p->getSize() >= n) ||
					(p->getClass() == CLASS_FACEVERTEX && p->getSize() >= facevertices)
				  ) {
					const std::vector<IndexType> &idx = p->getClass() == CLASS_VERTEX ? vertexIdx : facevertexIdx;
					m_vertexData.resize(m_vertexData.size()+1);
					TNamedVector<RtFloat> &v = m_vertexData.back();
					if ( !strcmp(TRi::RI_P, p->getName()) ) {
						if ( !m_vertexStrip ) {
							nuBlendNormals(*p, v, useg, vseg, uBasis, vBasis, idx);
							m_freeVertexStrip = false;
							m_vertexStrip = &v;
						} else {
							nuBlend(*p, v, useg, vseg, uBasis, vBasis, idx);
						}
					} else if ( !strcmp(TRi::RI_PW, p->getName()) ) {
						if ( !m_vertexStrip ) {
							nuBlendNormals(*p, v, useg, vseg, uBasis, vBasis, idx);
							m_freeVertexStrip = true;
							m_vertexStrip = new TNamedVector<RtFloat>;
							if ( !m_vertexStrip )
								throw TSystemException(__FILE__, __LINE__);
							m_vertexStrip->initialize(TRi::RI_P, 3, 1, v.numElems());
							const RtFloat *src;
							RtFloat *dest;
							for ( src = v.begin(), dest = m_vertexStrip->begin(); src+3 < v.end(); src += 4 ) {
								if ( src[3] != 0.0 ) {
									*(dest++) = src[0]/src[3];
									*(dest++) = src[1]/src[3];
									*(dest++) = src[2]/src[3];
								} else {
									*(dest++) = src[0];
									*(dest++) = src[1];
									*(dest++) = src[2];
								}
							}
						} else {
							nuBlend(*p, v, useg, vseg, uBasis, vBasis, idx);
						}
					} else if ( !strcmp(TRi::RI_PZ, p->getName()) ) {
						if ( !m_vertexStrip ) {
							TNamedVector<RtFloat> ri_p("RI_P", 3, 1, verticesPerFace);
							const RtFloat *src = (const RtFloat *)p->getValue();
							RtFloat *dest = ri_p.begin();
							IndexType stepsize = p->getComponents() * p->getCardinality();
							IndexType zidx = 0;
							IndexType uzidx;
							IndexType vzidx;
							RtFloat yval;
							for ( vzidx = 0; vzidx < (IndexType)vBasis.m_order; ++vzidx ) {
								yval = vBasis.m_knots[vBasis.m_order+vseg+vzidx-1];
								for ( uzidx = 0; uzidx < (IndexType)uBasis.m_order; ++uzidx, ++zidx ) {
									*(dest++) = uBasis.m_knots[uBasis.m_order+useg+uzidx-1];
									*(dest++) = yval;
									*(dest++) = src[idx[zidx]];
								}
							}
							nuBlendNormals(TRi::RI_PZ, 3, 1, ri_p.begin(), v, useg, vseg, uBasis, vBasis, identityIdx, true);
							m_freeVertexStrip = false;
							m_vertexStrip = &v;
						} else {
							nuBlend(*p, v, useg, vseg, uBasis, vBasis, idx);
						}
					} else {
						nuBlend(*p, v, useg, vseg, uBasis, vBasis, idx);
					}
				}
				if ( !strcmp(TRi::RI_S, p->getName()) )
					texture_s_found = m_vertexData.back().begin();
				else if ( !strcmp(TRi::RI_T, p->getName()) )
					texture_t_found = m_vertexData.back().begin();
				else if ( !strcmp(TRi::RI_ST, p->getName()) )
					texture_st_found = m_vertexData.back().begin();
			}
		}
	}

	m_v.initialize("v", 1, 1, m_tessV+1);

	IndexType idx;
	for ( idx = 0; idx <= m_tessV; ++idx )
		m_v[idx] = vBasis.m_tVals[vBasis.m_valOffs[vspan]+idx];
	m_u.initialize("u", 1, 1, m_tessU+1);
	for ( idx = 0; idx <= m_tessU; ++idx )
		m_u[idx] = uBasis.m_tVals[uBasis.m_valOffs[uspan]+idx];

	insertVertexIndexData();
}

void TNuMesh::transformTextureST() {
	assert(m_st != NULL);
	// m_stRect

	RtFloat *st = m_st->begin();
	for ( IndexType idxv = 0; idxv <= m_tessV; ++idxv ) {
		for ( IndexType idxu = 0; idxu <= m_tessU; ++idxu ) {
			if ( m_transformS )
				*st = bilerp(m_u[idxu], m_v[idxv], m_stRect[IDX_S1], m_stRect[IDX_S2], m_stRect[IDX_S3], m_stRect[IDX_S4]);
			++st;
			if ( m_transformT )
				*st = bilerp(m_u[idxu], m_v[idxv], m_stRect[IDX_T1], m_stRect[IDX_T2], m_stRect[IDX_T3], m_stRect[IDX_T4]);
			++st;
		}
	}
}

// ============================================================================
// TUnitCircle

// ----------------------------------------------------------------------------
void TUnitCircleCache::getUnitCircle(std::vector<RtFloat> &circledata, IndexType tess, RtFloat thetamax) {
	RtFloat thetamin = 0.0;
	getUnitCircle(circledata, tess, thetamin, thetamax);
}


// ----------------------------------------------------------------------------
void TUnitCircleCache::getUnitCircle(std::vector<RtFloat> &circledata, IndexType tess, RtFloat thetamin, RtFloat thetamax) {
	if ( tess < 1 ) tess = 1;
	IndexType nverts = tess+1;
	IndexType i;
	RtFloat p[2];

	RtFloat delta = (thetamax-thetamin)/(RtFloat)tess;

	circledata.clear();
	circledata.resize(nverts*3);
	IndexType endIdx = (nverts-1)*3;
	for ( i=0; i<endIdx; thetamin+=delta) {
		p[0] = circledata[  i] = (RtFloat)cos(thetamin);
		p[1] = circledata[++i] = (RtFloat)sin(thetamin);
		++i;
	}
	p[0] = circledata[  i] = (RtFloat)cos(thetamax);
	p[1] = circledata[++i] = (RtFloat)sin(thetamax);
}


// ============================================================================
// TUnitSphere

// ----------------------------------------------------------------------------
const TSphereMesh &TUnitSphereCache::getUnitSphereRef(IndexType tessU, IndexType tessV) {
	bool isNew = false;
	TSphereMesh &mesh = findUV(m_meshData, tessU, tessV, m_lastIndex, isNew);
	if ( isNew ) {
		mesh.createPN(tessU, tessV, 1.0, -1.0, 1.0, 360.0);
	}
	return mesh;
}


// ----------------------------------------------------------------------------
void TUnitSphereCache::getUnitSphere(TSphereMesh &sphereMesh, IndexType tessU, IndexType tessV) {
	sphereMesh = getUnitSphereRef(tessU, tessV);
}


// ============================================================================
// TSphere

// ----------------------------------------------------------------------------
TSphereMesh &TSphere::tesselate(IndexType tessU, IndexType tessV) {
	if ( tessU < 1 ) {
		tessU = 1;
	}
	if ( tessV < 1 ) {
		tessV = 1;
	}

	bool isNew = false;
	TSphereMesh &mesh = findUV(m_meshData, tessU, tessV, m_lastIndex, isNew);
	if ( isNew ) {
		if ( m_zmin <= -m_radius && m_zmax >= m_radius && fabs(m_thetamax) >= (RtFloat)360.0 ) {
			s_unitSphereCache.getUnitSphere(mesh, tessU, tessV);
			mesh.geometry(this);
			mesh.resizeVerticesBy(m_radius);
		} else {
			mesh.geometry(this);
			mesh.createPN(tessU, tessV, m_radius, m_zmin, m_zmax, m_thetamax);
		}
		// Enter interpolated
		mesh.insertVertexData(m_parameters);
	}
	return mesh;
}


// ============================================================================
// TCylinder

// ----------------------------------------------------------------------------
TCylinderMesh &TCylinder::tesselate(IndexType tessU, IndexType tessV) {
	if ( tessU < 1 ) {
		tessU = 1;
	}
	if ( tessV < 1 ) {
		tessV = 1;
	}

	bool isNew = false;
	TCylinderMesh &mesh = findUV(m_meshData, tessU, tessV, m_lastIndex, isNew);
	if ( isNew ) {
		mesh.geometry(this);
		mesh.createPN(tessU, tessV, m_radius, m_zmin, m_zmax, m_thetamax);
		// Enter interpolated
		mesh.insertVertexData(m_parameters);
	}
	return mesh;
}


// ============================================================================
// TParaboloid

// ----------------------------------------------------------------------------
TParaboloidMesh &TParaboloid::tesselate(IndexType tessU, IndexType tessV) {
	if ( tessU < 1 ) {
		tessU = 1;
	}
	if ( tessV < 1 ) {
		tessV = 1;
	}

	bool isNew = false;
	TParaboloidMesh &mesh = findUV(m_meshData, tessU, tessV, m_lastIndex, isNew);
	if ( isNew ) {
		mesh.geometry(this);
		mesh.createPN(tessU, tessV, m_rmax, m_zmin, m_zmax, m_thetamax);
		// Enter interpolated
		mesh.insertVertexData(m_parameters);
	}
	return mesh;
}


// ============================================================================
// TCone

// ----------------------------------------------------------------------------
TConeMesh &TCone::tesselate(IndexType tessU, IndexType tessV) {
	if ( tessU < 1 ) {
		tessU = 1;
	}
	if ( tessV < 1 ) {
		tessV = 1;
	}

	bool isNew = false;
	TConeMesh &mesh = findUV(m_meshData, tessU, tessV, m_lastIndex, isNew);
	if ( isNew ) {
		mesh.geometry(this);
		mesh.createPN(tessU, tessV, m_height, m_radius, m_thetamax, 0.0);
		// Enter interpolated
		mesh.insertVertexData(m_parameters);
	}
	return mesh;
}


// ============================================================================
// TDisk

// ----------------------------------------------------------------------------
TConeMesh &TDisk::tesselate(IndexType tessU, IndexType tessV) {
	if ( tessU < 1 ) {
		tessU = 1;
	}
	if ( tessV < 1 ) {
		tessV = 1;
	}

	bool isNew = false;
	TConeMesh &mesh = findUV(m_meshData, tessU, tessV, m_lastIndex, isNew);
	if ( isNew ) {
		mesh.geometry(this);
		mesh.createPN(tessU, tessV, 0.0, m_radius, m_thetamax, m_height);
		// Enter interpolated
		mesh.insertVertexData(m_parameters);
	}
	return mesh;
}


// ============================================================================
// TTorus

// ----------------------------------------------------------------------------
TTorusMesh &TTorus::tesselate(IndexType tessU, IndexType tessV) {
	if ( tessU < 1 ) {
		tessU = 1;
	}
	if ( tessV < 1 ) {
		tessV = 1;
	}

	bool isNew = false;
	TTorusMesh &mesh = findUV(m_meshData, tessU, tessV, m_lastIndex, isNew);
	if ( isNew ) {
		mesh.geometry(this);
		mesh.createPN(tessU, tessV, m_majorrad, m_minorrad, m_phimin, m_phimax, m_thetamax);
		// Enter interpolated
		mesh.insertVertexData(m_parameters);
	}
	return mesh;
}


// ============================================================================
// THyperbolid

// ----------------------------------------------------------------------------
THyperboloidMesh &THyperboloid::tesselate(IndexType tessU, IndexType tessV) {
	if ( tessU < 1 ) {
		tessU = 1;
	}
	if ( tessV < 1 ) {
		tessV = 1;
	}

	bool isNew = false;
	THyperboloidMesh &mesh = findUV(m_meshData, tessU, tessV, m_lastIndex, isNew);
	if ( isNew ) {
		mesh.geometry(this);
		mesh.createPN(tessU, tessV, m_point1, m_point2, m_thetamax);
		// Enter interpolated
		mesh.insertVertexData(m_parameters);
	}
	return mesh;
}


// ============================================================================
// TBilinearPatch

// ----------------------------------------------------------------------------
TBilinearMesh &TBilinearPatch::tesselate(IndexType tessU, IndexType tessV) {
	if ( tessU < 1 ) {
		tessU = 1;
	}
	if ( tessV < 1 ) {
		tessV = 1;
	}

	bool isNew = false;
	TBilinearMesh &mesh = findUV(m_bilinearMeshes, tessU, tessV, m_lastIndex, isNew);
	if ( isNew ) {
		mesh.geometry(this);
		mesh.insertVertexData(m_parameters);
	}
	return mesh;
}


// ============================================================================
// TBicubicPatch

// ----------------------------------------------------------------------------
TBicubicMesh &TBicubicPatch::tesselate(IndexType tessU, IndexType tessV, const RtBasis &uBasis, const RtBasis &vBasis) {
	if ( tessU < 1 ) {
		tessU = 1;
	}
	if ( tessV < 1 ) {
		tessV = 1;
	}

	bool isNew = false;
	TBicubicMesh &mesh = findBicubicUV(m_bicubicMeshes, tessU, tessV, uBasis, vBasis, m_lastIndex, isNew);
	if ( isNew ) {
		mesh.geometry(this);
		TUVVector &uv = s_uvVectorCache.findUVVector(tessU, tessV, uBasis, vBasis);
		mesh.insertVertexDataUV(m_parameters, uv);
	}
	return mesh;
}


// ============================================================================
// TBilinearPatchMesh

// ----------------------------------------------------------------------------
IndexType TBilinearPatchMesh::tesselate(IndexType tessU, IndexType tessV) {
	if ( tessU < 1 ) {
		tessU = 1;
	}
	if ( tessV < 1 ) {
		tessV = 1;
	}

	bool isNew = false;
	findUV(m_bilinearMeshes, tessU, tessV, m_lastIndex, isNew, m_upatches*m_vpatches);
	if ( isNew ) {
		IndexType idx = m_lastIndex;
		IndexType numFaces = m_upatches*m_vpatches;
		for ( IndexType v = 0; v < m_vpatches; ++v )
			for ( IndexType u = 0; u < m_upatches; ++u, ++idx ) {
				TBilinearMesh &mesh = m_bilinearMeshes[idx];
				mesh.geometry(this);
				mesh.faceIdx(idx-m_lastIndex);
				mesh.faceIdxU(u);
				mesh.faceIdxV(v);
				mesh.numFaces(numFaces);
				mesh.numFacesU(m_upatches);
				mesh.numFacesV(m_vpatches);
				mesh.insertVertexData(m_parameters, m_nu, m_nv, u, v);
			}
	}
	return m_lastIndex;
}


// ============================================================================
// TBicubicPatchMesh

// ----------------------------------------------------------------------------
IndexType TBicubicPatchMesh::tesselate(IndexType tessU, IndexType tessV, const RtBasis &uBasis, const RtBasis &vBasis) {
	if ( tessU < 1 ) {
		tessU = 1;
	}
	if ( tessV < 1 ) {
		tessV = 1;
	}

	bool isNew = false;
	findUV(m_bicubicMeshes, tessU, tessV, m_lastIndex, isNew, m_upatches*m_vpatches);
	TUVVector &uv = s_uvVectorCache.findUVVector(tessU, tessV, uBasis, vBasis);
	if ( isNew ) {
		IndexType idx = m_lastIndex;
		IndexType numFaces = m_upatches*m_vpatches;
		for ( IndexType v = 0; v < m_vpatches; ++v )
			for ( IndexType u = 0; u < m_upatches; ++u, ++idx ) {
				TBicubicMesh &mesh = m_bicubicMeshes[idx];
				mesh.geometry(this);
				mesh.faceIdx(idx-m_lastIndex);
				mesh.faceIdxU(u);
				mesh.faceIdxV(v);
				mesh.numFaces(numFaces);
				mesh.numFacesU(m_upatches);
				mesh.numFacesV(m_vpatches);
				mesh.insertVertexDataUV(m_parameters, uv, m_nu, m_nv, m_nuBilin, m_nvBilin, u, v, m_ustep, m_vstep);
			}
	}
	return m_lastIndex;
}


// ============================================================================
// TParameterGrid

void TParameterGrid::fillaxis(std::vector<TParameterAxisCell> &axis,
	const std::vector<RtFloat> &knots, RtFloat mini, RtFloat maxi,
	RtInt ncpts, RtInt order, IndexType tess)
{
	// s.a. TBSplineBasis::calc()
	axis.clear();
	RtInt segments = 1 + ncpts - order;

	axis.clear();
	axis.reserve(segments*tess+1+2); // no duplicated borders like in TBSplineBasis::calc(), but -infinity, infinity

	TParameterAxisCell cell;
	RtFloat ts=mini, te=maxi, dt;
	cell.m_coord = -riInfinity;
	cell.m_isBorder = false;
	axis.push_back(cell);
	for ( RtInt knotIdx = order-1; knotIdx < ncpts; ++knotIdx ) {
		ts = knots[knotIdx];
		te = knots[knotIdx+1];
		if ( (te-ts) < riEpsilon ) continue;

		dt = (te-ts)/(RtFloat)tess;
		if ( dt < riEpsilon )
			dt = riEpsilon;

		if ( maxi < ts )
			break;
		if ( mini >= te )
			continue;

		// maxi >= ts && mini < te

		if ( mini > ts )
			ts = mini;
		if ( maxi < te )
			te = maxi;

		cell.m_coord = ts;
		cell.m_isBorder = true;
		axis.push_back(cell);
		RtFloat t;
		IndexType cnt;
		for ( t = knots[knotIdx], cnt = 0; t < te && cnt < tess; t += dt, ++cnt ) {
			if ( t <= ts )
				continue;
			cell.m_coord = t;
			cell.m_isBorder = false;
			axis.push_back(cell);
		}
	}
	if ( axis.size() > 0 ) {
		cell.m_coord = te;
		cell.m_isBorder = true;
		axis.push_back(cell);
	}
	cell.m_coord = riInfinity;
	cell.m_isBorder = false;
	axis.push_back(cell);
}

TParameterGrid::TParameterGrid(
	const std::vector<RtFloat> &uknots, RtFloat umin, RtFloat umax, RtInt uncpts, RtInt uorder, IndexType utess,
	const std::vector<RtFloat> &vknots, RtFloat vmin, RtFloat vmax, RtInt vncpts, RtInt vorder, IndexType vtess)
{
	reset(uknots, umin, umax, uncpts, uorder, utess, vknots, vmin, vmax, vncpts, vorder, vtess);
}


void TParameterGrid::reset(
	const std::vector<RtFloat> &uknots, RtFloat umin, RtFloat umax, RtInt uncpts, RtInt uorder, IndexType utess,
	const std::vector<RtFloat> &vknots, RtFloat vmin, RtFloat vmax, RtInt vncpts, RtInt vorder, IndexType vtess)
{
	fillaxis(m_uaxis, uknots, umin, umax, uncpts, uorder, utess);
	fillaxis(m_vaxis, vknots, vmin, vmax, vncpts, vorder, vtess);

	m_cells.clear();

	m_width  = (IndexType)m_uaxis.size();
	m_height = (IndexType)m_vaxis.size();

	if ( m_width <= 0 || m_height <= 0 )
		return;

	m_cells.resize((m_width-1)*(m_height-1));

	IndexType ui, vi, idx;

	for ( idx=0, vi = 0; vi < m_height-1; ++vi ) {
		for ( ui = 0; ui < m_width-1; ++ui, ++idx ) {
			m_cells[idx].m_left = &m_uaxis[ui];
			m_cells[idx].m_right = &m_uaxis[ui+1];
			m_cells[idx].m_top =  &m_vaxis[vi];
			m_cells[idx].m_bottom =  &m_vaxis[vi+1];
		}
	}
}


void TParameterGrid::addIntersections(const TTrimVertex &from, const TTrimVertex &to) {
	TTrimLoop &tLoop = m_trimLoops.back();
	bool nextHoriz = false;
	bool nextVerti = false;
	RtFloat hu = 0.0, hv = 0.0;
	RtFloat vu = 0.0, vv = 0.0;
	IndexType hui = 0, hvi = 0;
	IndexType vui = 0, vvi = 0;
	unsigned int regcode;
	unsigned const int left=1, right=2, bottom=4, top=8;
	TTrimVertex newFrom = from;
	TTrimVertex newTo = to;
	bool calcVert = false;

	// Special case: u to-coordinate is on a (left) border
	if ( newTo.m_vtype & uaxisCoord ) {
		if ( newTo.m_ui > newFrom.m_ui )
			newTo.m_ui--;
	}

	// Special case: v to-coordinate is on a (top) border
	if ( newTo.m_vtype & vaxisCoord ) {
		if ( newTo.m_vi > newFrom.m_vi )
			newTo.m_vi--;
	}

	newTo.m_idx = newTo.m_vi * (m_width-1) + newTo.m_ui;


	for ( ;; ) {
		// Is intersection with the left border and direction runs left? Jump to next u segment.
		if ( newFrom.m_vtype & uaxisCoord ) {
			if ( newFrom.m_ui > newTo.m_ui  )
				newFrom.m_ui--;
		}

		// Is intersection with the top border and direction runs up? Jump to next v segment.
		if ( newFrom.m_vtype & vaxisCoord ) {
			if ( newFrom.m_vi > newTo.m_vi )
				newFrom.m_vi--;
		}

		newFrom.m_idx = newFrom.m_vi * (m_width-1) + newFrom.m_ui;

		if ( newFrom.m_idx == newTo.m_idx )
			return;

		// Calculate region code and axis intersections
		regcode = 0;
		TParameterCell &cell = m_cells[newFrom.m_idx];

		RtFloat du = newTo.m_u - newFrom.m_u;
		RtFloat dv = newTo.m_v - newFrom.m_v;

		if ( newTo.m_ui < newFrom.m_ui ) {
			// left: du < 0
			regcode |= left;
			if ( !nextVerti ) {
				// calc intersection with left border of cell(newFrom.m_ui, newFrom.m_vi)
				vui = newFrom.m_ui;
				vvi = newFrom.m_vi;
				vu = cell.m_left->m_coord;
				vv = newTo.m_v + (vu - newTo.m_u) * dv / du;
				nextVerti = true;
			}
		} else if ( to.m_ui > newFrom.m_ui ) {
			// right: du > 0
			regcode |= right;
			if ( !nextVerti ) {
				// calc intersection with left border of cell(newFrom.m_ui+1, newFrom.m_vi)
				vui = newFrom.m_ui+1;
				vvi = newFrom.m_vi;
				vu = cell.m_right->m_coord;
				vv = newTo.m_v + (vu - newTo.m_u) * dv / du;
				nextVerti = true;
			}
		}

		if ( to.m_vi > newFrom.m_vi ) {
			// bottom: dv < 0
			regcode |= bottom;
			if ( !nextHoriz ) {
				// calc intersection with left border of cell(from.m_ui, from.m_vi+1)
				hui = newFrom.m_ui; 
				hvi = newFrom.m_vi+1;
				hv = cell.m_bottom->m_coord;
				hu = newTo.m_u + (hv - newTo.m_v) * du / dv;
				nextHoriz = true;
			}
		} else if ( to.m_vi < newFrom.m_vi ) {
			// top: dv > 0
			regcode |= top;
			if ( !nextHoriz ) {
				// calc intersection with left border of cell(from.m_ui, from.m_vi)
				hui = newFrom.m_ui;
				hvi = newFrom.m_vi;
				hv = cell.m_top->m_coord;
				hu = newTo.m_u + (hv - newTo.m_v) * du / dv;
				nextHoriz = true;
			}
		}

		if ( regcode == 0 )
			return;

		assert(nextVerti || nextHoriz);

		// Push neares intersection as newFrom
		calcVert = nextVerti;
		newFrom.m_vtype = 0;

		if ( nextVerti && nextHoriz ) {
			// calcVert == true?
			if ( vu == hu && vv == hv ) {
				// Possibly upper left corner (uaxisCoord | vaxisCoord)
				newFrom.m_vtype |= vaxisCoord;
				nextHoriz = false;
				calcVert = true;
			} else {
				calcVert = fabs(vu - newFrom.m_u) < fabs(hu - newFrom.m_u);
			}
		}

		if ( calcVert ) {
			nextVerti = false;
			if ( newTo.m_u != vu || newTo.m_v != vv ) {
				newFrom.m_u = vu;
				newFrom.m_v = vv;
				newFrom.m_ui = vui;
				newFrom.m_vi = vvi;
				newFrom.m_vtype |= uaxisCoord;
				newFrom.m_idx = newFrom.m_vi * (m_width-1) + newFrom.m_ui;
				tLoop.m_trimVertices.push_back(newFrom);
			}
		} else {
			nextHoriz = false;
			if ( newTo.m_u != hu || newTo.m_v != hv ) {
				newFrom.m_u = hu;
				newFrom.m_v = hv;
				newFrom.m_ui = hui;
				newFrom.m_vi = hvi;
				newFrom.m_vtype |= vaxisCoord;
				newFrom.m_idx = newFrom.m_vi * (m_width-1) + newFrom.m_ui;
				tLoop.m_trimVertices.push_back(newFrom);
			}
		}
	}
}


void TParameterGrid::addCurveSegment(const TNuCurveSegment &cs) {
	if ( cs.m_points.size() <= 0 || m_trimLoops.empty() )
		return;

	TTrimLoop &tLoop = m_trimLoops.back();

	bool inserted = !tLoop.m_trimVertices.empty();

	std::vector<RtFloat>::const_iterator iter = cs.m_points.begin();
	while ( iter != cs.m_points.end() ) {
		RtFloat u = *(iter++);
		if ( iter == cs.m_points.end() )
			break;
		RtFloat v = *(iter++);
		unsigned int vtype = trimVertex;

		IndexType ui = findIdx(u, m_uaxis);
		if ( u == m_uaxis[ui].m_coord ) vtype |= uaxisCoord;

		IndexType vi = findIdx(v, m_vaxis);
		if ( v == m_vaxis[vi].m_coord ) vtype |= vaxisCoord;

		IndexType idx = vi * (m_width-1) + ui;

		TTrimVertex newvert(ui, vi, idx, u, v, vtype);
		if ( inserted ) {
			const TTrimVertex &from = tLoop.m_trimVertices.back();
			if ( from.m_ui != newvert.m_ui || from.m_vi != newvert.m_vi )
				addIntersections(from, newvert);
		}

		tLoop.m_trimVertices.push_back(newvert);
		inserted = true;
	}
}


void TParameterGrid::closepath() {
	// insert end -> start
	if ( m_trimLoops.empty() )
			return;

	TTrimLoop &tLoop = m_trimLoops.back();

	if ( tLoop.m_trimVertices.empty() )
		return;

	const TTrimVertex &back = tLoop.m_trimVertices.back();
	const TTrimVertex &front = tLoop.m_trimVertices.front();

	if ( back.m_u != front.m_u || back.m_v != front.m_v ) {
		addIntersections(back, front);
		tLoop.m_trimVertices.push_back(front); // Close explicitly
	}


}

bool TParameterCell::insertTrim(
	std::list<TTrimVertex>::const_iterator &vertexIter,
	const std::list<TTrimVertex> &vertices,
	IndexType startVertexIdx)
{
	assert(vertexIter != vertices.end());

	bool searchOnly = startVertexIdx == vertices.size();

	// First vertex of the thread ( like the end vertex
	// it is in general not of the type of the other vertices between start and end )
	IndexType startIdx = (*vertexIter).m_vidx;
	const TTrimVertex &startVertex = *vertexIter;

	// If there is no trimmed cell (thread container) for this cell, create one
	if ( !searchOnly ) {
		if ( !m_trim ) {
			m_trim = new TTrimmedCell;
			// memory error
			if ( !m_trim )
				return false;
		}
	}

	// Create a new trim thread
	/* constructor
	t.m_edgeType = noEdge;
	t.m_isClosed = false;
	*/
	TTrimThread t;
	if ( !searchOnly ) {
		m_trim->m_threads.push_back(t);
	}

	// Get the new thread out of the list
	TTrimThread *tt = &t;
	if ( !searchOnly )
		tt = &m_trim->m_threads.back();

	// The start vertex is part of the thread
	tt->m_thread.push_back(&startVertex);

	// Get the next vertex
	std::list<TTrimVertex>::const_iterator prevIter = vertexIter;
	const TTrimVertex *prevVertex = tt->m_thread.back();
	++vertexIter;
	if ( vertexIter == vertices.end() ) {
		vertexIter = vertices.begin();
	}
	if ( !searchOnly && (*vertexIter).m_vidx == startVertexIdx ) {
		if ( tt->m_edgeType == onCorner ) {
			// curve is a point
			tt->m_edgeType = noEdge;
		}
		return false;
	}

	while ( (*vertexIter).m_vidx != startIdx ) {
		const TTrimVertex &vertex = *vertexIter;
		enum ETrimEdgeType curEdge = noEdge;

		// Examine the edge from prevVertex to vertex
		unsigned int vt = (vertex.m_vtype & prevVertex->m_vtype) & ~trimVertex;
		if ( vt == 0 ) {
			curEdge = inCell;
		} else if ( vt == uaxisCoord ) {
			if ( vertex.m_idx == prevVertex->m_idx || (fabs(vertex.m_u - prevVertex->m_u) < riEpsilon) ) {
				curEdge = onUAxis;
			} else {
				curEdge = inCell;
			}
		} else if ( vt == vaxisCoord ) {
			if ( vertex.m_idx == prevVertex->m_idx || (fabs(vertex.m_v - prevVertex->m_v) < riEpsilon)  ) {
				curEdge = onVAxis;
			} else {
				curEdge = inCell;
			}
		} else {
			// Corner (uaxisCoord + vaxisCoord)
			assert(vt == (uaxisCoord + vaxisCoord));
			if ( vertex.m_idx == prevVertex->m_idx ) {
				curEdge = onCorner;
			} else {
				if ( fabs(vertex.m_u - prevVertex->m_u) < riEpsilon ) {
					curEdge = onUAxis;
				} else if ( fabs(vertex.m_v - prevVertex->m_v) < riEpsilon ) {
					curEdge = onVAxis;
				} else {
					curEdge = inCell;
				}
			}
		}

		if ( tt->m_edgeType != noEdge && tt->m_edgeType != onCorner ) {
			if ( tt->m_edgeType != curEdge ) {
				vertexIter = prevIter;
				return ((*vertexIter).m_vidx != startVertexIdx);
			}
		} else {
			if ( tt->m_edgeType == onCorner && startVertex.m_idx != vertex.m_idx ) {
				tt->m_edgeType = noEdge;
				vertexIter = prevIter;
				return ((*vertexIter).m_vidx != startVertexIdx);
			}
			tt->m_edgeType = curEdge;
		}

		
		tt->m_thread.push_back(&vertex);
	
		// if the thread is not the same as the start vertex - stop
		if ( startVertex.m_idx != vertex.m_idx ) {
			vertexIter = prevIter;
			return ((*vertexIter).m_vidx != startVertexIdx);
		}

		prevIter = vertexIter;
		prevVertex = tt->m_thread.back();
		if (!searchOnly && (*vertexIter).m_vidx == startVertexIdx) {
			if ( tt->m_edgeType == onCorner ) {
				// curve is a point
				tt->m_edgeType = noEdge;
			}
			return false;
		}
		vertexIter++;
		if ( vertexIter == vertices.end() ) {
			vertexIter = vertices.begin();
		}
	}

	// The thread is closed, i.e. the whole trim loop is within part of the cell, without
	// touching the border
	tt->m_thread.push_back(&startVertex);
	if ( tt->m_edgeType == onCorner ) {
		// curve is a point
		tt->m_edgeType = noEdge;
	}
	tt->m_isClosed = true;
	return false;
}

void TParameterCell::cut() {
}

void TParameterCell::triangulate() {
	if ( m_trim ) {
	} 
}

void TParameterGrid::clear() {
	m_uaxis.clear();
	m_vaxis.clear();
	m_width = 0;
	m_height = 0;
	m_cells.clear();
	m_trimLoops.clear();
}

void TParameterGrid::split() {
	std::vector<TTrimLoop>::iterator loopIter = m_trimLoops.begin();

	IndexType loopIdx = 0;
	for ( ; loopIter != m_trimLoops.end(); loopIter++, ++loopIdx ) {
		TTrimLoop &loop = *loopIter;
		loop.buildIndices(loopIdx);
		std::list<TTrimVertex>::const_iterator vertexIter = loop.m_trimVertices.begin();
		// empty trim loop
		if ( vertexIter == loop.m_trimVertices.end() )
			continue;

		m_cells[(*vertexIter).m_idx].insertTrim(vertexIter, loop.m_trimVertices, (IndexType)loop.m_trimVertices.size());
		IndexType startVertexIdx = (*vertexIter).m_vidx;
		IndexType idx = (*vertexIter).m_idx;
		while ( m_cells[idx].insertTrim(vertexIter, loop.m_trimVertices, startVertexIdx) ) {
			// split cell id on last thread
			m_cells[idx].cut();
			idx = (*vertexIter).m_idx;
		}
	}
}


// ============================================================================
// TNuCurveSegment

IndexType TNuCurveSegment::nuBlend(
		const std::vector<RtFloat> &source,
		RtInt offs,
		RtInt seg,
		const TBSplineBasis &basis,
		bool clearAndReserve)
{
	RtInt span = seg + basis.m_order - 1;

	IndexType pn = basis.m_valCnts[span];
	IndexType koffset = basis.m_valOffs[span];

	if ( clearAndReserve ) {
		m_points.clear();
		if ( pn > 0 )
			m_points.resize(pn*2);
	}

	if ( pn <= 0 )
		return pn;

	IndexType i, cnt;
	RtFloat u, v, w, U, V, W, baseElem;
	for ( cnt = 0; cnt < pn; ++cnt ) {
		U = V = W = 0;
		for ( i = 0; i < (IndexType)basis.m_order; ++i ) {
			baseElem = basis.m_basis[(IndexType)((koffset+cnt)*basis.m_order+i)];
			u = source[offs+(seg+i)*3  ];
			v = source[offs+(seg+i)*3+1];
			w = source[offs+(seg+i)*3+2];
			U += u * baseElem;
			V += v * baseElem;
			W += w * baseElem;
		}
		if ( W > 0 ) {
			U /= W;
			V /= W;
		}
		m_points[2*cnt  ] = U;
		m_points[2*cnt+1] = V;
	}
	return pn;
}


// ============================================================================
// TTrimCurve

bool TTrimCurve::calcBasis(IndexType tess) {
	if ( m_trimCurve.m_total <= 0 ) {
		m_segments = 0;
		return false;
	}

	bool isNew = false;
	if ( m_basis.size() != m_trimCurve.m_total ) {
		m_basis.resize(m_trimCurve.m_total);
		isNew = true;
	}

	if ( isNew || m_basis[0].m_tess != tess ) {
		
		RtInt i,         // loops
		      j,         // curves 0..n
		      cnt,       // curve counter
		      knotoffs;  // offset into knot vector for current curve 0..m_trimCurve.m_total

		isNew = true;
		m_segments = 0;

		for ( i = 0, cnt=0, knotoffs=0; i < m_trimCurve.m_nLoops; ++i ) {
			for ( j = 0; j < m_trimCurve.m_nCurves[i]; ++j, ++cnt ) {
				assert(cnt < m_trimCurve.m_total);
				m_basis[cnt].reset(
					m_trimCurve.m_n[cnt],
					m_trimCurve.m_order[cnt],
					m_trimCurve.m_knots,
					knotoffs,
					m_trimCurve.m_min[cnt],
					m_trimCurve.m_max[cnt],
					tess);
				m_segments += (IndexType)m_basis[cnt].m_segments;
				knotoffs += m_trimCurve.m_n[cnt] + m_trimCurve.m_order[cnt];
			}
		}
		assert(cnt == m_trimCurve.m_total);
	}

	return isNew;
}


bool TTrimCurve::isTrim(const TTrimCurveData &trimCurve, IndexType tess) {
	if ( tess != m_tess )
		return false;
	return m_trimCurve.compareWith(trimCurve);
}


void TTrimCurve::insertIntoGrid(TParameterGrid &grid) const {
	// Insert the points into the trimloops
	// loop -> curves -> segments
	IndexType segCnt = 0;
	grid.initpath(m_trimCurve.m_nLoops);
	for ( RtInt loop = 0, cnt = 0; loop < m_trimCurve.m_nLoops; ++loop ) {
		grid.newpath();
		for ( RtInt curve = 0; curve < m_trimCurve.m_nCurves[loop]; ++curve, ++cnt ) {
			for ( IndexType seg = 0; seg < m_curveSegSize[cnt]; ++seg, ++segCnt ) {
				grid.addCurveSegment(m_curveSegs[segCnt]);
			}
		}
		grid.closepath();
	}
	grid.split();
}


IndexType TTrimCurve::tesselate(IndexType tess) {
	if ( tess < 1 ) {
		tess = 1;
	}
	bool change = calcBasis(tess);

	bool isNew = false;
	if ( m_segments > 0 ) {
		IndexType gseg = 0;	// Number of segments that are not empty	
		for ( std::vector<TBSplineBasis>::iterator bi = m_basis.begin(); bi != m_basis.end(); bi++) {
			for ( RtInt seg = 0; seg < bi->m_segments; ++seg ) {
				RtInt span = seg + bi->m_order - 1;
				IndexType pn = bi->m_valCnts[span]; // Number of points in segment
				if ( pn > 0 )
					++gseg;
			}
		}
		findCurve(m_curveSegs, tess, m_lastIndex, isNew, gseg);
	}

	if ( isNew ) {
		// Tesselate the curve
		IndexType gseg = 0;
		IndexType pn;
		RtInt offs = 0;
		RtInt cnt;
		std::vector<TBSplineBasis>::iterator bi = m_basis.begin();

		m_curveSegOffs.clear();
		m_curveSegOffs.resize(m_basis.size());
		m_curveSegSize.resize(m_basis.size());
		m_curveSegPoints.resize(m_basis.size());
		assert (m_basis.size() == m_trimCurve.m_total);

		for ( cnt = 0; bi != m_basis.end(); bi++, ++cnt ) {
			m_curveSegOffs[cnt] = gseg;
			m_curveSegPoints[cnt]= 0;
			for ( RtInt seg = 0; seg < bi->m_segments; ++seg ) {
				pn = m_curveSegs[m_lastIndex+gseg].nuBlend(m_trimCurve.m_points, offs, seg, *bi, true);
				if ( pn > 0 ) {
					++gseg;
					m_curveSegPoints[cnt] += pn;
				}
			}
			offs += m_trimCurve.m_n[cnt]*3;
			m_curveSegSize[cnt] = gseg - m_curveSegOffs[cnt];
		}

		// Copy the curve segments to the loops
		// loop -> curves -> segments -> points
		m_trimLoops.clear();
		m_trimLoops.resize(m_trimCurve.m_nLoops);

		// Count the maximal number of points for each loop
		RtInt loop;
		for ( loop = 0, cnt = 0; loop < m_trimCurve.m_nLoops; ++loop ) {
			IndexType npoints = 0;
			for ( RtInt curve = 0; curve < m_trimCurve.m_nCurves[loop]; ++curve, ++cnt ) {
				npoints += m_curveSegPoints[cnt];
			}
			++npoints; // one more to close the curve if it is not already done
			m_trimLoops[loop].m_points.clear();
			m_trimLoops[loop].m_points.resize(npoints*2);
		}

		// Insert the points into the trimloops
		IndexType segCnt = 0;
		for ( loop = 0, cnt = 0; loop < m_trimCurve.m_nLoops; ++loop ) {
			IndexType pntIdx = 0;
			for ( RtInt curve = 0; curve < m_trimCurve.m_nCurves[loop]; ++curve, ++cnt ) {
				for ( IndexType seg = 0; seg < m_curveSegSize[cnt]; ++seg, ++segCnt ) {
					for ( IndexType segpt = 0; segpt < m_curveSegs[segCnt].m_points.size()-1; segpt+=2 ) {
						if ( pntIdx > 1 ) {
							if (
								m_trimLoops[loop].m_points[pntIdx-2] == m_curveSegs[segCnt].m_points[segpt] &&
								m_trimLoops[loop].m_points[pntIdx-1] == m_curveSegs[segCnt].m_points[segpt+1]
							)
								continue;
						}
						m_trimLoops[loop].m_points[pntIdx++] = m_curveSegs[segCnt].m_points[segpt];
						m_trimLoops[loop].m_points[pntIdx++] = m_curveSegs[segCnt].m_points[segpt+1];
					}
				}
			}
			if ( pntIdx > 1 ) {
				if (
					m_trimLoops[loop].m_points[0] != m_trimLoops[loop].m_points[pntIdx-2] &&
					m_trimLoops[loop].m_points[1] != m_trimLoops[loop].m_points[pntIdx-1]
				) {
					m_trimLoops[loop].m_points[pntIdx++] = m_trimLoops[loop].m_points[0];
					m_trimLoops[loop].m_points[pntIdx++] = m_trimLoops[loop].m_points[1];
				}
			}
			m_trimLoops[loop].m_points.resize(pntIdx);
		}
	}
	m_tess = tess;

#ifdef _DEBUG
	{
		TLogfile pslog("c:\\temp\\trimcurve.ps", "TTrimCurve", false, "% ");
		pslog << "%!PS-Adobe-2.0" << TLogfile::endl() << "%%Title: trimcurve.ps" << TLogfile::endl() << "%%EndComments";

		pslog.ReportLogText("TrimCurve:tesselate()");

		pslog.ReportLogText("Init");
		pslog << TLogfile::endl() << "200 400 translate";
		pslog << TLogfile::endl() << "0.01 setlinewidth";
		pslog << TLogfile::endl() << "150 150 scale";

		pslog << TLogfile::endl() << "gsave";
		pslog.ReportLogText("Variables");
		pslog << " m_lastIndex = " << m_lastIndex << " m_segments = " << m_segments;

		pslog.ReportLogText("Coordinate Rectangle");
		pslog << TLogfile::endl() << "newpath";
		pslog << TLogfile::endl() << "0.9 setgray";
		pslog << TLogfile::endl() << "0 0 moveto";
		pslog << TLogfile::endl() << "1 0 lineto";
		pslog << TLogfile::endl() << "1 1 lineto";
		pslog << TLogfile::endl() << "0 1 lineto";
		pslog << TLogfile::endl() << "closepath stroke";

		pslog.ReportLogText("Segments");
		std::deque<TNuCurveSegment>::const_iterator ci = m_curveSegs.begin();
		
		for ( IndexType segcnt = 0; ci != m_curveSegs.end(); ci++, segcnt++ ) {
			int size = (int)(ci->m_points.size());
			if ( size > 1 ) {
				int cnt;
				pslog.ReportLogText("Segment, size = ");
				pslog << size << " segcnt = " << segcnt;
				pslog << TLogfile::endl() << "0.75 setgray";
				pslog << TLogfile::endl() << "newpath";
				for ( cnt = 0; cnt < size-1; cnt+=2 ) {
					pslog << TLogfile::endl() << ci->m_points[cnt] << " " << ci->m_points[cnt+1];
					if ( cnt == 0 ) {
						pslog << " moveto";
						pslog << " 0 0.02 rmoveto";
						pslog << " 0 -0.04 rlineto stroke";
						pslog << TLogfile::endl() << "newpath";
						pslog << TLogfile::endl() << ci->m_points[cnt] << " " << ci->m_points[cnt+1];
						pslog << " moveto";
						pslog << " 0.02 0 rmoveto";
						pslog << " -0.04 0 rlineto stroke";
						pslog << TLogfile::endl() << "0.5 setgray";
						pslog << TLogfile::endl() << "newpath";
						pslog << TLogfile::endl() << ci->m_points[cnt] << " " << ci->m_points[cnt+1];
						pslog << " moveto";
					} else {
						pslog << " lineto";
						if ( cnt == 4 ) {
							pslog << TLogfile::endl() << "stroke newpath 0 setgray";
							pslog << TLogfile::endl() << ci->m_points[cnt] << " " << ci->m_points[cnt+1];
							pslog << " moveto";
						}
					}
				}
				pslog << TLogfile::endl() << "stroke";
			}
		}

		pslog.ReportLogText("TRIMLOOP");
		pslog << TLogfile::endl() << "0 1.1 translate";
		pslog << TLogfile::endl() << "newpath";
		pslog << TLogfile::endl() << "0.9 setgray";
		pslog << TLogfile::endl() << "0 0 moveto";
		pslog << TLogfile::endl() << "1 0 lineto";
		pslog << TLogfile::endl() << "1 1 lineto";
		pslog << TLogfile::endl() << "0 1 lineto";
		pslog << TLogfile::endl() << "closepath stroke";

		std::vector<TSimpleTrimLoop>::iterator ti = m_trimLoops.begin();
		pslog << TLogfile::endl() << "newpath";
		for ( ; ti != m_trimLoops.end(); ti++ ) {
			int size = (int)(ti->m_points.size());
			for ( int cnt = 0; cnt < size-2; cnt+=2 ) {
				pslog << TLogfile::endl() << ti->m_points[cnt] << " " << ti->m_points[cnt+1];
				if ( cnt == 0 ) {
					pslog << " moveto";
				} else {
					pslog << " lineto";
				}
			}
			pslog << TLogfile::endl() << "closepath";
		}
		pslog << TLogfile::endl() << "gsave 0.9 setgray fill grestore 0 setgray stroke";
		pslog.ReportLogText("END");
		pslog << TLogfile::endl() << "grestore";
	}
#endif // _DEBUG

	return m_lastIndex;
}

// ============================================================================
// TNuPatch

bool TNuPatch::calcBasis(IndexType tessU, IndexType tessV) {
	bool change = false;
	if ( m_uBasis.m_tess != tessU ) {
		m_uBasis.reset(m_nu, m_uorder, m_uknot, m_umin, m_umax, tessU);
		change = true;
	}
	if ( m_vBasis.m_tess != tessV ) {
		m_vBasis.reset(m_nv, m_vorder, m_vknot, m_vmin, m_vmax, tessV);
		change = true;
	}
	return change;
}


IndexType TNuPatch::tesselate(IndexType tessU, IndexType tessV, bool newTrim) {
	if ( tessU < 1 ) {
		tessU = 1;
	}
	if ( tessV < 1 ) {
		tessV = 1;
	}
	
	RtInt useg, vseg;
	IndexType idx;

	bool isNew = false;
	findUV(m_nuMeshes, tessU, tessV, m_lastIndex, isNew, segments());
	isNew |= newTrim;

	if ( isNew ) {
		bool change = calcBasis(tessU, tessV);
		IndexType numFaces = m_uBasis.m_segments*m_vBasis.m_segments;
		assert(segments()==numFaces);

		if ( m_tessTrim ) {
			m_paramGrid.reset(
				m_uknot, m_umin, m_umax, m_nu, m_uorder, tessU,
				m_vknot, m_vmin, m_vmax, m_nv, m_vorder, tessV
				);
			m_tessTrim->insertIntoGrid(m_paramGrid);
		} else {
			m_paramGrid.clear();
		}

		idx = m_lastIndex;
		for ( vseg = 0; vseg < m_vBasis.m_segments; ++vseg ) {
			for ( useg = 0; useg < m_uBasis.m_segments; ++useg, ++idx ) {
				TNuMesh &mesh = m_nuMeshes[idx];
				mesh.geometry(this);
				mesh.faceIdx(idx-m_lastIndex);
				mesh.faceIdxU(useg);
				mesh.faceIdxV(vseg);
				mesh.numFaces(numFaces);
				mesh.numFacesU(m_uBasis.m_segments);
				mesh.numFacesV(m_vBasis.m_segments);
				mesh.insertVertexDataNU(m_parameters, useg, vseg, m_uBasis, m_vBasis, m_paramGrid);
			}
		}
		idx = m_lastIndex;

#ifdef _DEBUG
		{
			TLogfile pslog("c:\\temp\\trimcurve.ps", "TTrimCurve::tesselate", true, "% ");
			IndexType cnt;

			pslog << TLogfile::endl() << "gsave"; 
			pslog << TLogfile::endl() << "0 1.1 translate";

			pslog << TLogfile::endl() << "0.005 setlinewidth"; 
			pslog << TLogfile::endl() << "0.5 setgray"; 
			for ( cnt = 0; cnt < m_paramGrid.m_cells.size(); ++cnt ) {
				if ( m_paramGrid.m_cells[cnt].isFragmented() && m_paramGrid.m_cells[cnt].isValid() ) {
					pslog << TLogfile::endl() << "newpath"; 
					pslog << TLogfile::endl() << m_paramGrid.m_cells[cnt].m_left->m_coord << " " << m_paramGrid.m_cells[cnt].m_top->m_coord << " moveto";
					pslog << TLogfile::endl() << m_paramGrid.m_cells[cnt].m_right->m_coord << " " << m_paramGrid.m_cells[cnt].m_top->m_coord << " lineto";
					pslog << TLogfile::endl() << m_paramGrid.m_cells[cnt].m_right->m_coord << " " << m_paramGrid.m_cells[cnt].m_bottom->m_coord << " lineto";
					pslog << TLogfile::endl() << m_paramGrid.m_cells[cnt].m_left->m_coord << " " << m_paramGrid.m_cells[cnt].m_bottom->m_coord << " lineto";
					pslog << TLogfile::endl() << "closepath fill"; 
				}
			}

			pslog << TLogfile::endl() << "0.005 setlinewidth"; 
			pslog.ReportLogText("Grid U");
			for ( cnt = 0; cnt < m_paramGrid.m_uaxis.size(); ++cnt ) {
				if ( m_paramGrid.m_uaxis[cnt].m_coord > -riInfinity && m_paramGrid.m_uaxis[cnt].m_coord < riInfinity ) {
					pslog << TLogfile::endl() << "newpath"; 
					pslog << TLogfile::endl() << m_paramGrid.m_uaxis[cnt].m_coord << " " << this->m_vmin << " moveto";
					pslog << TLogfile::endl() << m_paramGrid.m_uaxis[cnt].m_coord << " " << this->m_vmax << " lineto";
					if ( m_paramGrid.m_uaxis[cnt].m_isBorder ) {
						pslog << TLogfile::endl() << "0.5 setgray"; 
					} else {
						pslog << TLogfile::endl() << "0.8 setgray"; 
					}
					pslog << TLogfile::endl() << "stroke"; 
				}
			}
			pslog.ReportLogText("Grid V");

			for ( cnt = 0; cnt < m_paramGrid.m_vaxis.size(); ++cnt ) {
				if ( m_paramGrid.m_vaxis[cnt].m_coord > -riInfinity && m_paramGrid.m_vaxis[cnt].m_coord < riInfinity ) {
					pslog << TLogfile::endl() << "newpath"; 
					pslog << TLogfile::endl() << this->m_umin << " " << m_paramGrid.m_vaxis[cnt].m_coord << " moveto";
					pslog << TLogfile::endl() << this->m_umax << " " << m_paramGrid.m_vaxis[cnt].m_coord << " lineto";
					if ( m_paramGrid.m_vaxis[cnt].m_isBorder ) {
						pslog << TLogfile::endl() << "0.5 setgray"; 
					} else {
						pslog << TLogfile::endl() << "0.8 setgray"; 
					}
					pslog << TLogfile::endl() << "stroke"; 
				}
			}

			pslog.ReportLogText("Intersections");
			pslog << TLogfile::endl() << "0.95 setgray";
			pslog << TLogfile::endl() << "0.001 setlinewidth";
			std::vector<TTrimLoop>::const_iterator i;
			for ( i = m_paramGrid.m_trimLoops.begin();
				  i != m_paramGrid.m_trimLoops.end();
				  i++ )
			{
				for ( std::list<TTrimVertex>::const_iterator li = i->m_trimVertices.begin();
					  li != i->m_trimVertices.end();
					  li++ )
				{
//					if ( 1 )
//					if ( li->m_vtype & trimVertex )
//					if ( li->m_vtype & uaxisCoord )
//					if ( li->m_vtype & vaxisCoord )
//					if ( !(li->m_vtype & trimVertex) && (li->m_vtype & vaxisCoord || li->m_vtype & uaxisCoord) )
					if ( li->m_vtype & vaxisCoord || li->m_vtype & uaxisCoord )
					{
						pslog << TLogfile::endl() << "newpath";
						pslog << TLogfile::endl() << li->m_u << " " << li->m_v;
						pslog << " moveto";
						pslog << " -0.005  0.005 rmoveto";
						pslog << "  0.01 -0.01 rlineto stroke";
						pslog << TLogfile::endl() << "newpath";
						pslog << TLogfile::endl() << li->m_u << " " << li->m_v;
						pslog << " moveto";
						pslog << "  0.005  0.005 rmoveto";
						pslog << " -0.01 -0.01 rlineto stroke";
					}
				}
			}

			pslog.ReportLogText("Stroke Intersections");
			pslog << TLogfile::endl() << "0.95 setgray";
			pslog << TLogfile::endl() << "0.001 setlinewidth";
			for ( i = m_paramGrid.m_trimLoops.begin();
				  i != m_paramGrid.m_trimLoops.end();
				  i++ )
			{
				bool moved = false;
				pslog << TLogfile::endl() << "newpath";
				for ( std::list<TTrimVertex>::const_iterator li = i->m_trimVertices.begin();
					  li != i->m_trimVertices.end();
					  li++ )
				{
//					if ( 1 )
					if ( (li->m_vtype & trimVertex) )
//					if ( li->m_vtype & uaxisCoord )
//					if ( li->m_vtype & vaxisCoord )
					{
						pslog << TLogfile::endl() << li->m_u << " " << li->m_v;
						if ( !moved ) pslog << " moveto";
						else pslog << " lineto";
						moved = true;
					}
				}
				pslog << TLogfile::endl() << "stroke";
			}

			pslog << TLogfile::endl() << "grestore"; 
			pslog << TLogfile::endl() << "showpage";
			pslog << TLogfile::endl();
			pslog.Close();
		}
#endif // _DEBUG
	}
	return m_lastIndex;
}

void TNuPatch::replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
	if ( dp ) {
		bool newTrim = false;
		if ( (1+m_nu-m_uorder)>0 && (1+m_nv-m_vorder)>0 ) {
			TTrimCurveData *t = ricb.curTrimCurve();
			if ( t && t->m_nLoops > 0 ) {
				/// compare
				if ( m_tessTrim ) {
					if ( !m_tessTrim->isTrim(*t, (IndexType)ricb.curTessTrim()) ) {
						delete m_tessTrim;
						m_tessTrim = NULL;
					}
				}
				if ( !m_tessTrim ) {
					m_tessTrim = new TTrimCurve(ricb.lineNo(), *t);
					if ( m_tessTrim )
						m_tessTrim->tesselate((IndexType)ricb.curTessTrim());
					newTrim = true;
				}
			} else {
				if ( m_tessTrim ) {
					delete m_tessTrim;
					m_tessTrim = NULL;
					newTrim = true;
				}
			}

			IndexType tessU = (IndexType)ricb.curUTesselation();
			IndexType tessV = (IndexType)ricb.curVTesselation();
			IndexType meshIdx = tesselate(tessU, tessV, newTrim);

			for ( RtInt v = 0; v < m_vBasis.m_segments; ++v )
				for ( RtInt u = 0; u < m_uBasis.m_segments; ++u, ++meshIdx )
					if ( m_nuMeshes[meshIdx].tessU() > 0 && m_nuMeshes[meshIdx].tessV() > 0 )
						dp->drawGeometricPrim(m_nuMeshes[meshIdx]);
		}
	} else  {
		TRiNuPatch::replay(ri);
	}
}
