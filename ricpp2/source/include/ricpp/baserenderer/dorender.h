#ifndef _RICPP_BASERENDERER_DORENDER_H
#define _RICPP_BASERENDERER_DORENDER_H

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

/** @file dorender.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declaration of an interface for handling the rendering jobs of render contexts.
 */

#ifndef _RICPP_BASERENDERER_RICONTEXT_H
#include "ricpp/baserenderer/ricontext.h"
#endif // _RICPP_BASERENDERER_RICONTEXT_H

namespace RiCPP {

/** @brief Members called called with checked parameters (here from \a CBaseRenderer).
 *
 *  Renderer contexts can inherit from \a CBaseRenderer to implement these
 *  functions to do the rendering.
*/
class IDoRender : public IRiContext {
public:
	/** @brief The virtual destructor of the interface
	 */
	inline virtual ~IDoRender() {}

protected:

	/** @brief Special context handling (see IRiContext::abort(), IRiContext::activate(), IRiContext::deactivate()
	 */
	//@{
	virtual RtVoid doAbort(void) = 0;
	virtual RtVoid doActivate(void) = 0;
	virtual RtVoid doDeactivate(void) = 0;

	/** The interface functions similar to IRiCPP
	 */
	//@{

	// There is no user defined error handler at the backend
	// virtual RtVoid doErrorHandler(const IErrorHandler &handler) = 0;

	virtual RtToken doDeclare(RtString name, RtString declaration) = 0;

	virtual RtVoid doSynchronize(RtToken name) = 0;

	virtual RtVoid doBegin(RtString name) = 0;
	virtual RtVoid doEnd(void) = 0;

	virtual RtVoid doFrameBegin(RtInt number) = 0;
	virtual RtVoid doFrameEnd(void) = 0;

	virtual RtVoid doWorldBegin(void) = 0;
	virtual RtVoid doWorldEnd(void) = 0;

	virtual RtVoid doAttributeBegin(void) = 0;
	virtual RtVoid doAttributeEnd(void) = 0;

	virtual RtVoid doTransformBegin(void) = 0;
	virtual RtVoid doTransformEnd(void) = 0;

    virtual RtVoid doSolidBegin(RtToken type) = 0;
    virtual RtVoid doSolidEnd(void) = 0;

	virtual RtObjectHandle doObjectBegin(void) = 0;
	virtual RtVoid doObjectEnd(void) = 0;
    virtual RtVoid doObjectInstance(RtObjectHandle handle) = 0;

    virtual RtVoid doMotionBeginV(RtInt N, RtFloat times[]) = 0;
    virtual RtVoid doMotionEnd(void) = 0;

    virtual RtVoid doFormat(RtInt xres, RtInt yres, RtFloat aspect) = 0;
    virtual RtVoid doFrameAspectRatio(RtFloat aspect) = 0;

	virtual RtVoid doScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) = 0;
    virtual RtVoid doCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) = 0;
    virtual RtVoid doProjectionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doClipping(RtFloat hither, RtFloat yon) = 0;
    virtual RtVoid doClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) = 0;
    virtual RtVoid doDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) = 0;
    virtual RtVoid doShutter(RtFloat smin, RtFloat smax) = 0;
	virtual RtVoid doPixelVariance(RtFloat variation) = 0;
    virtual RtVoid doPixelSamples(RtFloat xsamples, RtFloat ysamples) = 0;
    virtual RtVoid doPixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) = 0;
    virtual RtVoid doExposure(RtFloat gain, RtFloat gamma) = 0;
    virtual RtVoid doImagerV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) = 0;
    virtual RtVoid doDisplayV(RtString name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doHiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doColorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn) = 0;
    virtual RtVoid doRelativeDetail(RtFloat relativedetail) = 0;
    virtual RtVoid doOptionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	
    virtual RtLightHandle doLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtLightHandle doAreaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	
    virtual RtVoid doAttributeV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doColor(RtColor Cs) = 0;
	virtual RtVoid doOpacity(RtColor Cs) = 0;
    virtual RtVoid doSurfaceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doAtmosphereV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doInteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doExteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doIlluminate(RtLightHandle light, RtBoolean onoff) = 0;
    virtual RtVoid doDisplacementV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) = 0;
    virtual RtVoid doShadingRate(RtFloat size) = 0;
	virtual RtVoid doShadingInterpolation(RtToken type) = 0;
    virtual RtVoid doMatte(RtBoolean onoff) = 0;
	virtual RtVoid doBound(RtBound bound) = 0;
	virtual RtVoid doDetail(RtBound bound) = 0;
	virtual RtVoid doDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) = 0;
    virtual RtVoid doGeometricApproximation(RtToken type, RtFloat value) = 0;
	virtual RtVoid doGeometricRepresentation(RtToken type) = 0;
	virtual RtVoid doOrientation(RtToken orientation) = 0;
	virtual RtVoid doReverseOrientation(void) = 0;
	virtual RtVoid doSides(RtInt nsides) = 0;
    virtual RtVoid doBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) = 0;
    virtual RtVoid doTrimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) = 0;

	virtual RtVoid doIdentity(void) = 0;
	virtual RtVoid doTransform(RtMatrix transform) = 0;
	virtual RtVoid doConcatTransform(RtMatrix transform) = 0;
	virtual RtVoid doPerspective(RtFloat fov) = 0;
	virtual RtVoid doTranslate(RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid doRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid doScale(RtFloat dx, RtFloat dy, RtFloat dz) = 0;
    virtual RtVoid doSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) = 0;
	virtual RtVoid doDeformationV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doCoordinateSystem(RtToken space) = 0;
	virtual RtVoid doCoordSysTransform(RtToken space) = 0;
	virtual RtPoint *doTransformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) = 0;

    virtual RtVoid doPolygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doGeneralPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doPointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doPointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	virtual RtVoid doPatchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid doPatchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doNuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	virtual RtVoid doSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	virtual RtVoid doSphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doConeV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doCylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doHyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doParaboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doDiskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doTorusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

    virtual RtVoid doPointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doCurvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	virtual RtVoid doBlobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	virtual RtVoid doProcedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc &freefunc) = 0;

	virtual RtVoid doGeometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	virtual RtVoid doMakeTextureV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doMakeBumpV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doMakeLatLongEnvironmentV(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doMakeCubeFaceEnvironmentV(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid doMakeShadowV(RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	virtual RtVoid doArchiveRecordV(RtToken type, RtString line) = 0;
	virtual RtVoid doReadArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
}; // IDoRender
} // namespace RiCPP

#endif // _RICPP_BASERENDERER_DORENDER_H