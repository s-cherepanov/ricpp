#ifndef _RICPP_BASERENDERER_PARAMCLASSES_H
#define _RICPP_BASERENDERER_PARAMCLASSES_H

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

/*! \file paramclasses.h
 *  \brief A bunch of objects that can be used to calculate the
 *         sizes of parameters for the (sl)classes:
 *         constant, uniform, varying, vertex, facevarying, facevertex
 *  \author Andreas Pidde (andreas@pidde.de)
 *
 *  The (sl)class determines how many values has to be in the parameterlist for
 *  a given variable, and how intermediate values are interpolated.
 *  (s.a. [The RenderMan Interface, Version 3.2, July, 2000]) facevertex and
 *  facevarying classes are added.
 *
 *  - constant has always one value, it stays constant
 *  - uniform has as many values as there are faces, the values stays uniform (constant) for each face
 *  - varying has as many values as corners, the values are linear interpolated
 *  - vertex has as many values as different vertices, the values are interpolated like points
 *  - facevarying has as many values as corners per face, linear interpolated and
 *  - facevertex has as many values as vertices per face, interpolated like points
 *
 * The number of corners is often the same as the number
 * of vertices, but not neccessarily, e.g. in a bicubic
 * patch there are 4 corners, but 16 vertices.
 *
 * If a primitive contains many faces with shared vertices (e.g. CRi::patchMesh()),
 * the number of facevarying and facevertex can be different to varying or
 * vertex.
 */

#ifndef _RICPP_RICPP_RICPPTOKENS_H
#include "ricpp/ricpp/ricpptokens.h"
#endif // _RICPP_RICPP_RICPPTOKENS_H

#ifndef _RICPP_TOOLS_TEMPLATEFUNCS_H
#include "ricpp/tools/templatefuncs.h"
#endif // _RICPP_TOOLS_TEMPLATEFUNCS_H

#include <algorithm>

namespace RiCPP {


//! CParameterClasses, base class for the parameter classes

/*! CParameterClasses contain the signature for all C...Classes.
    An instance of this class can be used for various interface calls,
    like shaders, lightsources, projection,
	this means all parameters have to be constant.
	CParameterClasses can be used directly for these interface calls:

	- IRi::projection()
	- IRi::imager()
	- IRi::display()
	- IRi::hider()
	- IRi::option()
	- IRi::lightSource()
	- IRi::areaLightSource()
	- IRi::attribute()
	- IRi::surface()
	- IRi::atmosphere()
	- IRi::interior()
	- IRi::exterior()
	- IRi::displacement()
	- IRi::deformation()
	- IRi::geometry()
	- IRi::makeTexture()
	- IRi::makeBump()
	- IRi::makeLatLongEnvironment()
	- IRi::makeCubeFaceEnvironment()
	- IRi::makeShadow()
	- IRi::readArchive()

  The derived classes returns the number of values for the different
  classes.
 */
class CParameterClasses {
public:
	//! constant() always returns 1
	inline virtual RtInt constant() const {return 1;}

	//! uniform() returns the number of faces
	inline virtual RtInt uniform() const {return 1;}

	//! varying() returns the number of corners
	inline virtual RtInt varying() const {return 1;}

	//! vertex() returns the number of vertices
	inline virtual RtInt vertex() const {return 1;}

	//! faceVarying() returns the number of corners, counted independently
	//! for each face.
	inline virtual RtInt faceVarying() const {return 1;}

	//! faceVertex() returns the number of corners, counted independently
	//! for each face.
	inline virtual RtInt faceVertex() const {return 1;}
}; // CParameterClasses


//! CQuadricClasses parameter classes for all quadrics.

/*! The classes alway have the sizes:

    - constant() == 1 (constant is always one)
    - uniform() == 1 (uniform is one, thats the number of faces)
    - varying() == 4 (always 4 corners in parametric space u,v: (0,0), (1,0), (0,1), (1,1))
    - vertex() == 4 (always 4 vertices, that are the 4 corners)
    - faceVarying() = 4 (same as varying, because there is always only one face)
	- faceVertex() = 4 (same as vertex, because there is always only one face)

	CQuadricParamClasses can be used for the following interface calls:

	- TRi::sphere()
	- TRi::cone()
	- TRi::cylinder()
	- TRi::hyperboloid()
	- TRi::paraboloid()
	- TRi::disk()
	- TRi::torus()
 */
class CQuadricClasses : public CParameterClasses {
public:
	//! uniform(), there is one face
	inline virtual RtInt uniform() const {return 1;}

