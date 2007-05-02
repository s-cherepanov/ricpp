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

/*! \file libmesh.cpp
 *  \brief Building scene graph and meshes
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"

#include "libmesh.h"

#if 0
TRiObjectBegin *TMeshContext::objectInstance(RtObjectHandle handle)
{
	TRInterfaceCall *node = (TRInterfaceCall *)m_objects.first();
	while ( node ) {
		if ( node->interfaceIdx() == IDX_OBJECT_BEGIN ) {
			TRiObjectBegin *inst = (TRiObjectBegin *)node;
			if ( inst->handle2() == handle ) {
				return inst;
			}
		}
	}
	return NULL;
}
#endif


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

TRiCPPContext *TMesh::getNewContext() {
	return new TMeshContext;
}

RtVoid TMesh::doBegin(RtToken name) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndDescent(new TRiBegin(lineNo(), name));
}

RtVoid TMesh::doEnd(void) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndAscent(new TRiEnd(lineNo()));
}


RtVoid TMesh::doFrameBegin(RtInt number) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndDescent(new TRiFrameBegin(lineNo(), number));
}

RtVoid TMesh::doFrameEnd(void){
	((TMeshContext *)m_currentContext)->m_scene->insertAndAscent(new TRiFrameEnd(lineNo()));
}

RtVoid TMesh::doWorldBegin(void) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndDescent(new TRiWorldBegin(lineNo()));
}

RtVoid TMesh::doWorldEnd(void) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndAscent(new TRiWorldEnd(lineNo()));
}


RtVoid TMesh::doAttributeBegin(void) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndDescent(new TRiAttributeBegin(lineNo()));
}

RtVoid TMesh::doAttributeEnd(void) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndAscent(new TRiAttributeEnd(lineNo()));
}


RtVoid TMesh::doTransformBegin(void) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndDescent(new TRiTransformBegin(lineNo()));
}

RtVoid TMesh::doTransformEnd(void) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndAscent(new TRiTransformEnd(lineNo()));
}


RtVoid TMesh::doSolidBegin(RtToken type) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndDescent(new TRiSolidBegin(lineNo(), type));
}

RtVoid TMesh::doSolidEnd(void) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndAscent(new TRiSolidEnd(lineNo()));
}


RtObjectHandle TMesh::doObjectBegin(RtObjectHandle handle) {
//	if ( handle >= (RtObjectHandle)0 ) {
//		((TMeshContext *)m_currentContext)->m_scene->insertAndDescent(new TRiObjectBegin(lineNo(), handle));
//	}
	return handle;
}

RtVoid TMesh::doObjectEnd(void) {
//	((TMeshContext *)m_currentContext)->m_scene->insertAndAscent(new TRiObjectEnd(lineNo()));
}


RtVoid TMesh::doObjectInstance(RtObjectHandle handle) {
	((TMeshContext *)m_currentContext)->m_scene->insert(new TRiObjectInstance(lineNo(), handle));

	// TRiObjectBegin *link = ((TMeshContext *)m_currentContext)->objectInstance(handle);
	// if ( !link ) {
		// Illegal ObjectHandle
	// } else {
	//	((TMeshContext *)m_currentContext)->m_scene->insertAndLink(new TRiObjectInstance(lineNo(), link->handleRef()), link);
	// }
}


RtVoid TMesh::doMotionBeginV(RtInt N, RtFloat times[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndDescent(new TRiMotionBegin(lineNo(), N, times));
}

RtVoid TMesh::doMotionEnd(void) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndAscent(new TRiMotionEnd(lineNo()));
}


RtVoid TMesh::doFormat(RtInt xres, RtInt yres, RtFloat aspect) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiFormat(lineNo(), xres, yres, aspect));
}

RtVoid TMesh::doFrameAspectRatio(RtFloat aspect) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiFrameAspectRatio(lineNo(), aspect));
}

RtVoid TMesh::doScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiScreenWindow(lineNo(), left, right, bot, top));
}

RtVoid TMesh::doCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiCropWindow(lineNo(), xmin, xmax, ymin, ymax));
}

RtVoid TMesh::doProjectionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiProjection(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, name, n, tokens, params));
}

RtVoid TMesh::doClipping(RtFloat hither, RtFloat yon) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiClipping(lineNo(), hither, yon));
}

RtVoid TMesh::doClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiClippingPlane(lineNo(), x, y, z, nx, ny, nz));
}

RtVoid TMesh::doDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiDepthOfField(lineNo(), fstop, focallength, focaldistance));
}

RtVoid TMesh::doShutter(RtFloat smin, RtFloat smax) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiShutter(lineNo(), smin, smax));
}

RtVoid TMesh::doPixelVariance(RtFloat variance) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiPixelVariance(lineNo(), variance));
}

RtVoid TMesh::doPixelSamples(RtFloat xsamples, RtFloat ysamples) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiPixelSamples(lineNo(), xsamples, ysamples));
}

RtVoid TMesh::doPixelFilter(RtFilterFunc function, RtFloat xwidth, RtFloat ywidth) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiPixelFilter(lineNo(), function, xwidth, ywidth));
}

RtVoid TMesh::doExposure(RtFloat gain, RtFloat gamma) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiExposure(lineNo(), gain, gamma));
}

RtVoid TMesh::doImagerV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiImager(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, name, n, tokens, params));
}

RtVoid TMesh::doQuantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiQuantize(lineNo(), type, one, qmin, qmax, ampl));
}

RtVoid TMesh::doDisplayV(RtToken name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiDisplay(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, name, type, mode, n, tokens, params));
}

RtVoid TMesh::doHiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiHider(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, type, n, tokens, params));
}

RtVoid TMesh::doColorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiColorSamples(lineNo(), N, nRGB, RGBn));
}

RtVoid TMesh::doRelativeDetail(RtFloat relativedetail) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiRelativeDetail(lineNo(), relativedetail));
}

RtVoid TMesh::doOptionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiOption(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, name, n, tokens, params));
}


RtLightHandle TMesh::doLightSourceV(RtLightHandle handle, const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiLightSource(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, name, handle, n, tokens, params));
	return handle;
}

RtLightHandle TMesh::doAreaLightSourceV(RtLightHandle handle, const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiAreaLightSource(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, name, handle, n, tokens, params));
	return handle;
}

RtVoid TMesh::doAttributeV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiAttribute(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, name, n, tokens, params));
}

RtVoid TMesh::doColor(RtColor Cs) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiColor(lineNo(), m_currentContext->m_currentOptions->m_iNColor, Cs));
}

RtVoid TMesh::doOpacity(RtColor Cs) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiOpacity(lineNo(), m_currentContext->m_currentOptions->m_iNColor, Cs));
}

RtVoid TMesh::doSurfaceV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiSurface(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, name, n, tokens, params));
}

RtVoid TMesh::doAtmosphereV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiAtmosphere(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, name, n, tokens, params));
}

RtVoid TMesh::doInteriorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiInterior(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, name, n, tokens, params));
}

RtVoid TMesh::doExteriorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiExterior(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, name, n, tokens, params));
}

RtVoid TMesh::doIlluminate(RtLightHandle light, RtBoolean onoff) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiIlluminate(lineNo(), light, onoff));
}

RtVoid TMesh::doDisplacementV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiDisplacement(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, name, n, tokens, params));
}

RtVoid TMesh::doTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiTextureCoordinates(lineNo(), s1, t1, s2, t2, s3, t3, s4, t4));
}

RtVoid TMesh::doShadingRate(RtFloat size) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiShadingRate(lineNo(), size));
}

RtVoid TMesh::doShadingInterpolation(RtToken type) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiShadingInterpolation(lineNo(), type));
}

RtVoid TMesh::doMatte(RtBoolean onoff) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiMatte(lineNo(), onoff));
}

RtVoid TMesh::doBound(RtBound bound) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiBound(lineNo(), bound));
}

RtVoid TMesh::doDetail(RtBound bound) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiDetail(lineNo(), bound));
}

RtVoid TMesh::doDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiDetailRange(lineNo(), minvis, lowtran, uptran, maxvis));
}

RtVoid TMesh::doGeometricApproximation(RtToken type, RtFloat value) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiGeometricApproximation(lineNo(), type, value));
}

RtVoid TMesh::doGeometricRepresentation(RtToken type) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiGeometricRepresentation(lineNo(), type));
}

RtVoid TMesh::doOrientation(RtToken orientation) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiOrientation(lineNo(), orientation));
}

RtVoid TMesh::doReverseOrientation(void) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiReverseOrientation(lineNo()));
}

RtVoid TMesh::doSides(RtInt nsides) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiSides(lineNo(), nsides));
}

RtVoid TMesh::doBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiBasis(lineNo(), ubasis, ustep, vbasis, vstep));
}

RtVoid TMesh::doTrimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiTrimCurve(lineNo(), nloops, ncurves, order, knot, amin, amax, n, u, v, w));
}


RtVoid TMesh::doIdentity(void) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiIdentity(lineNo()));
}

RtVoid TMesh::doTransform(RtMatrix transform) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiTransform(lineNo(), transform));
}

RtVoid TMesh::doConcatTransform(RtMatrix transform) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiConcatTransform(lineNo(), transform));
}

RtVoid TMesh::doPerspective(RtFloat fov) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiPerspective(lineNo(), fov));
}

RtVoid TMesh::doTranslate(RtFloat dx, RtFloat dy, RtFloat dz) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiTranslate(lineNo(), dx, dy, dz));
}

RtVoid TMesh::doRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiRotate(lineNo(), angle, dx, dy, dz));
}

RtVoid TMesh::doScale(RtFloat dx, RtFloat dy, RtFloat dz) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiScale(lineNo(), dx, dy, dz));
}

RtVoid TMesh::doSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiSkew(lineNo(), angle, dx1, dy1, dz1, dx2, dy2, dz2));
}

RtVoid TMesh::doDeformationV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiDeformation(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, name, n, tokens, params));
}

RtVoid TMesh::doCoordinateSystem(RtToken space) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiCoordinateSystem(lineNo(), space));
}

RtVoid TMesh::doCoordSysTransform(RtToken space) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiCoordSysTransform(lineNo(), space));
}


RtVoid TMesh::doPolygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiPolygon(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, nvertices, n, tokens, params));
}

RtVoid TMesh::doGeneralPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiGeneralPolygon(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, nloops, nverts, n, tokens, params));
}

RtVoid TMesh::doPointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiPointsPolygons(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, npolys, nverts, verts, n, tokens, params));
}

RtVoid TMesh::doPointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiPointsGeneralPolygons(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, npolys, nloops, nverts, verts, n, tokens, params));
}


RtVoid TMesh::doPatchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiPatch(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, type, n, tokens, params));
}

RtVoid TMesh::doPatchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiPatchMesh(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, m_currentContext->m_currentAttributes->m_uStep, m_currentContext->m_currentAttributes->m_vStep, type, nu, uwrap, nv, vwrap, n, tokens, params));
}

RtVoid TMesh::doNuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiNuPatch(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, tokens, params));
}


RtVoid TMesh::doSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiSubdivisionMesh(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, n, tokens, params));
}


RtVoid TMesh::doSphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiSphere(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, radius, zmin, zmax, thetamax, n, tokens, params));
}

RtVoid TMesh::doConeV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiCone(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, height, radius, thetamax, n, tokens, params));
}

RtVoid TMesh::doCylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiCylinder(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, radius, zmin, zmax, thetamax, n, tokens, params));
}

RtVoid TMesh::doHyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiHyperboloid(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, point1, point2, thetamax, n, tokens, params));
}

RtVoid TMesh::doParaboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiParaboloid(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, rmax, zmin, zmax, thetamax, n, tokens, params));
}

RtVoid TMesh::doDiskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiDisk(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, height, radius, thetamax, n, tokens, params));
}

RtVoid TMesh::doTorusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiTorus(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, majorrad, minorrad, phimin, phimax, thetamax, n, tokens, params));
}


RtVoid TMesh::doPointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiPoints(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, npts, n, tokens, params));
}

RtVoid TMesh::doCurvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiCurves(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, m_currentContext->m_currentAttributes->m_uStep, m_currentContext->m_currentAttributes->m_vStep, type, ncurves, nverts, wrap, n, tokens, params));
}


RtVoid TMesh::doBlobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiBlobby(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, nleaf, ncode, code, nflt, flt, nstr, str, n, tokens, params));
}


RtVoid TMesh::doProcedural(RtPointer data, RtBound bound, RtProcSubdivFunc subdivfunc, RtProcFreeFunc freefunc) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiProcedural(lineNo(), data, bound, subdivfunc, freefunc, *this));
}


RtVoid TMesh::doGeometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiGeometry(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, type, n, tokens, params));
}


RtVoid TMesh::doMakeTextureV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiMakeTexture(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params));
}

RtVoid TMesh::doMakeBumpV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiMakeBump(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params));
}

RtVoid TMesh::doMakeLatLongEnvironmentV(const char *pic, const char *tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiMakeLatLongEnvironment(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, pic, tex, filterfunc, swidth, twidth, n, tokens, params));
}

RtVoid TMesh::doMakeCubeFaceEnvironmentV(const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz, const char *tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiMakeCubeFaceEnvironment(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, n, tokens, params));
}

RtVoid TMesh::doMakeShadowV(const char *pic, const char *tex, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiMakeShadow(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, pic, tex, n, tokens, params));
}


RtVoid TMesh::doArchiveRecordV(RtToken type, const char *line) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiArchiveRecord(lineNo(), type, line));
}

RtVoid TMesh::doReadArchiveV(RtString filename, RtArchiveCallback callback, RtInt n, RtToken tokens[], RtPointer params[]) {
	((TMeshContext *)m_currentContext)->m_scene->insertAndNext(new TRiReadArchive(lineNo(), getDeclMap(), m_currentContext->m_currentOptions->m_iNColor, filename, callback, n, tokens, params));
}
