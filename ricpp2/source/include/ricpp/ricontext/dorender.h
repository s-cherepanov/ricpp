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
 *  @todo Overload preProcess(), doProcess(), postProcess() using parameter object only (drop
 *        special parameters). E.g.: virtual RtVoid preProcess(CRiSphere &obj) instead of the
 *        preSphere() member function.
 */

#ifndef _RICPP_RICONTEXT_RICONTEXT_H
#include "ricpp/ricontext/ricontext.h"
#endif // _RICPP_RICONTEXT_RICONTEXT_H

namespace RiCPP {

/** @brief Members called called with checked parameters (here from @a CBaseRenderer).
 *
 *  Renderer contexts can inherit from @a CBaseRenderer to implement these
 *  functions to do the rendering. Since this are callbacks, all functions have no return values.
*/
class IDoRender : public IRiContext {
public:
	/** @brief The virtual destructor of the interface
	 */
	inline virtual ~IDoRender() {}

	/** @brief Checks validity of state for common requests.
	 *  @param req Number of the requests.
	 *  @return false if checking fails.
	 *  @exception ExceptRiCPPError Implementation can also throw an exception on errors.
	 */
	virtual bool preCheck(EnumRequests req) = 0;

	/** Replays a single interface call
	 */
	virtual void replayRequest(class CRManInterfaceCall &aRequest, const IArchiveCallback *cb) = 0;

	/** @brief Special context handling (@see IRiContext::abort(), IRiContext::activate(), IRiContext::deactivate()
	 */
	//@{
	virtual RtVoid doAbort(void) = 0;
	virtual RtVoid doActivate(void) = 0;
	virtual RtVoid doDeactivate(void) = 0;
	//@}

	////////////////////////////////

	/** The interface functions similar to IRiCPP, RtToken are tokenized strings here (CBaseRenderer used CTokenMap)
	 */
	//@{
	virtual RtVoid preSynchronize(CRiSynchronize &obj, RtToken name) = 0;

	virtual RtVoid preErrorHandler(CRiErrorHandler &obj, const IErrorHandler &handler) = 0;
	virtual RtVoid preDeclare(CRiDeclare &obj, RtToken name, RtString declaration) = 0;
    virtual RtVoid preControl(CRiControl &obj, RtToken name, const CParameterList &params) = 0;

	virtual RtVoid preSystem(CRiSystem &obj, RtString cmd) = 0;

