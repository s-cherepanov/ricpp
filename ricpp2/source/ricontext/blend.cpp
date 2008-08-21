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

/** @file blend.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Bilinear and bicubic blending (later via basis vectors for bicubic spline patches of given parametric intervals).
 */

#include "ricpp/ricontext/blend.h"

#ifndef _RICPP_TOOLS_TEMPLATEFUNCS_H
#include "ricpp/tools/templatefuncs.h"
#endif // _RICPP_TOOLS_TEMPLATEFUNCS_H

using namespace RiCPP;

// =============================================================================

CBilinearBlend::CBilinearBlend() : m_tessU(1), m_tessV(1)
{
}

CBilinearBlend::CBilinearBlend(IndexType aTessU, IndexType aTessV) : m_tessU(aTessU), m_tessV(aTessV)
{
}

void CBilinearBlend::bilinearBlend(IndexType elemSize, const IndexType (&cornerIdx)[4], const std::vector<RtFloat> &vals, std::vector<RtFloat> &results)
{
	assert(tessU() > 0);
	assert(tessV() > 0);
	assert(elemSize > 0);
	
	IndexType size = (tessU()+1) * (tessV()+1) * elemSize;
	if ( results.size() != size ) {
		results.clear();
		results.resize(size);
	}
	
	RtFloat deltau = deltaNotZero<RtFloat>(0, 1, static_cast<RtFloat>(tessU()));
	RtFloat deltav = deltaNotZero<RtFloat>(0, 1, static_cast<RtFloat>(tessV()));
	
	RtFloat u, v;
	IndexType ui, vi, ei, idx;
	IndexType startPos, endPos;
	
	for ( v = 0, vi = 0; vi < tessV()+1; ++vi, v += deltav ) {
		if ( v > 1.0 || vi == tessV() ) {
			v = 1.0;
		}
		startPos = vi * (tessU() + 1) * elemSize;
		endPos   = startPos + tessU() * elemSize;
		assert(startPos != endPos);
		for ( ei = 0; ei < elemSize; ++ei ) {
			results[startPos+ei] = lerp(v, vals[cornerIdx[0]*elemSize+ei], vals[cornerIdx[2]*elemSize+ei]);
		}
		for ( ei = 0; ei < elemSize; ++ei ) {
			results[endPos+ei]   = lerp(v, vals[cornerIdx[1]*elemSize+ei], vals[cornerIdx[3]*elemSize+ei]);
		}
		idx = startPos+elemSize;
		for ( u = deltau, ui = 1; ui < tessU(); ++ui, u += deltau ) {
			if ( u > 1.0 ) {
				u = 1.0;
			}
			for ( ei = 0; ei < elemSize; ++ei, ++idx ) {
				results[idx] = lerp(u, results[startPos+ei], results[endPos+ei]);
			}
		}
	}
}

// =============================================================================

CBicubicVectors::CBicubicVectors() : m_tessU(1), m_tessV(1)
{
	memset(&m_uBasis[0], 0, sizeof(m_uBasis));
	memset(&m_vBasis[0], 0, sizeof(m_vBasis));
}

bool CBicubicVectors::hasBasis(const RtBasis aUBasis, const RtBasis aVBasis) const
{
	if ( memcmp(m_uBasis, aUBasis, sizeof(RtBasis)) != 0 )
		return false;
	if ( memcmp(m_vBasis, aVBasis, sizeof(RtBasis)) != 0 )
		return false;
	return true;
}

