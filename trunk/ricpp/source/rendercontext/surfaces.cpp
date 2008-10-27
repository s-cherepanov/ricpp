// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
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

/*! \file surfaces.cpp
 *  \brief Implementation of the base classes that contain
 *         geometric data (meshes) for different tesselations.
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "surfaces.h"

// ============================================================================
// TPrimFace

// ----------------------------------------------------------------------------
TPrimFace &TPrimFace::operator=(const TPrimFace &prim) {
	if ( &prim == this )
		return *this;

	if ( m_freeVertexStrip && m_vertexStrip ) {
		delete m_vertexStrip;
	}
	m_vertexStrip = NULL;

	if ( m_freeNormalStrip && m_normalStrip ) {
		delete m_normalStrip;
	}
	m_normalStrip = NULL;

	if ( m_freeSTStrip && m_stStrip ) {
		delete m_stStrip;
	}
	m_stStrip = NULL;

	m_vertexData = prim.m_vertexData;

	m_freeVertexStrip = prim.m_freeVertexStrip;
	if ( m_freeVertexStrip ) {
		if ( prim.m_vertexStrip )
			m_vertexStrip = new TNamedVector<RtFloat>(*prim.m_vertexStrip);
	} else {
		if ( prim.m_vertexStrip )
			m_vertexStrip = getInterpolatedData(TRi::RI_P);
	}

	m_freeNormalStrip = prim.m_freeNormalStrip;
	if ( m_freeNormalStrip ) {
		if ( prim.m_normalStrip )
			m_normalStrip = new TNamedVector<RtFloat>(*prim.m_normalStrip);
	} else {
		if ( prim.m_normalStrip )
			m_normalStrip = getInterpolatedData(TRi::RI_N);
	}

	m_freeSTStrip = prim.m_freeSTStrip;
	if ( m_freeSTStrip ) {
		if ( prim.m_stStrip )
			m_stStrip = new TNamedVector<RtFloat>(*prim.m_stStrip);
	} else {
		// Recalculate next time
		m_stStrip = NULL;
	}

	m_useUniformNormal = prim.m_useUniformNormal;
	memcpy(m_uniformNormal, prim.m_uniformNormal, sizeof(m_uniformNormal));
	m_setInvUniformNormal = prim.m_setInvUniformNormal;
	memcpy(m_invUniformNormal, prim.m_invUniformNormal, sizeof(m_invUniformNormal));

	m_uniformS = prim.m_uniformS;
	m_uniformT = prim.m_uniformT;
	m_useUniformS = prim.m_useUniformS;
	m_useUniformT = prim.m_useUniformT;

	m_invNormalStrip = prim.m_invNormalStrip;

	m_vertexIndices = prim.m_vertexIndices;

	m_geometry = prim.m_geometry;
	m_faceIdx = prim.m_faceIdx;
	m_faceIdxU = prim.m_faceIdxU;
	m_faceIdxV = prim.m_faceIdxV;
	m_numFaces = prim.m_numFaces;
	m_numFacesU = prim.m_numFacesU;
	m_numFacesV = prim.m_numFacesV;

	return *this;
}


// ----------------------------------------------------------------------------
TNamedVector<RtFloat> *TPrimFace::getInterpolatedData(const char *name) {
	return findElementVector(name, m_vertexData);
}


// ----------------------------------------------------------------------------
const TParameter *TPrimFace::getUniformData(const char *name, RtPointer *value) {
	if ( !m_geometry ) {
		return NULL;
	}

	const TParameter *p = m_geometry->getParameterOf(name);
	if ( p ) {
		RtInt aClass = p->getClass();
		if ( aClass == CLASS_UNIFORM ) {
			if ( value )
				*value = p->getValue(m_faceIdx);
		} else if ( aClass == CLASS_CONSTANT ) {
			if ( value )
				*value = p->getValue();
		} else {
			p = NULL;
		}
	}

	return p;
}


const RtFloat *TPrimFace::getTextureST(IndexType &size, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) {
	if ( m_stStrip != NULL ) {
		size = m_stStrip->size();
		return m_stStrip->begin();
	}

	m_useUniformS = false;
	m_useUniformT = false;
	m_freeSTStrip = false;

	// Interpolated ST
	m_stStrip = getInterpolatedData(TRi::RI_ST);
	if ( m_stStrip != NULL ) {
		size = m_stStrip->size();
		return m_stStrip->begin();
	}

	// Uniform or constant ST, S or T ?
	if ( m_geometry ) {
		const TParameter *p = m_geometry->getParameterOf(TRi::RI_S);
		if ( p && p->getValue() ) {
			if ( p->getClass() == CLASS_UNIFORM ) {
				const RtFloat *ps = (const RtFloat *)p->getValue(m_faceIdx);
				if ( ps ) {
					m_uniformS = *ps;
					m_useUniformS = true;
				}
			} else if ( p->getClass() == CLASS_CONSTANT ) {
				const RtFloat *ps = (const RtFloat *)p->getValue();
				if ( ps ) {
					m_uniformS = *ps;
					m_useUniformS = true;
				}
			}
		}
		
		p = m_geometry->getParameterOf(TRi::RI_T);
		if ( p ) {
			if ( p->getClass() == CLASS_UNIFORM ) {
				const RtFloat *pt = (const RtFloat *)p->getValue(m_faceIdx);
				if ( pt ) {
					m_uniformT = *pt;
					m_useUniformT = true;
				}
			} else if ( p->getClass() == CLASS_CONSTANT ) {
				const RtFloat *pt = (const RtFloat *)p->getValue();
				if ( pt ) {
					m_uniformT = *pt;
					m_useUniformT = true;
				}
			}
		}
	}

	if ( m_useUniformT && m_useUniformT ) {
		m_stStrip = new TNamedVector<RtFloat>(TRi::RI_ST, 2, 1, 1);
		if ( m_stStrip == NULL ) {
			size = 0;
			return NULL;
		}
		m_freeSTStrip = true;
		(*m_stStrip)[0] = m_uniformS;
		(*m_stStrip)[1] = m_uniformT;
		size = m_stStrip->size();
		return m_stStrip->begin();
	}
	return NULL;
}
// ----------------------------------------------------------------------------
// bilinearBlend
// Bilinear blending of a 2x2 patch, u major
// Index (idx)
// 0    1    u->
// 
// 2    3    
void TPrimFace::bilinearBlend(
	IndexType tessu,
	IndexType tessv,
	const IndexType (& cornerIdx)[4],
	const TParameter &source,
	TNamedVector<RtFloat> &dest,
	IndexType startIdx,
	IndexType stride,
	bool init)
{
	// Ignore if no floats
	if ( source.getBasicType() != TYPE_FLOAT )
		return;

	// For bilinear blending one needs 4 elems.
	IndexType sz = source.getSize();
	if ( sz <= cornerIdx[0] || sz <= cornerIdx[1] || sz <= cornerIdx[2] || sz <= cornerIdx[3] )
		return;

	bilinearBlend(
		tessu, tessv,
		cornerIdx,
		source.getComponents(),
		(RtFloat *)source.getValue(),
		dest,
		source.getParameterName(),
		source.getCardinality(),
		startIdx,
		stride,
		init);
}


// ----------------------------------------------------------------------------
void TPrimFace::bilinearBlend(
	IndexType tessu,
	IndexType tessv,
	const IndexType (& cornerIdx)[4],
	IndexType components,
	const RtFloat *elemPtr,
	TNamedVector<RtFloat> &dest,
	const char *name,
	IndexType cardinality,
	IndexType startIdx,
	IndexType stride,
	bool init)
{
	if ( !elemPtr )
		return;

	RtFloat deltau = (RtFloat)1.0/(RtFloat)(tessu);
	RtFloat deltav = (RtFloat)1.0/(RtFloat)(tessv);

	RtFloat   u, v;
	IndexType i;
	IndexType ui, vi;

	IndexType numElems = (tessu+1)*(tessv+1);

	if ( init )
		dest.initialize(name, components, cardinality, numElems);

	components *= cardinality;

	IndexType startpos, endpos;
	IndexType pos = startIdx;
	for ( v = (RtFloat)0.0, vi = 0; vi < tessv+1; vi++, v += deltav ) {
		startpos = startIdx +  vi * (tessu+1)          * components;
		endpos   = startIdx + (vi * (tessu+1) + tessu) * components;
		// The first element of a row
		for ( i = 0; i < components-stride; ++i ) {
			dest[startpos+i]  = elemPtr[cornerIdx[0]*components+i] * ((RtFloat)1.0-v) +
			                    elemPtr[cornerIdx[2]*components+i] * v;
		}
		// The last element of a row
		for ( i = 0; i < components-stride; ++i ) {
			dest[endpos+i]     = elemPtr[cornerIdx[1]*components+i] * ((RtFloat)1.0-v) +
								 elemPtr[cornerIdx[3]*components+i] * v;
		}
		// The 2nd to the 2nd last element of a row are bilinear blended
		pos += components;
		for ( u = (RtFloat)deltau, ui = 1; ui < tessu; ui++, u += deltau ) {
			for ( i = 0; i < components-stride; ++i, ++pos ) {
				dest[pos] =
					dest[startpos+i] * ((RtFloat)1.0-u) +
					dest[endpos+i]   * u;
			}
			pos += stride;
		}
		pos += components;
	}
}
