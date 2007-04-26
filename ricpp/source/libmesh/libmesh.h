#ifndef _RICPP_RIMESH_RIMESH_H
#define _RICPP_RIMESH_RIMESH_H

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

/*! \file libmesh.h
 *  \brief Building scene graph and meshes
 *         RenderMan is (R) by Pixar
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "rendercontext/rendercontext.h"

class TMeshContext : public TRiCPPContext {
public:
	TRiSceneBuilder m_main;
	TRiSceneBuilder m_files;
	TRiSceneBuilder m_objects;
	TRiSceneBuilder *m_scene;
public:
	inline TMeshContext() {m_scene = &m_main;}
	inline virtual ~TMeshContext() {}

	inline static const char *myClassName(void) {return "TMeshContext";}
	inline virtual const char *className() const { return TMeshContext::myClassName(); }
	inline virtual bool isKindOf(const char *aClass) const {
		if ( !aClass )
			return false;
		if  ( !stricmp(aClass, TMeshContext::myClassName()) ) {
			return true;
		}
		return TRiCPPContext::isKindOf(aClass);
	}

	inline virtual bool isValid(unsigned int idxRoutine, unsigned int idxContext) {
		return TRiCPPContext::isValid(idxRoutine, idxContext) && (m_scene != NULL);
	}
	// TRiObjectBegin *objectInstance(RtObjectHandle handle);
}; // TMeshContext

class TMesh : public TIntermediateRenderer {
protected:
	virtual TRiCPPContext *getNewContext();
public:
	inline TMesh() {}
	inline virtual ~TMesh() { }

	//@{
	//! Graphics state

	//@{
	//! Begin-end block

	virtual RtVoid doBegin(RtToken name = (RtToken)RI_NULL);
	virtual RtVoid doEnd(void);
	//@}

	//@{
	//! Frame block

	virtual RtVoid doFrameBegin(RtInt number);
	virtual RtVoid doFrameEnd(void);
	//@}

	//@{
	//! World block

	virtual RtVoid doWorldBegin(void);
	virtual RtVoid doWorldEnd(void);
	//@}

	//@{
	//! Attribute block

	virtual RtVoid doAttributeBegin(void);
	virtual RtVoid doAttributeEnd(void);
	//@}

	//@{
	//! Transformation block
	virtual RtVoid doTransformBegin(void);
	virtual RtVoid doTransformEnd(void);
	//@}

	//@{
	//! Solid block
    virtual RtVoid doSolidBegin(RtToken type);
    virtual RtVoid doSolidEnd(void);
	//@}

	//@{
	//! Object block
	virtual RtObjectHandle doObjectBegin(RtObjectHandle handle);
	virtual RtVoid doObjectEnd(void);

	//! Retained geometry (objectBegin(), objectEnd())
    virtual RtVoid doObjectInstance(RtObjectHandle handle);
	//@}

	//@{
	//! Motion block
    virtual RtVoid doMotionBeginV(RtInt N, RtFloat times[]);
    virtual RtVoid doMotionEnd(void);
	//@}

	//@}

	//! Synchronize state (abort)
	// inline virtual RtVoid doSynchronize(RtToken name) {}

	//@{
	//! Options
    virtual RtVoid doFormat(RtInt xres, RtInt yres, RtFloat aspect);
    virtual RtVoid doFrameAspectRatio(RtFloat aspect);
    virtual RtVoid doScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top);
    virtual RtVoid doCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax);
    virtual RtVoid doProjectionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid doClipping(RtFloat hither, RtFloat yon);
    virtual RtVoid doClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz);
    virtual RtVoid doDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance);
    virtual RtVoid doShutter(RtFloat smin, RtFloat smax);
	virtual RtVoid doPixelVariance(RtFloat variance);
    virtual RtVoid doPixelSamples(RtFloat xsamples, RtFloat ysamples);
    virtual RtVoid doPixelFilter(RtFilterFunc function, RtFloat xwidth, RtFloat ywidth);
    virtual RtVoid doExposure(RtFloat gain, RtFloat gamma);
    virtual RtVoid doImagerV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl);
    virtual RtVoid doDisplayV(RtToken name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doHiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doColorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn);
    virtual RtVoid doRelativeDetail(RtFloat relativedetail);
    virtual RtVoid doOptionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
	//@}

	//@{
	//! Lights
    virtual RtLightHandle doLightSourceV(RtLightHandle handle, const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtLightHandle doAreaLightSourceV(RtLightHandle handle, const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
	//@}

	//@{
	//! Attributes
    virtual RtVoid doAttributeV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid doColor(RtColor Cs);
	virtual RtVoid doOpacity(RtColor Cs);
    virtual RtVoid doSurfaceV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doAtmosphereV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doInteriorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doExteriorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid doIlluminate(RtLightHandle light, RtBoolean onoff);
    virtual RtVoid doDisplacementV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);
    virtual RtVoid doShadingRate(RtFloat size);
    virtual RtVoid doShadingInterpolation(RtToken type);
    virtual RtVoid doMatte(RtBoolean onoff);
	virtual RtVoid doBound(RtBound bound);
	virtual RtVoid doDetail(RtBound bound);
    virtual RtVoid doDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis);
    virtual RtVoid doGeometricApproximation(RtToken type, RtFloat value);
    virtual RtVoid doGeometricRepresentation(RtToken type);
    virtual RtVoid doOrientation(RtToken orientation);
	virtual RtVoid doReverseOrientation(void);
    virtual RtVoid doSides(RtInt nsides);
    virtual RtVoid doBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep);
    virtual RtVoid doTrimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w);
	//@}

	//@{
	//! Transformations
	virtual RtVoid doIdentity(void);
	virtual RtVoid doTransform(RtMatrix transform);
	virtual RtVoid doConcatTransform(RtMatrix transform);
    virtual RtVoid doPerspective(RtFloat fov);
    virtual RtVoid doTranslate(RtFloat dx, RtFloat dy, RtFloat dz);
    virtual RtVoid doRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
    virtual RtVoid doScale(RtFloat dx, RtFloat dy, RtFloat dz);
    virtual RtVoid doSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2);
    virtual RtVoid doDeformationV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doCoordinateSystem(RtToken space);
    virtual RtVoid doCoordSysTransform(RtToken space);
	// inline virtual RtPoint *doTransformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) { return &points[0]; }
	//@}

	//@{
	//! Polygons
    virtual RtVoid doPolygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid doGeneralPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doPointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doPointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]);
	//@}

	//@{
	//! Patches
    virtual RtVoid doPatchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doPatchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doNuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]);
	//@}

	//@{
	//! Subdivision Surfaces
    virtual RtVoid doSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]);
	//@}

	//@{
	//! Quadrics
    virtual RtVoid doSphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doConeV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doCylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doHyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doParaboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doDiskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doTorusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
	//@}

	//@{
	//! Point and curve primitives
    virtual RtVoid doPointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doCurvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]);
	//@}
    
	//@{
	//! Blobby implicit surfaces
    virtual RtVoid doBlobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]);
	//@}

	//! Procedural primitives
	virtual RtVoid doProcedural(RtPointer data, RtBound bound, RtProcSubdivFunc subdivfunc, RtProcFreeFunc freefunc);

	//@{
    //! General objects
    virtual RtVoid doGeometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);
	//@}

	//@{
    //! Map-making
    virtual RtVoid doMakeTextureV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doMakeBumpV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doMakeLatLongEnvironmentV(const char *pic, const char *tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doMakeCubeFaceEnvironmentV(const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz, const char *tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doMakeShadowV(const char *pic, const char *tex, RtInt n, RtToken tokens[], RtPointer params[]);
	//@}

	//@{
	//! Archive files
	virtual RtVoid doArchiveRecordV(RtToken type, const char *line);
	virtual RtVoid doReadArchiveV(RtString filename, RtArchiveCallback callback, RtInt n, RtToken tokens[], RtPointer params[]);
	//@}
}; // TMesh

#endif // _RICPP_RIMESH_RIMESH_H