void CBicubicVectors::reset(IndexType aTessU, IndexType aTessV, const RtBasis aUBasis, const RtBasis aVBasis)
{
	m_tessU = aTessU;
	m_tessV = aTessV;
	memcpy(m_uBasis, aUBasis, sizeof(RtBasis));
	memcpy(m_vBasis, aVBasis, sizeof(RtBasis));
	
	m_uVector.clear();
	m_vVector.clear();
	m_duVector.clear();
	m_dvVector.clear();
	
	m_uVector.resize((m_tessU+1)*4);
	m_vVector.resize((m_tessV+1)*4);
	m_duVector.resize((m_tessU+1)*4);
	m_dvVector.resize((m_tessV+1)*4);
	
	RtFloat t, t2, t3, s2, s3;
	IndexType i, k, u, v;
	
	RtFloat deltau = (RtFloat)(1.0 / (RtFloat)m_tessU);
	RtFloat deltav = (RtFloat)(1.0 / (RtFloat)m_tessV);
	
	for ( t = 0.0, k = 0, u = 0; u < m_tessU+1; ++u, t += deltau ) {
		if ( u == m_tessU || t > 1.0 ) {
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
	
	for ( t = 0.0, k = 0, v = 0; v < m_tessV+1; ++v, t += deltav ) {
		if ( v == m_tessV || t > 1.0 ) {
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


void CBicubicVectors::bicubicBlend(IndexType elemSize,
								   const IndexType (&controlIdx)[16],
								   const std::vector<RtFloat> &vals,
								   std::vector<RtFloat> &results) const
{
	assert(tessU() > 0);
	assert(tessV() > 0);
	assert(elemSize > 0);
	assert(uVector().size() == duVector().size());
	assert(vVector().size() == dvVector().size());
	assert(uVector().size() == (size_t)(tessU()+1)*4);
	assert(vVector().size() == (size_t)(tessV()+1)*4);
	
	IndexType tessSize = (tessU()+1)*(tessV()+1)*elemSize;
	
	if ( results.size() != tessSize ) {
		results.clear();
		results.resize(tessSize);
	}
	
	std::vector<RtFloat> temp(4*elemSize);
	IndexType id, v, u, i, j, k;
	
	id = 0;
	for ( v = 0; v < (IndexType)tessV()+1; ++v ) {
		k = 0;
		for ( i = 0; i < 4; ++i ) {
			for ( j = 0; j < elemSize; ++j ) {
				temp[k++] =
				vVector()[v*4]   * vals[controlIdx[ i    ] * elemSize + j] +
				vVector()[v*4+1] * vals[controlIdx[(i+ 4)] * elemSize + j] +
				vVector()[v*4+2] * vals[controlIdx[(i+ 8)] * elemSize + j] +
				vVector()[v*4+3] * vals[controlIdx[(i+12)] * elemSize + j];
			}
		}
		for ( u = 0; u < (IndexType)tessU()+1; ++u ) {
			for ( j = 0; j < elemSize; ++j ) {
				results[id++] =
				uVector()[u*4]   * temp[           j] +
				uVector()[u*4+1] * temp[  elemSize+j] +
				uVector()[u*4+2] * temp[2*elemSize+j] +
				uVector()[u*4+3] * temp[3*elemSize+j];
			}
		}
	}
}

void CBicubicVectors::bicubicBlendWithNormals(IndexType elemSize,
											  const IndexType (&controlIdx)[16],
											  const std::vector<RtFloat> &vals,
											  bool flipNormal,
											  std::vector<RtFloat> &results,
											  std::vector<RtFloat> &normals) const
{
	assert(elemSize == 3 || elemSize == 4);
	assert(tessU() > 0);
	assert(tessV() > 0);
	assert(uVector().size() == duVector().size());
	assert(vVector().size() == dvVector().size());
	assert(uVector().size() == (size_t)(tessU()+1)*4);
	assert(vVector().size() == (size_t)(tessV()+1)*4);
	
	const IndexType normElemSize = 3;
	IndexType sumPos = (tessU()+1)*(tessV()+1);
	IndexType normTessSize = sumPos*normElemSize;
	IndexType tessSize = sumPos*elemSize;
	
	if ( normals.size() != normTessSize ) {
		normals.clear();
		normals.resize(normTessSize);
	}
	
	if ( results.size() != tessSize ) {
		results.clear();
		results.resize(tessSize);
	}
	
	std::vector<RtFloat> temp(4*elemSize);
	
	std::vector<RtFloat> pdu(tessSize);
	std::vector<RtFloat> pdv(tessSize);
	
	IndexType id, i, j, k, u, v;
	
	id = 0;
	for ( v = 0; v < (IndexType)tessV()+1; ++v ) {
		k = 0;
		for ( i = 0; i < 4; ++i ) {
			for ( j = 0; j < elemSize; ++j ) {
				temp[k++] =
				vVector()[v*4]   * vals[controlIdx[ i    ]*elemSize + j] +
				vVector()[v*4+1] * vals[controlIdx[(i+ 4)]*elemSize + j] +
				vVector()[v*4+2] * vals[controlIdx[(i+ 8)]*elemSize + j] +
				vVector()[v*4+3] * vals[controlIdx[(i+12)]*elemSize + j];
			}
		}
		for ( u = 0; u < (IndexType)tessU()+1; ++u ) {
			for ( j = 0; j < elemSize; ++j ) {
				pdu[id] =
				duVector()[u*4]   * temp[           j] +
				duVector()[u*4+1] * temp[  elemSize+j] +
				duVector()[u*4+2] * temp[2*elemSize+j] +
				duVector()[u*4+3] * temp[3*elemSize+j];
				
				results[id] =
				uVector()[u*4]   * temp[           j] +
				uVector()[u*4+1] * temp[  elemSize+j] +
				uVector()[u*4+2] * temp[2*elemSize+j] +
				uVector()[u*4+3] * temp[3*elemSize+j];
				++id;
			}
		}
	}
	
	IndexType idSav;
	id = 0;
	for ( u = 0; u < (IndexType)tessU()+1; ++u ) {
		k = 0;
		for ( i = 0; i < 4; ++i ) {
			for ( j = 0; j < elemSize; ++j ) {
				temp[k++] =
				uVector()[u*4]   * vals[controlIdx[(i*4)  ]*elemSize+j] +
				uVector()[u*4+1] * vals[controlIdx[(i*4+1)]*elemSize+j] +
				uVector()[u*4+2] * vals[controlIdx[(i*4+2)]*elemSize+j] +
				uVector()[u*4+3] * vals[controlIdx[(i*4+3)]*elemSize+j];
			}
		}
		
		idSav = id;
		for ( v = 0; v < (IndexType)tessV()+1; ++v ) {
			for ( j = 0; j < elemSize; ++j ) {
				pdv[id+j] =
				dvVector()[v*4]   * temp[           j] +
				dvVector()[v*4+1] * temp[  elemSize+j] +
				dvVector()[v*4+2] * temp[2*elemSize+j] +
				dvVector()[v*4+3] * temp[3*elemSize+j];
			}
			id += (tessU()+1) * elemSize;
		}
		id = idSav + elemSize;
	}
	
	id = 0;
	if ( elemSize == 4 ) {
		RtFloat w2;
		for ( i = 0; i < sumPos; ++i ) {
			w2 = results[id+3] * results[id+3];
			for ( j = 0; j < 3; ++j ) {
				pdu[id+j] = (results[id+3]*pdu[id+j] - results[id+j]*pdu[id+3])/w2;
				pdv[id+j] = (results[id+3]*pdv[id+j] - results[id+j]*pdv[id+3])/w2;
			}
			id += elemSize;
		}
	}
	
	id = 0;
	IndexType idnrm = 0;
	for ( i = 0; i < sumPos; ++i ) {
		if ( flipNormal )
			planeRH(&normals[idnrm], &pdv[id], &pdu[id]);
		else
			planeLH(&normals[idnrm], &pdv[id], &pdu[id]);
		idnrm += normElemSize;
		id += elemSize;
	}
}

// =============================================================================

void CBSplineBasis::bsplineBasisDerivate(RtFloat t, RtInt span,
										 std::vector<RtFloat> &N,
										 std::vector<RtFloat> &Nd)
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
				if ( temp >= eps<RtFloat>() ) {
					temp = N[i] / temp;
				}
				Nd[i] = temp;
				temp = m_knots[i+j+1] - m_knots[i+1];
				if ( temp >= eps<RtFloat>() ) {
					temp = N[i+1] / temp;
					Nd[i] -= temp;
				}
			} else {
				temp = m_knots[i+j] - m_knots[i];
				if ( temp >= eps<RtFloat>() ) {
					temp = (N[i] + (t-m_knots[i])*Nd[i]) / temp;
				}
				Nd[i] = temp;
				temp = m_knots[i+j+1] - m_knots[i+1];
				if ( temp >= eps<RtFloat>() ) {
					temp = (N[i+1] - (m_knots[i+j+1]-t)*Nd[i+1]) / temp;
					Nd[i] -= temp;
				}
			}
			
			// Calc the basis
			if ( N[i] >= eps<RtFloat>() ) {
				temp = m_knots[i+j]-m_knots[i];
				if ( temp >= eps<RtFloat>() ) {
					temp = N[i] * ((t - m_knots[i]) / temp);
				}
				N[i] = temp;
			}
			
			if ( N[i+1] >= eps<RtFloat>() ) {
				temp = m_knots[i+j+1]-m_knots[i+1];
				if ( temp >= eps<RtFloat>() ) {
					temp = N[i+1] * ((m_knots[i+j+1] - t) / temp);
					N[i] += temp;
				}
			}
		}
	}
}


void CBSplineBasis::sortKnots()
{
	// std::sort(m_knots.begin(), m_knots.end());
}

bool CBSplineBasis::empty() const
{
	return knotSize() == 0;
}

void CBSplineBasis::validate()
{
	m_ncpts = clamp(m_ncpts, 0, m_ncpts);
	m_order = clamp(m_order, 0, m_order);
	if ( empty() )
		return;

	m_tess = clamp(m_tess,1, m_tess);
	m_order = clamp<RtInt>(m_ncpts, 0, m_ncpts);
	
	sortKnots();
	
	if ( m_knots.size() < knotSize() ) {
		RtFloat lastVal = m_knots.size() > 0 ? m_knots[m_knots.size()-1] : (RtFloat)0;
		IndexType sz = m_knots.size();
		m_knots.resize(knotSize());
		for ( IndexType i = sz; i < knotSize(); ++i )
			m_knots[i] = lastVal;
	}
	
	if ( m_knots.size() > knotSize() )
		m_knots.resize(knotSize());
	
	validateMinMax(m_tmin, m_tmax);
	m_tmin = clamp(m_tmin, m_knots[m_order-1], m_knots[m_ncpts]);
	m_tmax = clamp(m_tmax, m_knots[m_order-1], m_knots[m_ncpts]);

}


void CBSplineBasis::calc()
{
	// Test parameter conditions
	m_segments = 0;
	if ( empty() )
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
		if ( (te-ts) < eps<RtFloat>() ) continue;
		
		dt = (te-ts)/m_tess;
		
		if ( dt < eps<RtFloat>() )
			dt = eps<RtFloat>();
		
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
		for ( t = m_knots[knotIdx], cnt = 0;
			  t < te && cnt < m_tess;
			  t += dt, ++cnt )
		{
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
		for ( j = m_valCnts[span];
			  j > 0 && iter != m_tVals.end();
			  iter++, --j )
		{
			bsplineBasisDerivate(*iter, span, Ntemp, Ndtemp);
			for ( i = span-m_order+1; i <= span; ++i, ++GIdx ) {
				// Cache the results (basis and derivative) for parameter t
				m_basis[GIdx] = Ntemp[i];
				m_basisDeriv[GIdx] = Ndtemp[i];
			}
		}
	}
}


CBSplineBasis &CBSplineBasis::operator=(const CBSplineBasis &sp)
{
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


void CBSplineBasis::clearResults()
{
	m_segments = 0;
	m_tVals.clear();
	m_valCnts.clear();
	m_valOffs.clear();
	m_basis.clear();
	m_basisDeriv.clear();
}

void CBSplineBasis::insertVals(RtInt theNCpts, RtInt theOrder,
							   RtFloat theTMin, RtFloat theTMax, RtInt theTess)
{
	m_ncpts = theNCpts;
	m_order = theOrder;
	m_tmin  = theTMin;
	m_tmax  = theTMax;
	m_tess = theTess;
	
}

void CBSplineBasis::insertKnots(const std::vector<RtFloat> &theKnots)
{
	m_knots = theKnots;
}

void CBSplineBasis::insertKnots(const std::vector<RtFloat> &theKnots, IndexType theKnotOffs)
{
	assert(theKnotOffs < theKnots.size());
	if ( theKnotOffs > theKnots.size() )
		return;

	m_knots.resize(knotSize());
	
	IndexType i, end = knotSize();

	if ( theKnotOffs + end > theKnots.size() ) {
		end = theKnots.size() - theKnotOffs;
	}
	
	RtFloat lastKnot = 0;
	for (i = 0; i < end; ++i) {
		RtFloat lastKnot =  theKnots[theKnotOffs+i];
		m_knots[i] = lastKnot;
	}
	for (i = end; i < knotSize(); ++i) {
		m_knots[i] = lastKnot;
	}
}

void CBSplineBasis::reset(RtInt theNCpts, RtInt theOrder,
						  const std::vector<RtFloat> &theKnots,
						  RtFloat theTMin, RtFloat theTMax, RtInt theTess)
{
	insertVals(theNCpts, theOrder, theTMin, theTMax, theTess);
	insertKnots(theKnots);
	validate();
	calc();
}


void CBSplineBasis::reset(RtInt theNCpts, RtInt theOrder,
						  const std::vector<RtFloat> &theKnots, IndexType theKnotOffs,
						  RtFloat theTMin, RtFloat theTMax, RtInt theTess)
{
	insertVals(theNCpts, theOrder, theTMin, theTMax, theTess);
	insertKnots(theKnots, theKnotOffs);
	validate();
	calc();
}


void CBSplineBasis::nuBlend(const std::vector<RtFloat> &source,
							RtInt offs,
							RtInt seg,
							std::vector<RtFloat> &pos) const
{
	RtInt span = seg + m_order - 1;
	IndexType pn = m_valCnts[span];
	IndexType koffset = m_valOffs[span];
	IndexType size = (IndexType) pn+pn;
	
	if ( pos.size() != size ) {
		pos.resize(size);
	}
	
	if ( pn <= 0 )
		return;
	
	IndexType i, cnt;
	RtFloat u, v, w, U, V, W, baseElem;
	for ( cnt = 0; cnt < pn; ++cnt ) {
		U = V = W = 0;
		for ( i = 0; i < (IndexType)m_order; ++i ) {
			baseElem = m_basis[(IndexType)((koffset+cnt)*m_order+i)];
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
		pos[cnt+cnt  ] = U;
		pos[cnt+cnt+1] = V;
	}
}

void CUVBSplineBasis::nuBlend(IndexType elemSize,
							  const std::vector<RtFloat> &source,
							  RtInt useg,
							  RtInt vseg,
							  const std::vector<IndexType> &idx,
							  std::vector<RtFloat> &results) const
{
	if ( source.size() <= 0 ) {
		results.clear();
		return;
	}

	RtInt uspan = useg + uBasis().order() - 1;
	RtInt vspan = vseg + vBasis().order() - 1;
	
	IndexType pnu     = uBasis().valCnts()[uspan];
	IndexType pnv     = vBasis().valCnts()[vspan];
	IndexType uoffset = uBasis().valOffs()[uspan];
	IndexType voffset = vBasis().valOffs()[vspan];
	
	IndexType numResults = pnu*pnv;
	
	if ( numResults == 0 )
		return;

	if ( results.size() != numResults ) {
		results.clear();
		results.resize(numResults*elemSize);
	}
	
	
	const RtFloat *s = &source[0];
	if ( !s )
		return;
	
	RtFloat *ptr = &results[0];
	if ( !ptr )
		return;
	
	std::vector<RtFloat> temp;
	temp.resize(elemSize * uBasis().order());
	
	IndexType i, j, k, u, v;
	
	RtFloat vBaseElem, uBaseElem, comp;
	
	for ( v = 0; v < pnv; ++v ) {
		
		for ( i = 0; i < (IndexType)uBasis().order(); ++i ) {
			for ( j = 0; j < elemSize; ++j ) {
				temp[i*elemSize + j] = 0;
			}
			for ( k = 0; k < (IndexType)vBasis().order(); ++k ) {
				vBaseElem = vBasis().basis()[(IndexType)((voffset+v)*vBasis().order()+k)];
				for ( j = 0; j < elemSize; ++j ) {
					comp = s[idx[k*uBasis().order()+i]*elemSize+j];
					temp[i*elemSize + j] += comp * vBaseElem;
				}
			}
		}
		
		for ( u = 0; u < pnu; ++u, ptr += elemSize ) {
			for ( j = 0; j < elemSize; ++j ) {
				*(ptr+j) = 0;
			}
			for ( k = 0; k < (IndexType)uBasis().order(); ++k ) {
				uBaseElem = uBasis().basis()[(IndexType)((uoffset+u)*uBasis().order()+k)];
				for ( j = 0; j < elemSize; ++j ) {
					comp = temp[k*elemSize + j];
					*(ptr+j) += comp * uBaseElem;
				}
			}
		}
	}
}

void CUVBSplineBasis::nuBlendWithNormals(IndexType elemSize,
										 const std::vector<RtFloat> &source,
										 RtInt useg,
										 RtInt vseg,
										 const std::vector<IndexType> &idx,
										 std::vector<RtFloat> &results,
										 std::vector<RtFloat> &normals) const
{
	if ( source.size() <= 0 ) {
		results.clear();
		return;
	}

	RtInt uspan = useg + uBasis().order() - 1;
	RtInt vspan = vseg + vBasis().order() - 1;
	
	IndexType pnu     = uBasis().valCnts()[uspan];
	IndexType pnv     = vBasis().valCnts()[vspan];
	IndexType uoffset = uBasis().valOffs()[uspan];
	IndexType voffset = vBasis().valOffs()[vspan];
	
	IndexType numResults = pnu*pnv;
	
	if ( numResults == 0 )
		return;

	if ( results.size() != numResults ) {
		results.clear();
		results.resize(numResults*elemSize);
	}

	if ( normals.size() != numResults ) {
		normals.clear();
		normals.resize(numResults*3);
	}
	
	// calc the partial derivatives with respect to u and v and the normals
	// example for homogene system:
	// calculate in the first step: dx/du, dy/du, dz/du
	// then dX/du = (w*dx/du - x*dw/du)/w**2
	// dY/du, dZ/du analog
	// then the dv and at last the normals as crossproduct of the partial derivatives
	
	
	std::vector<RtFloat> pdu(numResults*elemSize);
	std::vector<RtFloat> pdv(numResults*elemSize);

	const RtFloat *s = &source[0];
	RtFloat *ptru = &pdu[0];
	RtFloat *ptrv = &pdv[0];
	RtFloat *nrm = &normals[0];
	RtFloat *ptr = &results[0];
	
	std::vector<RtFloat> temp;
	temp.resize(elemSize * uBasis().order());
	
	IndexType i, j, k, u, v;
	RtFloat w2;
	
	RtFloat uBaseElem, vBaseElem, duBaseElem, dvBaseElem, comp;
	
	for ( v = 0; v < pnv; ++v ) {
		
		for ( i = 0; i < (IndexType)uBasis().order(); ++i ) {
			for ( j = 0; j < elemSize; ++j ) {
				temp[i*elemSize + j] = 0;
			}
			for ( k = 0; k < (IndexType)vBasis().order(); ++k ) {
				vBaseElem = vBasis().basis()[(IndexType)((voffset+v)*vBasis().order()+k)];
				for ( j = 0; j < elemSize; ++j ) {
					comp = s[ idx[k * uBasis().order() + i] * elemSize + j] ;
					temp[i * elemSize + j] += comp * vBaseElem;
				}
			}
		}
		
		for ( u = 0; u < pnu; ++u, ptr += elemSize, ptru += elemSize ) {
			for ( j = 0; j < elemSize; ++j ) {
				*(ptr+j) = 0;
				*(ptru+j) = 0;
			}
			for ( k = 0; k < (IndexType)uBasis().order(); ++k ) {
				uBaseElem  = uBasis().basis()[(uoffset+u)*uBasis().order()+k];
				duBaseElem = uBasis().basisDeriv()[(uoffset+u)*uBasis().order()+k];
				for ( j = 0; j < elemSize; ++j ) {
					comp = temp[k*elemSize + j];
					*(ptr+j)  += comp * uBaseElem;
					*(ptru+j) += comp * duBaseElem;
				}
			}
		}
	}
	
	temp.clear();
	temp.resize(elemSize * vBasis().order());
	RtFloat *ptrvsav;
	
	for ( u = 0; u < pnu; ++u ) {
		
		for ( i = 0; i < (IndexType)vBasis().order(); ++i ) {
			for ( j = 0; j < elemSize; ++j ) {
				temp[i * elemSize + j] = 0;
			}
			for ( k = 0; k < (IndexType)uBasis().order(); ++k ) {
				uBaseElem = uBasis().basis()[(IndexType)((uoffset+u)*uBasis().order()+k)];
				for ( j = 0; j < elemSize; ++j ) {
					comp = s[idx[i*uBasis().order()+k]*elemSize + j];
					temp[i*elemSize + j] += comp * uBaseElem;
				}
			}
		}
		
		ptrvsav = ptrv + elemSize;
		for ( v = 0; v < pnv; ++v, ptrv += (elemSize * pnu)  ) {
			for ( j = 0; j < elemSize; ++j ) {
				*(ptrv+j) = 0;
			}
			for ( k = 0; k < (IndexType)vBasis().order(); ++k ) {
				dvBaseElem = vBasis().basisDeriv()[(IndexType)((voffset+v)*vBasis().order()+k)];
				for ( j = 0; j < elemSize; ++j ) {
					comp = temp[k*elemSize + j];
					*(ptrv+j) += comp * dvBaseElem;
				}
			}
		}
		ptrv = ptrvsav;
	}
	
	ptr  = &results[0];
	ptru = &pdu[0];
	ptrv = &pdv[0];
	
	if ( elemSize == 4 ) {
		for ( i = 0; i < numResults; ++i ) {
			w2 = ptr[3] * ptr[3];
			for ( j = 0; j < 3; ++j ) {
				ptru[j] = (ptr[3]*ptru[j] - ptr[j]*ptru[3])/w2;
				ptrv[j] = (ptr[3]*ptrv[j] - ptr[j]*ptrv[3])/w2;
			}
			ptr  += elemSize;
			ptru += elemSize;
			ptrv += elemSize;
		}
		ptr  = &results[0];
		ptru = &pdu[0];
		ptrv = &pdv[0];
	}
	
	for ( i = 0; i < numResults; ++i ) {
		planeLH(nrm, ptrv, ptru);
		nrm  += 3;
		ptru += elemSize;
		ptrv += elemSize;
	}
}
