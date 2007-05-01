#ifndef _RICPP_BASERENDERER_RICONTEXT_H
#define _RICPP_BASERENDERER_RICONTEXT_H

// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 200,, 2005 Pixar
//                 All rights Reservered
//
// Copyright © of RiCPP 2007, Andreas Pidde
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

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

namespace RiCPP {

	
const RtContextHandle illContextHandle = (RtContextHandle)RI_NULL;
const RtObjectHandle illObjectHandle = (RtObjectHandle)RI_NULL;
const RtLightHandle illLightHandle = (RtLightHandle)RI_NULL;
// const RtArchiveHandle illArchiveHandle = (RtArchiveHandle)RI_NULL;

const RtToken ricpp_rib = "RIB";
const RtToken ricpp_preview = "Preview";
const RtToken ricpp_photorealistic = "Photorealistic";

/** RenderMan Interface without the ellipsis (...) calls,
 * interface for a render context
 * called from the CRiCPPBridge
 */
class IRiContext {
	friend class CContextCreator; // can activate and deactivate
protected:
	virtual RtVoid activate(void) = 0;
	virtual RtVoid deactivate(void) = 0;
public:
	static const unsigned long majorVersion;
	static const unsigned long minorVersion;

	/** The virtual destructor of the interface
	 */
	inline virtual ~IRiContext() {}

	virtual RtToken rendererName() = 0;
	virtual RtToken rendererType() = 0;
	virtual RtVoid abort(void) = 0;

	/** The interface functions of IRi
	 */
	virtual RtToken declare(RtString name, RtString declaration) = 0;

	virtual RtVoid synchronize(RtToken name) = 0;

	virtual RtVoid begin(RtString name) = 0;
	virtual RtVoid end(void) = 0;

	virtual RtVoid frameBegin(RtInt number) = 0;
	virtual RtVoid frameEnd(void) = 0;

	virtual RtVoid worldBegin(void) = 0;
	virtual RtVoid worldEnd(void) = 0;

	virtual RtVoid attributeBegin(void) = 0;
	virtual RtVoid attributeEnd(void) = 0;

	virtual RtVoid transformBegin(void) = 0;
	virtual RtVoid transformEnd(void) = 0;

    virtual RtVoid solidBegin(RtToken type) = 0;
    virtual RtVoid solidEnd(void) = 0;

	virtual RtObjectHandle objectBegin(void) = 0;
	virtual RtVoid objectEnd(void) = 0;
    virtual RtVoid objectInstance(RtObjectHandle handle) = 0;

    virtual RtVoid motionBeginV(RtInt N, RtFloat times[]) = 0;
    virtual RtVoid motionEnd(void) = 0;

    virtual RtVoid format(RtInt xres, RtInt yres, RtFloat aspect) = 0;
    virtual RtVoid frameAspectRatio(RtFloat aspect) = 0;

	virtual RtVoid screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) = 0;
    virtual RtVoid cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) = 0;
    virtual RtVoid projectionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid clipping(RtFloat hither, RtFloat yon) = 0;
    virtual RtVoid clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) = 0;
    virtual RtVoid depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) = 0;
    virtual RtVoid shutter(RtFloat smin, RtFloat smax) = 0;
	virtual RtVoid pixelVariance(RtFloat variation) = 0;
    virtual RtVoid pixelSamples(RtFloat xsamples, RtFloat ysamples) = 0;
    virtual RtVoid pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth) = 0;
    virtual RtVoid exposure(RtFloat gain, RtFloat gamma) = 0;
    virtual RtVoid imagerV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) = 0;
    virtual RtVoid displayV(RtString name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid colorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn) = 0;
    virtual RtVoid relativeDetail(RtFloat relativedetail) = 0;
    virtual RtVoid optionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	
    virtual RtLightHandle lightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtLightHandle areaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	

    virtual RtVoid attributeV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid color(RtColor Cs) = 0;
	virtual RtVoid opacity(RtColor Cs) = 0;
    virtual RtVoid surfaceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid atmosphereV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid interiorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid exteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid illuminate(RtLightHandle light, RtBoolean onoff) = 0;
    virtual RtVoid displacementV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) = 0;
    virtual RtVoid shadingRate(RtFloat size) = 0;
	virtual RtVoid shadingInterpolation(RtToken type) = 0;
    virtual RtVoid matte(RtBoolean onoff) = 0;
	virtual RtVoid bound(RtBound bound) = 0;
	virtual RtVoid detail(RtBound bound) = 0;
	virtual RtVoid detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) = 0;
    virtual RtVoid geometricApproximation(RtToken type, RtFloat value) = 0;
	virtual RtVoid geometricRepresentation(RtToken type) = 0;
	virtual RtVoid orientation(RtToken orientation) = 0;
	virtual RtVoid reverseOrientation(void) = 0;
	virtual RtVoid sides(RtInt nsides) = 0;
    virtual RtVoid basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) = 0;
    virtual RtVoid trimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) = 0;

	virtual RtVoid identity(void) = 0;
	virtual RtVoid transform(RtMatrix transform) = 0;
	virtual RtVoid concatTransform(RtMatrix transform) = 0;
	virtual RtVoid perspective(RtFloat fov) = 0;
	virtual RtVoid translate(RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) = 0;
	virtual RtVoid scale(RtFloat dx, RtFloat dy, RtFloat dz) = 0;
    virtual RtVoid skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) = 0;
	virtual RtVoid deformationV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid coordinateSystem(RtToken space) = 0;
	virtual RtVoid coordSysTransform(RtToken space) = 0;
	virtual RtPoint *transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) = 0;

    virtual RtVoid polygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid generalPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid pointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid pointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	virtual RtVoid patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
	virtual RtVoid patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid nuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	virtual RtVoid subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	virtual RtVoid sphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid coneV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid cylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid hyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid paraboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid diskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid torusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

    virtual RtVoid pointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid curvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	virtual RtVoid blobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	virtual RtVoid procedural(IRi &callee, RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc &freefunc) = 0;

	virtual RtVoid geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	virtual RtVoid makeTextureV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid makeBumpV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid makeLatLongEnvironmentV(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid makeCubeFaceEnvironmentV(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) = 0;
    virtual RtVoid makeShadowV(RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	virtual RtVoid archiveRecordV(RtToken type, RtString line) = 0;
	virtual RtVoid readArchiveV(IRi &callee, RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[]) = 0;

	/*
	// -> RenderMan 11.5.2
	virtual RtVoid scopedCoordinateSystem(RtToken space) = 0;

	// -> QRM ?
	virtual RtVoid resourceV(RtToken handle, RtToken type, RtInt n, RtToken tokens[], RtPointer parms[]) = 0;

	// -> 3Delight
    virtual RtArchiveHandle archiveBeginV(RtString name, RtInt n, RtToken tokens[], RtPointer parms[]) = 0;
	virtual RtVoid archiveEnd(void) = 0;
	virtual RtVoid archiveInstance(RtArchiveHandle handle) = 0;

	// -> RenderMan 2004 ???
	virtual RtVoid ifBeginV(RtString expr, RtInt n, RtToken tokens[], RtPointer parms[]) = 0;
	virtual RtVoid elseIfV(RtString expr, RtInt n, RtToken tokens[], RtPointer parms[]) = 0;
	virtual RtVoid elsePart(void) = 0; // was RiElse
	virtual RtVoid ifEnd(void) = 0;
	*/
}; // IRiContext
} // namespace RiCPP

#endif // _RICPP_BASERENDERER_RICONTEXT_H
