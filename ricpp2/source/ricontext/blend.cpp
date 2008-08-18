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

void CBilinearBlend::bilinearBlend(IndexType elemSize, const IndexType (&cornerIdx)[4], const std::vector<RtFloat> &vals, std::vector<RtFloat> &retvals)
{
	assert(tessU() > 0);
	assert(tessV() > 0);
	assert(elemSize > 0);
	
	IndexType size = (tessU()+1) * (tessV()+1) * elemSize;
	if ( retvals.size() != size ) {
		retvals.clear();
		retvals.resize(size);
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
			retvals[startPos+ei] = lerp(v, vals[cornerIdx[0]*elemSize+ei], vals[cornerIdx[2]*elemSize+ei]);
		}
		for ( ei = 0; ei < elemSize; ++ei ) {
			retvals[endPos+ei]   = lerp(v, vals[cornerIdx[1]*elemSize+ei], vals[cornerIdx[3]*elemSize+ei]);
		}
		idx = startPos+elemSize;
		for ( u = deltau, ui = 1; ui < tessU(); ++ui, u += deltau ) {
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
								   std::vector<RtFloat> &retvals) const
{
	assert(tessU() > 0);
	assert(tessV() > 0);
	assert(elemSize > 0);
	assert(uVector().size() == duVector().size());
	assert(vVector().size() == dvVector().size());
	assert(uVector().size() == (size_t)(tessU()+1)*4);
	assert(vVector().size() == (size_t)(tessV()+1)*4);
	
	IndexType tessSize = (tessU()+1)*(tessV()+1)*elemSize;
	
	if ( retvals.size() != tessSize ) {
		retvals.clear();
		retvals.resize(tessSize);
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
				retvals[id++] =
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
											  std::vector<RtFloat> &retvals,
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
	
	if ( retvals.size() != tessSize ) {
		retvals.clear();
		retvals.resize(tessSize);
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
				
				retvals[id] =
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
			w2 = retvals[id+3] * retvals[id+3];
			for ( j = 0; j < 3; ++j ) {
				pdu[id+j] = (retvals[id+3]*pdu[id+j] - retvals[id+j]*pdu[id+3])/w2;
				pdv[id+j] = (retvals[id+3]*pdv[id+j] - retvals[id+j]*pdv[id+3])/w2;
			}
			id += elemSize;
		}
	}
	
	id = 0;
	IndexType idnrm = 0;
	for ( i = 0; i < sumPos; ++i ) {
		if ( flipNormal )
			plane(&normals[idnrm], &pdv[id], &pdu[id]);
		else
			planeLH(&normals[idnrm], &pdv[id], &pdu[id]);
		idnrm += normElemSize;
		id += elemSize;
	}
}


