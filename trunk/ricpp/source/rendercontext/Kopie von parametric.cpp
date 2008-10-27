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

#include <math.h>

// Caches are static by now - must implement some other way to avoid clashes while using DLLs
static TUVVectorCache s_uvVectorCache;
static TUnitCircleCache s_unitCircleCache;
static TUnitSphereCache s_unitSphereCache;


TBSplineBasis::TBSplineBasis(RtInt n, RtInt k, const std::vector<RtFloat> &x, RtFloat tmini, RtFloat tmaxi, RtInt tesselation) {
	ncpts = n;
	order = k;
	knots = x;
	tmin  = tmini;
	tmax  = tmaxi;
	tess = tesselation;
	segments = 0;
	calc();
}

void TBSplineBasis::bsplineBasisDerivate(RtFloat t, RtInt span,
	std::vector<RtFloat> &N, std::vector<RtFloat> &Nd)
{
	const RtInt size  = ncpts+order-1;

	// Init N
	N.assign(size, 0.0);
	N[span] = 1.0;

	// Calculate
	RtInt i, j;
	RtFloat temp;

	for ( j=1; j < order; ++j ) {
		for ( i=0; i < size-j; ++i ) {

			// Calc the first Derivate
			if ( j == 1 ) {
				temp = knots[i+j] - knots[i];
				if ( temp >= riEpsilon ) {
					temp = N[i] / temp;
				}
				Nd[i] = temp;
				temp = knots[i+j+1] - knots[i+1];
				if ( temp >= riEpsilon ) {
					temp = N[i+1] / temp;
					Nd[i] -= temp;
				}
			} else {
				temp = knots[i+j] - knots[i];
				if ( temp >= riEpsilon ) {
					temp = (N[i] + (t-knots[i])*Nd[i]) / temp;
				}
				Nd[i] = temp;
				temp = knots[i+j+1] - knots[i+1];
				if ( temp >= riEpsilon ) {
					temp = (N[i+1] - (knots[i+j+1]-t)*Nd[i+1]) / temp;
					Nd[i] -= temp;
				}
			}

			// Calc the basis
			if ( N[i] >= riEpsilon ) {
				temp = knots[i+j]-knots[i];
				if ( temp >= riEpsilon ) {
					temp = N[i] * ((t - knots[i]) / temp);
				}
				N[i] = temp;
			}

			if ( N[i+1] >= riEpsilon ) {
				temp = knots[i+j+1]-knots[i+1];
				if ( temp >= riEpsilon ) {
					temp = N[i+1] * ((knots[i+j+1] - t) / temp);
					N[i] += temp;
				}
			}
		}
	}
}

bool TBSplineBasis::unvalidParams() {
	if ( order > ncpts )
		order = ncpts;
	if ( ncpts == 0 || order == 0 )
		return true;

	if ( (RtInt)knots.size() < ncpts+order )
		return true;

	if ( (RtInt)knots.size() > ncpts+order )
		knots.resize(ncpts+order);

	RtInt i;
	for ( i = 1; i < ncpts+order; ++i ) {
		if ( knots[i] < knots[i-1] )
			knots[i-1] = knots[i];
	}

	if ( tmin > tmax ) {
		RtFloat t = tmax;
		tmax = tmin;
		tmin = t;
	}

	if ( tmin < knots[order-1] )
		tmin = knots[order-1];

	if ( tmax > knots[ncpts] )
		tmax = knots[ncpts];

	return false;
}

void TBSplineBasis::calc() {
	// Test parameter conditions
	segments = 0;
	if ( unvalidParams() )
		return;

	RtFloat ts, te;
	RtFloat t, dt;
	RtInt cnt;

	segments = 1 + ncpts - order;

	tVals.clear();
	tVals.reserve(segments*tess+1);
	valCnts.clear();
	valCnts.resize(ncpts);
	valCnts.assign(ncpts, 0);

	for ( RtInt knotIdx = order-1; knotIdx < ncpts; ++knotIdx ) {
		ts = knots[knotIdx];
		te = knots[knotIdx+1];
		dt = (te-ts)/tess;

		if ( dt < riEpsilon )
			continue;

		if ( tmax < ts )
			break;
		if ( tmin >= te )
			continue;

		// tmax >= ts && tmin < te

		if ( tmin > ts )
			ts = tmin;
		if ( tmax < te )
			te = tmax;

		valCnts[knotIdx] = (RtInt)tVals.size();
		tVals.push_back(ts);
		for ( t = knots[knotIdx], cnt = 0; t < te && cnt < tess; t += dt, ++cnt ) {
			if ( t <= ts )
				continue;
			tVals.push_back(t);
		}
		tVals.push_back(te);
		valCnts[knotIdx] = (RtInt)tVals.size()-valCnts[knotIdx];
	}

	std::vector<RtFloat> Ntemp(ncpts+order-1);     // BASIS, result can be found in N[0..n-1]
	std::vector<RtFloat> Ndtemp(ncpts+order-1);    // BASIS derivative, result can be found in  Nd[0..n-1]

	basis.clear();
	basis.resize(tVals.size()*order);
	basisDeriv.clear();
	basisDeriv.resize(tVals.size()*order);

	RtInt GIdx = 0;
	RtInt i, j;
	std::vector<RtFloat>::iterator iter = tVals.begin();
	for ( RtInt span = order-1; span < ncpts; ++span ) {
		for ( j = valCnts[span]; j > 0 && iter != tVals.end(); iter++, --j ) {
			bsplineBasisDerivate(*iter, span, Ntemp, Ndtemp);
			for ( i = span-order+1; i <= span; ++i, ++GIdx ) {
				// Cache the results (basis and derivative) for parameter t
				basis[GIdx] = Ntemp[i];
				basisDeriv[GIdx] = Ndtemp[i];
			}
		}
	}
}


TBSplineBasis &TBSplineBasis::operator=(const TBSplineBasis &sp) {
	ncpts = sp.ncpts;
	order = sp.order;
	tmin  = sp.tmin;
	tmax  = sp.tmax;
	knots = sp.knots;
	tess = sp.tess;
	segments = sp.segments;
	tVals = sp.tVals;
	valCnts = sp.valCnts;

	basis = sp.basis;
	basisDeriv = sp.basisDeriv;
	return *this;
}