	//! varying(), there are 4 corners.
	inline virtual RtInt varying() const {return 4;}

	//! vertex(), there are 4 vertices.
	inline virtual RtInt vertex() const {return 4;}

	//! faceVarying(), there is one face with 4 corners (same as corners).
	inline virtual RtInt faceVarying() const {return 4;}

	//! faceVertex(), there is one face with 4 vertices used (same as corners).
	inline virtual RtInt faceVertex() const {return 4;}
}; // CQuadricClasses


//! CPolygonClasses, parameter classes for TRi::polygon().

/*! There is always one convex polygon. The varying, vertex, facevarying,
	facevertex classes have the size of the number of vertices of the polygon.

    Remark: The sizes can be calculated with TPointsGeneralPolygons,
	but it's easier to use to have a different class.
 */
class CPolygonClasses : public CParameterClasses {
	RtInt m_nvertices;  //!< Sum of all vertices.
public:
	//! reset(), sets the number of vertices.
	inline void reset(RtInt nvertices) {m_nvertices = nvertices;}

    //! CPolygonClasses(), convex polygon with nvertices vertices.
	inline CPolygonClasses(RtInt nvertices) {reset (nvertices);}

	//! uniform(), there is one face
	inline virtual RtInt uniform() const {return 1;}

	//! varying(), the number of corners is the number of vertices.
	inline virtual RtInt varying() const {return m_nvertices;}

	//! vertex(), the number of vertices.
	inline virtual RtInt vertex() const {return m_nvertices;}

	//! faveVarying(), the number of corners (there is only one face).
	inline virtual RtInt faceVarying() const {return m_nvertices;}

	//! faceVertex(), the number of vertices (there is only one face).
	inline virtual RtInt faceVertex() const {return m_nvertices;}
}; // CPolygonClasses


//! CGeneralPolygonClasses, parameter classes for TRi::generalPolygon()

/*! There is always one polygon, evtually with holes. The varying, vertex,
	facevarying, facevertex classes have the size of the sum of
	all vertices of the general polygon.
 */
class CGeneralPolygonClasses : public CParameterClasses {
	RtInt m_nvertices;       //!< Sum of vertices.
public:
	//! reset() recalculates the sum of the vertices.
	inline void reset(RtInt nloops, RtInt *nverts) {
		m_nvertices = sum(nloops, nverts);
	}

	// CGeneralPolygonClasses(), a general polygon with holes.
	inline CGeneralPolygonClasses(RtInt nloops, RtInt *nverts) { reset(nloops, nverts); }

	//! uniform(), there is one face
	inline virtual RtInt uniform() const {return 1;}

	//! varying(), the number of corners is the sum of all vertices.
	inline virtual RtInt varying() const {return m_nvertices;}

	//! vertex(), sum of all vertices.
	inline virtual RtInt vertex() const {return m_nvertices;}

	//! varying(), the number of corners is the sum of all vertices, there is only one face.
	inline virtual RtInt faceVarying() const {return m_nvertices;}

	//! vertex(), sum of all vertices, there is only one face.
	inline virtual RtInt faceVertex() const {return m_nvertices;}
}; // CGeneralPolygonClasses


//! CPointsPolygonsClasses, parameter classes for TRi::pointsPolygons()

/*! There are many convex polygons.
 */
class CPointsPolygonsClasses : public CParameterClasses {
	RtInt m_npolys;     //!< Number of polygons.
	RtInt m_nvertices;  //!< Sum of all vertices per face.
	RtInt m_nmaxvertex; //!< Sum of all vertices, shared vertices count one.
public:
	//! reset() recalculates the sum of the vertices and the number of vertices.
	inline void reset(RtInt npolys, RtInt *nverts, RtInt *verts) {
		m_npolys = npolys;
		m_nvertices = sum(npolys, nverts);
		m_nmaxvertex = tmax(m_nvertices, verts);
		++m_nmaxvertex; // Index+1 == number of vertices
	}