	virtual RtVoid preBegin(CRiBegin &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid preEnd(CRiEnd &obj) = 0;

	virtual RtVoid preFrameBegin(CRiFrameBegin &obj, RtInt number) = 0;
	virtual RtVoid preFrameEnd(CRiFrameEnd &obj) = 0;

	virtual RtVoid preWorldBegin(CRiWorldBegin &obj) = 0;
	virtual RtVoid preWorldEnd(CRiWorldEnd &obj) = 0;

	virtual RtVoid preAttributeBegin(CRiAttributeBegin &obj) = 0;
	virtual RtVoid preAttributeEnd(CRiAttributeEnd &obj) = 0;

	virtual RtVoid preTransformBegin(CRiTransformBegin &obj) = 0;
	virtual RtVoid preTransformEnd(CRiTransformEnd &obj) = 0;

    virtual RtVoid preSolidBegin(CRiSolidBegin &obj, RtToken type) = 0;
    virtual RtVoid preSolidEnd(CRiSolidEnd &obj) = 0;

	virtual RtVoid preObjectBegin(CRiObjectBegin &obj, RtString name) = 0;
	virtual RtVoid preObjectEnd(CRiObjectEnd &obj) = 0;
    virtual RtVoid preObjectInstance(CRiObjectInstance &obj, RtObjectHandle handle) = 0;

    virtual RtVoid preMotionBegin(CRiMotionBegin &obj, RtInt N, RtFloat times[]) = 0;
    virtual RtVoid preMotionEnd(CRiMotionEnd &obj) = 0;

	virtual RtVoid preResourceBegin(CRiResourceBegin &obj) = 0;
	virtual RtVoid preResourceEnd(CRiResourceEnd &obj) = 0;
	virtual RtVoid preResource(CRiResource &obj, RtToken handle, RtToken type, const CParameterList &params) = 0;

	virtual RtVoid preArchiveBegin(CRiArchiveBegin &obj, RtToken name, const CParameterList &params) = 0;
	virtual RtVoid preArchiveEnd(CRiArchiveEnd &objArchiveEnd) = 0;

    virtual RtVoid preFormat(CRiFormat &obj, RtInt xres, RtInt yres, RtFloat aspect) = 0;
    virtual RtVoid preFrameAspectRatio(CRiFrameAspectRatio &obj, RtFloat aspect) = 0;

	virtual RtVoid preScreenWindow(CRiScreenWindow &obj, RtFloat left, RtFloat right, RtFloat bot, RtFloat top) = 0;
    virtual RtVoid preCropWindow(CRiCropWindow &obj, RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) = 0;
	virtual RtVoid preProjection(CRiProjection &obj, RtToken name, const CParameterList &params) = 0;
	virtual RtVoid preClipping(CRiClipping &obj, RtFloat hither, RtFloat yon) = 0;
    virtual RtVoid preClippingPlane(CRiClippingPlane &obj, RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) = 0;
    virtual RtVoid preDepthOfField(CRiDepthOfField &obj, RtFloat fstop, RtFloat focallength, RtFloat focaldistance) = 0;
    virtual RtVoid preShutter(CRiShutter &obj, RtFloat smin, RtFloat smax) = 0;
	virtual RtVoid prePixelVariance(CRiPixelVariance &obj, RtFloat variation) = 0;
    virtual RtVoid prePixelSamples(CRiPixelSamples &obj, RtFloat xsamples, RtFloat ysamples) = 0;
    virtual RtVoid prePixelFilter(CRiPixelFilter &obj, const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) = 0;
    virtual RtVoid preExposure(CRiExposure &obj, RtFloat gain, RtFloat gamma) = 0;
    virtual RtVoid preImager(CRiImager &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid preQuantize(CRiQuantize &obj, RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) = 0;
    virtual RtVoid preDisplayChannel(CRiDisplayChannel &obj, RtString channel, const CParameterList &params) = 0;
    virtual RtVoid preDisplay(CRiDisplay &obj, RtString name, RtToken type, RtString mode, const CParameterList &params) = 0;
    virtual RtVoid preHider(CRiHider &obj, RtToken type, const CParameterList &params) = 0;
    virtual RtVoid preColorSamples(CRiColorSamples &obj, RtInt N, RtFloat nRGB[], RtFloat RGBn[]) = 0;
    virtual RtVoid preRelativeDetail(CRiRelativeDetail &obj, RtFloat relativedetail) = 0;
    virtual RtVoid preOption(CRiOption &obj, RtToken name, const CParameterList &params) = 0;
	
    virtual RtVoid preLightSource(CRiLightSource &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid preAreaLightSource(CRiAreaLightSource &obj, RtString name, const CParameterList &params) = 0;
	
    virtual RtVoid preAttribute(CRiAttribute &obj, RtToken name, const CParameterList &params) = 0;
	virtual RtVoid preColor(CRiColor &obj, RtColor Cs) = 0;
	virtual RtVoid preOpacity(CRiOpacity &obj, RtColor Os) = 0;
    virtual RtVoid preSurface(CRiSurface &obj, RtString name, const CParameterList &params) = 0;
    virtual RtVoid preAtmosphere(CRiAtmosphere &obj, RtString name, const CParameterList &params) = 0;
    virtual RtVoid preInterior(CRiInterior &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid preExterior(CRiExterior &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid preIlluminate(CRiIlluminate &obj, RtLightHandle light, RtBoolean onoff) = 0;
    virtual RtVoid preDisplacement(CRiDisplacement &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid preTextureCoordinates(CRiTextureCoordinates &obj, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) = 0;
    virtual RtVoid preShadingRate(CRiShadingRate &obj, RtFloat size) = 0;
	virtual RtVoid preShadingInterpolation(CRiShadingInterpolation &obj, RtToken type) = 0;
    virtual RtVoid preMatte(CRiMatte &obj, RtBoolean onoff) = 0;
	virtual RtVoid preBound(CRiBound &obj, RtBound bound) = 0;
	virtual RtVoid preDetail(CRiDetail &obj, RtBound bound) = 0;
	virtual RtVoid preDetailRange(CRiDetailRange &obj, RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) = 0;
    virtual RtVoid preGeometricApproximation(CRiGeometricApproximation &obj, RtToken type, RtFloat value) = 0;
	virtual RtVoid preGeometricRepresentation(CRiGeometricRepresentation &obj, RtToken type) = 0;
	virtual RtVoid preOrientation(CRiOrientation &obj, RtToken anOrientation) = 0;
	virtual RtVoid preReverseOrientation(CRiReverseOrientation &obj) = 0;
	virtual RtVoid preSides(CRiSides &obj, RtInt nsides) = 0;
    virtual RtVoid preBasis(CRiBasis &obj, RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) = 0;
    virtual RtVoid preTrimCurve(CRiTrimCurve &obj, RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]) = 0;

	virtual RtVoid preIdentity(CRiIdentity &obj) = 0;
	virtual RtVoid preTransform(CRiTransform &obj, RtMatrix aTransform) = 0;
	virtual RtVoid preConcatTransform(CRiConcatTransform &obj, RtMatrix aTransform) = 0;
	virtual RtVoid prePerspective(CRiPerspective &obj, RtFloat fov) = 0;
	virtual RtVoid preTranslate(CRiTranslate &obj, RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid preRotate(CRiRotate &obj, RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid preScale(CRiScale &obj, RtFloat dx, RtFloat dy, RtFloat dz) = 0;
    virtual RtVoid preSkew(CRiSkew &obj, RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) = 0;
	virtual RtVoid preDeformation(CRiDeformation &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid preScopedCoordinateSystem(CRiScopedCoordinateSystem &obj, RtToken space) = 0;
	virtual RtVoid preCoordinateSystem(CRiCoordinateSystem &obj, RtToken space) = 0;
	virtual RtVoid preCoordSysTransform(CRiCoordSysTransform &obj, RtToken space) = 0;
	virtual RtVoid preTransformPoints(CRiTransformPoints &obj, RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) = 0;

    virtual RtVoid prePolygon(CRiPolygon &obj, RtInt nvertices, const CParameterList &params) = 0;
	virtual RtVoid preGeneralPolygon(CRiGeneralPolygon &obj, RtInt nloops, RtInt nverts[], const CParameterList &params) = 0;
	virtual RtVoid prePointsPolygons(CRiPointsPolygons &obj, RtInt npolys, RtInt nverts[], RtInt verts[], const CParameterList &params) = 0;
    virtual RtVoid prePointsGeneralPolygons(CRiPointsGeneralPolygons &obj, RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], const CParameterList &params) = 0;

	virtual RtVoid prePatch(CRiPatch &obj, RtToken type, const CParameterList &params) = 0;
	virtual RtVoid prePatchMesh(CRiPatchMesh &obj, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params) = 0;
    virtual RtVoid preNuPatch(CRiNuPatch &obj, RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, const CParameterList &params) = 0;

	virtual RtVoid preSubdivisionMesh(CRiSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params) = 0;
	virtual RtVoid preHierarchicalSubdivisionMesh(CRiHierarchicalSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params) = 0;

	virtual RtVoid preSphere(CRiSphere &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid preCone(CRiCone &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid preCylinder(CRiCylinder &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid preHyperboloid(CRiHyperboloid &obj, RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid preParaboloid(CRiParaboloid &obj, RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid preDisk(CRiDisk &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid preTorus(CRiTorus &obj, RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params) = 0;

    virtual RtVoid prePoints(CRiPoints &obj, RtInt npts, const CParameterList &params) = 0;
    virtual RtVoid preCurves(CRiCurves &obj, RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params) = 0;

	virtual RtVoid preBlobby(CRiBlobby &obj, RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params) = 0;

	virtual RtVoid preProcedural(CRiProcedural &obj, RtPointer data, RtBound bound, ISubdivFunc &subdivfunc, IFreeFunc *freefunc) = 0;

	virtual RtVoid preGeometry(CRiGeometry &obj, RtToken type, const CParameterList &params) = 0;

	virtual RtVoid preMakeTexture(CRiMakeTexture &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid preMakeBump(CRiMakeBump &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid preMakeLatLongEnvironment(CRiMakeLatLongEnvironment &obj, RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid preMakeCubeFaceEnvironment(CRiMakeCubeFaceEnvironment &obj, RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid preMakeShadow(CRiMakeShadow &obj, RtString pic, RtString tex, const CParameterList &params) = 0;
    virtual RtVoid preMakeBrickMap(CRiMakeBrickMap &obj, RtInt nNames, RtString ptcnames[], RtString bkmname, const CParameterList &params) = 0;

	virtual RtVoid preArchiveRecord(CRiArchiveRecord &obj, RtToken type, RtString line) = 0;
	virtual RtVoid preReadArchive(CRiReadArchive &obj, RtString name, const IArchiveCallback *callback, const CParameterList &params) = 0;

	virtual RtVoid preIfBegin(CRiIfBegin &obj, RtString expr) = 0;
	virtual RtVoid preElseIfBegin(CRiElseIfBegin &obj, RtString expr) = 0;
	virtual RtVoid preElseBegin(CRiElseBegin &obj) = 0;
	virtual RtVoid preIfEnd(CRiIfEnd &obj) = 0;

	////////////////////////////////

	virtual RtVoid doSynchronize(CRiSynchronize &obj, RtToken name) = 0;

	virtual RtVoid doErrorHandler(CRiErrorHandler &obj,const IErrorHandler &handler) = 0;
	virtual RtVoid doDeclare(CRiDeclare &obj,RtToken name, RtString declaration) = 0;
    virtual RtVoid doControl(CRiControl &obj,RtToken name, const CParameterList &params) = 0;

	virtual RtVoid doSystem(CRiSystem &obj, RtString cmd) = 0;

	virtual RtVoid doBegin(CRiBegin &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid doEnd(CRiEnd &obj) = 0;

	virtual RtVoid doFrameBegin(CRiFrameBegin &obj, RtInt number) = 0;
	virtual RtVoid doFrameEnd(CRiFrameEnd &obj) = 0;

	virtual RtVoid doWorldBegin(CRiWorldBegin &obj) = 0;
	virtual RtVoid doWorldEnd(CRiWorldEnd &obj) = 0;

	virtual RtVoid doAttributeBegin(CRiAttributeBegin &obj) = 0;
	virtual RtVoid doAttributeEnd(CRiAttributeEnd &obj) = 0;

	virtual RtVoid doTransformBegin(CRiTransformBegin &obj) = 0;
	virtual RtVoid doTransformEnd(CRiTransformEnd &obj) = 0;

    virtual RtVoid doSolidBegin(CRiSolidBegin &obj, RtToken type) = 0;
    virtual RtVoid doSolidEnd(CRiSolidEnd &obj) = 0;

	virtual RtVoid doObjectBegin(CRiObjectBegin &obj, RtString name) = 0;
	virtual RtVoid doObjectEnd(CRiObjectEnd &obj) = 0;
    virtual RtVoid doObjectInstance(CRiObjectInstance &obj, RtObjectHandle handle) = 0;

    virtual RtVoid doMotionBegin(CRiMotionBegin &obj, RtInt N, RtFloat times[]) = 0;
    virtual RtVoid doMotionEnd(CRiMotionEnd &obj) = 0;

	virtual RtVoid doResourceBegin(CRiResourceBegin &obj) = 0;
	virtual RtVoid doResourceEnd(CRiResourceEnd &obj) = 0;
	virtual RtVoid doResource(CRiResource &obj, RtToken handle, RtToken type, const CParameterList &params) = 0;

	virtual RtVoid doArchiveBegin(CRiArchiveBegin &obj, RtToken name, const CParameterList &params) = 0;
	virtual RtVoid doArchiveEnd(CRiArchiveEnd &objArchiveEnd) = 0;

    virtual RtVoid doFormat(CRiFormat &obj, RtInt xres, RtInt yres, RtFloat aspect) = 0;
    virtual RtVoid doFrameAspectRatio(CRiFrameAspectRatio &obj, RtFloat aspect) = 0;

	virtual RtVoid doScreenWindow(CRiScreenWindow &obj, RtFloat left, RtFloat right, RtFloat bot, RtFloat top) = 0;
    virtual RtVoid doCropWindow(CRiCropWindow &obj, RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) = 0;
	virtual RtVoid doProjection(CRiProjection &obj, RtToken name, const CParameterList &params) = 0;
	virtual RtVoid doClipping(CRiClipping &obj, RtFloat hither, RtFloat yon) = 0;
    virtual RtVoid doClippingPlane(CRiClippingPlane &obj, RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) = 0;
    virtual RtVoid doDepthOfField(CRiDepthOfField &obj, RtFloat fstop, RtFloat focallength, RtFloat focaldistance) = 0;
    virtual RtVoid doShutter(CRiShutter &obj, RtFloat smin, RtFloat smax) = 0;
	virtual RtVoid doPixelVariance(CRiPixelVariance &obj, RtFloat variation) = 0;
    virtual RtVoid doPixelSamples(CRiPixelSamples &obj, RtFloat xsamples, RtFloat ysamples) = 0;
    virtual RtVoid doPixelFilter(CRiPixelFilter &obj, const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) = 0;
    virtual RtVoid doExposure(CRiExposure &obj, RtFloat gain, RtFloat gamma) = 0;
    virtual RtVoid doImager(CRiImager &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid doQuantize(CRiQuantize &obj, RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) = 0;
    virtual RtVoid doDisplayChannel(CRiDisplayChannel &obj, RtString channel, const CParameterList &params) = 0;
    virtual RtVoid doDisplay(CRiDisplay &obj, RtString name, RtToken type, RtString mode, const CParameterList &params) = 0;
    virtual RtVoid doHider(CRiHider &obj, RtToken type, const CParameterList &params) = 0;
    virtual RtVoid doColorSamples(CRiColorSamples &obj, RtInt N, RtFloat nRGB[], RtFloat RGBn[]) = 0;
    virtual RtVoid doRelativeDetail(CRiRelativeDetail &obj, RtFloat relativedetail) = 0;
    virtual RtVoid doOption(CRiOption &obj, RtToken name, const CParameterList &params) = 0;
	
    virtual RtVoid doLightSource(CRiLightSource &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid doAreaLightSource(CRiAreaLightSource &obj, RtString name, const CParameterList &params) = 0;
	
    virtual RtVoid doAttribute(CRiAttribute &obj, RtToken name, const CParameterList &params) = 0;
	virtual RtVoid doColor(CRiColor &obj, RtColor Cs) = 0;
	virtual RtVoid doOpacity(CRiOpacity &obj, RtColor Os) = 0;
    virtual RtVoid doSurface(CRiSurface &obj, RtString name, const CParameterList &params) = 0;
    virtual RtVoid doAtmosphere(CRiAtmosphere &obj, RtString name, const CParameterList &params) = 0;
    virtual RtVoid doInterior(CRiInterior &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid doExterior(CRiExterior &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid doIlluminate(CRiIlluminate &obj, RtLightHandle light, RtBoolean onoff) = 0;
    virtual RtVoid doDisplacement(CRiDisplacement &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid doTextureCoordinates(CRiTextureCoordinates &obj, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) = 0;
    virtual RtVoid doShadingRate(CRiShadingRate &obj, RtFloat size) = 0;
	virtual RtVoid doShadingInterpolation(CRiShadingInterpolation &obj, RtToken type) = 0;
    virtual RtVoid doMatte(CRiMatte &obj, RtBoolean onoff) = 0;
	virtual RtVoid doBound(CRiBound &obj, RtBound bound) = 0;
	virtual RtVoid doDetail(CRiDetail &obj, RtBound bound) = 0;
	virtual RtVoid doDetailRange(CRiDetailRange &obj, RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) = 0;
    virtual RtVoid doGeometricApproximation(CRiGeometricApproximation &obj, RtToken type, RtFloat value) = 0;
	virtual RtVoid doGeometricRepresentation(CRiGeometricRepresentation &obj, RtToken type) = 0;
	virtual RtVoid doOrientation(CRiOrientation &obj, RtToken anOrientation) = 0;
	virtual RtVoid doReverseOrientation(CRiReverseOrientation &obj) = 0;
	virtual RtVoid doSides(CRiSides &obj, RtInt nsides) = 0;
    virtual RtVoid doBasis(CRiBasis &obj, RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) = 0;
    virtual RtVoid doTrimCurve(CRiTrimCurve &obj, RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]) = 0;

	virtual RtVoid doIdentity(CRiIdentity &obj) = 0;
	virtual RtVoid doTransform(CRiTransform &obj, RtMatrix aTransform) = 0;
	virtual RtVoid doConcatTransform(CRiConcatTransform &obj, RtMatrix aTransform) = 0;
	virtual RtVoid doPerspective(CRiPerspective &obj, RtFloat fov) = 0;
	virtual RtVoid doTranslate(CRiTranslate &obj, RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid doRotate(CRiRotate &obj, RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid doScale(CRiScale &obj, RtFloat dx, RtFloat dy, RtFloat dz) = 0;
    virtual RtVoid doSkew(CRiSkew &obj, RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) = 0;
	virtual RtVoid doDeformation(CRiDeformation &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid doScopedCoordinateSystem(CRiScopedCoordinateSystem &obj, RtToken space) = 0;
	virtual RtVoid doCoordinateSystem(CRiCoordinateSystem &obj, RtToken space) = 0;
	virtual RtVoid doCoordSysTransform(CRiCoordSysTransform &obj, RtToken space) = 0;
	virtual RtVoid doTransformPoints(CRiTransformPoints &obj, RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) = 0;

    virtual RtVoid doPolygon(CRiPolygon &obj, RtInt nvertices, const CParameterList &params) = 0;
	virtual RtVoid doGeneralPolygon(CRiGeneralPolygon &obj, RtInt nloops, RtInt nverts[], const CParameterList &params) = 0;
	virtual RtVoid doPointsPolygons(CRiPointsPolygons &obj, RtInt npolys, RtInt nverts[], RtInt verts[], const CParameterList &params) = 0;
    virtual RtVoid doPointsGeneralPolygons(CRiPointsGeneralPolygons &obj, RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], const CParameterList &params) = 0;

	virtual RtVoid doPatch(CRiPatch &obj, RtToken type, const CParameterList &params) = 0;
	virtual RtVoid doPatchMesh(CRiPatchMesh &obj, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params) = 0;
    virtual RtVoid doNuPatch(CRiNuPatch &obj, RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, const CParameterList &params) = 0;

	virtual RtVoid doSubdivisionMesh(CRiSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params) = 0;
	virtual RtVoid doHierarchicalSubdivisionMesh(CRiHierarchicalSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params) = 0;

	virtual RtVoid doSphere(CRiSphere &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid doCone(CRiCone &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid doCylinder(CRiCylinder &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid doHyperboloid(CRiHyperboloid &obj, RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid doParaboloid(CRiParaboloid &obj, RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid doDisk(CRiDisk &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid doTorus(CRiTorus &obj, RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params) = 0;

    virtual RtVoid doPoints(CRiPoints &obj, RtInt npts, const CParameterList &params) = 0;
    virtual RtVoid doCurves(CRiCurves &obj, RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params) = 0;

	virtual RtVoid doBlobby(CRiBlobby &obj, RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params) = 0;

	virtual RtVoid doProcedural(CRiProcedural &obj, RtPointer data, RtBound bound, ISubdivFunc &subdivfunc, IFreeFunc *freefunc) = 0;

	virtual RtVoid doGeometry(CRiGeometry &obj, RtToken type, const CParameterList &params) = 0;

	virtual RtVoid doMakeTexture(CRiMakeTexture &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid doMakeBump(CRiMakeBump &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid doMakeLatLongEnvironment(CRiMakeLatLongEnvironment &obj, RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid doMakeCubeFaceEnvironment(CRiMakeCubeFaceEnvironment &obj, RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid doMakeShadow(CRiMakeShadow &obj, RtString pic, RtString tex, const CParameterList &params) = 0;
    virtual RtVoid doMakeBrickMap(CRiMakeBrickMap &obj, RtInt nNames, RtString ptcnames[], RtString bkmname, const CParameterList &params) = 0;

	virtual RtVoid doArchiveRecord(CRiArchiveRecord &obj, RtToken type, RtString line) = 0;
	virtual RtVoid doReadArchive(CRiReadArchive &obj, RtString name, const IArchiveCallback *callback, const CParameterList &params) = 0;

	virtual RtVoid doIfBegin(CRiIfBegin &obj, RtString expr) = 0;
	virtual RtVoid doElseIfBegin(CRiElseIfBegin &obj, RtString expr) = 0;
	virtual RtVoid doElseBegin(CRiElseBegin &obj) = 0;
	virtual RtVoid doIfEnd(CRiIfEnd &obj) = 0;

	////////////////////////////////

	virtual RtVoid postSynchronize(CRiSynchronize &obj, RtToken name) = 0;

	virtual RtVoid postErrorHandler(CRiErrorHandler &obj,const IErrorHandler &handler) = 0;
	virtual RtVoid postDeclare(CRiDeclare &obj,RtToken name, RtString declaration) = 0;
    virtual RtVoid postControl(CRiControl &obj,RtToken name, const CParameterList &params) = 0;

	virtual RtVoid postSystem(CRiSystem &obj, RtString cmd) = 0;

	virtual RtVoid postBegin(CRiBegin &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid postEnd(CRiEnd &obj) = 0;

	virtual RtVoid postFrameBegin(CRiFrameBegin &obj, RtInt number) = 0;
	virtual RtVoid postFrameEnd(CRiFrameEnd &obj) = 0;

	virtual RtVoid postWorldBegin(CRiWorldBegin &obj) = 0;
	virtual RtVoid postWorldEnd(CRiWorldEnd &obj) = 0;

	virtual RtVoid postAttributeBegin(CRiAttributeBegin &obj) = 0;
	virtual RtVoid postAttributeEnd(CRiAttributeEnd &obj) = 0;

	virtual RtVoid postTransformBegin(CRiTransformBegin &obj) = 0;
	virtual RtVoid postTransformEnd(CRiTransformEnd &obj) = 0;

    virtual RtVoid postSolidBegin(CRiSolidBegin &obj, RtToken type) = 0;
    virtual RtVoid postSolidEnd(CRiSolidEnd &obj) = 0;

	virtual RtVoid postObjectBegin(CRiObjectBegin &obj, RtString name) = 0;
	virtual RtVoid postObjectEnd(CRiObjectEnd &obj) = 0;
    virtual RtVoid postObjectInstance(CRiObjectInstance &obj, RtObjectHandle handle) = 0;

    virtual RtVoid postMotionBegin(CRiMotionBegin &obj, RtInt N, RtFloat times[]) = 0;
    virtual RtVoid postMotionEnd(CRiMotionEnd &obj) = 0;

	virtual RtVoid postResourceBegin(CRiResourceBegin &obj) = 0;
	virtual RtVoid postResourceEnd(CRiResourceEnd &obj) = 0;
	virtual RtVoid postResource(CRiResource &obj, RtToken handle, RtToken type, const CParameterList &params) = 0;

	virtual RtVoid postArchiveBegin(CRiArchiveBegin &obj, RtToken name, const CParameterList &params) = 0;
	virtual RtVoid postArchiveEnd(CRiArchiveEnd &objArchiveEnd) = 0;

    virtual RtVoid postFormat(CRiFormat &obj, RtInt xres, RtInt yres, RtFloat aspect) = 0;
    virtual RtVoid postFrameAspectRatio(CRiFrameAspectRatio &obj, RtFloat aspect) = 0;

	virtual RtVoid postScreenWindow(CRiScreenWindow &obj, RtFloat left, RtFloat right, RtFloat bot, RtFloat top) = 0;
    virtual RtVoid postCropWindow(CRiCropWindow &obj, RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) = 0;
	virtual RtVoid postProjection(CRiProjection &obj, RtToken name, const CParameterList &params) = 0;
	virtual RtVoid postClipping(CRiClipping &obj, RtFloat hither, RtFloat yon) = 0;
    virtual RtVoid postClippingPlane(CRiClippingPlane &obj, RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) = 0;
    virtual RtVoid postDepthOfField(CRiDepthOfField &obj, RtFloat fstop, RtFloat focallength, RtFloat focaldistance) = 0;
    virtual RtVoid postShutter(CRiShutter &obj, RtFloat smin, RtFloat smax) = 0;
	virtual RtVoid postPixelVariance(CRiPixelVariance &obj, RtFloat variation) = 0;
    virtual RtVoid postPixelSamples(CRiPixelSamples &obj, RtFloat xsamples, RtFloat ysamples) = 0;
    virtual RtVoid postPixelFilter(CRiPixelFilter &obj, const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) = 0;
    virtual RtVoid postExposure(CRiExposure &obj, RtFloat gain, RtFloat gamma) = 0;
    virtual RtVoid postImager(CRiImager &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid postQuantize(CRiQuantize &obj, RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) = 0;
    virtual RtVoid postDisplayChannel(CRiDisplayChannel &obj, RtString channel, const CParameterList &params) = 0;
    virtual RtVoid postDisplay(CRiDisplay &obj, RtString name, RtToken type, RtString mode, const CParameterList &params) = 0;
    virtual RtVoid postHider(CRiHider &obj, RtToken type, const CParameterList &params) = 0;
    virtual RtVoid postColorSamples(CRiColorSamples &obj, RtInt N, RtFloat nRGB[], RtFloat RGBn[]) = 0;
    virtual RtVoid postRelativeDetail(CRiRelativeDetail &obj, RtFloat relativedetail) = 0;
    virtual RtVoid postOption(CRiOption &obj, RtToken name, const CParameterList &params) = 0;
	
    virtual RtVoid postLightSource(CRiLightSource &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid postAreaLightSource(CRiAreaLightSource &obj, RtString name, const CParameterList &params) = 0;
	
    virtual RtVoid postAttribute(CRiAttribute &obj, RtToken name, const CParameterList &params) = 0;
	virtual RtVoid postColor(CRiColor &obj, RtColor Cs) = 0;
	virtual RtVoid postOpacity(CRiOpacity &obj, RtColor Os) = 0;
    virtual RtVoid postSurface(CRiSurface &obj, RtString name, const CParameterList &params) = 0;
    virtual RtVoid postAtmosphere(CRiAtmosphere &obj, RtString name, const CParameterList &params) = 0;
    virtual RtVoid postInterior(CRiInterior &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid postExterior(CRiExterior &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid postIlluminate(CRiIlluminate &obj, RtLightHandle light, RtBoolean onoff) = 0;
    virtual RtVoid postDisplacement(CRiDisplacement &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid postTextureCoordinates(CRiTextureCoordinates &obj, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) = 0;
    virtual RtVoid postShadingRate(CRiShadingRate &obj, RtFloat size) = 0;
	virtual RtVoid postShadingInterpolation(CRiShadingInterpolation &obj, RtToken type) = 0;
    virtual RtVoid postMatte(CRiMatte &obj, RtBoolean onoff) = 0;
	virtual RtVoid postBound(CRiBound &obj, RtBound bound) = 0;
	virtual RtVoid postDetail(CRiDetail &obj, RtBound bound) = 0;
	virtual RtVoid postDetailRange(CRiDetailRange &obj, RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) = 0;
    virtual RtVoid postGeometricApproximation(CRiGeometricApproximation &obj, RtToken type, RtFloat value) = 0;
	virtual RtVoid postGeometricRepresentation(CRiGeometricRepresentation &obj, RtToken type) = 0;
	virtual RtVoid postOrientation(CRiOrientation &obj, RtToken anOrientation) = 0;
	virtual RtVoid postReverseOrientation(CRiReverseOrientation &obj) = 0;
	virtual RtVoid postSides(CRiSides &obj, RtInt nsides) = 0;
    virtual RtVoid postBasis(CRiBasis &obj, RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) = 0;
    virtual RtVoid postTrimCurve(CRiTrimCurve &obj, RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]) = 0;

	virtual RtVoid postIdentity(CRiIdentity &obj) = 0;
	virtual RtVoid postTransform(CRiTransform &obj, RtMatrix aTransform) = 0;
	virtual RtVoid postConcatTransform(CRiConcatTransform &obj, RtMatrix aTransform) = 0;
	virtual RtVoid postPerspective(CRiPerspective &obj, RtFloat fov) = 0;
	virtual RtVoid postTranslate(CRiTranslate &obj, RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid postRotate(CRiRotate &obj, RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid postScale(CRiScale &obj, RtFloat dx, RtFloat dy, RtFloat dz) = 0;
    virtual RtVoid postSkew(CRiSkew &obj, RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) = 0;
	virtual RtVoid postDeformation(CRiDeformation &obj, RtString name, const CParameterList &params) = 0;
	virtual RtVoid postScopedCoordinateSystem(CRiScopedCoordinateSystem &obj, RtToken space) = 0;
	virtual RtVoid postCoordinateSystem(CRiCoordinateSystem &obj, RtToken space) = 0;
	virtual RtVoid postCoordSysTransform(CRiCoordSysTransform &obj, RtToken space) = 0;
	virtual RtVoid postTransformPoints(CRiTransformPoints &obj, RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) = 0;

    virtual RtVoid postPolygon(CRiPolygon &obj, RtInt nvertices, const CParameterList &params) = 0;
	virtual RtVoid postGeneralPolygon(CRiGeneralPolygon &obj, RtInt nloops, RtInt nverts[], const CParameterList &params) = 0;
	virtual RtVoid postPointsPolygons(CRiPointsPolygons &obj, RtInt npolys, RtInt nverts[], RtInt verts[], const CParameterList &params) = 0;
    virtual RtVoid postPointsGeneralPolygons(CRiPointsGeneralPolygons &obj, RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], const CParameterList &params) = 0;

	virtual RtVoid postPatch(CRiPatch &obj, RtToken type, const CParameterList &params) = 0;
	virtual RtVoid postPatchMesh(CRiPatchMesh &obj, RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params) = 0;
    virtual RtVoid postNuPatch(CRiNuPatch &obj, RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, const CParameterList &params) = 0;

	virtual RtVoid postSubdivisionMesh(CRiSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params) = 0;
	virtual RtVoid postHierarchicalSubdivisionMesh(CRiHierarchicalSubdivisionMesh &obj, RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params) = 0;

	virtual RtVoid postSphere(CRiSphere &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid postCone(CRiCone &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid postCylinder(CRiCylinder &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid postHyperboloid(CRiHyperboloid &obj, RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid postParaboloid(CRiParaboloid &obj, RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid postDisk(CRiDisk &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) = 0;
    virtual RtVoid postTorus(CRiTorus &obj, RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params) = 0;

    virtual RtVoid postPoints(CRiPoints &obj, RtInt npts, const CParameterList &params) = 0;
    virtual RtVoid postCurves(CRiCurves &obj, RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params) = 0;

	virtual RtVoid postBlobby(CRiBlobby &obj, RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params) = 0;

	virtual RtVoid postProcedural(CRiProcedural &obj, RtPointer data, RtBound bound, ISubdivFunc &subdivfunc, IFreeFunc *freefunc) = 0;

	virtual RtVoid postGeometry(CRiGeometry &obj, RtToken type, const CParameterList &params) = 0;

	virtual RtVoid postMakeTexture(CRiMakeTexture &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid postMakeBump(CRiMakeBump &obj, RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid postMakeLatLongEnvironment(CRiMakeLatLongEnvironment &obj, RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid postMakeCubeFaceEnvironment(CRiMakeCubeFaceEnvironment &obj, RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) = 0;
    virtual RtVoid postMakeShadow(CRiMakeShadow &obj, RtString pic, RtString tex, const CParameterList &params) = 0;
    virtual RtVoid postMakeBrickMap(CRiMakeBrickMap &obj, RtInt nNames, RtString ptcnames[], RtString bkmname, const CParameterList &params) = 0;

	virtual RtVoid postArchiveRecord(CRiArchiveRecord &obj, RtToken type, RtString line) = 0;
	virtual RtVoid postReadArchive(CRiReadArchive &obj, RtString name, const IArchiveCallback *callback, const CParameterList &params) = 0;

	virtual RtVoid postIfBegin(CRiIfBegin &obj, RtString expr) = 0;
	virtual RtVoid postElseIfBegin(CRiElseIfBegin &obj, RtString expr) = 0;
	virtual RtVoid postElseBegin(CRiElseBegin &obj) = 0;
	virtual RtVoid postIfEnd(CRiIfEnd &obj) = 0;
	//@}
}; // IDoRender
} // namespace RiCPP

#endif // _RICPP_RICONTEXT_DORENDER_H
