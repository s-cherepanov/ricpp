#ifndef _RICPP_RENDERCONTEXT_SURFACES_H
#define _RICPP_RENDERCONTEXT_SURFACES_H

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

/*! \file surfaces.h
 *  \brief Declaration of the base classes that contain geometric data
 *         (meshes) for different tesselations.
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "rimacro.h"

#include <vector>
#include <deque>


//! Base class for geometric primaries (a face)
class TPrimFace : public TGeometricPrimFace {

public:
	TNamedVector<RtFloat> *m_vertexStrip;       //!< The vertex points 3D
	bool m_freeVertexStrip;                     //!< Free vertex strip at destruction
	TNamedVector<RtFloat> *m_normalStrip;       //!< The normal vectors 3D
	bool m_freeNormalStrip;                     //!< Free normal strip at destruction
	TNamedVector<RtFloat> m_invNormalStrip;     //!< The inverted normal vectors
	bool m_useUniformNormal;                    //!< Use the uniform normal instead of the strip
	RtPoint m_uniformNormal;                    //!< Uniform normal
	bool m_setInvUniformNormal;                 //!< Use the uniform normal instead of the strip
	RtPoint m_invUniformNormal;                 //!< Inverted uniform normal
	const TNamedVector<RtFloat> *m_stStrip;     //!< The default texture coordinates
	bool m_freeSTStrip;                         //!< Free texture strip at destruction
	RtFloat m_uniformS;                         //!< Uniform or constant texture coordinate S 
	RtFloat m_uniformT;                         //!< Uniform or constant texture coordinate T
	bool m_useUniformS;                         //!< Use Uniform or constant texture coordinate S
	bool m_useUniformT;                         //!< Use Uniform or constant texture coordinate T

	std::deque<TGeometricIndices> m_vertexIndices; //!< Indices of the vertices. Type is int because that's common for OpenGL.

	IndexType m_faceIdx;                        //!< Index number of the face in the primary
	IndexType m_faceIdxU;                       //!< Index number of the face in the primary U
	IndexType m_faceIdxV;                       //!< Index number of the face in the primary V
	IndexType m_numFaces;                       //!< Number of the faces in the primary
	IndexType m_numFacesU;                      //!< Number of the faces in the primary U
	IndexType m_numFacesV;                      //!< Number of the faces in the primary V
	TGeometryRInterfaceCall *m_geometry;        //!< Pointer for uniform data

	inline void clearStrips() {
		if ( m_freeVertexStrip && m_vertexStrip )
			delete m_vertexStrip;
		m_vertexStrip = NULL;
		m_freeVertexStrip = false;
		if ( m_freeNormalStrip && m_normalStrip )
			delete m_normalStrip;
		m_normalStrip = NULL;
		m_freeNormalStrip = false;
		m_vertexIndices.clear();
		m_invNormalStrip.clear();
		if ( m_freeSTStrip && m_stStrip )
			delete m_stStrip;
		m_stStrip = NULL;
		m_freeSTStrip = false;
	}

	inline void newStrips() {
		clearStrips();
		m_freeVertexStrip = true;
		m_freeNormalStrip = true;
		m_vertexStrip = new TNamedVector<RtFloat>;
		m_normalStrip = new TNamedVector<RtFloat>;
		if ( m_vertexStrip == NULL || m_normalStrip == NULL ) {
			throw TSystemException(__FILE__, __LINE__);
		}
	}

public:
	std::deque< TNamedVector<RtFloat> > m_vertexData;      //!< Float data for each vertex, interpolated (either vertex, facevertex, varying, facevarying).

public:
	//! Standard constructor, sets all to defaults.
	inline TPrimFace() :
		m_vertexStrip(NULL), m_freeVertexStrip(false),
		m_normalStrip(NULL), m_freeNormalStrip(false),
		m_useUniformNormal(false), m_setInvUniformNormal(false),
		m_stStrip(NULL), m_freeSTStrip(false),
		m_useUniformS(false), m_useUniformT(false),
		m_faceIdx(0), m_faceIdxU(0), m_faceIdxV(0),
		m_numFaces(1), m_numFacesU(1), m_numFacesV(1), m_geometry(NULL)
	{
	}
	//! Copy constructor, sets all to the value of prim.
	/*! \param prim values to copy from.
	 */
	inline TPrimFace(const TPrimFace &prim) :
		m_vertexStrip(NULL), m_freeVertexStrip(false),
		m_normalStrip(NULL), m_freeNormalStrip(false),
		m_useUniformNormal(false), m_setInvUniformNormal(false),
		m_stStrip(NULL), m_freeSTStrip(false),
		m_useUniformS(false), m_useUniformT(false),
		m_faceIdx(0), m_faceIdxU(0), m_faceIdxV(0),
		m_numFaces(1), m_numFacesU(1), m_numFacesV(1), m_geometry(NULL)
	{
		*this = prim;
	}
	//! Standard destructor.
	inline virtual ~TPrimFace() {
		if ( m_freeVertexStrip && m_vertexStrip )
			delete m_vertexStrip;
		if ( m_freeNormalStrip && m_normalStrip )
			delete m_normalStrip;
		if ( m_freeSTStrip && m_stStrip )
			delete m_stStrip;
	}

	//! Duplication
	/*! \return A clone of *this
	 */
	inline virtual TPrimFace &duplicate() const {
		TPrimFace *retVal = new TPrimFace(*this);
		if ( !retVal ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *retVal;
	}

	//! Assignment operator
	/*! \param prim values to copy from.
	 *  \return *this
	 */
	TPrimFace &operator=(const TPrimFace &prim);

	// Interpolated
	virtual TNamedVector<RtFloat> *getInterpolatedData(const char *name);

	// Uniform or Constant
	virtual const TParameter *getUniformData(const char *name, RtPointer *value);

	inline virtual const RtFloat *getVertices() {
		return m_vertexStrip ? m_vertexStrip->begin() : NULL;
	}

	inline virtual const RtFloat *getNormals(IndexType &size, bool inverted=false) {
		if ( m_useUniformNormal ) {
			size = 3;
			if ( !inverted ) {
				return &(m_uniformNormal[0]);
			}
			if ( !m_setInvUniformNormal ) {
				m_invUniformNormal[0] = (RtFloat)-1.0 * m_uniformNormal[0];
				m_invUniformNormal[1] = (RtFloat)-1.0 * m_uniformNormal[1];
				m_invUniformNormal[2] = (RtFloat)-1.0 * m_uniformNormal[2];
				m_setInvUniformNormal = true;
			}
			return &(m_invUniformNormal[0]);
		}
		if ( !m_normalStrip ) {
			size = 0;
			return NULL;
		}
		if ( inverted && m_invNormalStrip.size() == 0 ) {
			m_invNormalStrip.initialize(m_normalStrip->name(), m_normalStrip->components(), m_normalStrip->cardinality(), m_normalStrip->numElems());
			RtFloat *src = m_normalStrip->begin();
			RtFloat *dest = m_invNormalStrip.begin();
			for ( ; src != m_normalStrip->end(); ++src, ++dest ) {
				*dest = (RtFloat)-1.0 * *src;
			}
		}
		size = m_normalStrip->size();
		return inverted ? m_invNormalStrip.begin() : m_normalStrip->begin();
	}

	inline virtual std::deque<TGeometricIndices> &getVertexIndices() {
		return m_vertexIndices;
	}

	virtual const RtFloat *getTextureST(IndexType &size, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);

	static void bilinearBlend(
		IndexType tessu,
		IndexType tessv,
		const IndexType (& cornerIdx)[4],
		const TParameter &source,
		TNamedVector<RtFloat> &dest,
	    IndexType startIdx = 0,
	    IndexType stride = 0,
		bool init = true
		);

	static void bilinearBlend(
		IndexType tessu,
		IndexType tessv,
		const IndexType (& cornerIdx)[4],
		IndexType components,
		const RtFloat *elemPtr,
		TNamedVector<RtFloat> &dest,
		const char *name,
		IndexType cardinality = 1,
		IndexType startIdx = 0,
	    IndexType stride = 0,
		bool init = true
		);

	inline TGeometryRInterfaceCall *geometry() const { return m_geometry; }
	inline TGeometryRInterfaceCall *geometry(TGeometryRInterfaceCall *g) {
		TGeometryRInterfaceCall *old = m_geometry;
		m_geometry = g;
		return old;
	}

	inline IndexType faceIdx() const { return m_faceIdx; }
	inline IndexType faceIdx(IndexType fi) {
		IndexType old = m_faceIdx;
		m_faceIdx = fi;
		return old;
	}

	inline IndexType faceIdxU() const { return m_faceIdxU; }
	inline IndexType faceIdxU(IndexType fiu) {
		IndexType old = m_faceIdxU;
		m_faceIdxU = fiu;
		return old;
	}

	inline IndexType faceIdxV() const { return m_faceIdxV; }
	inline IndexType faceIdxV(IndexType fiv) {
		IndexType old = m_faceIdxV;
		m_faceIdxV = fiv;
		return old;
	}

	inline IndexType numFaces() const { return m_numFaces; }
	inline IndexType numFaces(IndexType nf) {
		IndexType old = m_numFaces;
		m_numFaces = nf;
		return old;
	}

	inline IndexType numFacesU() const { return m_numFacesU; }
	inline IndexType numFacesU(IndexType nfu) {
		IndexType old = m_numFacesU;
		m_numFacesU = nfu;
		return old;
	}

	inline IndexType numFacesV() const { return m_numFacesV; }
	inline IndexType numFacesV(IndexType nfv) {
		IndexType old = m_numFacesV;
		m_numFacesV = nfv;
		return old;
	}
}; // class TPrimFace

#endif // _RICPP_RENDERCONTEXT_SURFACES_H
