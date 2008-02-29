/* RICPP - RenderMan(R) Interface CPP Language Binding
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
*/
/** @file ri.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Interface for the RenderMan(R) C Binding
 *
 *  Adapter from C-Binding to CPP-Binding
 *
 @verbatim
       RenderMan(R) is a registered trademark of Pixar
   The RenderMan(R) Interface Procedures and Protocol are:
           Copyright 1988, 1989, 2000, 2005 Pixar
                   All rights Reservered
 @endverbatim
 */

#define RI_EXPORTS
#include "ricpp/ri/ri.h"

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

#ifndef _RICPP_RICPP_FILTERS_H
#include "ricpp/ricpp/filters.h"
#endif // _RICPP_RICPP_FILTERS_H

#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif // _RICPP_TOOLS_INLINETOOLS_H

#ifndef _RICPP_RI_RIINTERNAL_H
#include "ricpp/ri/riinternal.h"
#endif // _RICPP_RI_RIINTERNAL_H

#include <cassert>
#include <cmath>
#include <vector>

namespace RiCPP {

// ----------------------------------------------------------------------------

std::map<RtFilterFunc, CFilterSlot> CFilterSlot::ms_filters;
std::map<RtArchiveCallback, CArchiveCallbackSlot> CArchiveCallbackSlot::ms_callbacks;
std::map<RtErrorHandler, CErrorHandlerSlot> CErrorHandlerSlot::ms_errorHandlers;
std::map<RtProcSubdivFunc, CSubdivFuncSlot> CSubdivFuncSlot::ms_procs;
std::map<RtProcFreeFunc, CFreeFuncSlot> CFreeFuncSlot::ms_procs;
std::map<RtPointer, CSubdivDataSlot> CSubdivDataSlot::ms_data;

// ----------------------------------------------------------------------------

static IRi *_riRoot = 0; ///< The connection to the C++ interface

IRi *RiCPPRoot() {
	return _riRoot;
}

void RiCPPRoot(IRi *aRoot) {
	_riRoot = aRoot;
}

// ----------------------------------------------------------------------------

RtInt RiCPPGetArgs(va_list marker, RtToken **tokens, RtPointer **params) {
	static std::vector<RtToken> vec_tokens;   // The tokens of the parameter list of an interface call
	static std::vector<RtPointer> vec_params; // The values of the parameter list of an interface call

	assert(tokens != 0);
	assert(params != 0);

	vec_tokens.clear(); *tokens = 0;
	vec_params.clear(); *params = 0;

	RtInt n = 0;
	RtPointer param;
	RtToken token;
	while ( RI_NULL != (token = va_arg(marker, RtToken)) ) {
		param = va_arg(marker, RtPointer);
		vec_tokens.push_back(token);
		vec_params.push_back(param);
		++n;
	}
	va_end(marker);

	if ( n == 0 ) {
		vec_tokens.resize(1);
		vec_params.resize(1);
		vec_tokens[0] = 0;
		vec_params[0] = 0;
	}

	*tokens = &(vec_tokens[0]);
	*params = &(vec_params[0]);

	return n;
}

// ----------------------------------------------------------------------------
RtContextHandle RiCPPInternalBeginV(RtToken name, int n, RtToken tokens[], RtPointer params[])
{
	RtContextHandle h = illContextHandle;
	PREAMBLE_RET(h)
		h = RiCPPRoot()->beginV(name, n, tokens, params);
	POSTAMBLE_RET(h)
}

RtVoid RiCPPInternalEnd(void)
{
	PREAMBLE
		RiCPPRoot()->end();
	POSTAMBLE
}

// ----------------------------------------------------------------------------
RtVoid RiCPPInternalErrorHandler(RtErrorHandler handler)
{
	PREAMBLE
		RiCPPRoot()->errorHandler(CErrorHandlerSlot::getSingleton(RiCPPRoot(), handler));
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RtVoid RiCPPInternalControlV(RtToken name, int n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->controlV(name, n, tokens, params);
	POSTAMBLE
}



// ----------------------------------------------------------------------------
} // namespace RiCPP
// ----------------------------------------------------------------------------

using namespace RiCPP;

extern "C" {

// ----------------------------------------------------------------------------
RICPP_INTERN(RtObjectHandle) RiCPPObjectBegin(RtToken name)
{
	RtObjectHandle h = illObjectHandle;
	PREAMBLE_RET(h)
		h = RiCPPRoot()->objectBegin(name);
	POSTAMBLE_RET(h)
}

RICPP_INTERN(RtVoid) RiCPPObjectEnd(void)
{
	PREAMBLE
		RiCPPRoot()->objectEnd();
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtToken) RiDeclare (const char *name,const char *declaration)
{
	RtToken h = RI_NULL;
	PREAMBLE_RET(h)
		h = RiCPPRoot()->declare(name, declaration);
	POSTAMBLE_RET(h)
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiSynchronize(RtToken type)
{
	PREAMBLE
		RiCPPRoot()->synchronize(type);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiSystem(RtString cmd)
{
	PREAMBLE
		RiCPPRoot()->system(cmd);
	POSTAMBLE
}


// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiFrameBegin(RtInt number)
{
	PREAMBLE
		RiCPPRoot()->frameBegin(number);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiFrameEnd(void)
{
	PREAMBLE
	RiCPPRoot()->frameEnd();
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiWorldBegin(void)
{
	PREAMBLE
		RiCPPRoot()->worldBegin();
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiWorldEnd(void)
{
	PREAMBLE
		RiCPPRoot()->worldEnd();
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiFormat(RtInt xres, RtInt yres, RtFloat aspect)
{
	PREAMBLE
		RiCPPRoot()->format(xres, yres, aspect);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiFrameAspectRatio(RtFloat aspect)
{
	PREAMBLE
		RiCPPRoot()->frameAspectRatio(aspect);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	PREAMBLE
		RiCPPRoot()->screenWindow(left, right, bot, top);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
{
	PREAMBLE
		RiCPPRoot()->cropWindow(xmin, xmax, ymin, ymax);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiProjection(char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		RiCPPRoot()->projectionV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiProjectionV(char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->projectionV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiClipping(RtFloat hither, RtFloat yon)
{
	PREAMBLE
		RiCPPRoot()->clipping(hither, yon);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
{
	PREAMBLE
		RiCPPRoot()->clippingPlane(x, y, z, nx, ny, nz);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
{
	PREAMBLE
		RiCPPRoot()->depthOfField(fstop, focallength, focaldistance);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiShutter(RtFloat smin, RtFloat smax)
{
	PREAMBLE
		RiCPPRoot()->shutter(smin, smax);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiPixelVariance(RtFloat variation)
{
	PREAMBLE
		RiCPPRoot()->pixelVariance(variation);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPixelSamples(RtFloat xsamples, RtFloat ysamples)
{
	PREAMBLE
		RiCPPRoot()->pixelSamples(xsamples, ysamples);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPixelFilter(RtFilterFunc function, RtFloat xwidth, RtFloat ywidth)
{
	PREAMBLE
		RiCPPRoot()->pixelFilter(CFilterSlot::getSingleton(RiCPPRoot(), function), xwidth, ywidth);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiExposure (RtFloat gain, RtFloat gamma)
{
	PREAMBLE
		RiCPPRoot()->exposure(gain, gamma);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiImager(char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		RiCPPRoot()->imagerV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiImagerV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->imagerV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiQuantize (RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
{
	PREAMBLE
		RiCPPRoot()->quantize(type, one, qmin, qmax, ampl);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisplay (char *name, RtToken type, RtToken mode, ...)
{
	PREAMBLE
		GETARGS(mode)
		RiCPPRoot()->displayV(name, type, mode, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisplayV (char *name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->displayV(name, type, mode, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisplayChannel (RtToken channel, ...)
{
	PREAMBLE
		GETARGS(channel)
		RiCPPRoot()->displayChannelV(channel, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisplayChannelV (RtToken channel,RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->displayChannelV(channel, n, tokens, params);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiHider (RtToken type, ...)
{
	PREAMBLE
		GETARGS(type)
		RiCPPRoot()->hiderV(type, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiHiderV (RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->hiderV(type, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiColorSamples (RtInt N, RtFloat *nRGB, RtFloat *RGBn)
{
	PREAMBLE
		RiCPPRoot()->colorSamples(N, nRGB, RGBn);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiRelativeDetail (RtFloat relativedetail)
{
	PREAMBLE
		RiCPPRoot()->relativeDetail(relativedetail);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiOption (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		RiCPPRoot()->optionV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiOptionV(char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->optionV(name, n, tokens, params);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiAttributeBegin (void)
{
	PREAMBLE
		RiCPPRoot()->attributeBegin();
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiAttributeEnd (void)
{
	PREAMBLE
		RiCPPRoot()->attributeEnd();
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiColor (RtColor Cs)
{
	PREAMBLE
		RiCPPRoot()->color(Cs);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiOpacity (RtColor Cs)
{
	PREAMBLE
		RiCPPRoot()->opacity(Cs);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiTextureCoordinates (RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
{
	PREAMBLE
		RiCPPRoot()->textureCoordinates(s1, t1, s2, t2, s3, t3, s4, t4);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtLightHandle) RiLightSource (char *name, ...)
{
	RtLightHandle h = illLightHandle;
	PREAMBLE_RET(h)
		GETARGS(name)
		RiCPPRoot()->lightSourceV(name, n, tokens, params);
	POSTAMBLE_RET(h)
}

RICPP_INTERN(RtLightHandle) RiLightSourceV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RtLightHandle h = illLightHandle;
	PREAMBLE_RET(h)
		RiCPPRoot()->lightSourceV(name, n, tokens, params);
	POSTAMBLE_RET(h)
}

RICPP_INTERN(RtLightHandle) RiAreaLightSource (char *name, ...)
{
	RtLightHandle h = illLightHandle;
	PREAMBLE_RET(h)
		GETARGS(name)
		RiCPPRoot()->areaLightSourceV(name, n, tokens, params);
	POSTAMBLE_RET(h)
}

RICPP_INTERN(RtLightHandle) RiAreaLightSourceV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RtLightHandle h = illLightHandle;
	PREAMBLE_RET(h)
		RiCPPRoot()->areaLightSourceV(name, n, tokens, params);
	POSTAMBLE_RET(h)
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiIlluminate (RtLightHandle light, RtBoolean onoff)
{
	PREAMBLE
		RiCPPRoot()->illuminate(light, onoff);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSurface (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		RiCPPRoot()->surfaceV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSurfaceV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->surfaceV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiAtmosphere (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		RiCPPRoot()->atmosphereV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiAtmosphereV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->atmosphereV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiInterior (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		RiCPPRoot()->interiorV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiInteriorV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->interiorV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiExterior (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		RiCPPRoot()->exteriorV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiExteriorV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->exteriorV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiShadingRate (RtFloat size)
{
	PREAMBLE
		RiCPPRoot()->shadingRate(size);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiShadingInterpolation (RtToken type)
{
	PREAMBLE
		RiCPPRoot()->shadingInterpolation(type);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMatte (RtBoolean onoff)
{
	PREAMBLE
		RiCPPRoot()->matte(onoff);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiBound (RtBound bound)
{
	PREAMBLE
		RiCPPRoot()->bound(bound);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDetail (RtBound bound)
{
	PREAMBLE
		RiCPPRoot()->detail(bound);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDetailRange (RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
{
	PREAMBLE
		RiCPPRoot()->detailRange(minvis, lowtran, uptran, maxvis);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiGeometricApproximation (RtToken type, RtFloat value)
{
	PREAMBLE
		RiCPPRoot()->geometricApproximation(type, value);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiGeometricRepresentation (RtToken type)
{
	PREAMBLE
		RiCPPRoot()->geometricRepresentation(type);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiOrientation (RtToken orientation)
{
	PREAMBLE
		RiCPPRoot()->orientation(orientation);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiReverseOrientation (void)
{
	PREAMBLE
		RiCPPRoot()->reverseOrientation();
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSides (RtInt nsides)
{
	PREAMBLE
		RiCPPRoot()->sides(nsides);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiIdentity (void)
{
	PREAMBLE
		RiCPPRoot()->identity();
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiTransform (RtMatrix transform)
{
	PREAMBLE
		RiCPPRoot()->transform(transform);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiConcatTransform (RtMatrix transform)
{
	PREAMBLE
		RiCPPRoot()->concatTransform(transform);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPerspective (RtFloat fov)
{
	PREAMBLE
		RiCPPRoot()->perspective(fov);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiTranslate (RtFloat dx, RtFloat dy, RtFloat dz)
{
	PREAMBLE
		RiCPPRoot()->translate(dx, dy, dz);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiRotate (RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
{
	PREAMBLE
		RiCPPRoot()->rotate(angle, dx, dy, dz);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiScale (RtFloat dx, RtFloat dy, RtFloat dz)
{
	PREAMBLE
		RiCPPRoot()->scale(dx, dy, dz);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSkew (RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2)
{
	PREAMBLE
		RiCPPRoot()->skew(angle, dx1, dy1, dz1, dx2, dy2, dz2);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDeformation (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		RiCPPRoot()->deformationV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDeformationV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->deformationV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisplacement (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		RiCPPRoot()->displacementV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisplacementV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->displacementV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiCoordinateSystem (RtToken space)
{
	PREAMBLE
		RiCPPRoot()->coordinateSystem(space);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiCoordSysTransform (RtToken space)
{
	PREAMBLE
		RiCPPRoot()->coordSysTransform(space);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtPoint *) RiTransformPoints (RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint *points)
{
	RtPoint *p=0;
	PREAMBLE_RET(p)
		RiCPPRoot()->transformPoints(fromspace, tospace, npoints, points);
	POSTAMBLE_RET(p)
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiTransformBegin (void)
{
	PREAMBLE
		RiCPPRoot()->transformBegin();
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiTransformEnd (void)
{
	PREAMBLE
		RiCPPRoot()->transformEnd();
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiAttribute (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		RiCPPRoot()->attributeV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiAttributeV(char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->attributeV(name, n, tokens, params);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiPolygon (RtInt nvertices, ...)
{
	PREAMBLE
		GETARGS(nvertices)
		RiCPPRoot()->polygonV(nvertices, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPolygonV (RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->polygonV(nvertices, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiGeneralPolygon (RtInt nloops, RtInt *nverts, ...)
{
	PREAMBLE
		GETARGS(nverts)
		RiCPPRoot()->generalPolygonV(nloops, nverts, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiGeneralPolygonV (RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->generalPolygonV(nloops, nverts, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPointsPolygons (RtInt npolys, RtInt *nverts, RtInt *verts, ...)
{
	PREAMBLE
		GETARGS(verts)
		RiCPPRoot()->pointsPolygonsV(npolys, nverts, verts, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPointsPolygonsV (RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->pointsPolygonsV(npolys, nverts, verts, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPointsGeneralPolygons (RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts, ...)
{
	PREAMBLE
		GETARGS(verts)
		RiCPPRoot()->pointsGeneralPolygonsV(npolys, nloops, nverts, verts, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPointsGeneralPolygonsV (RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->pointsGeneralPolygonsV(npolys, nloops, nverts, verts, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiBasis (RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
{
	PREAMBLE
		RiCPPRoot()->basis(ubasis, ustep, vbasis, vstep);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPatch (RtToken type, ...)
{
	PREAMBLE
		GETARGS(type)
		RiCPPRoot()->patchV(type, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPatchV (RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->patchV(type, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPatchMesh (RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, ...)
{
	PREAMBLE
		GETARGS(vwrap)
		RiCPPRoot()->patchMeshV(type, nu, uwrap, nv, vwrap, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPatchMeshV (RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->patchMeshV(type, nu, uwrap, nv, vwrap, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiNuPatch (RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax, ...)
{
	PREAMBLE
		GETARGS(vmax)
		RiCPPRoot()->nuPatchV(nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiNuPatchV (RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->nuPatchV(nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiTrimCurve (RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w)
{
	PREAMBLE
		RiCPPRoot()->trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiSphere (RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, ...)
{
	PREAMBLE
		GETARGS(thetamax)
		RiCPPRoot()->sphereV(radius, zmin, zmax, thetamax, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSphereV (RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->sphereV(radius, zmin, zmax, thetamax, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiCone (RtFloat height, RtFloat radius, RtFloat thetamax, ...)
{
	PREAMBLE
		GETARGS(thetamax)
		RiCPPRoot()->coneV(height, radius, thetamax, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiConeV (RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->coneV(height, radius, thetamax, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiCylinder (RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, ...)
{
	PREAMBLE
		GETARGS(thetamax)
		RiCPPRoot()->cylinderV(radius, zmin, zmax, thetamax, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiCylinderV (RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->cylinderV(radius, zmin, zmax, thetamax, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiHyperboloid (RtPoint point1, RtPoint point2, RtFloat thetamax, ...)
{
	PREAMBLE
		GETARGS(thetamax)
		RiCPPRoot()->hyperboloidV(point1, point2, thetamax, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiHyperboloidV (RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->hyperboloidV(point1, point2, thetamax, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiParaboloid (RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, ...)
{
	PREAMBLE
		GETARGS(thetamax)
		RiCPPRoot()->paraboloidV(rmax, zmin, zmax, thetamax, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiParaboloidV (RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->paraboloidV(rmax, zmin, zmax, thetamax, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisk (RtFloat height, RtFloat radius, RtFloat thetamax, ...)
{
	PREAMBLE
		GETARGS(thetamax)
		RiCPPRoot()->diskV(height, radius, thetamax, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDiskV (RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->diskV(height, radius, thetamax, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiTorus (RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, ...)
{
	PREAMBLE
		GETARGS(thetamax)
		RiCPPRoot()->torusV(majorrad, minorrad, phimin, phimax, thetamax, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiTorusV (RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->torusV(majorrad, minorrad, phimin, phimax, thetamax, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiProcedural(RtPointer data, RtBound bound, RtVoid (*subdivfunc)(RtPointer, RtFloat), RtVoid (*freefunc)(RtPointer))
{
	PREAMBLE
		RiCPPRoot()->procedural(data, bound, CSubdivFuncSlot::getSingleton(RiCPPRoot(), subdivfunc), freefunc ? &CFreeFuncSlot::getSingleton(RiCPPRoot(), freefunc) : 0);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiGeometry (RtToken type, ...)
{
	PREAMBLE
		GETARGS(type)
		RiCPPRoot()->geometryV(type, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiGeometryV (RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->geometryV(type, n, tokens, params);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiCurves (RtToken degree, RtInt ncurves, RtInt nverts[], RtToken wrap, ...)
{
	PREAMBLE
		GETARGS(wrap)
		RiCPPRoot()->curvesV(degree, ncurves, nverts, wrap, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiCurvesV (RtToken degree, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->curvesV(degree, ncurves, nverts, wrap, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPoints (RtInt npts,...)
{
	PREAMBLE
		GETARGS(npts)
		RiCPPRoot()->pointsV(npts, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPointsV (RtInt npts, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->pointsV(npts, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSubdivisionMesh (RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], ...)
{
	PREAMBLE
		GETARGS(floatargs)
		RiCPPRoot()->subdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSubdivisionMeshV (RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->subdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiHierarchicalSubdivisionMesh (RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtToken stringargs[], ...)
{
	PREAMBLE
		GETARGS(stringargs)
		RiCPPRoot()->hierarchicalSubdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, stringargs, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiHierarchicalSubdivisionMeshV (RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtToken stringargs[], RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->hierarchicalSubdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, stringargs, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiBlobby (RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], ...)
{
	PREAMBLE
		GETARGS(str)
		RiCPPRoot()->blobbyV(nleaf, ncode, code, nflt, flt, nstr, str, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiBlobbyV (RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->blobbyV(nleaf, ncode, code, nflt, flt, nstr, str, n, tokens, params);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiSolidBegin(RtToken type)
{
	PREAMBLE
		RiCPPRoot()->solidBegin(type);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSolidEnd(void)
{
	PREAMBLE
		RiCPPRoot()->solidEnd();
	POSTAMBLE
}

RICPP_INTERN(RtObjectHandle) RiObjectBegin (void)
{
	RtObjectHandle h = illObjectHandle;
	PREAMBLE_RET(h)
		h = RiCPPRoot()->objectBegin(0);
	POSTAMBLE_RET(h)
}

RICPP_INTERN(RtVoid) RiObjectEnd (void)
{
	PREAMBLE
		RiCPPRoot()->objectEnd();
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiObjectInstance (RtObjectHandle handle)
{
	PREAMBLE
		RiCPPRoot()->objectInstance(handle);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMotionBegin (RtInt N, ...)
{
	PREAMBLE
		if ( N == 0 ) {
			RiCPPRoot()->motionBeginV(N, 0);
		} else {
			va_list marker;
			std::vector<RtFloat> times;
			RtInt n;

			times.reserve(N);

			RtFloat sample;
			va_start(marker, N);
			for ( n = 0; n < N; ++n ) {
				sample = (RtFloat)va_arg(marker, double); // 'default argument promotions' float -> double
				times.push_back(sample);
			}
			va_end(marker);

			RiCPPRoot()->motionBeginV(N, &times[0]);
		}
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMotionBeginV (RtInt N, RtFloat times[])
{
	PREAMBLE
		RiCPPRoot()->motionBeginV(N, times);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMotionEnd (void)
{
	PREAMBLE
		RiCPPRoot()->motionEnd();
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiMakeTexture (char *pic, char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...)
{
	PREAMBLE
		GETARGS(twidth)
		// RiCPPRoot()->makeTextureV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeTextureV (char *pic, char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		// RiCPPRoot()->makeTextureV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeBump (char *pic, char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...)
{
	PREAMBLE
		GETARGS(twidth)
		// RiCPPRoot()->makeBumpV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeBumpV (char *pic, char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		// RiCPPRoot()->makeBumpV(pic, tex, swrap, twrap, filterfunc, swidth, twidth, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeLatLongEnvironment (char *pic, char *tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...)
{
	PREAMBLE
		GETARGS(twidth)
		// RiCPPRoot()->makeLatLongEnvironmenV(pic, tex, filterfunc, swidth, twidth, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeLatLongEnvironmentV (char *pic, char *tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		// RiCPPRoot()->makeLatLongEnvironmenV(pic, tex, filterfunc, swidth, twidth, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeCubeFaceEnvironment (char *px, char *nx, char *py, char *ny, char *pz, char *nz, char *tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...)
{
	PREAMBLE
		GETARGS(twidth)
		// RiCPPRoot()->makeCubeFaceEnvironmentV(px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeCubeFaceEnvironmentV (char *px, char *nx, char *py, char *ny, char *pz, char *nz, char *tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		// RiCPPRoot()->makeCubeFaceEnvironmentV(px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeShadow (char *pic, char *tex, ...)
{
	PREAMBLE
		GETARGS(tex)
		RiCPPRoot()->makeShadowV(pic, tex, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeShadowV (char *pic, char *tex, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->makeShadowV(pic, tex, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeBrickMap (int num, char **src, char *dest, ...)
{
	PREAMBLE
		GETARGS(dest)
		RiCPPRoot()->makeBrickMapV(num, (RtString *)src, dest, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeBrickMapV(int num, char **src, char *dest, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->makeBrickMapV(num, (RtString *)src, dest, n, tokens, params);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiResource(RtToken handle, RtToken type, ...)
{
	PREAMBLE
		GETARGS(type)
		RiCPPRoot()->resourceV(handle, type, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiResourceV(RtToken handle, RtToken type,RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->resourceV(handle, type, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiResourceBegin(void)
{
	PREAMBLE
		RiCPPRoot()->resourceBegin();
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiResourceEnd(void)
{
	PREAMBLE
		RiCPPRoot()->resourceEnd();
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtArchiveHandle) RiArchiveBegin(RtToken name, ...)
{
	RtArchiveHandle h = illArchiveHandle;
	PREAMBLE_RET(h)
		GETARGS(name)
		h = RiCPPRoot()->archiveBeginV(name, n, tokens, params);
	POSTAMBLE_RET(h)
}

RICPP_INTERN(RtArchiveHandle) RiArchiveBeginV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RtArchiveHandle h = illArchiveHandle;
	PREAMBLE_RET(h)
		h = RiCPPRoot()->archiveBeginV(name, n, tokens, params);
	POSTAMBLE_RET(h)
}

RICPP_INTERN(RtVoid) RiArchiveEnd(void)
{
	PREAMBLE
		RiCPPRoot()->archiveEnd();
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiIfBegin(char *expr)
{
	PREAMBLE
		RiCPPRoot()->ifBegin(expr);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiElseIf(char *expr)
{
	PREAMBLE
		RiCPPRoot()->elseIfBegin(expr);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiElse(void)
{
	PREAMBLE
		RiCPPRoot()->elseBegin();
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiIfEnd(void)
{
	PREAMBLE
		RiCPPRoot()->ifEnd();
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiArchiveRecord (RtToken type, char *format, ...)
{
	PREAMBLE
		static const int ARCHIVE_BUF_SIZE = 512;
		char archiveBuf[ARCHIVE_BUF_SIZE];
		va_list argList;
		va_start(argList, format);
		archiveBuf[0] = (char)0;
#ifdef _WIN32
		_vsnprintf_s(archiveBuf, ARCHIVE_BUF_SIZE-1, ARCHIVE_BUF_SIZE-2, format ? format : "", argList);
#else
		vsnprintf(archiveBuf, ARCHIVE_BUF_SIZE-1, format ? format : "", argList);
#endif
		archiveBuf[ARCHIVE_BUF_SIZE-1] = (char)0;
		va_end(argList);
		RiCPPRoot()->archiveRecordV(type, archiveBuf);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiReadArchive (RtString filename, RtArchiveCallback callback, ...)
{
	PREAMBLE
		GETARGS(callback)
		RiCPPRoot()->readArchiveV(filename, callback ? &CArchiveCallbackSlot::getSingleton(callback) : 0, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiReadArchiveV (RtString filename, RtArchiveCallback callback, int n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		RiCPPRoot()->readArchiveV(filename, callback ? &CArchiveCallbackSlot::getSingleton(callback) : 0, n, tokens, params);
	POSTAMBLE
}

} // extern "C"