TBSplineBasis::reset(RtInt n, RtInt k, const std::vector<RtFloat> &x, RtFloat tmini, RtFloat tmaxi, RtInt tesselation) {
	ncpts = n;
	order = k;
	knots = x;
	tmin  = tmini;
	tmax  = tmaxi;
	tess = tesselation;
	segments = 0;
	tVals.clear();
	valCnts.clear();
	basis.clear();
	basisDeriv.clear();
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


// ----------------------------------------------------------------------------
// bicubicBlend
// Bicubic blending of a 4x4 patch, u major
// Indexes
//  0  1  2  3     u->
//  4  5  6  7
//  8  9 10 11
// 12 13 14 15
void TUVVector::bicubicBlend(
							 const IndexType (& idx)[16],
							 const TParameter &source,
							 TNamedVector<RtFloat> &dest,
							 bool clearAndReserve) const
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
		clearAndReserve);
}


// ----------------------------------------------------------------------------
void TUVVector::bicubicBlend(
							 const IndexType (& idx)[16],
							 const char *name,
							 IndexType components,
							 IndexType cardinality,
							 const RtFloat *source,
							 TNamedVector<RtFloat> &dest,
							 bool clearAndReserve) const
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
					m_vVector[v*4]   * source[idx[ i    ] * numComponents + j] +
					m_vVector[v*4+1] * source[idx[(i+ 4)] * numComponents + j] +
					m_vVector[v*4+2] * source[idx[(i+ 8)] * numComponents + j] +
					m_vVector[v*4+3] * source[idx[(i+12)] * numComponents + j];
			}
		}
		for ( u = 0; u < m_tessU+1; ++u ) {
			for ( j = 0; j < numComponents; ++j ) {
				*(ptr++) = m_uVector[u*4]   * temp[                j] +
					m_uVector[u*4+1] * temp[  numComponents+j] +
					m_uVector[u*4+2] * temp[2*numComponents+j] +
					m_uVector[u*4+3] * temp[3*numComponents+j];
			}
		}
	}
}


// ----------------------------------------------------------------------------
void TUVVector::bicubicBlendNormals(
									const IndexType (& idx)[16],
									const char *name,
									IndexType components,
									IndexType cardinality,
									const RtFloat *source,
									TNamedVector<RtFloat> &dest,
									bool clearAndReserve) const
{
	if ( components != 3 && cardinality != 1 )
		return;

	const IndexType numComponents = components * cardinality;
	IndexType numElems = (m_tessU+1)*(m_tessV+1);

	std::vector<RtFloat> temp;
	temp.resize(4*numComponents);
	std::vector<RtFloat> ds;
	ds.resize(numComponents);

	if ( clearAndReserve ) {
		dest.clear();
		dest.initialize(TRi::RI_N, components, 1, numElems);
	}

	RtFloat *ptr = dest.begin();

	IndexType i, j, k, u, v;

	for ( v = 0; v < m_tessV+1; ++v ) {
		for ( i = 0, k = 0; i < 4; ++i ) {
			for ( j = 0; j < numComponents; ++j, ++k ) {
				temp[k] =
					m_vVector[v*4]   * source[idx[ i    ]*numComponents + j] +
					m_vVector[v*4+1] * source[idx[(i+ 4)]*numComponents + j] +
					m_vVector[v*4+2] * source[idx[(i+ 8)]*numComponents + j] +
					m_vVector[v*4+3] * source[idx[(i+12)]*numComponents + j];
			}
		}
		for ( u = 0; u < m_tessU+1; ++u ) {
			for ( j = 0; j < numComponents; ++j ) {
				*(ptr++) = m_duVector[u*4]   * temp[                j] +
					m_duVector[u*4+1] * temp[  numComponents+j] +
					m_duVector[u*4+2] * temp[2*numComponents+j] +
					m_duVector[u*4+3] * temp[3*numComponents+j];
			}
		}
	}

	for ( u = 0; u < m_tessU+1; ++u ) {
		for ( i = 0, k = 0; i < 4; ++i ) {
			for ( j = 0; j < numComponents; ++j, ++k ) {
				temp[k] =
					m_uVector[u*4]   * source[idx[(i*4)  ]*numComponents+j] +
					m_uVector[u*4+1] * source[idx[(i*4+1)]*numComponents+j] +
					m_uVector[u*4+2] * source[idx[(i*4+2)]*numComponents+j] +
					m_uVector[u*4+3] * source[idx[(i*4+3)]*numComponents+j];
			}
		}
		for ( v = 0, i = u*numComponents; v < m_tessV+1; ++v, i += (m_tessU+1)*numComponents) {
			for ( j = 0; j < numComponents; ++j ) {
				ds[j] =
					m_dvVector[v*4]   * temp[                j] +
					m_dvVector[v*4+1] * temp[  numComponents+j] +
					m_dvVector[v*4+2] * temp[2*numComponents+j] +
					m_dvVector[v*4+3] * temp[3*numComponents+j];
			}
			planeNorm(&(ds[0]), &(dest[i]), &(dest[i]));
		}
	}
}

