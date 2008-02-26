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
#include <map>

namespace RiCPP {

/** @brief An arbitrary pixel filter for C binding
 */
class CFilterSlot : public IFilterFunc {
	RtFilterFunc m_filter;
public:
	inline CFilterSlot(RtFilterFunc aFilter=0) { m_filter = aFilter; }
	inline CFilterSlot(const CFilterSlot &o) { *this = o; }

	static RtToken myName();

	inline virtual IFilterFunc *duplicate() const { return new CFilterSlot(*this); }
	inline virtual RtToken name() const { return myName(); }

	virtual RtFloat operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const;

	inline virtual const IFilterFunc &singleton() const { return *this; }

	inline RtFilterFunc filterPtr() const { return m_filter; }
	inline void filterPtr(RtFilterFunc aFilter) { m_filter = aFilter; }
	inline CFilterSlot &operator=(const CFilterSlot &o) {
		if ( this == &o )
			return *this;
		filterPtr(o.filterPtr());
		return *this;
	}
};

//
// CFilterSlot
//
RtToken CFilterSlot::myName() {return RI_EMPTY; }
RtFloat	CFilterSlot::operator()(RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth) const
{
	if ( m_filter )
		return (*m_filter)(x, y, xwidth, ywidth);
	// Error
	return 0;
}

static std::map<RtFilterFunc, CFilterSlot> m_filters;

static IRiRoot *_riRoot = 0;

IRiRoot *_ricppRoot() {
	return _riRoot;
}

void _ricppRoot(IRiRoot *aRoot) {
	_riRoot = aRoot;
}

RtInt _ricppGetArgs(va_list marker, RtToken token, RtToken **tokens, RtPointer **params) {
	static std::vector<RtToken> vec_tokens;   // The tokens of the parameter list of an interface call
	static std::vector<RtPointer> vec_params; // The values of the parameter list of an interface call

	RtInt n;
	RtPointer param;

	assert(tokens != 0);
	assert(params != 0);

	vec_tokens.clear(); *tokens = 0;
	vec_params.clear(); *params = 0;

	for ( n = 0; token != RI_NULL; ++n, token = va_arg(marker, RtToken) ) {
		param = va_arg(marker, RtPointer);
		vec_tokens.push_back(token);
		vec_params.push_back(param);
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

}

using namespace RiCPP;

extern "C" {

// ----------------------------------------------------------------------------
RICPP_INTERN(RtObjectHandle) RiObjectBegin()
{
	return RiCPPObjectBegin(0);
}

RICPP_INTERN(RtVoid) RiObjectEnd(void)
{
	RiCPPObjectEnd();
}

// ----------------------------------------------------------------------------
RICPP_INTERN(RtObjectHandle) RiCPPObjectBegin(RtToken name)
{
	RtObjectHandle h = illObjectHandle;
	PREAMBLE_RET(h)
		h = _ricppRoot()->objectBegin(name);
	POSTAMBLE_RET(h)
}

RICPP_INTERN(RtVoid) RiCPPObjectEnd(void)
{
	PREAMBLE
		_ricppRoot()->objectEnd();
	POSTAMBLE
}

// ----------------------------------------------------------------------------
RICPP_INTERN(RtVoid) RiCPPControl(RtToken name, ...)
{
	PREAMBLE
	GETARGS(name)
		_ricppRoot()->controlV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiCPPControlV(RtToken name, int n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->controlV(name, n, tokens, params);
	POSTAMBLE
}

// ----------------------------------------------------------------------------
RICPP_INTERN(RtToken) RiDeclare (const char *name,const char *declaration)
{
	RtToken h = RI_NULL;
	PREAMBLE_RET(h)
		h = _ricppRoot()->declare(name, declaration);
	POSTAMBLE_RET(h)
}

// ----------------------------------------------------------------------------
RICPP_INTERN(RtVoid) RiSynchronize(RtToken type)
{
	PREAMBLE
		_ricppRoot()->synchronize(type);
	POSTAMBLE
}

// ----------------------------------------------------------------------------
RICPP_INTERN(RtVoid) RiSystem(RtString cmd)
{
	PREAMBLE
		_ricppRoot()->system(cmd);
	POSTAMBLE
}


// ----------------------------------------------------------------------------
RICPP_INTERN(RtVoid) RiFrameBegin(RtInt number)
{
	PREAMBLE
		_ricppRoot()->frameBegin(number);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiFrameEnd(void)
{
	PREAMBLE
	_ricppRoot()->frameEnd();
	POSTAMBLE
}

// ----------------------------------------------------------------------------
RICPP_INTERN(RtVoid) RiWorldBegin(void)
{
	PREAMBLE
		_ricppRoot()->worldBegin();
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiWorldEnd(void)
{
	PREAMBLE
		_ricppRoot()->worldEnd();
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiFormat(RtInt xres, RtInt yres, RtFloat aspect)
{
	PREAMBLE
		_ricppRoot()->format(xres, yres, aspect);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiFrameAspectRatio(RtFloat aspect)
{
	PREAMBLE
		_ricppRoot()->frameAspectRatio(aspect);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	PREAMBLE
		_ricppRoot()->screenWindow(left, right, bot, top);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
{
	PREAMBLE
		_ricppRoot()->cropWindow(xmin, xmax, ymin, ymax);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiProjection(char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		_ricppRoot()->projectionV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiProjectionV(char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->projectionV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiClipping(RtFloat hither, RtFloat yon)
{
	PREAMBLE
		_ricppRoot()->clipping(hither, yon);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
{
	PREAMBLE
		_ricppRoot()->clippingPlane(x, y, z, nx, ny, nz);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
{
	PREAMBLE
		_ricppRoot()->depthOfField(fstop, focallength, focaldistance);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiShutter(RtFloat smin, RtFloat smax)
{
	PREAMBLE
		_ricppRoot()->shutter(smin, smax);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiPixelVariance(RtFloat variation)
{
	PREAMBLE
		_ricppRoot()->pixelVariance(variation);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPixelSamples(RtFloat xsamples, RtFloat ysamples)
{
	PREAMBLE
		_ricppRoot()->pixelSamples(xsamples, ysamples);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiPixelFilter(RtFilterFunc function, RtFloat xwidth, RtFloat ywidth)
{
	PREAMBLE

	if ( function == RiGaussianFilter )
		_ricppRoot()->pixelFilter(CGaussianFilter::func, xwidth, ywidth);
	else if ( function == RiBoxFilter )
		_ricppRoot()->pixelFilter(CBoxFilter::func, xwidth, ywidth);
	else if ( function == RiTriangleFilter )
		_ricppRoot()->pixelFilter(CCatmullRomFilter::func, xwidth, ywidth);
	else if ( function == RiSincFilter )
		_ricppRoot()->pixelFilter(CSincFilter::func, xwidth, ywidth);
	else {
		m_filters[function] = CFilterSlot(function);
		_ricppRoot()->pixelFilter(m_filters[function], xwidth, ywidth);
	}
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiExposure (RtFloat gain, RtFloat gamma)
{
	PREAMBLE
		_ricppRoot()->exposure(gain, gamma);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiImager(char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		_ricppRoot()->imagerV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiImagerV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->imagerV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiQuantize (RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
{
	PREAMBLE
		_ricppRoot()->quantize(type, one, qmin, qmax, ampl);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisplay (char *name, RtToken type, RtToken mode, ...)
{
	PREAMBLE
		GETARGS(mode)
		_ricppRoot()->displayV(name, type, mode, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisplayV (char *name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->displayV(name, type, mode, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisplayChannel (RtToken channel, ...)
{
	PREAMBLE
		GETARGS(channel)
		_ricppRoot()->displayChannelV(channel, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDisplayChannelV (RtToken channel,RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->displayChannelV(channel, n, tokens, params);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiHider (RtToken type, ...)
{
	PREAMBLE
		GETARGS(type)
		_ricppRoot()->hiderV(type, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiHiderV (RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->hiderV(type, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiColorSamples (RtInt N, RtFloat *nRGB, RtFloat *RGBn)
{
	PREAMBLE
		_ricppRoot()->colorSamples(N, nRGB, RGBn);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiRelativeDetail (RtFloat relativedetail)
{
	PREAMBLE
		_ricppRoot()->relativeDetail(relativedetail);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiOption (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		_ricppRoot()->optionV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiOptionV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->optionV(name, n, tokens, params);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiAttributeBegin (void)
{
	PREAMBLE
		_ricppRoot()->attributeBegin();
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiAttributeEnd (void)
{
	PREAMBLE
		_ricppRoot()->attributeEnd();
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiColor (RtColor Cs)
{
	PREAMBLE
		_ricppRoot()->color(Cs);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiOpacity (RtColor Cs)
{
	PREAMBLE
		_ricppRoot()->opacity(Cs);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiTextureCoordinates (RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
{
	PREAMBLE
		_ricppRoot()->textureCoordinates(s1, t1, s2, t2, s3, t3, s4, t4);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtLightHandle) RiLightSource (char *name, ...)
{
	RtLightHandle h = illLightHandle;
	PREAMBLE_RET(h)
		GETARGS(name)
		_ricppRoot()->lightSourceV(name, n, tokens, params);
	POSTAMBLE_RET(h)
}

RICPP_INTERN(RtLightHandle) RiLightSourceV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RtLightHandle h = illLightHandle;
	PREAMBLE_RET(h)
		_ricppRoot()->lightSourceV(name, n, tokens, params);
	POSTAMBLE_RET(h)
}

RICPP_INTERN(RtLightHandle) RiAreaLightSource (char *name, ...)
{
	RtLightHandle h = illLightHandle;
	PREAMBLE_RET(h)
		GETARGS(name)
		_ricppRoot()->areaLightSourceV(name, n, tokens, params);
	POSTAMBLE_RET(h)
}

RICPP_INTERN(RtLightHandle) RiAreaLightSourceV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RtLightHandle h = illLightHandle;
	PREAMBLE_RET(h)
		_ricppRoot()->areaLightSourceV(name, n, tokens, params);
	POSTAMBLE_RET(h)
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiIlluminate (RtLightHandle light, RtBoolean onoff)
{
	PREAMBLE
		_ricppRoot()->illuminate(light, onoff);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSurface (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		_ricppRoot()->surfaceV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSurfaceV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->surfaceV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiAtmosphere (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		_ricppRoot()->atmosphereV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiAtmosphereV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->atmosphereV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiInterior (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		_ricppRoot()->interiorV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiInteriorV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->interiorV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiExterior (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		_ricppRoot()->exteriorV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiExteriorV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->exteriorV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiShadingRate (RtFloat size)
{
	PREAMBLE
		_ricppRoot()->shadingRate(size);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiShadingInterpolation (RtToken type)
{
	PREAMBLE
		_ricppRoot()->shadingInterpolation(type);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiMatte (RtBoolean onoff)
{
	PREAMBLE
		_ricppRoot()->matte(onoff);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiBound (RtBound bound)
{
	PREAMBLE
		_ricppRoot()->bound(bound);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDetail (RtBound bound)
{
	PREAMBLE
		_ricppRoot()->detail(bound);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiDetailRange (RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
{
	PREAMBLE
		_ricppRoot()->detailRange(minvis, lowtran, uptran, maxvis);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiGeometricApproximation (RtToken type, RtFloat value)
{
	PREAMBLE
		_ricppRoot()->geometricApproximation(type, value);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiGeometricRepresentation (RtToken type)
{
	PREAMBLE
		_ricppRoot()->geometricRepresentation(type);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiOrientation (RtToken orientation)
{
	PREAMBLE
		_ricppRoot()->orientation(orientation);
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiReverseOrientation (void)
{
	PREAMBLE
		_ricppRoot()->reverseOrientation();
	POSTAMBLE
}

RICPP_INTERN(RtVoid) RiSides (RtInt nsides)
{
	PREAMBLE
		_ricppRoot()->sides(nsides);
	POSTAMBLE
}

} // extern "C"
