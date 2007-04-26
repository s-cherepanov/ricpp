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

/*! \file polygons.cpp
 *  \brief Implementation of the classes for polygon tesselation
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "polygons.h"

// ============================================================================
// TPolygonMesh

// ----------------------------------------------------------------------------
void TPolygonMesh::insertParameterData(const TParameterMap &pm, RtInt nvertices) {
	IndexType size = (IndexType)pm.getSize();
	IndexType i;
	for ( i=0; i<size; ++i ) {
		const TParameter *p = pm.getParameterOf(i);
		if ( p ) {
			if ( p->getBasicType() == TYPE_FLOAT && p->getSize() >= (IndexType)nvertices ) {
				// CLASS_(FACE)VARYING and CLASS_(FACE)VERTEX, handled in the same way in simple polygons
				if ( p->getClass() == CLASS_VARYING ||
				     p->getClass() == CLASS_VERTEX  ||
				     p->getClass() == CLASS_FACEVARYING ||
				     p->getClass() == CLASS_FACEVERTEX )
				{
					m_vertexData.resize(m_vertexData.size()+1);
					p->referNamedVectorTo(m_vertexData.back());
				}
			}
		}
	}
}


// ----------------------------------------------------------------------------
void TPolygonMesh::insertParameterData(const TParameterMap &pm, RtInt nvertices, RtInt sumvertices, RtInt offs, std::vector<RtInt> &verts, bool hasFaceParameters) {
	IndexType size = (IndexType)pm.getSize();
	IndexType i;
	for ( i=0; i<size; ++i ) {
		const TParameter *p = pm.getParameterOf(i);
		if ( p ) {
			if ( p->getBasicType() == TYPE_FLOAT && p->getSize() >= (IndexType)nvertices ) {
				// CLASS_VARYING and CLASS_VERTEX, handled in the same way in simple points polygons
				if ( p->getClass() == CLASS_VARYING ||
				     p->getClass() == CLASS_VERTEX )
				{
					if ( hasFaceParameters ) {
						// Copy data
						RtFloat *ptr = (RtFloat *)(p->getValue());
						m_vertexData.resize(m_vertexData.size()+1);
						TNamedVector<RtFloat> &dataDest = m_vertexData.back();
						dataDest.initialize(p->getName(), p->getComponents(), p->getCardinality(), nvertices);
						RtFloat *dest = dataDest.begin();
						IndexType i, j, cnt, elems;
						elems = p->getComponents() * p->getCardinality();
						for ( i = 0, cnt = 0; i < (IndexType)nvertices; ++i ) {
							for ( j = 0; j < elems; ++j, ++cnt ) {
								dest[cnt] = ptr[verts[offs+i]*elems+j];
							}
						}
					} else {
						// Insert reference
						m_vertexData.resize(m_vertexData.size()+1);
						p->referNamedVectorTo(m_vertexData.back());
					}
				}
				// CLASS_FACEVARYING and CLASS_FACEVERTEX, handled in the same way in simple points polygons
				if ( p->getClass() == CLASS_FACEVARYING ||
				     p->getClass() == CLASS_FACEVERTEX )
				{
					// Insert reference
					m_vertexData.resize(m_vertexData.size()+1);
					p->referNamedVectorTo(m_vertexData.back());
				}
			}
		}
	}
}


// ----------------------------------------------------------------------------
const RtFloat *TPolygonMesh::getTextureST(IndexType &size, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) {
	const RtFloat *pst = TPrimFace::getTextureST(size, s1, t1, s2, t2, s3, t3, s4, t4);
	if ( pst != NULL ) {
		return pst;
	}

	// If there are no vertices, there are no texture coordinates also
	if ( m_vertexStrip == NULL ) {
		size = 0;
		return NULL;
	}

	m_stStrip = new TNamedVector<RtFloat>(TRi::RI_ST, 2, 1, (m_vertexStrip->size()/3)*2);
	if ( m_stStrip != NULL ) {
		RtFloat *to = m_stStrip->begin();
		RtFloat *from = m_vertexStrip->begin();
		for ( ; from < m_vertexStrip->end();  ) {
			// x
			if ( !m_useUniformS ) {
				*to = *from;
			} else {
				*to = m_uniformS;
			}
			++to;
			++from;

			// y
			if ( !m_useUniformT ) {
				*to = *from;
			} else {
				*to = m_uniformT;
			}
			++to;
			++from;

			// z is not used
			++from;
		}
	} else {
		// ERROR
		size = 0;
		return NULL;
	}
	m_freeSTStrip = true;

	const TNamedVector<RtFloat> *data = getInterpolatedData(TRi::RI_S);
	if ( data ) {
		if ( data->size() != m_stStrip->size()/2 ) {
			data = NULL;
		} else {
			RtFloat *to = m_stStrip->begin();
			RtFloat *from = data->begin();
			for ( ; from != data->end(); ++from ) {
				*to = *from;
				to += 2;
			}
		}
	}

	data = getInterpolatedData(TRi::RI_T);
	if ( data ) {
		if ( data->size() != m_stStrip->size()/2 ) {
			data = NULL;
		} else {
			RtFloat *to = m_stStrip->begin();
			++to;
			RtFloat *from = data->begin();
			for ( ; from != data->end(); ++from ) {
				*to = *from;
				to += 2;
			}
		}
	}

	size = m_stStrip->size();
	return m_stStrip->begin();
}


// ----------------------------------------------------------------------------
void TPolygonMesh::insertVertexData(const TParameterMap &pm, RtInt nvertices) {
	clearStrips();
	if ( nvertices <= 0 )
		return;

	insertParameterData(pm, nvertices);

	// Insert vertex data
	TNamedVector<RtFloat> *p = findElementVector(TRi::RI_P, m_vertexData);
	if ( p ) {
		m_freeVertexStrip = false;
		m_vertexStrip = p;
	} else {
		return;
	}

	p = findElementVector(TRi::RI_N, m_vertexData);
	if ( p ) {
		m_freeNormalStrip = false;
		m_normalStrip = p;
	} else {
		const TParameter *n = pm.getParameterOf(TRi::RI_N);
		if ( n &&
			n->getBasicType() == TYPE_FLOAT &&
			(n->getComponents() * n->getCardinality()) == 1 &&
			(n->getClass() == CLASS_UNIFORM || n->getClass() == CLASS_CONSTANT) && 
			n->getSize() == 1 )
		{
			RtFloat *ptr = (RtFloat *)(n->getValue());
			m_useUniformNormal = true;
			m_uniformNormal[0] = ptr[0];
			m_uniformNormal[1] = ptr[1];
			m_uniformNormal[2] = ptr[2];
		} else {
			m_uniformNormal[0] = m_uniformNormal[1] = m_uniformNormal[2] = 0.0;
			RtFloat *ptr = m_vertexStrip->begin();
			if ( nvertices >= 3 ) {
				planeNorm(ptr+3, ptr, ptr+6, &(m_uniformNormal[0]));
			}
			m_useUniformNormal = true;
		}
	}

	// Insert the Indicees

	m_vertexIndices.clear();
	m_vertexIndices.resize(1);

	RtInt i;
	TGeometricIndices &gi = m_vertexIndices.back();
	std::vector<TVertexIndexType> &v = gi.m_indices;
	v.clear();

	/*

	gi.type(TGeometricIndices::polygon);
	v.resize(nvertices);
	for ( i = 0; i < nvertices; ++i ) {
		v[i]=i;
	}

	return;
	
	*/

	gi.type(TGeometricIndices::strip);
	if ( nvertices < 3 ) {
		v.resize(3);
	} else {
		v.resize(nvertices);
	}

	v[0] = 0;

	if ( nvertices == 1 ) {
		v[1] = 0;
		v[2] = 0;
	} else if ( nvertices == 2 ) {
		v[1] = 1;
		v[2] = 0;
	} else {
		RtInt startIdx = 0;
		RtInt endIdx = nvertices;
		RtInt j;
		for ( i=1, j=1; i<nvertices; ++j ) {
			v[i] = startIdx+j;
			++i;
			if ( i < nvertices ) {
				v[i] = endIdx-j;
				++i;
			}
		}
	}
}