#if 0
// ----------------------------------------------------------------------------
void TUVVector::bicubicBlendNormals(
									const IndexType (& idx)[16],
									const char *name,
									IndexType components,
									IndexType cardinality,
									const RtFloat *source,
									TNamedVector<RtFloat> &dest,
									bool clearAndReserve) const
{
	// Only for RI_P and RI_PW
	if ( components != 3 && components != 4 )
		return;

	const IndexType numComponents = components * cardinality;
	IndexType numElems = (m_tessU+1)*(m_tessV+1);

	std::vector<RtFloat> temp;
	temp.resize(4*numComponents);
	std::vector<RtFloat> ds;
	ds.resize(numComponents);

	if ( clearAndReserve ) {
		// Normal vectors always have 3 components
		dest.clear();
		dest.initialize(TRi::RI_N, 3, cardinality, numElems);
	}

	TNamedVector<RtFloat> tempU;
	if ( components == 4 ) {
		tempU.initialize(TRi::RI_PW, components, cardinality, numElems);
	}

	RtFloat *ptrDest  = dest.begin();
	RtFloat *ptrTempU = NULL;

	RtFloat *ptr = ptrDest;
	if ( components == 4 ) {
		ptrTempU = tempU.begin();
		ptr = ptrTempU;
	}

	IndexType i, j, k, u, v;

	for ( v = 0; v < m_tessV+1; ++v ) {
		for ( i = 0, k = 0; i < 4; ++i ) {
			for ( j = 0; j < numComponents; ++j, ++k ) {
				temp[k] =
					m_vVector[v*4]   * source[idx[ i    ]*numComponents + j] +
					m_vVector[v*4+1] * source[idx[(i+ 4)]*numComponents + j] +
					m_vVector[v*4+2] * source[idx[(i+ 8)]*numComponents + j] +
					m_vVector[v*4+3] * source[idx[(i+12)]*numComponents + j];
			}
		}
		for ( u = 0; u < m_tessU+1; ++u ) {
			for ( j = 0; j < numComponents; ++j ) {
				*(ptr++) =
					m_duVector[u*4]   * temp[                j] +
					m_duVector[u*4+1] * temp[  numComponents+j] +
					m_duVector[u*4+2] * temp[2*numComponents+j] +
					m_duVector[u*4+3] * temp[3*numComponents+j];
			}
		}

		if ( components == 4 ) {
			for ( u = 0; u < m_tessU+1; ++u ) {
				if ( ptrTempU[3] != 0 ) {
					ptrDest[0] = ptrTempU[0]/ptrTempU[3];
					ptrDest[1] = ptrTempU[1]/ptrTempU[3];
					ptrDest[2] = ptrTempU[2]/ptrTempU[3];
				} else {
					ptrDest[0] = ptrTempU[0];
					ptrDest[1] = ptrTempU[1];
					ptrDest[2] = ptrTempU[2];
				}
				ptrDest  += 3;
				ptrTempU += numComponents;
			}
		}
	}

	for ( u = 0; u < m_tessU+1; ++u ) {
		for ( i = 0, k = 0; i < 4; ++i ) {
			for ( j = 0; j < numComponents; ++j, ++k ) {
				temp[k] =
					m_uVector[u*4]   * source[idx[(i*4)  ]*numComponents+j] +
					m_uVector[u*4+1] * source[idx[(i*4+1)]*numComponents+j] +
					m_uVector[u*4+2] * source[idx[(i*4+2)]*numComponents+j] +
					m_uVector[u*4+3] * source[idx[(i*4+3)]*numComponents+j];
			}
		}
		for ( v = 0, i = u*3*cardinality; v < m_tessV+1; ++v, i += (m_tessU+1)*3*cardinality ) {
			for ( j = 0; j < numComponents; ++j ) {
				ds[j] =
					m_dvVector[v*4]   * temp[                j] +
					m_dvVector[v*4+1] * temp[  numComponents+j] +
					m_dvVector[v*4+2] * temp[2*numComponents+j] +
					m_dvVector[v*4+3] * temp[3*numComponents+j];
			}
			if ( components == 4 ) {
				if ( ds[3] != 0 ) {
					ds[0] /= ds[3];
					ds[1] /= ds[3];
					ds[2] /= ds[3];
				}
			}
			planeNorm(&(ds[0]), &(dest[i]), &(dest[i]));
		}
	}
}
#endif

// ============================================================================
// TUVPatch

// ----------------------------------------------------------------------------
TUVVector &TUVVectorCache::findUVVector(IndexType tessU, IndexType tessV, const RtBasis &uBasis, const RtBasis &vBasis) {
	bool isNew;
	return findBicubicUV(m_cache, tessU, tessV, uBasis, vBasis, m_lastIndex, isNew);
}


// ============================================================================
// TNuBasis

// Cox DeBoor recursion formular
void TNuBasis::Cox_DeBoor(RtInt i, RtInt k, RtInt deg, RtFloat t, RtFloat tmax, RtInt n, const std::vector<RtFloat> &knotvec, std::vector<RtFloat> &N) {
	assert((i+k+1) <= n+deg);

	// End the recursion on degree 0
	if ( k == 0 ) {
		N[i] = 0;
		if ( (knotvec[i] <= t) ) {
			if ( t < knotvec[i+1] ) // ???? t < knotvec[i+1]
				N[i] = 1;
			else
			{
				// Include the last value of the patch...
				// if ( i == n+degree-2 && (t == knotvec[i+1]) )
				if ( t == tmax && (t == knotvec[i+1]) )
					N[i] = 1;
			}
		}
		return;
	}

	assert(k > 0);

	// calculate the 'left' values of degree-1, k-i <= 0 are already
	// calculated
	if ( i == 0 ) {
		Cox_DeBoor(i, k-1, deg, t, tmax, n, knotvec, N);
	}
	RtFloat NLeft  = N[(k-1)*(n+deg+1) + i];

	// Calculate the 'right' values
	RtFloat NRight = 0;
	if ( i < n-1 ) {
		Cox_DeBoor(i+1, k-1, deg, t, tmax, n, knotvec, N);
		NRight = N[(k-1)*(n+deg+1) + (i+1)];
	}

	RtInt temp = k*(n+deg+1)+i; // Index in N for parameter t
	RtFloat temp2;

	// Calculate the value of the basis function for parameter t
	N[temp] = 0;
	temp2 = knotvec[i+k] - knotvec[i];
	if ( temp2 != (RtFloat)0.0 )
		N[temp] =  ((t-knotvec[i])     * NLeft)  / temp2;
	temp2 = knotvec[i+k+1] - knotvec[i+1];
	if ( temp2 != (RtFloat)0.0 )
		N[temp] += ((knotvec[i+k+1]-t) * NRight) / temp2;
}


// Cox DeBoor recursion formular for derivates
void TNuBasis::Cox_DeBoor_derivate(RtInt i, RtInt k, RtInt deg, RtFloat t, RtFloat tmax, RtInt n, const std::vector<RtFloat> &knotvec, std::vector<RtFloat> &N) {
	assert((i+k+1) <= n+deg);

	// Constant on degree 0
	if ( k == 0 ) {
		N[i] = 0;
		return;
	}

	assert(k > 0);

	// calculate the 'left' values of degree-1, k-i <= 0 are already
	// calculated
	if ( i == 0 ) {
		Cox_DeBoor(i, k-1, deg, t, tmax, n, knotvec, N);
	}
	RtFloat NLeft  = N[(k-1)*(n+deg+1) + i];

	// Calculate the 'right' values
	RtFloat NRight = 0;
	//	if ( i < n-1 ) {
	Cox_DeBoor(i+1, k-1, deg, t, tmax, n, knotvec, N);
	NRight = N[(k-1)*(n+deg+1) + (i+1)];
	//	}

	RtInt temp = k*(n+deg+1)+i; // Index in N for parameter t
	RtFloat temp2;

	// Calculate the value of the basis function for parameter t
	N[temp] = 0;
	temp2 = knotvec[i+k] - knotvec[i];
	if ( temp2 != (RtFloat)0.0 )
		N[temp] =  (k * NLeft)  / temp2;
	temp2 = knotvec[i+k+1] - knotvec[i+1];
	if ( temp2 != (RtFloat)0.0 )
		N[temp] -= (k * NRight) / temp2;
}


