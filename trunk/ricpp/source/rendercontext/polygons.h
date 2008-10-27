#ifndef _RICPP_RENDERCONTEXT_POLYGONS_H
#define _RICPP_RENDERCONTEXT_POLYGONS_H

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

/*! \file polygons.h
 *  \brief Declaration of the classes for polygon tesselation
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "surfaces.h"

class TPolygonMesh : public TPrimFace {
protected:
	virtual void insertParameterData(const TParameterMap &pm, RtInt nvertices);
	virtual void insertParameterData(const TParameterMap &pm, RtInt nvertices, RtInt sumvertices, RtInt offs, std::vector<RtInt> &verts, bool hasFaceParameters);
public:
	inline TPolygonMesh() {}
	inline TPolygonMesh(const TPolygonMesh &p) { *this = p; }
	inline virtual ~TPolygonMesh() { }

	//! Duplication
	/*! \return A clone of *this
	 */
	inline virtual TPrimFace &duplicate() const {
		TPrimFace *retVal = new TPolygonMesh(*this);
		if ( !retVal ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *retVal;
	}
	inline TPolygonMesh &operator=(const TPolygonMesh &mesh) {
		TPrimFace::operator=(mesh);
		return *this;
	}

	virtual const RtFloat *getTextureST(IndexType &size, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);

	virtual void insertVertexData(const TParameterMap &pm, RtInt nvertices);
	virtual void insertVertexData(const TParameterMap &pm, RtInt nvertices, RtInt sumvertices, RtInt offs, std::vector<RtInt> &verts, bool hasFaceParameters);
}; // TPolygonMesh


class TPolygon : public TRiPolygon {
	TPolygonMesh m_polygonMesh;
public:
	inline TPolygon(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt nvertices,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TRiPolygon(aLineNo, declmap, colorComps, nvertices, n, tokens, params)
	{
	}
	inline virtual ~TPolygon() {}


	//! Tesselation of the polygon.
	/*! \return The tesselated polygon
	 */
	TPolygonMesh &tesselate();

	inline virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
		if ( dp ) {
			dp->drawGeometricPrim(tesselate());
		} else {
			TRiPolygon::replay(ri);
		}
	}
}; // TPolygon


class TPointsPolygons : public TRiPointsPolygons {
	std::deque<TPolygonMesh> m_polygonMeshes;
public:
	inline TPointsPolygons(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt npolys, RtInt *nverts, RtInt *verts,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TRiPointsPolygons(aLineNo, declmap, colorComps, npolys, nverts, verts, n, tokens, params)
	{
	}
	inline virtual ~TPointsPolygons() {	}


	//! Tesselation of the polygon.
	/*! \return The tesselated polygon
	 */
	TPolygonMesh &tesselate(RtInt poly);

	inline virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
		if ( dp && m_nPolys > 0 ) {
			RtInt i;
			for ( i=0; i<m_nPolys; ++i ) {
				dp->drawGeometricPrim(tesselate(i));
			}
		} else {
			TRiPointsPolygons::replay(ri);
		}
	}
}; // TPointsPolygons

#endif // _RICPP_RENDERCONTEXT_POLYGONS_H
