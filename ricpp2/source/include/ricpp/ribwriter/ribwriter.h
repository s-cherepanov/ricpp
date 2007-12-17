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

#ifndef _RICPP_RICONTEXT_CONTEXTCREATOR_H
#include "ricpp/ricontext/contextcreator.h"
#endif // _RICPP_RICONTEXT_CONTEXTCREATOR_H

namespace RiCPP {

/** @brief Rendering context that writes a RIB stream
 */
class CRibWriter : public CBaseRenderer {
public:
	inline CRibWriter() {}
	inline virtual ~CRibWriter() {}

	static const char *myName();
	static const char *myType();
	static unsigned long myMajorVersion();
	static unsigned long myMinorVersion();
	static unsigned long myRevision();

	static RtToken myRendererType();

	inline virtual const char *name() const { return myName(); }
	inline virtual const char *type() const { return myType(); }
	inline virtual unsigned long majorVersion() const { return myMajorVersion(); }
	inline virtual unsigned long minorVersion() const { return myMinorVersion(); }
	inline virtual unsigned long revision() const { return myRevision(); }

	inline virtual RtToken rendererType() const { return myRendererType(); }


public:
	inline virtual RtVoid doAbort(void) {}
	inline virtual RtVoid doActivate(void) {}
	inline virtual RtVoid doDeactivate(void) {}

	inline virtual RtVoid doErrorHandler(const IErrorHandler &handler) {}

	inline virtual RtVoid doDeclare(RtToken name, RtString declaration) {}
	inline virtual RtVoid doSynchronize(RtToken name) {}
	inline virtual RtVoid doSystem(RtToken cmd) {}
	inline virtual RtVoid doResource(RtString handle, RtString type, const CParameterList &params) {}

	inline virtual RtVoid doBegin(RtString name, const CParameterList &params) {}
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

	inline virtual RtVoid doObjectBegin(RtObjectHandle h) {}
	inline virtual RtVoid doObjectEnd(void) {}
    inline virtual RtVoid doObjectInstance(RtObjectHandle handle)
	{
		CBaseRenderer::doObjectInstance(handle);
	}

    inline virtual RtVoid doMotionBegin(RtInt N, RtFloat times[]) {}
    inline virtual RtVoid doMotionEnd(void) {}

	inline virtual RtVoid doResourceBegin(void) {}
	inline virtual RtVoid doResourceEnd(void) {}

	inline virtual RtVoid doArchiveBegin(RtArchiveHandle h, RtToken name, const CParameterList &params) {}
	inline virtual RtVoid doArchiveEnd(void) {}

    inline virtual RtVoid doFormat(RtInt xres, RtInt yres, RtFloat aspect) {}
    inline virtual RtVoid doFrameAspectRatio(RtFloat aspect) {}