    //! CPointsPolygonsClasses(), many convex polygons
	inline CPointsPolygonsClasses(RtInt npolys, RtInt *nverts, RtInt *verts) { reset(npolys, nverts, verts); }

	//! uniform(), the number of polygons.
	inline virtual RtInt uniform() const {return m_npolys;}

	//! varying(), the number of different corners (vertices), shared vertices count one.
	inline virtual RtInt varying() const {return m_nmaxvertex;}

	//! vertex(), the number of different vertices, shared vertices count one.
	inline virtual RtInt vertex() const {return m_nmaxvertex;}

	//! faceVarying(), the sum of corners (vertices) per face.
	inline virtual RtInt faceVarying() const {return m_nvertices;}

	//! faceVertex(), the sum of vertices per face.
	inline virtual RtInt faceVertex() const {return m_nvertices;}
}; // CPointsPolygonsClasses


//! CPointsGeneralPolygonsClasses, parameter classes for TRi::pointsGeneralPolygons()

/*! There are many faces (polygons) which can contain holes.
 */
class CPointsGeneralPolygonsClasses : public CParameterClasses {
	RtInt m_npolys;     //!< Number of polygons.
	RtInt m_nvertices;  //!< Sum of all vertices per face.
	RtInt m_nmaxvertex; //!< Sum of all vertices, shared vertices count one.
public:
	//! reset() recalculates the sum of the vertices and the number of vertices
	inline void reset(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts) {
		m_npolys = npolys;
		RtInt sumloops = sum(npolys, nloops);
		m_nvertices = sum(sumloops, nverts);
		m_nmaxvertex = tmax(m_nvertices, verts);
		++m_nmaxvertex; // Index+1 == number of vertices
	}

	//! TPointsPolygonsGeneralClasses(), many general polygons with holes.
	inline CPointsGeneralPolygonsClasses(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts) { reset(npolys, nloops, nverts, verts); }

	//! uniform(), the number of polygons.
	inline virtual RtInt uniform() const {return m_npolys;}

	//! varying(), the number of different corners (vertices), shared count one.
	inline virtual RtInt varying() const {return m_nmaxvertex;}

	//! vertex(), the number of different vertices, shared count one.
	inline virtual RtInt vertex() const {return m_nmaxvertex;}

	//! faceVarying(), the number of different corners (vertices), counted per face.
	inline virtual RtInt faceVarying() const {return m_nvertices;}

	//! faceVertex(), the number of different vertices, counted per face.
	inline virtual RtInt faceVertex() const {return m_nvertices;}
}; // CPointsGeneralPolygonsClasses


//! CPatchClasses, parameter classes for TRi::patch()

/*! There is one face (one TRi::patch()). Patches can be either
	bilinear with 4 vertices or bicubic with 16 vertices.
 */
class CPatchClasses : public CParameterClasses {
	RtInt nVertices; //!< The number of vertices is either 4 (linear patch) or 16 (bicubic patch)
public:
	//! reset() sets the number of vertices, they default to 4, only for a bicubic type there are 16
	inline void reset(RtToken type) {
		nVertices = !strcmp(type, RI_BICUBIC) ? 16 : 4;
	}

	//! CPatchClasses(), either bilinear or bicubic patches.
	inline CPatchClasses(RtToken type) { reset(type); }

	//! uniform(), there is one face
	inline virtual RtInt uniform() const {return 1;}

	//! varying(), there are 4 corners in the patch.
	inline virtual RtInt varying() const {return 4;}

	//! vertex(), there are 4 vertices in bilinear and 16 in bicubic patches.
	inline virtual RtInt vertex() const {return nVertices;}

	//! faceVarying(), returns 4, same as varying (there is only one face).
	inline virtual RtInt faceVarying() const {return 4;}

