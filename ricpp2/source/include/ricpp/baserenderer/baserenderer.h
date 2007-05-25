#ifndef _RICPP_BASERENDERER_BASERENDERER_H
#define _RICPP_BASERENDERER_BASERENDERER_H

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

/** @file baserenderer.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declaration of the backend base class of a renderer context.
 */

#ifndef _RICPP_BASERENDERER_DORENDER_H
#include "ricpp/baserenderer/dorender.h"
#endif // _RICPP_BASERENDERER_DORENDER_H

#ifndef _RICPP_RENDERSTATE_RENDERSTATE_H
#include "ricpp/renderstate/renderstate.h"
#endif // _RICPP_RENDERSTATE_RENDERSTATE_H

#ifndef _RICPP_RICPP_RENDERERERROR_H
#include "ricpp/ricpp/renderererror.h"
#endif // _RICPP_RICPP_RENDERERERROR_H

namespace RiCPP {

/** @brief This class is used to implement the basis of a renderer context.
 *  
 *  Finds the unique strings (tokens) for RtTokens and
 *  it does type checking and calls the IDoRenderer do...() members if the checking was successful.
 *  Maintains a render state.
 *  The concrete context inherits from CBaseRenderer and implements the IDoRenderer members for
 *  its rendering jobs. The CBaseRenderer is the foundation of the backend renderer context.
 */
class CBaseRenderer : public IDoRender {
	/** @brief Initializes the m_renderState, called by begin()
	 */
	void initRenderState();

	/** @brief The error handler throws exceptions, default handler
	 *
	 *  Never used directly, ricppErrHandler() is used for error handling
	 */
	CErrorExceptionHandler m_errorExceptionHandler;
protected:
	/** @brief The render state contains all render state objects of this context
	 *
	 *  The render state is initialized at begin()
	 */
	CRenderState *m_renderState;

	/** @brief Creates a new modestack, called by initModeStack()
	 *
	 *  Overwrite this method if you want to return an own modestack
	 */
	inline virtual CModeStack *getNewModeStack() { return new CModeStack; }

	/** @brief The backend's error handler
	 *
	 *  A child class can overload ricppErrHandler() to use it's own handler
	 *  However it also should return an error handler that throws an ERendererError
	 * @return The error handler used by the backend
	 */
	virtual IRiCPPErrorHandler &ricppErrHandler()
	{
		return m_errorExceptionHandler;
	}

public:
	inline CBaseRenderer() : m_renderState(0) {}

	/** @brief Virtual destruction
	 */
	virtual inline ~CBaseRenderer() {}

	/** @brief Is called by the frontend, if the renderer is aborted due to severe errors.
	 */
	inline virtual RtVoid abort(void) {}
	/** @brief Is called, if the renderer is activated by context()
	 *
	 * If the context will be the new active rendering context, it is activated by it's
	 * context creator.
	 */
	inline virtual RtVoid activate(void) {}
	/** @brief Is called, if the renderer is deactivated by context()
	 *
	 * If the context will be the suspended rendering context, it is deactivated by it's
	 * context creator.
	 */
	inline virtual RtVoid deactivate(void) {}

	RtToken declare(RtString name, RtString declaration);

    inline virtual RtVoid synchronize(RtToken name) {}

	inline virtual RtToken resourceV(RtString name, RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) { return RI_NULL; }
	inline virtual RtVoid  freeResource(RtToken handle) {}

	/** @brief Initializes a new render context.
	 *
	 *  begin() is called exactly one time by the context creator
	 *  after the rendering context object is created.
	 *  This method must be called by child objects begin() at first
	 *  for initialization before they do their own job.
	 *  @param name Passed by the front end's begin()-method.
	 */
	virtual RtVoid begin(RtString name);

	/** @brief Clears the context.
	 *
	 *  end() is called exactly one time by the context creator before deletion. It
	 *  can be overwritten, but child classes must call CBaseRenderer::end()
	 *  as their last action.
	 */
	virtual RtVoid end(void);

	virtual RtVoid frameBegin(RtInt number);
	virtual RtVoid frameEnd(void);

	virtual RtVoid worldBegin(void);
	virtual RtVoid worldEnd(void);

	virtual RtVoid attributeBegin(void);
	virtual RtVoid attributeEnd(void);

	virtual RtVoid transformBegin(void);
	virtual RtVoid transformEnd(void);

	inline virtual RtVoid solidBegin(RtToken type) {}
	inline virtual RtVoid solidEnd(void) {}

	inline virtual RtObjectHandle objectBegin(void) { return illObjectHandle; }
	inline virtual RtVoid objectEnd(void) {}
    inline virtual RtVoid objectInstance(RtObjectHandle handle) {}
    inline virtual RtVoid freeObject(RtObjectHandle handle) {}

    inline virtual RtVoid motionBeginV(RtInt N, RtFloat times[]) {}
    inline virtual RtVoid motionEnd(void) {}

