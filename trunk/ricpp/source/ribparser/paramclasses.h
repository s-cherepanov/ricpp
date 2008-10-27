#ifndef _RICPP_RIBPARSER_PARAMCLASSES_H
#define _RICPP_RIBPARSER_PARAMCLASSES_H

// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2004, Andreas Pidde
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
 * If a primitive contains many faces with shared vertices (e.g. TRi::patchMesh()),
 * the number of facevarying and facevertex can be different to varying or
 * vertex.
 */

#include "rendererloader/ricpp.h"
#include "tools/helpers.h"

//! TParameterClasses, base class for the parameter classes

/*! TParameterClasses contain the signature for all T..Classes.
    An instance of this class can be used for various interface calls,
    like shaders, lightsources, projection,
	this means all parameters have to be constant.
	TParameterClasses can be used directly for these interface calls:

	- TRi::projection()
	- TRi::imager()
	- TRi::display()
	- TRi::hider()
	- TRi::option()
	- TRi::lightSource()
	- TRi::areaLightSource()
	- TRi::attribute()
	- TRi::surface()
	- TRi::atmosphere()
	- TRi::interior()
	- TRi::exterior()
	- TRi::displacement()
	- TRi::deformation()
	- TRi::geometry()
	- TRi::makeTexture()
	- TRi::makeBump()
	- TRi::makeLatLongEnvironment()
	- TRi::makeCubeFaceEnvironment()
	- TRi::makeShadow()
	- TRi::readArchive()

  The derived classes returns the number of values for the different
  classes.
 */
class TParameterClasses {
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
}; // TParameterClasses


//! TQuadricClasses parameter classes for all quadrics.

/*! The classes alway have the sizes:

    - constant() == 1 (constant is always one)
    - uniform() == 1 (uniform is one, thats the number of faces)
    - varying() == 4 (always 4 corners in parametric space u,v: (0,0), (1,0), (0,1), (1,1))
    - vertex() == 4 (always 4 vertices, that are the 4 corners)
    - faceVarying() = 4 (same as varying, because there is always only one face)
	- faceVertex() = 4 (same as vertex, because there is always only one face)

	TQuadricParamClasses can be used for the following interface calls:

	- TRi::sphere()
	- TRi::cone()
	- TRi::cylinder()
	- TRi::hyperboloid()
	- TRi::paraboloid()
	- TRi::disk()
	- TRi::torus()
 */
class TQuadricClasses : public TParameterClasses {
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
}; // TQuadricClasses


//! TPolygonClasses, parameter classes for TRi::polygon().

/*! There is always one convex polygon. The varying, vertex, facevarying,
	facevertex classes have the size of the number of vertices of the polygon.

    Remark: The sizes can be calculated with TPointsGeneralPolygons,
	but it's easier to use to have a different class.
 */
class TPolygonClasses : public TParameterClasses {
	RtInt m_nvertices;  //!< Sum of all vertices.
public:
	//! reset(), sets the number of vertices.
	inline void reset(RtInt nvertices) {m_nvertices = nvertices;}

    //! TPolygonClasses(), convex polygon with nvertices vertices.
	inline TPolygonClasses(RtInt nvertices) {reset (nvertices);}

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
}; // TPolygonClasses


//! TGeneralPolygonClasses, parameter classes for TRi::generalPolygon()

/*! There is always one polygon, evtually with holes. The varying, vertex,
	facevarying, facevertex classes have the size of the sum of
	all vertices of the general polygon.
 */
class TGeneralPolygonClasses : public TParameterClasses {
	RtInt m_nvertices;       //!< Sum of vertices.
public:
	//! reset() recalculates the sum of the vertices.
	inline void reset(RtInt nloops, RtInt *nverts) {
		m_nvertices = sum(nloops, nverts);
	}

	// TGeneralPolygonClasses(), a general polygon with holes.
	inline TGeneralPolygonClasses(RtInt nloops, RtInt *nverts) { reset(nloops, nverts); }

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
}; // TGeneralPolygonClasses


//! TPointsPolygonsClasses, parameter classes for TRi::pointsPolygons()

/*! There are many convex polygons.
 */
class TPointsPolygonsClasses : public TParameterClasses {
	RtInt m_npolys;     //!< Number of polygons.
	RtInt m_nvertices;  //!< Sum of all vertices per face.
	RtInt m_nmaxvertex; //!< Sum of all vertices, shared vertices count one.
public:
	//! reset() recalculates the sum of the vertices and the number of vertices.
	inline void reset(RtInt npolys, RtInt *nverts, RtInt *verts) {
		m_npolys = npolys;
		m_nvertices = sum(npolys, nverts);
		m_nmaxvertex = max(m_nvertices, verts);
		++m_nmaxvertex; // Index+1 == number of vertices
	}

    //! TPointsPolygonsClasses(), many convex polygons
	inline TPointsPolygonsClasses(RtInt npolys, RtInt *nverts, RtInt *verts) { reset(npolys, nverts, verts); }

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
}; // TPointsPolygonsClasses


//! TPointsGeneralPolygonsClasses, parameter classes for TRi::pointsGeneralPolygons()

/*! There are many faces (polygons) which can contain holes.
 */
