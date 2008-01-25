#ifndef _RICPP_RICONTEXT_DORENDER_H
#define _RICPP_RICONTEXT_DORENDER_H

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

#ifndef _RICPP_RICONTEXT_RICONTEXT_H
#include "ricpp/ricontext/ricontext.h"
#endif // _RICPP_RICONTEXT_RICONTEXT_H

namespace RiCPP {

/** @brief Members called called with checked parameters (here from \a CBaseRenderer).
 *
 *  Renderer contexts can inherit from \a CBaseRenderer to implement these
 *  functions to do the rendering. Since this are callbacks, all functions have no return values.
*/
class IDoRender : public IRiContext {
public:
	virtual RtVoid archiveInstanceV(RtArchiveHandle handle, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid preArchiveInstance(RtArchiveHandle handle, const IArchiveCallback *callback, const CParameterList &params) = 0;
	virtual RtVoid doArchiveInstance(RtArchiveHandle handle, const IArchiveCallback *callback, const CParameterList &params) = 0;
	virtual RtVoid postArchiveInstance(RtArchiveHandle handle, const IArchiveCallback *callback, const CParameterList &params) = 0;
public:
	/** @brief The virtual destructor of the interface
	 */
	inline virtual ~IDoRender() {}

	/** @brief Checks validity of state for common requests.
	 *  @param req Number of the requests.
	 *  @return false if checking fails.
	 *  @except ExceptRiCPPError Implementation can also throw an exception on errors.
	 */
	virtual bool preCheck(EnumRequests req) = 0;

	/** Replays a single interface call
	 */
	virtual void replayRequest(class CRManInterfaceCall &aRequest, const IArchiveCallback *cb) = 0;

	/** @brief Special context handling (see IRiContext::abort(), IRiContext::activate(), IRiContext::deactivate()
	 */
	//@{
	virtual RtVoid doAbort(void) = 0;
	virtual RtVoid doActivate(void) = 0;
	virtual RtVoid doDeactivate(void) = 0;
	//@}

	/** @brief Error handler
	 *
	 *  Error handler is normally a front end function, but
	 *  e.g. for RIB writing, it is also needed as a back end function
	 *  for output. But it will normally not handle errors at the back end.
	 *
	 *  @param handler The error handler
	 */
	virtual RtVoid preErrorHandler(const IErrorHandler &handler) = 0;

	virtual RtVoid preDeclare(RtToken name, RtString declaration) = 0;
	virtual RtVoid preSynchronize(RtToken name) = 0;
	virtual RtVoid preSystem(RtString cmd) = 0;
	virtual RtVoid preResource(RtString handle, RtString type, const CParameterList &params) = 0;

	virtual RtVoid preBegin(RtString name, const CParameterList &params) = 0;
	virtual RtVoid preEnd(void) = 0;

	virtual RtVoid preFrameBegin(RtInt number) = 0;
	virtual RtVoid preFrameEnd(void) = 0;

	virtual RtVoid preWorldBegin(void) = 0;
	virtual RtVoid preWorldEnd(void) = 0;

	virtual RtVoid preAttributeBegin(void) = 0;
	virtual RtVoid preAttributeEnd(void) = 0;

	virtual RtVoid preTransformBegin(void) = 0;
	virtual RtVoid preTransformEnd(void) = 0;

    virtual RtVoid preSolidBegin(RtToken type) = 0;
    virtual RtVoid preSolidEnd(void) = 0;

	virtual RtObjectHandle preObjectBegin(void) = 0;
	virtual RtVoid preObjectEnd(void) = 0;
    virtual RtVoid preObjectInstance(RtObjectHandle handle) = 0;

    virtual RtVoid preMotionBegin(RtInt N, RtFloat times[]) = 0;
    virtual RtVoid preMotionEnd(void) = 0;

	virtual RtVoid preResourceBegin(void) = 0;
	virtual RtVoid preResourceEnd(void) = 0;

	virtual RtArchiveHandle preArchiveBegin(RtToken name, const CParameterList &params) = 0;
	virtual RtVoid preArchiveEnd(void) = 0;

    virtual RtVoid preFormat(RtInt xres, RtInt yres, RtFloat aspect) = 0;
    virtual RtVoid preFrameAspectRatio(RtFloat aspect) = 0;

	virtual RtVoid preScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) = 0;
    virtual RtVoid preCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) = 0;
	virtual RtVoid preProjection(RtToken name, const CParameterList &params) = 0;
	virtual RtVoid preClipping(RtFloat hither, RtFloat yon) = 0;
    virtual RtVoid preClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) = 0;
    virtual RtVoid preDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) = 0;
    virtual RtVoid preShutter(RtFloat smin, RtFloat smax) = 0;
	virtual RtVoid prePixelVariance(RtFloat variation) = 0;
    virtual RtVoid prePixelSamples(RtFloat xsamples, RtFloat ysamples) = 0;
    virtual RtVoid prePixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) = 0;
    virtual RtVoid preExposure(RtFloat gain, RtFloat gamma) = 0;
    virtual RtVoid preImager(RtString name, const CParameterList &params) = 0;
	virtual RtVoid preQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) = 0;
    virtual RtVoid preDisplayChannel(RtString channel, const CParameterList &params) = 0;
    virtual RtVoid preDisplay(RtString name, RtToken type, RtString mode, const CParameterList &params) = 0;
    virtual RtVoid preHider(RtToken type, const CParameterList &params) = 0;
    virtual RtVoid preColorSamples(RtInt N, RtFloat nRGB[], RtFloat RGBn[]) = 0;
    virtual RtVoid preRelativeDetail(RtFloat relativedetail) = 0;
    virtual RtVoid preOption(RtString name, const CParameterList &params) = 0;
	
    virtual RtLightHandle preLightSource(RtString name, const CParameterList &params) = 0;
	virtual RtLightHandle preAreaLightSource(RtString name, const CParameterList &params) = 0;
	
    virtual RtVoid preAttribute(RtString name, const CParameterList &params) = 0;
	virtual RtVoid preColor(RtColor Cs) = 0;
	virtual RtVoid preOpacity(RtColor Os) = 0;
    virtual RtVoid preSurface(RtString name, const CParameterList &params) = 0;
    virtual RtVoid preAtmosphere(RtString name, const CParameterList &params) = 0;
    virtual RtVoid preInterior(RtString name, const CParameterList &params) = 0;
	virtual RtVoid preExterior(RtString name, const CParameterList &params) = 0;
	virtual RtVoid preIlluminate(RtLightHandle light, RtBoolean onoff) = 0;
    virtual RtVoid preDisplacement(RtString name, const CParameterList &params) = 0;
	virtual RtVoid preTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) = 0;
    virtual RtVoid preShadingRate(RtFloat size) = 0;
	virtual RtVoid preShadingInterpolation(RtToken type) = 0;
    virtual RtVoid preMatte(RtBoolean onoff) = 0;
	virtual RtVoid preBound(RtBound bound) = 0;
	virtual RtVoid preDetail(RtBound bound) = 0;
	virtual RtVoid preDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) = 0;
    virtual RtVoid preGeometricApproximation(RtToken type, RtFloat value) = 0;
	virtual RtVoid preGeometricRepresentation(RtToken type) = 0;
	virtual RtVoid preOrientation(RtToken anOrientation) = 0;
	virtual RtVoid preReverseOrientation(void) = 0;
	virtual RtVoid preSides(RtInt nsides) = 0;
    virtual RtVoid preBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) = 0;
    virtual RtVoid preTrimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]) = 0;

	virtual RtVoid preIdentity(void) = 0;
	virtual RtVoid preTransform(RtMatrix aTransform) = 0;
	virtual RtVoid preConcatTransform(RtMatrix aTransform) = 0;
	virtual RtVoid prePerspective(RtFloat fov) = 0;
	virtual RtVoid preTranslate(RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid preRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid preScale(RtFloat dx, RtFloat dy, RtFloat dz) = 0;
    virtual RtVoid preSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) = 0;
	virtual RtVoid preDeformation(RtString name, const CParameterList &params) = 0;
	virtual RtVoid preScopedCoordinateSystem(RtToken space) = 0;
	virtual RtVoid preCoordinateSystem(RtToken space) = 0;
	virtual RtVoid preCoordSysTransform(RtToken space) = 0;
	virtual RtPoint *preTransformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) = 0;

    virtual RtVoid prePolygon(RtInt nvertices, const CParameterList &params) = 0;
	virtual RtVoid preGeneralPolygon(RtInt nloops, RtInt nverts[], const CParameterList &params) = 0;
	virtual RtVoid prePointsPolygons(RtInt npolys, RtInt nverts[], RtInt verts[], const CParameterList &params) = 0;
    virtual RtVoid prePointsGeneralPolygons(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], const CParameterList &params) = 0;

	virtual RtVoid prePatch(RtToken type, const CParameterList &params) = 0;
	virtual RtVoid prePatchMesh(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params) = 0;
    virtual RtVoid preNuPatch(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, const CParameterList &params) = 0;

	virtual RtVoid preSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params) = 0;
	virtual RtVoid preHierarchicalSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params) = 0;

	virtual RtVoid preSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid preCone(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid preCylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid preHyperboloid(RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid preParaboloid(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid preDisk(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid preTorus(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params) = 0;

    virtual RtVoid prePoints(RtInt npts, const CParameterList &params) = 0;
    virtual RtVoid preCurves(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params) = 0;

	virtual RtVoid preBlobby(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params) = 0;

	virtual RtVoid preProcedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc *freefunc) = 0;

	virtual RtVoid preGeometry(RtToken type, const CParameterList &params) = 0;

	virtual RtVoid preMakeTexture(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid preMakeBump(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid preMakeLatLongEnvironment(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid preMakeCubeFaceEnvironment(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid preMakeShadow(RtString pic, RtString tex, const CParameterList &params) = 0;
    virtual RtVoid preMakeBrickMap(RtInt nNames, RtString ptcnames[], RtString bkmname, const CParameterList &params) = 0;

	virtual RtVoid preArchiveRecord(RtToken type, RtString line) = 0;
	virtual RtVoid preReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params) = 0;

	virtual RtVoid preIfBegin(RtString expr) = 0;
	virtual RtVoid preElseIfBegin(RtString expr) = 0; // was RiElseIf (renamed to match elseBegin()
	virtual RtVoid preElseBegin(void) = 0; // was RiElse, else is a keyword ... moan, may be a reason for uppercase method names Else()
	virtual RtVoid preIfEnd(void) = 0;

	/** The interface functions similar to IRiCPP, RtToken are tokenized strings here (CBaseRenderer used CTokenMap)
	 */
	//@{

	/** @brief Error handler
	 *
	 *  @se preErrorHandler()
	 *
	 *  @param handler The error handler
	 */
	virtual RtVoid doErrorHandler(const IErrorHandler &handler) = 0;

	/** @brief used to control the renderer has no rib binding
	 *
	 *  The tokens have special meaning to the renderer and are not declared
	 *
	 */
	inline virtual RtVoid doControl(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {}

	/** @brief Called after declaration is done
	 * 
	 * After the declaration was performed, this callback is called with the new tokenized name.
	 *
	 * @param name Unique token for the declaration
	 * @param declaration declaration string
	 * @see CTokenMap, CBaseRenderer::declare()
	 */
	virtual RtVoid doDeclare(RtToken name, RtString declaration) = 0;
	virtual RtVoid doSynchronize(RtToken name) = 0;
	virtual RtVoid doSystem(RtString cmd) = 0;
	virtual RtVoid doResource(RtString handle, RtString type, const CParameterList &params) = 0;

	virtual RtVoid doBegin(RtString name, const CParameterList &params) = 0;
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

	virtual RtVoid doObjectBegin(RtObjectHandle handle) = 0;
	virtual RtVoid doObjectEnd(void) = 0;
    virtual RtVoid doObjectInstance(RtObjectHandle handle) = 0;

    virtual RtVoid doMotionBegin(RtInt N, RtFloat times[]) = 0;
    virtual RtVoid doMotionEnd(void) = 0;

	virtual RtVoid doResourceBegin(void) = 0;
	virtual RtVoid doResourceEnd(void) = 0;

	virtual RtVoid doArchiveBegin(RtArchiveHandle h, RtToken name, const CParameterList &params) = 0;
	virtual RtVoid doArchiveEnd(void) = 0;

    virtual RtVoid doFormat(RtInt xres, RtInt yres, RtFloat aspect) = 0;
    virtual RtVoid doFrameAspectRatio(RtFloat aspect) = 0;

	virtual RtVoid doScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) = 0;
    virtual RtVoid doCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) = 0;
	virtual RtVoid doProjection(RtToken name, const CParameterList &params) = 0;
	virtual RtVoid doClipping(RtFloat hither, RtFloat yon) = 0;
    virtual RtVoid doClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) = 0;
    virtual RtVoid doDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) = 0;
    virtual RtVoid doShutter(RtFloat smin, RtFloat smax) = 0;
	virtual RtVoid doPixelVariance(RtFloat variation) = 0;
    virtual RtVoid doPixelSamples(RtFloat xsamples, RtFloat ysamples) = 0;
    virtual RtVoid doPixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) = 0;
    virtual RtVoid doExposure(RtFloat gain, RtFloat gamma) = 0;
    virtual RtVoid doImager(RtString name, const CParameterList &params) = 0;
	virtual RtVoid doQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) = 0;
    virtual RtVoid doDisplayChannel(RtString channel, const CParameterList &params) = 0;
    virtual RtVoid doDisplay(RtString name, RtToken type, RtString mode, const CParameterList &params) = 0;
    virtual RtVoid doHider(RtToken type, const CParameterList &params) = 0;
    virtual RtVoid doColorSamples(RtInt N, RtFloat nRGB[], RtFloat RGBn[]) = 0;
    virtual RtVoid doRelativeDetail(RtFloat relativedetail) = 0;
    virtual RtVoid doOption(RtString name, const CParameterList &params) = 0;
	
    virtual RtVoid doLightSource(RtLightHandle h, RtString name, const CParameterList &params) = 0;
	virtual RtVoid doAreaLightSource(RtLightHandle h, RtString name, const CParameterList &params) = 0;
	
    virtual RtVoid doAttribute(RtString name, const CParameterList &params) = 0;
	virtual RtVoid doColor(RtColor Cs) = 0;
	virtual RtVoid doOpacity(RtColor Os) = 0;
    virtual RtVoid doSurface(RtString name, const CParameterList &params) = 0;
    virtual RtVoid doAtmosphere(RtString name, const CParameterList &params) = 0;
    virtual RtVoid doInterior(RtString name, const CParameterList &params) = 0;
	virtual RtVoid doExterior(RtString name, const CParameterList &params) = 0;
	virtual RtVoid doIlluminate(RtLightHandle light, RtBoolean onoff) = 0;
    virtual RtVoid doDisplacement(RtString name, const CParameterList &params) = 0;
	virtual RtVoid doTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) = 0;
    virtual RtVoid doShadingRate(RtFloat size) = 0;
	virtual RtVoid doShadingInterpolation(RtToken type) = 0;
    virtual RtVoid doMatte(RtBoolean onoff) = 0;
	virtual RtVoid doBound(RtBound bound) = 0;
	virtual RtVoid doDetail(RtBound bound) = 0;
	virtual RtVoid doDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) = 0;
    virtual RtVoid doGeometricApproximation(RtToken type, RtFloat value) = 0;
	virtual RtVoid doGeometricRepresentation(RtToken type) = 0;
	virtual RtVoid doOrientation(RtToken anOrientation) = 0;
	virtual RtVoid doReverseOrientation(void) = 0;
	virtual RtVoid doSides(RtInt nsides) = 0;
    virtual RtVoid doBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) = 0;
    virtual RtVoid doTrimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]) = 0;

	virtual RtVoid doIdentity(void) = 0;
	virtual RtVoid doTransform(RtMatrix aTransform) = 0;
	virtual RtVoid doConcatTransform(RtMatrix aTransform) = 0;
	virtual RtVoid doPerspective(RtFloat fov) = 0;
	virtual RtVoid doTranslate(RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid doRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid doScale(RtFloat dx, RtFloat dy, RtFloat dz) = 0;
    virtual RtVoid doSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) = 0;
	virtual RtVoid doDeformation(RtString name, const CParameterList &params) = 0;
	virtual RtVoid doScopedCoordinateSystem(RtToken space) = 0;
	virtual RtVoid doCoordinateSystem(RtToken space) = 0;
	virtual RtVoid doCoordSysTransform(RtToken space) = 0;
	virtual RtPoint *doTransformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) = 0;

    virtual RtVoid doPolygon(RtInt nvertices, const CParameterList &params) = 0;
	virtual RtVoid doGeneralPolygon(RtInt nloops, RtInt nverts[], const CParameterList &params) = 0;
	virtual RtVoid doPointsPolygons(RtInt npolys, RtInt nverts[], RtInt verts[], const CParameterList &params) = 0;
    virtual RtVoid doPointsGeneralPolygons(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], const CParameterList &params) = 0;

	virtual RtVoid doPatch(RtToken type, const CParameterList &params) = 0;
	virtual RtVoid doPatchMesh(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params) = 0;
    virtual RtVoid doNuPatch(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, const CParameterList &params) = 0;

	virtual RtVoid doSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params) = 0;
	virtual RtVoid doHierarchicalSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params) = 0;

	virtual RtVoid doSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid doCone(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid doCylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid doHyperboloid(RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid doParaboloid(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid doDisk(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid doTorus(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params) = 0;

    virtual RtVoid doPoints(RtInt npts, const CParameterList &params) = 0;
    virtual RtVoid doCurves(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params) = 0;

	virtual RtVoid doBlobby(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params) = 0;

	virtual RtVoid doProcedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc *freefunc) = 0;

	virtual RtVoid doGeometry(RtToken type, const CParameterList &params) = 0;

	virtual RtVoid doMakeTexture(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid doMakeBump(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid doMakeLatLongEnvironment(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid doMakeCubeFaceEnvironment(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid doMakeShadow(RtString pic, RtString tex, const CParameterList &params) = 0;
    virtual RtVoid doMakeBrickMap(RtInt nNames, RtString ptcnames[], RtString bkmname, const CParameterList &params) = 0;

	virtual RtVoid doArchiveRecord(RtToken type, RtString line) = 0;
	virtual RtVoid doReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params) = 0;

	virtual RtVoid doIfBegin(RtString expr) = 0;
	virtual RtVoid doElseIfBegin(RtString expr) = 0; // was RiElseIf (renamed to match elseBegin()
	virtual RtVoid doElseBegin(void) = 0; // was RiElse, else is a keyword ... moan, may be a reason for uppercase method names Else()
	virtual RtVoid doIfEnd(void) = 0;


	virtual RtVoid postErrorHandler(const IErrorHandler &handler) = 0;

	virtual RtVoid postDeclare(RtToken name, RtString declaration) = 0;
	virtual RtVoid postSynchronize(RtToken name) = 0;
	virtual RtVoid postSystem(RtString cmd) = 0;
	virtual RtVoid postResource(RtString handle, RtString type, const CParameterList &params) = 0;

	virtual RtVoid postBegin(RtString name, const CParameterList &params) = 0;
	virtual RtVoid postEnd(void) = 0;

	virtual RtVoid postFrameBegin(RtInt number) = 0;
	virtual RtVoid postFrameEnd(void) = 0;

	virtual RtVoid postWorldBegin(void) = 0;
	virtual RtVoid postWorldEnd(void) = 0;

	virtual RtVoid postAttributeBegin(void) = 0;
	virtual RtVoid postAttributeEnd(void) = 0;

	virtual RtVoid postTransformBegin(void) = 0;
	virtual RtVoid postTransformEnd(void) = 0;

    virtual RtVoid postSolidBegin(RtToken type) = 0;
    virtual RtVoid postSolidEnd(void) = 0;

	virtual RtVoid postObjectBegin(RtObjectHandle h) = 0;
	virtual RtVoid postObjectEnd(void) = 0;
    virtual RtVoid postObjectInstance(RtObjectHandle handle) = 0;

    virtual RtVoid postMotionBegin(RtInt N, RtFloat times[]) = 0;
    virtual RtVoid postMotionEnd(void) = 0;

	virtual RtVoid postResourceBegin(void) = 0;
	virtual RtVoid postResourceEnd(void) = 0;

	virtual RtVoid postArchiveBegin(RtArchiveHandle h, RtToken name, const CParameterList &params) = 0;
	virtual RtVoid postArchiveEnd(void) = 0;

    virtual RtVoid postFormat(RtInt xres, RtInt yres, RtFloat aspect) = 0;
    virtual RtVoid postFrameAspectRatio(RtFloat aspect) = 0;

	virtual RtVoid postScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) = 0;
    virtual RtVoid postCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) = 0;
	virtual RtVoid postProjection(RtToken name, const CParameterList &params) = 0;
	virtual RtVoid postClipping(RtFloat hither, RtFloat yon) = 0;
    virtual RtVoid postClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) = 0;
    virtual RtVoid postDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) = 0;
    virtual RtVoid postShutter(RtFloat smin, RtFloat smax) = 0;
	virtual RtVoid postPixelVariance(RtFloat variation) = 0;
    virtual RtVoid postPixelSamples(RtFloat xsamples, RtFloat ysamples) = 0;
    virtual RtVoid postPixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) = 0;
    virtual RtVoid postExposure(RtFloat gain, RtFloat gamma) = 0;
    virtual RtVoid postImager(RtString name, const CParameterList &params) = 0;
	virtual RtVoid postQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) = 0;
    virtual RtVoid postDisplayChannel(RtString channel, const CParameterList &params) = 0;
    virtual RtVoid postDisplay(RtString name, RtToken type, RtString mode, const CParameterList &params) = 0;
    virtual RtVoid postHider(RtToken type, const CParameterList &params) = 0;
    virtual RtVoid postColorSamples(RtInt N, RtFloat nRGB[], RtFloat RGBn[]) = 0;
    virtual RtVoid postRelativeDetail(RtFloat relativedetail) = 0;
    virtual RtVoid postOption(RtString name, const CParameterList &params) = 0;
	
    virtual RtVoid postLightSource(RtLightHandle h, RtString name, const CParameterList &params) = 0;
	virtual RtVoid postAreaLightSource(RtLightHandle h, RtString name, const CParameterList &params) = 0;
	
    virtual RtVoid postAttribute(RtString name, const CParameterList &params) = 0;
	virtual RtVoid postColor(RtColor Cs) = 0;
	virtual RtVoid postOpacity(RtColor Os) = 0;
    virtual RtVoid postSurface(RtString name, const CParameterList &params) = 0;
    virtual RtVoid postAtmosphere(RtString name, const CParameterList &params) = 0;
    virtual RtVoid postInterior(RtString name, const CParameterList &params) = 0;
	virtual RtVoid postExterior(RtString name, const CParameterList &params) = 0;
	virtual RtVoid postIlluminate(RtLightHandle light, RtBoolean onoff) = 0;
    virtual RtVoid postDisplacement(RtString name, const CParameterList &params) = 0;
	virtual RtVoid postTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) = 0;
    virtual RtVoid postShadingRate(RtFloat size) = 0;
	virtual RtVoid postShadingInterpolation(RtToken type) = 0;
    virtual RtVoid postMatte(RtBoolean onoff) = 0;
	virtual RtVoid postBound(RtBound bound) = 0;
	virtual RtVoid postDetail(RtBound bound) = 0;
	virtual RtVoid postDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) = 0;
    virtual RtVoid postGeometricApproximation(RtToken type, RtFloat value) = 0;
	virtual RtVoid postGeometricRepresentation(RtToken type) = 0;
	virtual RtVoid postOrientation(RtToken anOrientation) = 0;
	virtual RtVoid postReverseOrientation(void) = 0;
	virtual RtVoid postSides(RtInt nsides) = 0;
    virtual RtVoid postBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) = 0;
    virtual RtVoid postTrimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]) = 0;

	virtual RtVoid postIdentity(void) = 0;
	virtual RtVoid postTransform(RtMatrix aTransform) = 0;
	virtual RtVoid postConcatTransform(RtMatrix aTransform) = 0;
	virtual RtVoid postPerspective(RtFloat fov) = 0;
	virtual RtVoid postTranslate(RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid postRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid postScale(RtFloat dx, RtFloat dy, RtFloat dz) = 0;
    virtual RtVoid postSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) = 0;
	virtual RtVoid postDeformation(RtString name, const CParameterList &params) = 0;
	virtual RtVoid postScopedCoordinateSystem(RtToken space) = 0;
	virtual RtVoid postCoordinateSystem(RtToken space) = 0;
	virtual RtVoid postCoordSysTransform(RtToken space) = 0;
	virtual RtPoint *postTransformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) = 0;

    virtual RtVoid postPolygon(RtInt nvertices, const CParameterList &params) = 0;
	virtual RtVoid postGeneralPolygon(RtInt nloops, RtInt nverts[], const CParameterList &params) = 0;
	virtual RtVoid postPointsPolygons(RtInt npolys, RtInt nverts[], RtInt verts[], const CParameterList &params) = 0;
    virtual RtVoid postPointsGeneralPolygons(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], const CParameterList &params) = 0;

	virtual RtVoid postPatch(RtToken type, const CParameterList &params) = 0;
	virtual RtVoid postPatchMesh(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params) = 0;
    virtual RtVoid postNuPatch(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, const CParameterList &params) = 0;

	virtual RtVoid postSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params) = 0;
	virtual RtVoid postHierarchicalSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params) = 0;

	virtual RtVoid postSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid postCone(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid postCylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid postHyperboloid(RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid postParaboloid(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid postDisk(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid postTorus(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params) = 0;

    virtual RtVoid postPoints(RtInt npts, const CParameterList &params) = 0;
    virtual RtVoid postCurves(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params) = 0;

	virtual RtVoid postBlobby(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params) = 0;

	virtual RtVoid postProcedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc *freefunc) = 0;

	virtual RtVoid postGeometry(RtToken type, const CParameterList &params) = 0;

	virtual RtVoid postMakeTexture(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid postMakeBump(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid postMakeLatLongEnvironment(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid postMakeCubeFaceEnvironment(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid postMakeShadow(RtString pic, RtString tex, const CParameterList &params) = 0;
    virtual RtVoid postMakeBrickMap(RtInt nNames, RtString ptcnames[], RtString bkmname, const CParameterList &params) = 0;

	virtual RtVoid postArchiveRecord(RtToken type, RtString line) = 0;
	virtual RtVoid postReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params) = 0;

	virtual RtVoid postIfBegin(RtString expr) = 0;
	virtual RtVoid postElseIfBegin(RtString expr) = 0; // was RiElseIf (renamed to match elseBegin()
	virtual RtVoid postElseBegin(void) = 0; // was RiElse, else is a keyword ... moan, may be a reason for uppercase method names Else()
	virtual RtVoid postIfEnd(void) = 0;

}; // IDoRender
} // namespace RiCPP

#endif // _RICPP_RICONTEXT_DORENDER_H