	inline virtual RtVoid doScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) {}
    inline virtual RtVoid doCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) {}
    inline virtual RtVoid doProjection(RtToken name, const CParameterList &params) {}
	inline virtual RtVoid doClipping(RtFloat hither, RtFloat yon) {}
    inline virtual RtVoid doClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) {}
    inline virtual RtVoid doDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) {}
    inline virtual RtVoid doShutter(RtFloat smin, RtFloat smax) {}
	inline virtual RtVoid doPixelVariance(RtFloat variation) {}
    inline virtual RtVoid doPixelSamples(RtFloat xsamples, RtFloat ysamples) {}
    inline virtual RtVoid doPixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) {}
    inline virtual RtVoid doExposure(RtFloat gain, RtFloat gamma) {}
    inline virtual RtVoid doImager(RtString name, const CParameterList &params) {}
	inline virtual RtVoid doQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) {}
	inline virtual RtVoid doDisplayChannel(RtString channel, const CParameterList &params) {}
    inline virtual RtVoid doDisplay(RtString name, RtToken type, RtString mode, const CParameterList &params) {}
    inline virtual RtVoid doHider(RtToken type, const CParameterList &params) {}
    inline virtual RtVoid doColorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn) {}
    inline virtual RtVoid doRelativeDetail(RtFloat relativedetail) {}
    inline virtual RtVoid doOption(RtString name, const CParameterList &params) {}
	
    inline virtual RtVoid doLightSource(RtLightHandle h, RtString name, const CParameterList &params) {}
	inline virtual RtVoid doAreaLightSource(RtLightHandle h, RtString name, const CParameterList &params) {}

    inline virtual RtVoid doAttribute(RtString name, const CParameterList &params) {}
	inline virtual RtVoid doColor(RtColor Cs) {}
	inline virtual RtVoid doOpacity(RtColor Os) {}
    inline virtual RtVoid doSurface(RtString name, const CParameterList &params) {}
    inline virtual RtVoid doAtmosphere(RtString name, const CParameterList &params) {}
    inline virtual RtVoid doInterior(RtString name, const CParameterList &params) {}
	inline virtual RtVoid doExterior(RtString name, const CParameterList &params) {}
	inline virtual RtVoid doIlluminate(RtLightHandle light, RtBoolean onoff) {}
    inline virtual RtVoid doDisplacement(RtString name, const CParameterList &params) {}
	inline virtual RtVoid doTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) {}
    inline virtual RtVoid doShadingRate(RtFloat size) {}
	inline virtual RtVoid doShadingInterpolation(RtToken type) {}
    inline virtual RtVoid doMatte(RtBoolean onoff) {}
	inline virtual RtVoid doBound(RtBound bound) {}
	inline virtual RtVoid doDetail(RtBound bound) {}
	inline virtual RtVoid doDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) {}
    inline virtual RtVoid doGeometricApproximation(RtToken type, RtFloat value) {}
	inline virtual RtVoid doGeometricRepresentation(RtToken type) {}
	inline virtual RtVoid doOrientation(RtToken anOrientation) {}
	inline virtual RtVoid doReverseOrientation(void) {}
	inline virtual RtVoid doSides(RtInt nsides) {}
    inline virtual RtVoid doBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) {}
    inline virtual RtVoid doTrimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) {}

	inline virtual RtVoid doIdentity(void) {}
	inline virtual RtVoid doTransform(RtMatrix aTransform) {}
	inline virtual RtVoid doConcatTransform(RtMatrix aTransform) {}
	inline virtual RtVoid doPerspective(RtFloat fov) {}
	inline virtual RtVoid doTranslate(RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid doRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {}
	inline virtual RtVoid doScale(RtFloat dx, RtFloat dy, RtFloat dz) {}
    inline virtual RtVoid doSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) {}
	inline virtual RtVoid doDeformation(RtString name, const CParameterList &params) {}
	inline virtual RtVoid doScopedCoordinateSystem(RtToken space) {}
	inline virtual RtVoid doCoordinateSystem(RtToken space) {}
	inline virtual RtVoid doCoordSysTransform(RtToken space) {}

    inline virtual RtVoid doPolygon(RtInt nvertices, const CParameterList &params) {}
	inline virtual RtVoid doGeneralPolygon(RtInt nloops, RtInt *nverts, const CParameterList &params) {}
	inline virtual RtVoid doPointsPolygons(RtInt npolys, RtInt *nverts, RtInt *verts, const CParameterList &params) {}
    inline virtual RtVoid doPointsGeneralPolygons(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts, const CParameterList &params) {}

	inline virtual RtVoid doPatch(RtToken type, const CParameterList &params) {}
	inline virtual RtVoid doPatchMesh(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, const CParameterList &params) {}
    inline virtual RtVoid doNuPatch(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax, const CParameterList &params) {}

	inline virtual RtVoid doSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], const CParameterList &params) {}
	inline virtual RtVoid doHierarchicalSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[], const CParameterList &params) {}

	inline virtual RtVoid doSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid doCone(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid doCylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid doHyperboloid(RtPoint point1, RtPoint point2, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid doParaboloid(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid doDisk(RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params) {}
    inline virtual RtVoid doTorus(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, const CParameterList &params) {}

    inline virtual RtVoid doPoints(RtInt npts, const CParameterList &params) {}
    inline virtual RtVoid doCurves(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, const CParameterList &params) {}

	inline virtual RtVoid doBlobby(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], const CParameterList &params) {}

	inline virtual RtVoid doProcedural(IRi &callee, RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc &freefunc) {}

	inline virtual RtVoid doGeometry(RtToken type, const CParameterList &params) {}

	inline virtual RtVoid doMakeTexture(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
    inline virtual RtVoid doMakeBump(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
    inline virtual RtVoid doMakeLatLongEnvironment(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
    inline virtual RtVoid doMakeCubeFaceEnvironment(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, const CParameterList &params) {}
    inline virtual RtVoid doMakeShadow(RtString pic, RtString tex, const CParameterList &params) {}
	inline virtual RtVoid doMakeBrickMap(RtInt nNames, RtString *ptcnames, RtString bkmname, const CParameterList &params) {}

	inline virtual RtVoid doArchiveRecord(RtToken type, RtString line) {}
	inline virtual RtVoid doReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params)
	{
		CBaseRenderer::doReadArchive(name, callback, params);
	}

	inline virtual RtVoid doIfBegin(RtString expr) {}
	inline virtual RtVoid doElseIfBegin(RtString expr) {}
	inline virtual RtVoid doElseBegin(void) {}
	inline virtual RtVoid doIfEnd(void) {}
};

/** @brief Creator for the CRibWriter rendering context.
 */
class CRibWriterCreator : public CContextCreator {
protected:
	inline virtual IRiContext *getNewContext() { return new CRibWriter; }

public:
	static const char *myName();
	static const char *myType();
	static unsigned long myMajorVersion();
	static unsigned long myMinorVersion();
	static unsigned long myRevision();

	inline virtual const char *name() const { return myName(); }
	inline virtual const char *type() const { return myType(); }
	inline virtual unsigned long majorVersion() const { return myMajorVersion(); }
	inline virtual unsigned long minorVersion() const { return myMinorVersion(); }
	inline virtual unsigned long revision() const { return myRevision(); }

	inline virtual void startup() {}
	inline virtual void shutdown() {}

	inline virtual unsigned long majorContextVersion() const { return CRibWriter::myMajorVersion(); }
	inline virtual unsigned long minorContextVersion() const { return CRibWriter::myMinorVersion(); }
	inline virtual unsigned long contextRevision() const { return CRibWriter::myRevision(); }
	inline virtual RtToken contextName() const { return CRibWriter::myName(); }
	inline virtual RtToken contextType() const { return CRibWriter::myType(); }
	inline virtual RtToken rendererType() const { return CRibWriter::myRendererType(); }
};

} // namespace RiCPP

#endif // _RICPP_RIBWRITER_RIBWRITER_H