class TPointsGeneralPolygonsClasses : public TParameterClasses {
	RtInt m_npolys;     //!< Number of polygons.
	RtInt m_nvertices;  //!< Sum of all vertices per face.
	RtInt m_nmaxvertex; //!< Sum of all vertices, shared vertices count one.
public:
	//! reset() recalculates the sum of the vertices and the number of vertices
	inline void reset(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts) {
		m_npolys = npolys;
		RtInt sumloops = sum(npolys, nloops);
		m_nvertices = sum(sumloops, nverts);
		m_nmaxvertex = max(m_nvertices, verts);
		++m_nmaxvertex; // Index+1 == number of vertices
	}

	//! TPointsPolygonsGeneralClasses(), many general polygons with holes.
	inline TPointsGeneralPolygonsClasses(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts) { reset(npolys, nloops, nverts, verts); }

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
}; // TPointsGeneralPolygonsClasses


//! TPatchClasses, parameter classes for TRi::patch()

/*! There is one face (one TRi::patch()). Patches can be either
	bilinear with 4 vertices or bicubic with 16 vertices.
 */
class TPatchClasses : public TParameterClasses {
	RtInt nVertices; //!< The number of vertices is either 4 (linear patch) or 16 (bicubic patch)
public:
	//! reset() sets the number of vertices, they default to 4, only for a bicubic type there are 16
	inline void reset(RtToken type) {
		nVertices = !strcmp(type, TRi::RI_BICUBIC) ? 16 : 4;
	}

	//! TPatchClasses(), either bilinear or bicubic patches.
	inline TPatchClasses(RtToken type) { reset(type); }

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
}; // TPatchClasses


//! TPatchMeshClasses, parameter class for TRi::patchMesh()

/*! There can be bilinear patches with 4 vertices per face or bicubic ones with
    16 vertices per face. The parameters nustep and nvstep at reset() and the
	constructor are obtained from the current attributes, set by the
	interface call of TRi::basis()
 */
class TPatchMeshClasses : public TParameterClasses {
	RtInt m_nPatches;      //!< Number of patches (faces)
	RtInt m_nCorners;      //!< Number of corners
	RtInt m_nVertices;     //!< Number of vertices
	RtInt m_nFaceCorners;  //!< Number of corners per face, m_nPatches * 4
	RtInt m_nFaceVertices; //!< Number of vertices per face, m_nPatches * 4 (bilinear) or m_nPatches * 16 (bicubic)
public:
	//! reset() sets the values of the member variables
	void reset(RtToken type, RtInt nu, RtInt nustep, RtToken uwrap, RtInt nv, RtInt nvstep, RtToken vwrap);

	//! TPatchClasses(), either bilinear or bicubic patch meshes.
	// \sa reset()
	inline TPatchMeshClasses(
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
}; // TPatchMeshClasses


//! TNuPatchClasses, parameter class for TRi::nuPatch()

/*!
 */
class TNuPatchClasses : public TParameterClasses {
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

	//! TNuPatchClasses() parameters
	//! \sa reset()
	inline TNuPatchClasses(
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
}; // TNuPatchClasses


//! TSubdivisionMeshClasses, parameter class for TRi::subdivisionMesh()

/*!
 */
class TSubdivisionMeshClasses : public TParameterClasses {
	RtInt m_nVerts;
	RtInt m_nFaces;
	RtInt m_nFaceCorners;
	RtInt m_nFaceVerts;
public:
	//! reset()
	void reset(RtInt nfaces, RtInt nvertices[], RtInt vertices[]);

	//! TSubdivisionMeshClasses()
	inline TSubdivisionMeshClasses(RtInt nfaces, RtInt nvertices[], RtInt vertices[]) { reset(nfaces, nvertices, vertices); }

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
}; // TSubdivisionMeshClasses


//! TPointsClasses, parameter class for TRi::points()

/*!
 */
class TPointsClasses : public TParameterClasses {
	RtInt m_npts;
public:
	//! reset()
	inline void reset(RtInt npts) { m_npts = npts;}

	//! TPointsClasses()
	inline TPointsClasses(RtInt npts) { reset(npts); }

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
}; // TPointsClasses


//! TCurvesClasses, parameter class for TRi::curves()

/*!
 */
class TCurvesClasses : public TParameterClasses {
	RtInt m_ncurves;
	RtInt m_sumvarying;
	RtInt m_sumverts;
	RtInt m_sumfacevarying;
	RtInt m_sumfaceverts;
public:
	//! reset()
	void reset(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt vstep);

	//! TCurvesClasses()
	inline TCurvesClasses(
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
}; // TCurvesClasses


//! TBlobbyClasses, parameter class for TRi::blobby()

/*!
 */
class TBlobbyClasses : public TParameterClasses {
	RtInt m_nleaf;
public:
	//! reset()
	inline void reset(RtInt nleaf) {
		m_nleaf = nleaf;
	}

	//! TBlobbyClasses()
	inline TBlobbyClasses(RtInt nleaf) { reset(nleaf); }

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
}; // TBlobbyClasses

#endif // _RICPP_RIBPARSER_PARAMCLASSES_H