	//! faceVertex(), returns 4 or 16, same as vertex (there is only one face).
	inline virtual RtInt faceVertex() const {return nVertices;}
}; // CPatchClasses


//! CPatchMeshClasses, parameter class for TRi::patchMesh()

/*! There can be bilinear patches with 4 vertices per face or bicubic ones with
    16 vertices per face. The parameters nustep and nvstep at reset() and the
	constructor are obtained from the current attributes, set by the
	interface call of TRi::basis()
 */
class CPatchMeshClasses : public CParameterClasses {
	RtInt m_nPatches;      //!< Number of patches (faces)
	RtInt m_nCorners;      //!< Number of corners
	RtInt m_nVertices;     //!< Number of vertices
	RtInt m_nFaceCorners;  //!< Number of corners per face, m_nPatches * 4
	RtInt m_nFaceVertices; //!< Number of vertices per face, m_nPatches * 4 (bilinear) or m_nPatches * 16 (bicubic)
public:
	//! reset() sets the values of the member variables
	void reset(RtToken type, RtInt nu, RtInt nustep, RtToken uwrap, RtInt nv, RtInt nvstep, RtToken vwrap);

	//! CPatchMeshClasses(), either bilinear or bicubic patch meshes.
	// \sa reset()
	inline CPatchMeshClasses(
		RtToken type,
		RtInt nu, RtInt nustep, RtToken uwrap,
		RtInt nv, RtInt nvstep, RtToken vwrap)
	{
		reset(type, nu, nustep, uwrap, nv, nvstep, vwrap);
	}

	//! uniform(), returns the number of patches
	inline virtual RtInt uniform() const {return m_nPatches;}

	//! varying(), returns the number of corners, shared values count one
	inline virtual RtInt varying() const {return m_nCorners;}

	//! vertex(), returns the number of vertices, shared values count one
	inline virtual RtInt vertex() const {return m_nVertices;}

	//! faceVarying(), returns the number of corners per face, shared values count one
	inline virtual RtInt faceVarying() const {return m_nFaceCorners;}

	//! faceVertex(), returns the number of vertices per face, shared values count one
	inline virtual RtInt faceVertex() const {return m_nFaceVertices;}
}; // CPatchMeshClasses


//! CNuPatchClasses, parameter class for TRi::nuPatch()
class CNuPatchClasses : public CParameterClasses {
	RtInt m_nvertices;  //!< The number of vertices nu*nv
	RtInt m_nsegments;  //!< The number of patches
	RtInt m_ncorners;   //!< the number of corners
	RtInt m_nFaceCorners;  //!< Number of corners per face, 4 * m_nsegments
	RtInt m_nFaceVertices; //!< Number of vertices per face, m_nsegments * uorder * vorder
public:
	//! resets the number of vertices, segments and corners
	void reset(
		RtInt nu, RtInt uorder,
		RtInt nv, RtInt vorder);

	//! CNuPatchClasses() parameters
	//! \sa reset()
	inline CNuPatchClasses(
		RtInt nu, RtInt uorder,
		RtInt nv, RtInt vorder)
	{
		reset(nu, uorder, nv, vorder);
	}

	//! uniform(), returns the number of faces.
	inline virtual RtInt uniform() const {return m_nsegments;}

	//! varying(), returns the number of corners.
	inline virtual RtInt varying() const {return m_ncorners;}

	//! vertex(), returns the number of vartices.
	inline virtual RtInt vertex() const {return m_nvertices;}

	//! faceVarying(), returns the number of corners per face.
	inline virtual RtInt faceVarying() const {return m_nFaceCorners;}

	//! faceVertex(), returns the number of vertices per face
	inline virtual RtInt faceVertex() const {return m_nFaceVertices;}
}; // CNuPatchClasses


//! CSubdivisionMeshClasses, parameter class for TRi::subdivisionMesh()
class CSubdivisionMeshClasses : public CParameterClasses {
	RtInt m_nVerts;
	RtInt m_nFaces;
	RtInt m_nFaceCorners;
	RtInt m_nFaceVerts;
public:
	//! reset()
	void reset(RtInt nfaces, RtInt nvertices[], RtInt vertices[]);