RtFloat TNuBasis::derivative2(RtInt i, RtInt k, RtInt deg, RtInt degMax, RtFloat t, RtFloat tmax, const std::vector<RtFloat> &knotvec) {
	if ( k == 0 ) {
		if ( (knotvec[i] <= t) ) {
			if ( t < knotvec[i+1] ) // ???? t < knotvec[i+1]
				return 1.0;
			else
			{
				// Include the last value of the patch...
				// if ( i == n+degree-2 && (t == knotvec[i+1]) )
				if ( t == tmax && (t == knotvec[i+1]) )
					return 1.0;
			}
		}
		return 0.0;
	}

	RtFloat tempLeft, tempRight, temp;

	temp = knotvec[i+deg]-knotvec[i];
	tempLeft = 0.0;
	if ( temp != 0.0 ) {
		tempLeft = derivative2(i, k-1, deg-1, degMax, t, tmax, knotvec);
		tempLeft -= knotvec[i] * derivative2(i, k, deg-1, degMax, t, tmax, knotvec);
		tempLeft /= temp;
	}

	temp = knotvec[i+deg+1]-knotvec[i+1];
	tempRight = 0.0;
	if ( temp != 0.0 ) {
		tempRight = derivative2(i+1, k-1, deg-1, degMax, t, tmax, knotvec);
		tempRight -= knotvec[i+deg+1] * derivative2(i+1, k, deg-1, degMax, t, tmax, knotvec);
		tempRight /= temp;
	}

	return
		tempLeft - tempRight;
}


RtFloat TNuBasis::derivative1(RtInt i, RtInt deg, RtFloat t, RtFloat tmax, const std::vector<RtFloat> &knotvec) {
	if ( deg == 0 ) {
		if ( (knotvec[i] <= t) ) {
			if ( t < knotvec[i+1] ) // ???? t < knotvec[i+1]
				return 1.0;
			else
			{
				// Include the last value of the patch...
				// if ( i == n+degree-2 && (t == knotvec[i+1]) )
				if ( t == tmax && (t == knotvec[i+1]) )
					return 1.0;
			}
		}
		return 0.0;
	}

	RtFloat tempLeft, tempRight, temp;

	temp = knotvec[i+deg]-knotvec[i];
	tempLeft = 0.0;
	if ( temp != 0.0 ) {
		tempLeft = derivative2(i, deg-1, deg-1, deg, t, tmax, knotvec);
		tempLeft /= temp;
	}

	temp = knotvec[i+deg+1]-knotvec[i+1];
	tempRight = 0.0;
	if ( temp != 0.0 ) {
		tempRight = derivative2(i+1, deg-1, deg-1, deg, t, tmax, knotvec);
		tempRight /= temp;
	}
	return
		tempLeft - tempRight;

}


RtFloat TNuBasis::derivative(RtInt i, RtInt deg, RtFloat t, RtFloat tmax, const std::vector<RtFloat> &knotvec) {
	RtFloat temp;
	RtFloat tt = 1.0, sum = 0.0;

	for ( RtInt k = 0; k < deg; ++k ) {
		++k; // k = [1 .. deg-1]
		temp = derivative2(i, k, deg, deg, t, tmax, knotvec);
		sum += (RtFloat)k * temp * tt;
		tt *= t;
	}

	temp = derivative1(i, deg, t, tmax, knotvec);
	sum += (RtFloat)deg * temp * tt;

	return sum;
}


void TNuBasis::calc(
					IndexType tess,
					RtFloat tmin, RtFloat tmax,
					const std::vector<RtFloat> &knotvec,
					bool isDerivate
					)
{
	// Calculate the basis functions and store the values

	// The knot vectors size has to be the sum of the control points (n()) and
	// the order of the polynom
	assert (knotvec.size() == (n() + order()));

	// The Matrix (n()+order()) X order() to calculate the basis values at parameter value t
	std::vector<RtFloat> basisMat;
	RtInt colBasis = n() + order();     // size of the knot vector are the colums
	basisMat.resize(colBasis * order());// order of the polynome are the rows,
	// the result of the Cox DeBoor rec. form. will be in the order()-1 row

	m_values.clear();                   // Clear the basis values: Number of control points X Number of parameter t values
	m_values.resize(n() * (tess+1));    // Resize to hold the resuts of the Cox DeBoor formular
	// for each parametric t value and control point.


	RtFloat ftemp;          // For debugging; the last result of the Cox DeBoor formular
	RtInt i;                // Index for number of control points
	IndexType j;            // Index for the number of parameter t values
	IndexType bIdxCnt = 0;  // Index fpr m_values

	// Calculate the basis values for each parameter t
	RtFloat t = tmin;                       // Start at parameter t=tmin
	RtFloat dt = (tmax-tmin)/(RtFloat)tess; // Increment
	for ( j = 0; j <= tess; ++j, t += dt ) {
		if ( j == tess )
			t = tmax;
		for ( i = 0; i < n(); ++i, ++bIdxCnt ) {

			// Calculate the basis for parameter t using the Cox DeBoor formular
			if ( isDerivate ) {
				ftemp = derivative(i, degree(), t, tmax, knotvec);
				// Cox_DeBoor_derivate(i, degree(), degree(), t, tmax, n(), knotvec, basisMat);
				// Copy the result
				// ftemp = basisMat[degree()*colBasis+i];
			} else {
				Cox_DeBoor(i, degree(), degree(), t, tmax, n(), knotvec, basisMat);
				// Copy the result
				ftemp = basisMat[degree()*colBasis+i];
			}
			m_values[bIdxCnt] = ftemp;
		}
	}
}

