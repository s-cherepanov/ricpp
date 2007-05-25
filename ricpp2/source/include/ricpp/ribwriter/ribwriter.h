#ifndef _RICPP_RIBWRITER_RIBWRITER_H
#define _RICPP_RIBWRITER_RIBWRITER_H

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

/** @file ribwriter.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declaration of a context to write RIB streams
 */

#ifndef _RICPP_BASERENDERER_BASERENDERER_H
#include "ricpp/baserenderer/baserenderer.h"
#endif // _RICPP_BASERENDERER_BASERENDERER_H

#ifndef _RICPP_BASERENDERER_CONTEXTCREATOR_H
#include "ricpp/baserenderer/contextcreator.h"
#endif // _RICPP_BASERENDERER_CONTEXTCREATOR_H

namespace RiCPP {

/** @brief Rendering context that writes a RIB stream
 */
class CRibWriter : public CBaseRenderer {
public:
	inline CRibWriter() {}
	inline virtual ~CRibWriter() {}

	static const unsigned long ribWriterMinorVersion;
	inline virtual unsigned long minorVersion() const { return ribWriterMinorVersion; }
	static const unsigned long ribWriterRevision;
	inline virtual unsigned long revision() const { return ribWriterRevision; }

	static RtToken myRendererName();
	virtual RtToken rendererName() const;
	static RtToken myRendererType();
	virtual RtToken rendererType() const;

protected:
	inline virtual RtVoid doAbort(void) {}
	inline virtual RtVoid doActivate(void) {}
	inline virtual RtVoid doDeactivate(void) {}

	inline virtual RtToken doDeclare(RtToken name, RtString declaration)
	{
		return CBaseRenderer::doDeclare(name, declaration);
	}
	inline virtual RtVoid doSynchronize(RtToken name) {}

	virtual RtToken doResourceV(RtString name, RtToken type, RtInt n, RtToken tokens[], RtPointer parms[])
	{
		return CBaseRenderer::doResourceV(name, type, n, tokens, parms);
	}
	virtual RtVoid doFreeResource(RtToken handle) {}

	inline virtual RtVoid doBegin(RtString name) {}
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

	inline virtual RtObjectHandle doObjectBegin(void)
	{
		return CBaseRenderer::doObjectBegin();
	}
	inline virtual RtVoid doObjectEnd(void) {}
    inline virtual RtVoid doObjectInstance(RtObjectHandle handle) {}
    inline virtual RtVoid doFreeObject(RtObjectHandle handle) {}

    inline virtual RtVoid doMotionBeginV(RtInt N, RtFloat times[]) {}
    inline virtual RtVoid doMotionEnd(void) {}

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
	
    inline virtual RtLightHandle doLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
	{
		return CBaseRenderer::doLightSourceV(name, n, tokens, params);
	}
	inline virtual RtLightHandle doAreaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
	{
		return CBaseRenderer::doAreaLightSourceV(name, n, tokens, params);
	}
	

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
	inline virtual RtPoint *doTransformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[])
	{
		return CBaseRenderer::doTransformPoints(fromspace, tospace, npoints, points);
	}

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

	inline virtual RtVoid doProcedural(IRi &callee, RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc &freefunc) {}

	inline virtual RtVoid doGeometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {}

	inline virtual RtVoid doMakeTextureV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doMakeBumpV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doMakeLatLongEnvironmentV(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doMakeCubeFaceEnvironmentV(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {}
    inline virtual RtVoid doMakeShadowV(RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[]) {}

	inline virtual RtVoid doArchiveRecordV(RtToken type, RtString line) {}
	inline virtual RtVoid doReadArchiveV(IRi &callee, RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[]) {}
};

/** @brief Creator for the CRibWriter rendering context.
 */
class CRibWriterCreator : public CContextCreator {
protected:
	inline virtual IRiContext *getNewContext() { return new CRibWriter; }
	inline virtual unsigned long majorVersion() const { return IRiContext::riContextMajorVersion; }
	inline virtual unsigned long minorVersion() const { return CRibWriter::ribWriterMinorVersion; }
	inline virtual unsigned long revision() const { return CRibWriter::ribWriterRevision; }
	inline virtual RtToken rendererName() const { return CRibWriter::myRendererName(); }
	inline virtual RtToken rendererType() const { return CRibWriter::myRendererType(); }
};


} // namespace RiCPP

#endif // _RICPP_RIBWRITER_RIBWRITER_H