// ----------------------------------------------------------------------------
void TPolygonMesh::insertVertexData(const TParameterMap &pm, RtInt nvertices, RtInt sumvertices, RtInt offs, std::vector<RtInt> &verts, bool hasFaceParameters) {
	clearStrips();
	if ( nvertices <= 0 )
		return;

	insertParameterData(pm, nvertices, sumvertices, offs, verts, hasFaceParameters);

	// Copy vertex data (components 3, cardinality 1, size >= sumvertices, or >nvertices (if face)
	TNamedVector<RtFloat> *p = findElementVector(TRi::RI_P, m_vertexData);
	if ( p &&
		 (p->components() * p->cardinality()) == 3 &&
		 ( (hasFaceParameters && (p->size() >= (IndexType)nvertices)) ||
		   (p->size() >= (IndexType)sumvertices) ) )
	{
		m_freeVertexStrip = false;
		m_vertexStrip = p;
	} else {
		return;
	}

	// Normals
	p = findElementVector(TRi::RI_N, m_vertexData);
	if ( p &&
		 (p->components() * p->cardinality()) == 3 &&
		 ( (hasFaceParameters && (p->size() >= (IndexType)nvertices)) ||
		   (p->size() >= (IndexType)sumvertices) ) )
	{
		m_freeNormalStrip = false;
		m_normalStrip = p;
	} else {
		const TParameter *n = pm.getParameterOf(TRi::RI_N);
		if ( n &&
			n->getBasicType() == TYPE_FLOAT &&
			(n->getComponents() * n->getCardinality()) == 1 &&
			( (n->getClass() == CLASS_UNIFORM && n->getSize() == m_numFaces) ||
			  (n->getClass() == CLASS_CONSTANT && n->getSize() == 1) ) )
		{
			RtFloat *ptr = (RtFloat *)(n->getValue());
			if ( n->getSize() == m_numFaces ) {
				m_useUniformNormal = true;
				m_uniformNormal[0] = ptr[m_faceIdx*3+0];
				m_uniformNormal[1] = ptr[m_faceIdx*3+1];
				m_uniformNormal[2] = ptr[m_faceIdx*3+2];
			} else {
				m_useUniformNormal = true;
				m_uniformNormal[0] = ptr[0];
				m_uniformNormal[1] = ptr[1];
				m_uniformNormal[2] = ptr[2];
			}
		} else {
			RtFloat *ptr = m_vertexStrip->begin();
			m_uniformNormal[0] = m_uniformNormal[1] = m_uniformNormal[2] = 0.0;
			if ( nvertices >= 3 ) {
				if ( hasFaceParameters ) {
					planeNorm(ptr+3, ptr, ptr+6, &(m_uniformNormal[0]));
				} else {
					planeNorm(ptr+(verts[offs+1]*3), ptr+(verts[offs+0]*3), ptr+(verts[offs+2]*3), &(m_uniformNormal[0]));
				}
			}
			m_useUniformNormal = true;
		}
	}

	// Insert the Indicees
	m_vertexIndices.clear();
	m_vertexIndices.resize(1);

	TGeometricIndices &gi = m_vertexIndices.back();
	std::vector<TVertexIndexType> &v = gi.m_indices;
	v.clear();

	/*

	gi.type(TGeometricIndices::polygon);
	v.resize(nvertices);
	for ( RtInt i = 0; i < nvertices; ++i ) {
		if ( hasFaceParameters ) {
			v[i]=i;
		} else {
			v[i] = verts[offs+i];
		}
	}

	return;
	
	*/


	gi.type(TGeometricIndices::strip);

	if ( nvertices < 3 ) {
		v.resize(3);
	} else {
		v.resize(nvertices);
	}

	if ( hasFaceParameters ) {
		v[0] = 0;
	} else {
		v[0] = verts[offs+0];
	}

	if ( nvertices == 1 ) {
		if ( hasFaceParameters ) {
			v[1] = 0;
			v[2] = 0;
		} else {
			v[1] = verts[offs+0];;
			v[2] = verts[offs+0];;
		}
	} else if ( nvertices == 2 ) {
		if ( hasFaceParameters ) {
			v[1] = 1;
			v[2] = 0;
		} else {
			v[1] = verts[offs+1];
			v[2] = verts[offs+0];
		}
	} else {
		RtInt startIdx = 0;
		RtInt endIdx = nvertices;
		RtInt i, j;
		if ( hasFaceParameters ) {
			for ( i=1, j=1; i<nvertices; ++j ) {
				v[i] = startIdx+j;
				++i;
				if ( i < nvertices ) {
					v[i] = endIdx-j;
					++i;
				}
			}
		} else {
			for ( i=1, j=1; i<nvertices; ++j ) {
				v[i] = verts[offs+startIdx+j];
				++i;
				if ( i < nvertices ) {
					v[i] = verts[offs+endIdx-j];
					++i;
				}
			}
		}
	}
}