// ============================================================================
// TUVPatch

// ----------------------------------------------------------------------------
const RtFloat *TUVPatch::getTextureST(TNamedVector<RtFloat> &destST, IndexType &size, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) {
	RtFloat slenTop    = s2 - s1;
	RtFloat slenBottom = s4 - s3;
	RtFloat tlenTop    = t3 - t1;
	RtFloat tlenBottom = t4 - t2;

	slenTop    /= m_numFacesU;
	slenBottom /= m_numFacesU;
	tlenTop    /= m_numFacesV;
	tlenBottom /= m_numFacesV;

	s1 = s1 + m_faceIdxU * slenTop;
	s2 = s1 + slenTop;
	t1 = t1 + m_faceIdxV * tlenTop;
	t3 = t1 + tlenTop;

	s3 = s3 + m_faceIdxU * slenBottom;
	s4 = s3 + slenTop;
	t2 = t2 + m_faceIdxV * tlenBottom;
	t4 = t2 + tlenTop;

	const RtFloat *pst = TPrimFace::getTextureST(destST, size, s1, t1, s2, t2, s3, t3, s4, t4);
	if ( pst != NULL ) {
		return pst;
	}

	IndexType idx[4];
	fillCornerIdx(idx);
	const TNamedVector<RtFloat> *datas = getInterpolatedData(TRi::RI_S);
	const TNamedVector<RtFloat> *datat = getInterpolatedData(TRi::RI_T);

	RtFloat st[8];
	st[0] = s1;
	st[1] = t1;
	st[2] = s2;
	st[3] = t2;
	st[4] = s3;
	st[5] = t3;
	st[6] = s4;
	st[7] = t4;

	destST.initialize(TRi::RI_ST, 2, 1, (m_tessU+1)*(m_tessV+1));

	if ( datas ) {
		if ( datas->size() != destST.size()/2 ) {
			datas = NULL;
		} else {
			RtFloat *to = destST.begin();
			RtFloat *from = datas->begin();
			for ( ; from != datas->end(); ++from ) {
				*to = *from;
				to += 2;
			}
		}
	}
	if ( !datas ) {
		if ( !m_useUniformS ) {
			bilinearBlend(m_tessU, m_tessV, idx, 2, st, destST, TRi::RI_ST, 1, 0, 1, false);
		} else {
			RtFloat *to = destST.begin();
			for ( ; to < destST.end(); to += 2 ) {
				*to = m_uniformS;
			}
		}
	}

	if ( datat ) {
		if ( datat->size() != destST.size()/2 ) {
			datat = NULL;
		} else {
			RtFloat *to = destST.begin();
			++to;
			RtFloat *from = datat->begin();
			for ( ; from != datat->end(); ++from ) {
				*to = *from;
				to += 2;
			}
		}
	}
	if ( !datat ) {
		if ( !m_useUniformT ) {
			bilinearBlend(m_tessU, m_tessV, idx, 2, &(st[1]), destST, TRi::RI_ST, 1, 1, 1, false);
		} else {
			RtFloat *to = destST.begin();
			++to;
			for ( ; to < destST.end(); to += 2 ) {
				*to = m_uniformT;
			}
		}
	}

	size = destST.size();
	return destST.begin();
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
void TUVPatch::insertNormalData() {
	IndexType nverts = nu() * nv();
	if ( nu() < 2 || nv() < 2 )
		return;
	if ( !m_vertexStrip && m_vertexStrip->numElems() != nverts )
		return;
	if ( m_freeNormalStrip && m_normalStrip )
		delete m_normalStrip;
	m_normalStrip = new TNamedVector<RtFloat>(TRi::RI_N, 3, 1, nverts);
	if ( !m_normalStrip ) {
		return;
	}
	m_freeNormalStrip = true;

	std::vector<RtFloat> startDV(nu()*3);
	RtFloat startDU[3];

	IndexType u, v, i, pos;
	RtFloat dPdu[3], dPdv[3], prev_dPdu[3], prev_dPdv[3];
	RtFloat *dest = m_normalStrip->begin();
	bool uClosed;
	std::vector<bool> vClosed(nu());

	prev_dPdv[0] = prev_dPdv[1] = prev_dPdv[2] = 0;
	for ( v=0, pos = 0; v < nv(); ++v ) {
		prev_dPdu[0] = prev_dPdu[1] = prev_dPdu[2] = 0;
		uClosed = true;
		for ( i = 0; i<3 ; ++i ) {
			uClosed = uClosed &&
				( (*m_vertexStrip)[pos*3+i] == (*m_vertexStrip)[(pos+nu()-1)*3+i]);
		}
		for ( u=0; u < nu(); ++u, ++pos ) {
			if ( v == 0 ) {
				vClosed[u] = true;
				for ( i = 0; i<3 ; ++i ) {
					vClosed[u] = vClosed[u] && 
						((*m_vertexStrip)[pos*3+i] == (*m_vertexStrip)[(pos+(nv()-1)*nu())*3+i]);
				}
			}
			for ( i = 0; i<3 ; ++i ) {
				if ( u < nu()-1 ) {
					dPdu[i] = (*m_vertexStrip)[(pos+1)*3+i]-(*m_vertexStrip)[pos*3+i];
				} else {
					if ( u > 0 ) {
						if ( uClosed ) {
							dPdu[i] = startDU[i];
						} else {
							dPdu[i] = (*m_vertexStrip)[pos*3+i]-(*m_vertexStrip)[(pos-1)*3+i];
						}
					} else {
						dPdu[i] = 0;
					}
				}
				if ( u == 0 ) {
					startDU[i] = dPdu[i];
				}

				if ( v < nv()-1 ) {
					dPdv[i] = (*m_vertexStrip)[(pos+nv())*3+i]-(*m_vertexStrip)[pos*3+i];
				} else {
					if ( v > 0 ) {
						if ( vClosed[u] ) {
							dPdv[i] = startDV[u*3+i];
						} else {
							dPdv[i] = (*m_vertexStrip)[pos*3+i]-(*m_vertexStrip)[(pos-nv())*3+i];
						}
					} else {
						dPdv[i] = 0;
					}
				}
				if ( v == 0 ) {
					startDV[u*3+i] = dPdv[i];
				}
			}
			planeNorm(dPdv, dPdu, dest);
			dest += 3;
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
	RtFloat m = rmax/zmax; // 2D: f(x) = mx**2; F(x)=2x+m

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

		nn = (RtFloat)(-2.0*r-m);
		len = 0.0;
		uverts = m_tessU + 1;

		for ( u = 0.0, puidx=0; uverts > 0; --uverts, u+=deltau ) {
			if ( uverts == 1 )
				u = 1.0;

			n[0] = unitcircle[puidx];
			n[1] = unitcircle[++puidx];
			n[2] = nn;
			++puidx;

			(*m_vertexStrip)[pidx]   = r * n[0];
			(*m_vertexStrip)[++pidx] = r * n[1];
			(*m_vertexStrip)[++pidx] = z;
			++pidx;

			if ( uverts == m_tessU + 1 )
				len = (RtFloat)sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);

			if ( len == 0.0 ) {
				n[0] = 0.0;
				n[1] = 0.0;
				n[2] = (RtFloat)-1.0;
			} else {
				n[0] /= len;
				n[1] /= len;
				n[2] /= len;
			}

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
			if ( p->getBasicType() == TYPE_FLOAT && p->getSize() >= 4 ) {
				// Single bicubic patches using CLASS_VARYING and CLASS_VERTEX only
				if ( p->getClass() == CLASS_VARYING ) {
					m_vertexData.resize(m_vertexData.size()+1);
					bilinearBlend(m_tessU, m_tessV, cornerIdx, *p, m_vertexData.back());
				} else if ( p->getClass() == CLASS_FACEVARYING ) {
					m_vertexData.resize(m_vertexData.size()+1);
					bilinearBlend(m_tessU, m_tessV, faceCornerIdx, *p, m_vertexData.back());
				} else if ( p->getClass() == CLASS_VERTEX ) {
					m_vertexData.resize(m_vertexData.size()+1);
					uv.bicubicBlend(cubicIdx, *p, m_vertexData.back(), true);
				} else if ( p->getClass() == CLASS_FACEVERTEX ) {
					m_vertexData.resize(m_vertexData.size()+1);
					uv.bicubicBlend(faceCubicIdx, *p, m_vertexData.back(), true);
				}
			}
		}
	}

	// Insert P if not present
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
					*(dest++) = 0.0;
					*(dest++) = 0.0;
					*(dest++) = 0.0;
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
				IndexType idx[4] = {
					0, 1,
					2, 3
				};
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
		IndexType components = 3;
		IndexType cardinality = 1;

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
			m_normalStrip = new TNamedVector<RtFloat>;
			if ( !m_normalStrip )
				throw TSystemException(__FILE__, __LINE__);
			// Calculate normals
			RtFloat pts[16*4];

			const TParameter *p = pm.getParameterOf(TRi::RI_P);
			if ( p ) {
				if ( p->getComponents() == 3 && p->getBasicType() == TYPE_FLOAT && p->getClass() == CLASS_VERTEX ) {
					RtFloat *ptr = (RtFloat *)(p->getValue());
					IndexType num = p->getNumberOfElements();
					for ( int i=0; i<16; ++i ) {
						if ( cubicIdx[i] >= num ) {
							p = NULL;
							break;
						}
						pts[i*3+0] = ptr[cubicIdx[i]*3+0];
						pts[i*3+1] = ptr[cubicIdx[i]*3+1];
						pts[i*3+2] = ptr[cubicIdx[i]*3+2];
					}
					cardinality = p->getCardinality();
				} else {
					p = NULL;
				}
			}

			if ( !p ) {
				p = pm.getParameterOf(TRi::RI_PW);
				if ( p ) {
					if ( p->getComponents() == 4 && p->getBasicType() == TYPE_FLOAT && p->getClass() == CLASS_VERTEX ) {
						IndexType num = p->getNumberOfElements();
						RtFloat *ptr = (RtFloat *)(p->getValue());
						for ( int i=0; i<16; ++i ) {
							if ( cubicIdx[i] >= num ) {
								p = NULL;
								break;
							}
							if ( ptr[cubicIdx[i]*4+3] != 0 ) {
								pts[i*3+0] = ptr[cubicIdx[i]*4+0]/ptr[cubicIdx[i]*4+3];
								pts[i*3+1] = ptr[cubicIdx[i]*4+1]/ptr[cubicIdx[i]*4+3];
								pts[i*3+2] = ptr[cubicIdx[i]*4+2]/ptr[cubicIdx[i]*4+3];
							} else {
								pts[i*3+0] = ptr[cubicIdx[i]*4+0];
								pts[i*3+1] = ptr[cubicIdx[i]*4+1];
								pts[i*3+2] = ptr[cubicIdx[i]*4+2];
							}
						}
						cardinality = p->getCardinality();
					} else {
						p = NULL;
					}
				}
			}

#if 0
			if ( !p ) {
				p = pm.getParameterOf(TRi::RI_PW);
				if ( p ) {
					if ( p->getComponents() == 4 && p->getBasicType() == TYPE_FLOAT && p->getClass() == CLASS_VERTEX ) {
						IndexType num = p->getNumberOfElements();
						RtFloat *ptr = (RtFloat *)(p->getValue());
						components = 4;
						for ( int i=0; i<16; ++i ) {
							if ( cubicIdx[i] >= num ) {
								p = NULL;
								break;
							}
							pts[i*3+0] = ptr[cubicIdx[i]*4+0];
							pts[i*3+1] = ptr[cubicIdx[i]*4+1];
							pts[i*3+2] = ptr[cubicIdx[i]*4+2];
							pts[i*3+3] = ptr[cubicIdx[i]*4+3];
						}
						cardinality = p->getCardinality();
					} else {
						p = NULL;
					}
				}
			}
#endif

			if ( !p ) {
				p = pm.getParameterOf(TRi::RI_PZ);
				if ( p ) {
					if ( p->getComponents() == 1 && p->getBasicType() == TYPE_FLOAT && p->getClass() == CLASS_VERTEX ) {
						RtFloat *ptr = (RtFloat *)(p->getValue());
						RtFloat u = 0.0, v = 0.0;
						RtFloat delta = (RtFloat)(1.0/16.0);
						IndexType num = p->getNumberOfElements();
						for ( int j=0; j<16; ++j, v+=delta ) {
							if ( j == 15 )
								v = 1.0;
							for ( int i=0; i<16; ++i, u+=delta ) {
								if ( cubicIdx[i] >= num ) {
									p = NULL;
									break;
								}
								if ( i == 15 )
									u = 1.0;
								pts[i*3+0] = u;
								pts[i*3+1] = v;
								pts[i*3+2] = ptr[cubicIdx[i]];
							}
						}
						cardinality = p->getCardinality();
					} else {
						p = NULL;
					}
				}
			}

			// p != NULL if vertex data is copied to pts

			if ( p ) {
				IndexType idx[16] =
				{
					0,  1,  2,  3,
					4,  5,  6,  7,
					8,  9, 10, 11,
					12, 13, 14, 15
				};

				uv.bicubicBlendNormals(idx, TRi::RI_N, components, cardinality, pts, *m_normalStrip, true);
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
					  RtInt pnu,
					  RtInt pnv,
					  const TNuBasis &uBasis,
					  const TNuBasis &vBasis
					  ) const
{
	nuBlend(
		source.getParameterName(),
		source.getComponents(),
		source.getCardinality(),
		(const RtFloat *)source.getValue(),
		dest,
		pnu, pnv,
		uBasis, vBasis,
		true
		);
}


void TNuMesh::nuBlend(
					  const char *name,
					  IndexType components,
					  IndexType cardinality,
					  const RtFloat *source,
					  TNamedVector<RtFloat> &dest,
					  RtInt pnu,
					  RtInt pnv,
					  const TNuBasis &uBasis,
					  const TNuBasis &vBasis,
					  bool clearAndReserve) const
{
	IndexType numElems = nu()*nv();
	if ( clearAndReserve ) {
		dest.clear();
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

	IndexType numElementsSource = components * cardinality;

	std::vector<RtFloat> temp;
	temp.resize(numElementsSource * pnu);

	IndexType i, j, k, u, v;

	RtFloat vBaseElem, uBaseElem, comp;

	for ( v = 0; v < nv(); ++v ) {
		for ( i = 0; i < (IndexType)pnu; ++i ) {
			for ( j = 0; j < numElementsSource; ++j ) {
				temp[i*numElementsSource + j] = 0;
			}
			for ( k = 0; k < (IndexType)pnv; ++k ) {
				vBaseElem = vBasis[(IndexType)(pnv*v+k)];
				for ( j = 0; j < numElementsSource; ++j ) {
					comp = s[k*pnu*numElementsSource + i*numElementsSource + j];
					temp[i*numElementsSource + j] +=
						comp * vBaseElem;
				}
			}
		}

		for ( u = 0; u < nu(); ++u, ptr += numElementsSource ) {
			for ( j = 0; j < numElementsSource; ++j ) {
				*(ptr+j) = 0;
			}
			for ( k = 0; k < (IndexType)pnu; ++k ) {
				uBaseElem = uBasis[(IndexType)(u*pnu+k)];
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
							 RtInt pnu,
							 RtInt pnv,
							 const TNuBasis &uBasis,
							 const TNuBasis &vBasis,
							 const TNuBasis &duBasis,
							 const TNuBasis &dvBasis
							 ) const
{
	nuBlendNormals(
		source.getParameterName(),
		source.getComponents(),
		source.getCardinality(),
		(const RtFloat *)source.getValue(),
		dest,
		pnu, pnv,
		uBasis, vBasis,
		duBasis, dvBasis,
		true
		);
}


void TNuMesh::nuBlendNormals(
							 const char *name,
							 IndexType components,
							 IndexType cardinality,
							 const RtFloat *source,
							 TNamedVector<RtFloat> &dest,
							 RtInt pnu,
							 RtInt pnv,
							 const TNuBasis &uBasis,
							 const TNuBasis &vBasis,
							 const TNuBasis &duBasis,
							 const TNuBasis &dvBasis,
							 bool clearAndReserve) const
{
	if ( components != 3 && cardinality != 1 )
		return;

	const IndexType numComponents = components * cardinality;
	IndexType numElems = nu()*nv();
	if ( clearAndReserve ) {
		dest.clear();
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

	RtFloat *ptr = NULL;
	ptr = dest.begin();
	if ( !ptr )
		return;

	IndexType numElementsSource = components * cardinality;

	std::vector<RtFloat> temp;
	temp.resize(numElementsSource * pnu);

	std::vector<RtFloat> ds;
	ds.resize(numElementsSource);

	IndexType i, j, k, u, v;

	RtFloat vBaseElem, uBaseElem, comp;

	for ( v = 0; v < nv(); ++v ) {
		for ( i = 0; i < (IndexType)pnu; ++i ) {
			for ( j = 0; j < numElementsSource; ++j ) {
				temp[i*numElementsSource + j] = 0;
			}
			for ( k = 0; k < (IndexType)pnv; ++k ) {
				vBaseElem = vBasis[(IndexType)(pnv*v+k)]; // ***
				for ( j = 0; j < numElementsSource; ++j ) {
					comp = s[k*pnu*numElementsSource + i*numElementsSource + j];
					temp[i*numElementsSource + j] +=
						comp * vBaseElem;
				}
			}
		}

		for ( u = 0; u < nu(); ++u, ptr += numElementsSource ) {
			for ( j = 0; j < numElementsSource; ++j ) {
				*(ptr+j) = 0;
			}
			for ( k = 0; k < (IndexType)pnu; ++k ) {
				uBaseElem = duBasis[(IndexType)(pnu*u+k)];
				for ( j = 0; j < numElementsSource; ++j ) {
					comp = temp[k*numElementsSource + j];
					*(ptr+j) += comp * uBaseElem;
				}
			}
		}
	}


	temp.resize(numElementsSource * pnv);
	RtInt cnt = 0;

	for ( u = 0; u < nu(); ++u ) {
		for ( i = 0; i < (IndexType)pnv; ++i ) {
			for ( j = 0; j < numElementsSource; ++j ) {
				temp[i*numElementsSource + j] = 0;
			}
			for ( k = 0; k < (IndexType)pnu; ++k ) {
				uBaseElem = uBasis[(IndexType)(pnu*u+k)]; // ***
				for ( j = 0; j < numElementsSource; ++j ) {
					comp = s[k*pnv*numElementsSource + i*numElementsSource + j];
					temp[i*numElementsSource + j] +=
						comp * uBaseElem;
				}
			}
		}
		for ( v = 0, cnt = u * numElementsSource; v < nv(); ++v, cnt += nu() * numElementsSource ) {
			for ( j = 0; j < numElementsSource; ++j ) {
				ds[j] = 0;
			}
			for ( k = 0; k < (IndexType)pnv; ++k ) {
				vBaseElem = dvBasis[(IndexType)(pnv*v+k)];
				for ( j = 0; j < numElementsSource; ++j ) {
					comp = temp[k*numElementsSource + j];
					ds[j] += comp * vBaseElem;
				}
			}

			planeNorm(&(ds[0]), &(dest[cnt]), &(dest[cnt]));
		}
	}
}


void TNuMesh::insertVertexData(
							   const TParameterMap &pm, RtInt pnu, RtInt pnv, IndexType pTessU, IndexType pTessV,
							   const TNuBasis &uBasis, const TNuBasis &vBasis,
							   const TNuBasis &duBasis, const TNuBasis &dvBasis
							   )
{
	tessU(pTessU);
	tessV(pTessV);

	//	TUVPatch::insertVertexData(pm, nu(), nv());

	IndexType n = (IndexType)pnu * (IndexType)pnv;
	IndexType size = (IndexType)pm.getSize();

	IndexType i;
	for ( i=0; i<size; ++i ) {
		const TParameter *p = pm.getParameterOf(i);
		if ( p ) {
			if ( p->getClass() == CLASS_VERTEX && p->getBasicType() == TYPE_FLOAT && p->getSize() == n ) {
				m_vertexData.resize(m_vertexData.size()+1);
				TNamedVector<RtFloat> &v = m_vertexData.back();
				nuBlend(*p, v, pnu, pnv, uBasis, vBasis);
			}
		}
	}


	// Insert P if not present
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
					*(dest++) = src[0];
					*(dest++) = src[1];
					*(dest++) = src[2];
				}
			}
		} else {
			// P_Z ?
			m_freeVertexStrip = true;
			p = findElementVector(TRi::RI_PZ, m_vertexData);
			if ( p && p->components() == 1 && p->cardinality() == 1 ) {
				m_vertexStrip = new TNamedVector<RtFloat>;
				if ( !m_vertexStrip )
					throw TSystemException(__FILE__, __LINE__);
				m_vertexStrip->initialize(TRi::RI_P, 3, 1, p->numElems());
				RtFloat *src, *dest;
				for ( src = p->begin(), dest = m_vertexStrip->begin(); src < p->end(); src++ ) {
					*(dest++) = 0.0;
					*(dest++) = 0.0;
					*(dest++) = src[0];
				}
			}
		}
	}

	// Insert normal data
#if 0
	p = findElementVector(TRi::RI_N, m_vertexData);
	if ( p ) {
		m_freeNormalStrip = false;
		m_normalStrip = p;
	} else {
		const TParameter *param = pm.getParameterOf(TRi::RI_N);
		IndexType idx = (IndexType)0;
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
			m_normalStrip = new TNamedVector<RtFloat>;
			if ( !m_normalStrip )
				throw TSystemException(__FILE__, __LINE__);

			const TParameter *p = pm.getParameterOf(TRi::RI_P);
			if ( p ) {
				if ( p->getComponents() == 3 && p->getBasicType() == TYPE_FLOAT && p->getClass() == CLASS_VERTEX ) {
					RtFloat *ptr = (RtFloat *)(p->getValue());
					IndexType num = p->getNumberOfElements();
					nuBlendNormals(*p, *m_normalStrip, pnu, pnv, uBasis, vBasis, duBasis, dvBasis);
				} else {
					p = NULL;
				}
			}

			if ( !p ) {
				p = pm.getParameterOf(TRi::RI_PW);
				if ( p ) {
					if ( p->getComponents() == 4 && p->getBasicType() == TYPE_FLOAT && p->getClass() == CLASS_VERTEX ) {
						IndexType num = p->getNumberOfElements();
						RtFloat *ptr = (RtFloat *)(p->getValue());
						TNamedVector<RtFloat> *normalStrip = new TNamedVector<RtFloat>(TRi::RI_N, 3, 1, num);
						if ( normalStrip && ptr ) {
							RtFloat *src, *dest;
							for ( src = ptr, dest = normalStrip->begin(); dest+2 < normalStrip->end(); src += 4 ) {
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
							nuBlendNormals(TRi::RI_N, 3, 1, normalStrip->begin(), *m_normalStrip, pnu, pnv, uBasis, vBasis, duBasis, dvBasis, true);
						}
						delete normalStrip;
					} else {
						p = NULL;
					}
				}
			}

			if ( !p ) {
				p = pm.getParameterOf(TRi::RI_PZ);
				if ( p ) {
					if ( p->getComponents() == 1 && p->getBasicType() == TYPE_FLOAT && p->getClass() == CLASS_VERTEX ) {
						RtFloat *ptr = (RtFloat *)(p->getValue());
						IndexType num = p->getNumberOfElements();
					} else {
						p = NULL;
					}
				}
			}

			/*
			RtFloat *np = m_normalStrip->begin();
			for ( ; np < m_normalStrip->end(); ++np ) {
			*np *= (RtFloat)-1.0;
			}
			*/
		}
	}
#endif

	insertVertexIndexData();
	insertNormalData();
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
// TNuPatch

bool TNuPatch::calcBasis(IndexType tessU, IndexType tessV) {
	bool change=false;
	if ( m_tessU != tessU ) {
		m_tessU = tessU;
		m_uBasis.calc(tessU, m_umin, m_umax, m_uknot, false);
		m_duBasis.calc(tessU, m_umin, m_umax, m_uknot, true);
		change = true;
	}
	if ( m_tessV != tessV ) {
		m_tessV = tessV;
		m_vBasis.calc(tessV, m_vmin, m_vmax, m_vknot, false);
		m_dvBasis.calc(tessV, m_vmin, m_vmax, m_vknot, true);
		change = true;
	}
	return change;
}


TNuMesh &TNuPatch::tesselate(IndexType tessU, IndexType tessV) {
	bool change = calcBasis(tessU, tessV);
	if ( change ) {
		m_mesh.insertVertexData(m_parameters, m_nu, m_nv, tessU, tessV, m_uBasis, m_vBasis, m_duBasis, m_dvBasis);
	}
	return m_mesh;
}
