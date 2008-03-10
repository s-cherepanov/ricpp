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

#ifndef _RICPP_RI_RIINTERNAL_H
#include "ricpp/ri/riinternal.h"
#endif // _RICPP_RI_RIINTERNAL_H

#include <cassert>
#include <cmath>
#include <vector>

using namespace RiCPP;

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
	RI_PREAMBLE_RET(h)
		h = RiCPPRoot()->beginV(name, n, tokens, params);
	RI_POSTAMBLE_RET(h)
}

RtVoid RiCPPInternalEnd(void)
{
	RI_PREAMBLE
		RiCPPRoot()->end();
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------
RtVoid RiErrorHandler(RtErrorHandler handler)
{
	RI_PREAMBLE
		RiCPPRoot()->errorHandler(CErrorHandlerSlot::getSingleton(RiCPPRoot(), handler));
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiControl (RtToken name, ...)
{
	RI_PREAMBLE
		RI_GETARGS(name)
		RiCPPRoot()->controlV(name, n, tokens, params);
	RI_POSTAMBLE
}

RtVoid RiControlV(RtToken name, int n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->controlV(name, n, tokens, params);
	RI_POSTAMBLE
}



// ----------------------------------------------------------------------------
} // namespace RiCPP
// ----------------------------------------------------------------------------

extern "C" {

// ----------------------------------------------------------------------------
RICPP_INTERN(RtObjectHandle) RiCPPObjectBegin(RtToken name)
{
	RtObjectHandle h = illObjectHandle;
	RI_PREAMBLE_RET(h)
		h = RiCPPRoot()->objectBegin(name);
	RI_POSTAMBLE_RET(h)
}

RICPP_INTERN(RtVoid) RiCPPObjectEnd(void)
{
	RI_PREAMBLE
		RiCPPRoot()->objectEnd();
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtToken) RiDeclare (RtString name, RtString declaration)
{
	RtToken h = RI_NULL;
	RI_PREAMBLE_RET(h)
		h = RiCPPRoot()->declare(name, declaration);
	RI_POSTAMBLE_RET(h)
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiSynchronize(RtToken type)
{
	RI_PREAMBLE
		RiCPPRoot()->synchronize(type);
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiSystem(RtString cmd)
{
	RI_PREAMBLE
		RiCPPRoot()->system(cmd);
	RI_POSTAMBLE
}


// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiFrameBegin(RtInt number)
{
	RI_PREAMBLE
		RiCPPRoot()->frameBegin(number);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiFrameEnd(void)
{
	RI_PREAMBLE
	RiCPPRoot()->frameEnd();
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiWorldBegin(void)
{
	RI_PREAMBLE
		RiCPPRoot()->worldBegin();
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiWorldEnd(void)
{
	RI_PREAMBLE
		RiCPPRoot()->worldEnd();
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiFormat(RtInt xres, RtInt yres, RtFloat aspect)
{
	RI_PREAMBLE
		RiCPPRoot()->format(xres, yres, aspect);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiFrameAspectRatio(RtFloat aspect)
{
	RI_PREAMBLE
		RiCPPRoot()->frameAspectRatio(aspect);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	RI_PREAMBLE
		RiCPPRoot()->screenWindow(left, right, bot, top);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
{
	RI_PREAMBLE
		RiCPPRoot()->cropWindow(xmin, xmax, ymin, ymax);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiProjection(RtString name, ...)
{
	RI_PREAMBLE
		RI_GETARGS(name)
		RiCPPRoot()->projectionV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiProjectionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->projectionV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiClipping(RtFloat hither, RtFloat yon)
{
	RI_PREAMBLE
		RiCPPRoot()->clipping(hither, yon);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
{
	RI_PREAMBLE
		RiCPPRoot()->clippingPlane(x, y, z, nx, ny, nz);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
{
	RI_PREAMBLE
		RiCPPRoot()->depthOfField(fstop, focallength, focaldistance);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiShutter(RtFloat smin, RtFloat smax)
{
	RI_PREAMBLE
		RiCPPRoot()->shutter(smin, smax);
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiPixelVariance(RtFloat variation)
{
	RI_PREAMBLE
		RiCPPRoot()->pixelVariance(variation);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPixelSamples(RtFloat xsamples, RtFloat ysamples)
{
	RI_PREAMBLE
		RiCPPRoot()->pixelSamples(xsamples, ysamples);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPixelFilter(RtFilterFunc function, RtFloat xwidth, RtFloat ywidth)
{
	RI_PREAMBLE
		RiCPPRoot()->pixelFilter(CFilterSlot::getSingleton(RiCPPRoot(), function), xwidth, ywidth);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiExposure (RtFloat gain, RtFloat gamma)
{
	RI_PREAMBLE
		RiCPPRoot()->exposure(gain, gamma);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiImager(RtString name, ...)
{
	RI_PREAMBLE
		RI_GETARGS(name)
		RiCPPRoot()->imagerV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiImagerV (RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->imagerV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiQuantize (RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
{
	RI_PREAMBLE
		RiCPPRoot()->quantize(type, one, qmin, qmax, ampl);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisplay (RtString name, RtToken type, RtToken mode, ...)
{
	RI_PREAMBLE
		RI_GETARGS(mode)
		RiCPPRoot()->displayV(name, type, mode, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisplayV (RtString name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->displayV(name, type, mode, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisplayChannel (RtToken channel, ...)
{
	RI_PREAMBLE
		RI_GETARGS(channel)
		RiCPPRoot()->displayChannelV(channel, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisplayChannelV (RtToken channel,RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->displayChannelV(channel, n, tokens, params);
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiHider (RtToken type, ...)
{
	RI_PREAMBLE
		RI_GETARGS(type)
		RiCPPRoot()->hiderV(type, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiHiderV (RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->hiderV(type, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiColorSamples (RtInt N, RtFloat *nRGB, RtFloat *RGBn)
{
	RI_PREAMBLE
		RiCPPRoot()->colorSamples(N, nRGB, RGBn);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiRelativeDetail (RtFloat relativedetail)
{
	RI_PREAMBLE
		RiCPPRoot()->relativeDetail(relativedetail);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiOption (RtString name, ...)
{
	RI_PREAMBLE
		RI_GETARGS(name)
		RiCPPRoot()->optionV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiOptionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->optionV(name, n, tokens, params);
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiAttributeBegin (void)
{
	RI_PREAMBLE
		RiCPPRoot()->attributeBegin();
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiAttributeEnd (void)
{
	RI_PREAMBLE
		RiCPPRoot()->attributeEnd();
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiColor (RtColor Cs)
{
	RI_PREAMBLE
		RiCPPRoot()->color(Cs);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiOpacity (RtColor Cs)
{
	RI_PREAMBLE
		RiCPPRoot()->opacity(Cs);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiTextureCoordinates (RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
{
	RI_PREAMBLE
		RiCPPRoot()->textureCoordinates(s1, t1, s2, t2, s3, t3, s4, t4);
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtLightHandle) RiLightSource (RtString name, ...)
{
	RtLightHandle h = illLightHandle;
	RI_PREAMBLE_RET(h)
		RI_GETARGS(name)
		RiCPPRoot()->lightSourceV(name, n, tokens, params);
	RI_POSTAMBLE_RET(h)
}

RICPP_INTERN(RtLightHandle) RiLightSourceV (RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RtLightHandle h = illLightHandle;
	RI_PREAMBLE_RET(h)
		RiCPPRoot()->lightSourceV(name, n, tokens, params);
	RI_POSTAMBLE_RET(h)
}

RICPP_INTERN(RtLightHandle) RiAreaLightSource (RtString name, ...)
{
	RtLightHandle h = illLightHandle;
	RI_PREAMBLE_RET(h)
		RI_GETARGS(name)
		RiCPPRoot()->areaLightSourceV(name, n, tokens, params);
	RI_POSTAMBLE_RET(h)
}

RICPP_INTERN(RtLightHandle) RiAreaLightSourceV (RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RtLightHandle h = illLightHandle;
	RI_PREAMBLE_RET(h)
		RiCPPRoot()->areaLightSourceV(name, n, tokens, params);
	RI_POSTAMBLE_RET(h)
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiIlluminate (RtLightHandle light, RtBoolean onoff)
{
	RI_PREAMBLE
		RiCPPRoot()->illuminate(light, onoff);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSurface (RtString name, ...)
{
	RI_PREAMBLE
		RI_GETARGS(name)
		RiCPPRoot()->surfaceV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSurfaceV (RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->surfaceV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiAtmosphere (RtString name, ...)
{
	RI_PREAMBLE
		RI_GETARGS(name)
		RiCPPRoot()->atmosphereV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiAtmosphereV (RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->atmosphereV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiInterior (RtString name, ...)
{
	RI_PREAMBLE
		RI_GETARGS(name)
		RiCPPRoot()->interiorV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiInteriorV (RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->interiorV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiExterior (RtString name, ...)
{
	RI_PREAMBLE
		RI_GETARGS(name)
		RiCPPRoot()->exteriorV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiExteriorV (RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->exteriorV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiShadingRate (RtFloat size)
{
	RI_PREAMBLE
		RiCPPRoot()->shadingRate(size);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiShadingInterpolation (RtToken type)
{
	RI_PREAMBLE
		RiCPPRoot()->shadingInterpolation(type);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMatte (RtBoolean onoff)
{
	RI_PREAMBLE
		RiCPPRoot()->matte(onoff);
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiBound (RtBound bound)
{
	RI_PREAMBLE
		RiCPPRoot()->bound(bound);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDetail (RtBound bound)
{
	RI_PREAMBLE
		RiCPPRoot()->detail(bound);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDetailRange (RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
{
	RI_PREAMBLE
		RiCPPRoot()->detailRange(minvis, lowtran, uptran, maxvis);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiGeometricApproximation (RtToken type, RtFloat value)
{
	RI_PREAMBLE
		RiCPPRoot()->geometricApproximation(type, value);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiGeometricRepresentation (RtToken type)
{
	RI_PREAMBLE
		RiCPPRoot()->geometricRepresentation(type);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiOrientation (RtToken orientation)
{
	RI_PREAMBLE
		RiCPPRoot()->orientation(orientation);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiReverseOrientation (void)
{
	RI_PREAMBLE
		RiCPPRoot()->reverseOrientation();
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSides (RtInt nsides)
{
	RI_PREAMBLE
		RiCPPRoot()->sides(nsides);
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiIdentity (void)
{
	RI_PREAMBLE
		RiCPPRoot()->identity();
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiTransform (RtMatrix transform)
{
	RI_PREAMBLE
		RiCPPRoot()->transform(transform);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiConcatTransform (RtMatrix transform)
{
	RI_PREAMBLE
		RiCPPRoot()->concatTransform(transform);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPerspective (RtFloat fov)
{
	RI_PREAMBLE
		RiCPPRoot()->perspective(fov);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiTranslate (RtFloat dx, RtFloat dy, RtFloat dz)
{
	RI_PREAMBLE
		RiCPPRoot()->translate(dx, dy, dz);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiRotate (RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
{
	RI_PREAMBLE
		RiCPPRoot()->rotate(angle, dx, dy, dz);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiScale (RtFloat dx, RtFloat dy, RtFloat dz)
{
	RI_PREAMBLE
		RiCPPRoot()->scale(dx, dy, dz);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSkew (RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2)
{
	RI_PREAMBLE
		RiCPPRoot()->skew(angle, dx1, dy1, dz1, dx2, dy2, dz2);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDeformation (RtString name, ...)
{
	RI_PREAMBLE
		RI_GETARGS(name)
		RiCPPRoot()->deformationV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDeformationV (RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->deformationV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisplacement (RtString name, ...)
{
	RI_PREAMBLE
		RI_GETARGS(name)
		RiCPPRoot()->displacementV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisplacementV (RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->displacementV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiCoordinateSystem (RtToken space)
{
	RI_PREAMBLE
		RiCPPRoot()->coordinateSystem(space);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiCoordSysTransform (RtToken space)
{
	RI_PREAMBLE
		RiCPPRoot()->coordSysTransform(space);
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtPoint *) RiTransformPoints (RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint *points)
{
	RtPoint *p=0;
	RI_PREAMBLE_RET(p)
		RiCPPRoot()->transformPoints(fromspace, tospace, npoints, points);
	RI_POSTAMBLE_RET(p)
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiTransformBegin (void)
{
	RI_PREAMBLE
		RiCPPRoot()->transformBegin();
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiTransformEnd (void)
{
	RI_PREAMBLE
		RiCPPRoot()->transformEnd();
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiAttribute (RtString name, ...)
{
	RI_PREAMBLE
		RI_GETARGS(name)
		RiCPPRoot()->attributeV(name, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiAttributeV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->attributeV(name, n, tokens, params);
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiPolygon (RtInt nvertices, ...)
{
	RI_PREAMBLE
		RI_GETARGS(nvertices)
		RiCPPRoot()->polygonV(nvertices, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPolygonV (RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->polygonV(nvertices, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiGeneralPolygon (RtInt nloops, RtInt *nverts, ...)
{
	RI_PREAMBLE
		RI_GETARGS(nverts)
		RiCPPRoot()->generalPolygonV(nloops, nverts, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiGeneralPolygonV (RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->generalPolygonV(nloops, nverts, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPointsPolygons (RtInt npolys, RtInt *nverts, RtInt *verts, ...)
{
	RI_PREAMBLE
		RI_GETARGS(verts)
		RiCPPRoot()->pointsPolygonsV(npolys, nverts, verts, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPointsPolygonsV (RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->pointsPolygonsV(npolys, nverts, verts, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPointsGeneralPolygons (RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts, ...)
{
	RI_PREAMBLE
		RI_GETARGS(verts)
		RiCPPRoot()->pointsGeneralPolygonsV(npolys, nloops, nverts, verts, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPointsGeneralPolygonsV (RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->pointsGeneralPolygonsV(npolys, nloops, nverts, verts, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiBasis (RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
{
	RI_PREAMBLE
		RiCPPRoot()->basis(ubasis, ustep, vbasis, vstep);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPatch (RtToken type, ...)
{
	RI_PREAMBLE
		RI_GETARGS(type)
		RiCPPRoot()->patchV(type, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPatchV (RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->patchV(type, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPatchMesh (RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, ...)
{
	RI_PREAMBLE
		RI_GETARGS(vwrap)
		RiCPPRoot()->patchMeshV(type, nu, uwrap, nv, vwrap, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPatchMeshV (RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->patchMeshV(type, nu, uwrap, nv, vwrap, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiNuPatch (RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax, ...)
{
	RI_PREAMBLE
		RI_GETARGS(vmax)
		RiCPPRoot()->nuPatchV(nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiNuPatchV (RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->nuPatchV(nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiTrimCurve (RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w)
{
	RI_PREAMBLE
		RiCPPRoot()->trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiSphere (RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, ...)
{
	RI_PREAMBLE
		RI_GETARGS(thetamax)
		RiCPPRoot()->sphereV(radius, zmin, zmax, thetamax, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSphereV (RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->sphereV(radius, zmin, zmax, thetamax, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiCone (RtFloat height, RtFloat radius, RtFloat thetamax, ...)
{
	RI_PREAMBLE
		RI_GETARGS(thetamax)
		RiCPPRoot()->coneV(height, radius, thetamax, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiConeV (RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->coneV(height, radius, thetamax, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiCylinder (RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, ...)
{
	RI_PREAMBLE
		RI_GETARGS(thetamax)
		RiCPPRoot()->cylinderV(radius, zmin, zmax, thetamax, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiCylinderV (RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->cylinderV(radius, zmin, zmax, thetamax, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiHyperboloid (RtPoint point1, RtPoint point2, RtFloat thetamax, ...)
{
	RI_PREAMBLE
		RI_GETARGS(thetamax)
		RiCPPRoot()->hyperboloidV(point1, point2, thetamax, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiHyperboloidV (RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->hyperboloidV(point1, point2, thetamax, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiParaboloid (RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, ...)
{
	RI_PREAMBLE
		RI_GETARGS(thetamax)
		RiCPPRoot()->paraboloidV(rmax, zmin, zmax, thetamax, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiParaboloidV (RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->paraboloidV(rmax, zmin, zmax, thetamax, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisk (RtFloat height, RtFloat radius, RtFloat thetamax, ...)
{
	RI_PREAMBLE
		RI_GETARGS(thetamax)
		RiCPPRoot()->diskV(height, radius, thetamax, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDiskV (RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->diskV(height, radius, thetamax, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiTorus (RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, ...)
{
	RI_PREAMBLE
		RI_GETARGS(thetamax)
		RiCPPRoot()->torusV(majorrad, minorrad, phimin, phimax, thetamax, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiTorusV (RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->torusV(majorrad, minorrad, phimin, phimax, thetamax, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiProcedural(RtPointer data, RtBound bound, RtVoid (*subdivfunc)(RtPointer, RtFloat), RtVoid (*freefunc)(RtPointer))
{
	RI_PREAMBLE
		RiCPPRoot()->procedural(data, bound, CSubdivFuncSlot::getSingleton(RiCPPRoot(), subdivfunc), freefunc ? &CFreeFuncSlot::getSingleton(RiCPPRoot(), freefunc) : 0);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiGeometry (RtToken type, ...)
{
	RI_PREAMBLE
		RI_GETARGS(type)
		RiCPPRoot()->geometryV(type, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiGeometryV (RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->geometryV(type, n, tokens, params);
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiCurves (RtToken degree, RtInt ncurves, RtInt nverts[], RtToken wrap, ...)
{
	RI_PREAMBLE
		RI_GETARGS(wrap)
		RiCPPRoot()->curvesV(degree, ncurves, nverts, wrap, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiCurvesV (RtToken degree, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->curvesV(degree, ncurves, nverts, wrap, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPoints (RtInt npts,...)
{
	RI_PREAMBLE
		RI_GETARGS(npts)
		RiCPPRoot()->pointsV(npts, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPointsV (RtInt npts, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->pointsV(npts, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSubdivisionMesh (RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], ...)
{
	RI_PREAMBLE
		RI_GETARGS(floatargs)
		RiCPPRoot()->subdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSubdivisionMeshV (RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->subdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiHierarchicalSubdivisionMesh (RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtToken stringargs[], ...)
{
	RI_PREAMBLE
		RI_GETARGS(stringargs)
		RiCPPRoot()->hierarchicalSubdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, stringargs, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiHierarchicalSubdivisionMeshV (RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtToken stringargs[], RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->hierarchicalSubdivisionMeshV(scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, stringargs, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiBlobby (RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], ...)
{
	RI_PREAMBLE
		RI_GETARGS(str)
		RiCPPRoot()->blobbyV(nleaf, ncode, code, nflt, flt, nstr, str, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiBlobbyV (RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->blobbyV(nleaf, ncode, code, nflt, flt, nstr, str, n, tokens, params);
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiSolidBegin(RtToken type)
{
	RI_PREAMBLE
		RiCPPRoot()->solidBegin(type);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSolidEnd(void)
{
	RI_PREAMBLE
		RiCPPRoot()->solidEnd();
	RI_POSTAMBLE
}

RICPP_INTERN(RtObjectHandle) RiObjectBegin (void)
{
	RtObjectHandle h = illObjectHandle;
	RI_PREAMBLE_RET(h)
		h = RiCPPRoot()->objectBegin(0);
	RI_POSTAMBLE_RET(h)
}

RICPP_INTERN(RtVoid) RiObjectEnd (void)
{
	RI_PREAMBLE
		RiCPPRoot()->objectEnd();
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiObjectInstance (RtObjectHandle handle)
{
	RI_PREAMBLE
		RiCPPRoot()->objectInstance(handle);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMotionBegin (RtInt N, ...)
{
	RI_PREAMBLE
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
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMotionBeginV (RtInt N, RtFloat times[])
{
	RI_PREAMBLE
		RiCPPRoot()->motionBeginV(N, times);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMotionEnd (void)
{
	RI_PREAMBLE
		RiCPPRoot()->motionEnd();
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiMakeTexture (RtString pic, RtString tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...)
{
	RI_PREAMBLE
		RI_GETARGS(twidth)
		RiCPPRoot()->makeTextureV(pic, tex, swrap, twrap, CFilterSlot::getSingleton(RiCPPRoot(), filterfunc), swidth, twidth, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeTextureV (RtString pic, RtString tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->makeTextureV(pic, tex, swrap, twrap, CFilterSlot::getSingleton(RiCPPRoot(), filterfunc), swidth, twidth, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeBump (RtString pic, RtString tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...)
{
	RI_PREAMBLE
		RI_GETARGS(twidth)
		RiCPPRoot()->makeBumpV(pic, tex, swrap, twrap, CFilterSlot::getSingleton(RiCPPRoot(), filterfunc), swidth, twidth, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeBumpV (RtString pic, RtString tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->makeBumpV(pic, tex, swrap, twrap, CFilterSlot::getSingleton(RiCPPRoot(), filterfunc), swidth, twidth, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeLatLongEnvironment (RtString pic, RtString tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...)
{
	RI_PREAMBLE
		RI_GETARGS(twidth)
		RiCPPRoot()->makeLatLongEnvironmentV(pic, tex, CFilterSlot::getSingleton(RiCPPRoot(), filterfunc), swidth, twidth, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeLatLongEnvironmentV (RtString pic, RtString tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->makeLatLongEnvironmentV(pic, tex, CFilterSlot::getSingleton(RiCPPRoot(), filterfunc), swidth, twidth, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeCubeFaceEnvironment (RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...)
{
	RI_PREAMBLE
		RI_GETARGS(twidth)
		RiCPPRoot()->makeCubeFaceEnvironmentV(px, nx, py, ny, pz, nz, tex, fov, CFilterSlot::getSingleton(RiCPPRoot(), filterfunc), swidth, twidth, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeCubeFaceEnvironmentV (RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->makeCubeFaceEnvironmentV(px, nx, py, ny, pz, nz, tex, fov, CFilterSlot::getSingleton(RiCPPRoot(), filterfunc), swidth, twidth, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeShadow (RtString pic, RtString tex, ...)
{
	RI_PREAMBLE
		RI_GETARGS(tex)
		RiCPPRoot()->makeShadowV(pic, tex, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeShadowV (RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->makeShadowV(pic, tex, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeBrickMap (RtInt num, RtString *src, RtString dest, ...)
{
	RI_PREAMBLE
		RI_GETARGS(dest)
		RiCPPRoot()->makeBrickMapV(num, (RtString *)src, dest, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMakeBrickMapV(RtInt num, RtString *src, RtString dest, RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->makeBrickMapV(num, (RtString *)src, dest, n, tokens, params);
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiResource(RtToken handle, RtToken type, ...)
{
	RI_PREAMBLE
		RI_GETARGS(type)
		RiCPPRoot()->resourceV(handle, type, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiResourceV(RtToken handle, RtToken type,RtInt n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->resourceV(handle, type, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiResourceBegin(void)
{
	RI_PREAMBLE
		RiCPPRoot()->resourceBegin();
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiResourceEnd(void)
{
	RI_PREAMBLE
		RiCPPRoot()->resourceEnd();
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtArchiveHandle) RiArchiveBegin(RtToken name, ...)
{
	RtArchiveHandle h = illArchiveHandle;
	RI_PREAMBLE_RET(h)
		RI_GETARGS(name)
		h = RiCPPRoot()->archiveBeginV(name, n, tokens, params);
	RI_POSTAMBLE_RET(h)
}

RICPP_INTERN(RtArchiveHandle) RiArchiveBeginV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RtArchiveHandle h = illArchiveHandle;
	RI_PREAMBLE_RET(h)
		h = RiCPPRoot()->archiveBeginV(name, n, tokens, params);
	RI_POSTAMBLE_RET(h)
}

RICPP_INTERN(RtVoid) RiArchiveEnd(void)
{
	RI_PREAMBLE
		RiCPPRoot()->archiveEnd();
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiIfBegin(RtString expr)
{
	RI_PREAMBLE
		RiCPPRoot()->ifBegin(expr);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiElseIf(RtString expr)
{
	RI_PREAMBLE
		RiCPPRoot()->elseIfBegin(expr);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiElse(void)
{
	RI_PREAMBLE
		RiCPPRoot()->elseBegin();
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiIfEnd(void)
{
	RI_PREAMBLE
		RiCPPRoot()->ifEnd();
	RI_POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiArchiveRecord (RtToken type, RtString format, ...)
{
	RI_PREAMBLE
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
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiReadArchive (RtString filename, RtArchiveCallback callback, ...)
{
	RI_PREAMBLE
		RI_GETARGS(callback)
		RiCPPRoot()->readArchiveV(filename, callback ? &CArchiveCallbackSlot::getSingleton(callback) : 0, n, tokens, params);
	RI_POSTAMBLE
}

RICPP_INTERN(RtVoid) RiReadArchiveV (RtString filename, RtArchiveCallback callback, int n, RtToken tokens[], RtPointer params[])
{
	RI_PREAMBLE
		RiCPPRoot()->readArchiveV(filename, callback ? &CArchiveCallbackSlot::getSingleton(callback) : 0, n, tokens, params);
	RI_POSTAMBLE
}

} // extern "C"