    inline virtual RtVoid format(RtInt xres, RtInt yres, RtFloat aspect) {}
    inline virtual RtVoid frameAspectRatio(RtFloat aspect) {}

	inline virtual RtVoid screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) {}
    inline virtual RtVoid cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) {}
    inline virtual RtVoid projectionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid clipping(RtFloat hither, RtFloat yon) {}
    inline virtual RtVoid clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) {}
    inline virtual RtVoid depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) {}
    inline virtual RtVoid shutter(RtFloat smin, RtFloat smax) {}
	inline virtual RtVoid pixelVariance(RtFloat variation) {}
    inline virtual RtVoid pixelSamples(RtFloat xsamples, RtFloat ysamples) {}
    inline virtual RtVoid pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) {}
    inline virtual RtVoid exposure(RtFloat gain, RtFloat gamma) {}
    inline virtual RtVoid imagerV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) {}
    inline virtual RtVoid displayV(RtString name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid colorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn) {}
    inline virtual RtVoid relativeDetail(RtFloat relativedetail) {}
    inline virtual RtVoid optionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	
    inline virtual RtLightHandle lightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) { return illLightHandle; }
	inline virtual RtLightHandle areaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) { return illLightHandle; }
	
    inline virtual RtVoid attributeV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid color(RtColor Cs) {}
	inline virtual RtVoid opacity(RtColor Cs) {}
    inline virtual RtVoid surfaceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid atmosphereV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid interiorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid exteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid illuminate(RtLightHandle light, RtBoolean onoff) {}
    inline virtual RtVoid displacementV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) {}
    inline virtual RtVoid shadingRate(RtFloat size) {}
	inline virtual RtVoid shadingInterpolation(RtToken type) {}
    inline virtual RtVoid matte(RtBoolean onoff) {}
	inline virtual RtVoid bound(RtBound bound) {}
	inline virtual RtVoid detail(RtBound bound) {}
	inline virtual RtVoid detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) {}
    inline virtual RtVoid geometricApproximationV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid geometricRepresentation(RtToken type) {}
	inline virtual RtVoid orientation(RtToken orientation) {}
	inline virtual RtVoid reverseOrientation(void) {}
	inline virtual RtVoid sides(RtInt nsides) {}
    inline virtual RtVoid basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) {}
    inline virtual RtVoid trimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) {}

	inline virtual RtVoid identity(void) {}
	inline virtual RtVoid transform(RtMatrix transform) {}
	inline virtual RtVoid concatTransform(RtMatrix transform) {}
	inline virtual RtVoid perspective(RtFloat fov) {}
	inline virtual RtVoid translate(RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid scale(RtFloat dx, RtFloat dy, RtFloat dz) {}
    inline virtual RtVoid skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) {}
	inline virtual RtVoid deformationV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid coordinateSystem(RtToken space) {}
	inline virtual RtVoid coordSysTransform(RtToken space) {}
	inline virtual RtPoint *transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) { return &points[0]; }

    inline virtual RtVoid polygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid generalPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid pointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid pointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]) {}

	inline virtual RtVoid patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid nuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]) {}

	inline virtual RtVoid subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]) {}

	inline virtual RtVoid sphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid coneV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid cylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid hyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid paraboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid diskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid torusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}

    inline virtual RtVoid pointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid curvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]) {}

	inline virtual RtVoid blobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]) {}

	inline virtual RtVoid procedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc &freefunc) {}

	inline virtual RtVoid geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {}

	inline virtual RtVoid makeTextureV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid makeBumpV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid makeLatLongEnvironmentV(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid makeCubeFaceEnvironmentV(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid makeShadowV(RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[]) {}

	inline virtual RtVoid archiveRecordV(RtToken type, RtString line) {}
	inline virtual RtVoid readArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[]) {}

protected:
	inline virtual RtVoid doAbort(void) {}
	inline virtual RtVoid doActivate(void) {}
	inline virtual RtVoid doDeactivate(void) {}

	inline virtual RtVoid doDeclare(RtToken name, RtString declaration) {}
	inline virtual RtVoid doResourceV(RtString name, RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doFreeResource(RtToken handle) {}

	inline virtual RtVoid doBegin(RtString name = RI_NULL) {}
	inline virtual RtVoid doEnd(void) {}

	inline virtual RtVoid doFrameBegin(RtInt number) {}
	inline virtual RtVoid doFrameEnd(void) {}

	inline virtual RtVoid doWorldBegin(void) {}
	inline virtual RtVoid doWorldEnd(void) {}

	inline virtual RtVoid doAttributeBegin(void) {}
	inline virtual RtVoid doAttributeEnd(void) {}

	inline virtual RtVoid doTransformBegin(void) {}
	inline virtual RtVoid doTransformEnd(void) {}

    inline virtual RtVoid doSolidBegin(RtToken type) {}
    inline virtual RtVoid doSolidEnd(void) {}

	inline virtual RtVoid doObjectBegin(void) {}
	inline virtual RtVoid doObjectEnd(void) {}
    inline virtual RtVoid doObjectInstance(RtObjectHandle handle) {}
    inline virtual RtVoid doFreeObject(RtObjectHandle handle) {}

    inline virtual RtVoid doMotionBeginV(RtInt N, RtFloat times[]) {}
    inline virtual RtVoid doMotionEnd(void) {}

    inline virtual RtVoid doSynchronize(RtToken name) {}

    inline virtual RtVoid doFormat(RtInt xres, RtInt yres, RtFloat aspect) {}
    inline virtual RtVoid doFrameAspectRatio(RtFloat aspect) {}

	inline virtual RtVoid doScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) {}
    inline virtual RtVoid doCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) {}
    inline virtual RtVoid doProjectionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doClipping(RtFloat hither, RtFloat yon) {}
    inline virtual RtVoid doClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) {}
    inline virtual RtVoid doDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) {}
    inline virtual RtVoid doShutter(RtFloat smin, RtFloat smax) {}
	inline virtual RtVoid doPixelVariance(RtFloat variation) {}
    inline virtual RtVoid doPixelSamples(RtFloat xsamples, RtFloat ysamples) {}
    inline virtual RtVoid doPixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) {}
    inline virtual RtVoid doExposure(RtFloat gain, RtFloat gamma) {}
    inline virtual RtVoid doImagerV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) {}
    inline virtual RtVoid doDisplayV(RtString name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doHiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doColorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn) {}
    inline virtual RtVoid doRelativeDetail(RtFloat relativedetail) {}
    inline virtual RtVoid doOptionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	
    inline virtual RtVoid doLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) { }
	inline virtual RtVoid doAreaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) { }

    inline virtual RtVoid doAttributeV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doColor(RtColor Cs) {}
	inline virtual RtVoid doOpacity(RtColor Cs) {}
    inline virtual RtVoid doSurfaceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doAtmosphereV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doInteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doExteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doIlluminate(RtLightHandle light, RtBoolean onoff) {}
    inline virtual RtVoid doDisplacementV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) {}
    inline virtual RtVoid doShadingRate(RtFloat size) {}
	inline virtual RtVoid doShadingInterpolation(RtToken type) {}
    inline virtual RtVoid doMatte(RtBoolean onoff) {}
	inline virtual RtVoid doBound(RtBound bound) {}
	inline virtual RtVoid doDetail(RtBound bound) {}
	inline virtual RtVoid doDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) {}
    inline virtual RtVoid doGeometricApproximationV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doGeometricRepresentation(RtToken type) {}
	inline virtual RtVoid doOrientation(RtToken orientation) {}
	inline virtual RtVoid doReverseOrientation(void) {}
	inline virtual RtVoid doSides(RtInt nsides) {}
    inline virtual RtVoid doBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) {}
    inline virtual RtVoid doTrimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) {}

	inline virtual RtVoid doIdentity(void) {}
	inline virtual RtVoid doTransform(RtMatrix transform) {}
	inline virtual RtVoid doConcatTransform(RtMatrix transform) {}
	inline virtual RtVoid doPerspective(RtFloat fov) {}
	inline virtual RtVoid doTranslate(RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid doRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid doScale(RtFloat dx, RtFloat dy, RtFloat dz) {}
    inline virtual RtVoid doSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) {}
	inline virtual RtVoid doDeformationV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doCoordinateSystem(RtToken space) {}
	inline virtual RtVoid doCoordSysTransform(RtToken space) {}

	// no do-funcltion
	// inline virtual RtPoint *doTransformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) { return &points[0]; }

    inline virtual RtVoid doPolygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doGeneralPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doPointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doPointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]) {}

	inline virtual RtVoid doPatchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {}
	inline virtual RtVoid doPatchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doNuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]) {}

	inline virtual RtVoid doSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]) {}

	inline virtual RtVoid doSphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doConeV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doCylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doHyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doParaboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doDiskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doTorusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {}

    inline virtual RtVoid doPointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doCurvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]) {}

	inline virtual RtVoid doBlobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]) {}

	inline virtual RtVoid doProcedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc &freefunc) {}

	inline virtual RtVoid doGeometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {}

	inline virtual RtVoid doMakeTextureV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doMakeBumpV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doMakeLatLongEnvironmentV(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doMakeCubeFaceEnvironmentV(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doMakeShadowV(RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[]) {}

	inline virtual RtVoid doArchiveRecordV(RtToken type, RtString line) {}
	inline virtual RtVoid doReadArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[]) {}
}; // CBaseRenderer

} // namespace RiCPP

#endif // _RICPP_BASERENDERER_BASERENDERER_H