// ============================================================================
// TPolygon

// ----------------------------------------------------------------------------
TPolygonMesh &TPolygon::tesselate() {
	if ( m_polygonMesh.geometry() == NULL ) {
		m_polygonMesh.geometry(this);
	    m_polygonMesh.insertVertexData(m_parameters, m_nVertices);
	}
	return m_polygonMesh;
}


// ============================================================================
// TPointsPolygons

TPolygonMesh &TPointsPolygons::tesselate(RtInt poly) {
	if ( m_polygonMeshes.size() == 0 ) {
		RtInt i, offs, sumvertices;
		bool hasFaceParameters = false;

		sumvertices = max(m_verts.size(), &m_verts[0]) + 1;

		IndexType nParams = m_parameters.getSize();
		for ( IndexType n = 0; n < nParams; ++n ) {
			const TParameter *p = m_parameters.getParameterOf(n);
			if ( p && p->isFaceClass() ) {
				hasFaceParameters = true;
				break;
			}
		}

		m_polygonMeshes.resize(m_nPolys);
		for ( i=0, offs=0; i<m_nPolys; ++i ) {
			m_polygonMeshes[i].faceIdx(i);
			m_polygonMeshes[i].numFaces(m_nPolys);
			m_polygonMeshes[i].geometry(this);
			m_polygonMeshes[i].insertVertexData(m_parameters, m_nVerts[i], sumvertices, offs, m_verts, hasFaceParameters);
			offs += m_nVerts[i];
		}
	}

	return m_polygonMeshes[poly];
}