	//! CSubdivisionMeshClasses()
	inline CSubdivisionMeshClasses(RtInt nfaces, RtInt nvertices[], RtInt vertices[]) { reset(nfaces, nvertices, vertices); }

	//! uniform(), returns the number of face.
	inline virtual RtInt uniform() const {return m_nFaces;}

	//! varying(), returns the number of corners.
	inline virtual RtInt varying() const {return m_nVerts;}

	//! vertex(), returns the number of vertices.
	inline virtual RtInt vertex() const {return m_nVerts;}

	//! faceVarying(), returns the number of corners per face.
	inline virtual RtInt faceVarying() const {return m_nFaceCorners;}

	//! faceVertex(), returns the number of vertices per face.
	inline virtual RtInt faceVertex() const {return m_nFaceVerts;}
}; // CSubdivisionMeshClasses


//! CPointsClasses, parameter class for TRi::points()
class CPointsClasses : public CParameterClasses {
	RtInt m_npts;
public:
	//! reset()
	inline void reset(RtInt npts) { m_npts = npts;}

	//! CPointsClasses()
	inline CPointsClasses(RtInt npts) { reset(npts); }

	//! uniform(), returns the number of face.
	inline virtual RtInt uniform() const {return 1;}

	//! varying(), returns the number of corners.
	inline virtual RtInt varying() const {return m_npts;}

	//! vertex(), returns the number of vertices.
	inline virtual RtInt vertex() const {return m_npts;}

	//! faceVarying(), returns the number of corners per face.
	inline virtual RtInt faceVarying() const {return m_npts;}

	//! faceVertex(), returns the number of vertices per face.
	inline virtual RtInt faceVertex() const {return m_npts;}
}; // CPointsClasses


//! CCurvesClasses, parameter class for TRi::curves()
class CCurvesClasses : public CParameterClasses {
	RtInt m_ncurves;
	RtInt m_sumvarying;
	RtInt m_sumverts;
	RtInt m_sumfacevarying;
	RtInt m_sumfaceverts;
public:
	//! reset()
	void reset(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt vstep);

	//! CCurvesClasses()
	inline CCurvesClasses(
		RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt vstep)
	{
		reset(type, ncurves, nverts, wrap, vstep);
	}

	//! uniform(), returns the number of face.
	inline virtual RtInt uniform() const {return m_ncurves;}

	//! varying(), returns the number of corners.
	inline virtual RtInt varying() const {return m_sumvarying;}

	//! vertex(), returns the number of vertices.
	inline virtual RtInt vertex() const {return m_sumverts;}

	//! faceVarying(), returns the number of corners per face.
	inline virtual RtInt faceVarying() const {return m_sumfacevarying;}

	//! faceVertex(), returns the number of vertices per face.
	inline virtual RtInt faceVertex() const {return m_sumfaceverts;}
}; // CCurvesClasses


//! CBlobbyClasses, parameter class for TRi::blobby()
class CBlobbyClasses : public CParameterClasses {
	RtInt m_nleaf;
public:
	//! reset()
	inline void reset(RtInt nleaf) {
		m_nleaf = nleaf;
	}

	//! CBlobbyClasses()
	inline CBlobbyClasses(RtInt nleaf) { reset(nleaf); }

	//! uniform(), returns the number of face.
	inline virtual RtInt uniform() const {return 1;}

	//! varying(), returns the number of corners.
	inline virtual RtInt varying() const {return m_nleaf;}

	//! vertex(), returns the number of vertices.
	inline virtual RtInt vertex() const {return m_nleaf;}

	//! faceVarying(), returns the number of corners per face.
	inline virtual RtInt faceVarying() const {return m_nleaf;}

	//! faceVertex(), returns the number of vertices per face.
	inline virtual RtInt faceVertex() const {return m_nleaf;}
}; // CBlobbyClasses

} // namespace RiCPP

#endif // _RICPP_BASERENDERER_PARAMCLASSES_H
