// Adapter ri - ricpp Common for programs and dynamic load procedurals, external IRi
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

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

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

RICPP_EXTERN(RtInt) RiLastError = RIE_NOERROR;

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtObjectHandle) RiObjectBegin()
{
	return RiCPPObjectBegin(0);
}

RICPP_EXTERN(RtVoid) RiObjectEnd(void)
{
	RiCPPObjectEnd();
}

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtObjectHandle) RiCPPObjectBegin(RtToken name)
{
	RtObjectHandle h = illObjectHandle;
	PREAMBLE_RET(h)
		h = _ricppRoot()->objectBegin(name);
	POSTAMBLE_RET(h)
}

RICPP_EXTERN(RtVoid) RiCPPObjectEnd(void)
{
	PREAMBLE
		_ricppRoot()->objectEnd();
	POSTAMBLE
}

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtVoid) RiCPPControl(RtToken name, ...)
{
	PREAMBLE
	GETARGS(name)
		_ricppRoot()->controlV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiCPPControlV(RtToken name, int n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->controlV(name, n, tokens, params);
	POSTAMBLE
}

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtToken) RiDeclare (const char *name,const char *declaration)
{
	RtToken h = RI_NULL;
	PREAMBLE_RET(h)
		h = _ricppRoot()->declare(name, declaration);
	POSTAMBLE_RET(h)
}

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtVoid) RiSynchronize(RtToken type)
{
	PREAMBLE
		_ricppRoot()->synchronize(type);
	POSTAMBLE
}

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtVoid) RiSystem(RtString cmd)
{
	PREAMBLE
		_ricppRoot()->system(cmd);
	POSTAMBLE
}


// ----------------------------------------------------------------------------
RICPP_EXTERN(RtVoid) RiFrameBegin(RtInt number)
{
	PREAMBLE
		_ricppRoot()->frameBegin(number);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiFrameEnd(void)
{
	PREAMBLE
	_ricppRoot()->frameEnd();
	POSTAMBLE
}

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtVoid) RiWorldBegin(void)
{
	PREAMBLE
		_ricppRoot()->worldBegin();
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiWorldEnd(void)
{
	PREAMBLE
		_ricppRoot()->worldEnd();
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_EXTERN(RtVoid) RiFormat(RtInt xres, RtInt yres, RtFloat aspect)
{
	PREAMBLE
		_ricppRoot()->format(xres, yres, aspect);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiFrameAspectRatio(RtFloat aspect)
{
	PREAMBLE
		_ricppRoot()->frameAspectRatio(aspect);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	PREAMBLE
		_ricppRoot()->screenWindow(left, right, bot, top);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
{
	PREAMBLE
		_ricppRoot()->cropWindow(xmin, xmax, ymin, ymax);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiProjection(char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		_ricppRoot()->projectionV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiProjectionV(char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->projectionV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiClipping(RtFloat hither, RtFloat yon)
{
	PREAMBLE
		_ricppRoot()->clipping(hither, yon);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiClippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
{
	PREAMBLE
		_ricppRoot()->clippingPlane(x, y, z, nx, ny, nz);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiDepthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
{
	PREAMBLE
		_ricppRoot()->depthOfField(fstop, focallength, focaldistance);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiShutter(RtFloat smin, RtFloat smax)
{
	PREAMBLE
		_ricppRoot()->shutter(smin, smax);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_EXTERN(RtVoid) RiPixelVariance(RtFloat variation)
{
	PREAMBLE
		_ricppRoot()->pixelVariance(variation);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiPixelSamples(RtFloat xsamples, RtFloat ysamples)
{
	PREAMBLE
		_ricppRoot()->pixelSamples(xsamples, ysamples);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiPixelFilter(RtFilterFunc function, RtFloat xwidth, RtFloat ywidth)
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

RICPP_EXTERN(RtVoid) RiExposure (RtFloat gain, RtFloat gamma)
{
	PREAMBLE
		_ricppRoot()->exposure(gain, gamma);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiImager(char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		_ricppRoot()->imagerV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiImagerV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->imagerV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiQuantize (RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
{
	PREAMBLE
		_ricppRoot()->quantize(type, one, qmin, qmax, ampl);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiDisplay (char *name, RtToken type, RtToken mode, ...)
{
	PREAMBLE
		GETARGS(mode)
		_ricppRoot()->displayV(name, type, mode, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiDisplayV (char *name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->displayV(name, type, mode, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiDisplayChannel (RtToken channel, ...)
{
	PREAMBLE
		GETARGS(channel)
		_ricppRoot()->displayChannelV(channel, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiDisplayChannelV (RtToken channel,RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->displayChannelV(channel, n, tokens, params);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_EXTERN(RtVoid) RiHider (RtToken type, ...)
{
	PREAMBLE
		GETARGS(type)
		_ricppRoot()->hiderV(type, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiHiderV (RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->hiderV(type, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiColorSamples (RtInt N, RtFloat *nRGB, RtFloat *RGBn)
{
	PREAMBLE
		_ricppRoot()->colorSamples(N, nRGB, RGBn);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiRelativeDetail (RtFloat relativedetail)
{
	PREAMBLE
		_ricppRoot()->relativeDetail(relativedetail);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiOption (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		_ricppRoot()->optionV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiOptionV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->optionV(name, n, tokens, params);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_EXTERN(RtVoid) RiAttributeBegin (void)
{
	PREAMBLE
		_ricppRoot()->attributeBegin();
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiAttributeEnd (void)
{
	PREAMBLE
		_ricppRoot()->attributeEnd();
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiColor (RtColor Cs)
{
	PREAMBLE
		_ricppRoot()->color(Cs);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiOpacity (RtColor Cs)
{
	PREAMBLE
		_ricppRoot()->opacity(Cs);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiTextureCoordinates (RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
{
	PREAMBLE
		_ricppRoot()->textureCoordinates(s1, t1, s2, t2, s3, t3, s4, t4);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_EXTERN(RtLightHandle) RiLightSource (char *name, ...)
{
	RtLightHandle h = illLightHandle;
	PREAMBLE_RET(h)
		GETARGS(name)
		_ricppRoot()->lightSourceV(name, n, tokens, params);
	POSTAMBLE_RET(h)
}

RICPP_EXTERN(RtLightHandle) RiLightSourceV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RtLightHandle h = illLightHandle;
	PREAMBLE_RET(h)
		_ricppRoot()->lightSourceV(name, n, tokens, params);
	POSTAMBLE_RET(h)
}

RICPP_EXTERN(RtLightHandle) RiAreaLightSource (char *name, ...)
{
	RtLightHandle h = illLightHandle;
	PREAMBLE_RET(h)
		GETARGS(name)
		_ricppRoot()->areaLightSourceV(name, n, tokens, params);
	POSTAMBLE_RET(h)
}

RICPP_EXTERN(RtLightHandle) RiAreaLightSourceV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	RtLightHandle h = illLightHandle;
	PREAMBLE_RET(h)
		_ricppRoot()->areaLightSourceV(name, n, tokens, params);
	POSTAMBLE_RET(h)
}

// ----------------------------------------------------------------------------

RICPP_EXTERN(RtVoid) RiIlluminate (RtLightHandle light, RtBoolean onoff)
{
	PREAMBLE
		_ricppRoot()->illuminate(light, onoff);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiSurface (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		_ricppRoot()->surfaceV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiSurfaceV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->surfaceV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiAtmosphere (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		_ricppRoot()->atmosphereV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiAtmosphereV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->atmosphereV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiInterior (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		_ricppRoot()->interiorV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiInteriorV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->interiorV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiExterior (char *name, ...)
{
	PREAMBLE
		GETARGS(name)
		_ricppRoot()->exteriorV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiExteriorV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	PREAMBLE
		_ricppRoot()->exteriorV(name, n, tokens, params);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiShadingRate (RtFloat size)
{
	PREAMBLE
		_ricppRoot()->shadingRate(size);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiShadingInterpolation (RtToken type)
{
	PREAMBLE
		_ricppRoot()->shadingInterpolation(type);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiMatte (RtBoolean onoff)
{
	PREAMBLE
		_ricppRoot()->matte(onoff);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_EXTERN(RtVoid) RiBound (RtBound bound)
{
	PREAMBLE
		_ricppRoot()->bound(bound);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiDetail (RtBound bound)
{
	PREAMBLE
		_ricppRoot()->detail(bound);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiDetailRange (RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
{
	PREAMBLE
		_ricppRoot()->detailRange(minvis, lowtran, uptran, maxvis);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiGeometricApproximation (RtToken type, RtFloat value)
{
	PREAMBLE
		_ricppRoot()->geometricApproximation(type, value);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiGeometricRepresentation (RtToken type)
{
	PREAMBLE
		_ricppRoot()->geometricRepresentation(type);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiOrientation (RtToken orientation)
{
	PREAMBLE
		_ricppRoot()->orientation(orientation);
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiReverseOrientation (void)
{
	PREAMBLE
		_ricppRoot()->reverseOrientation();
	POSTAMBLE
}

RICPP_EXTERN(RtVoid) RiSides (RtInt nsides)
{
	PREAMBLE
		_ricppRoot()->sides(nsides);
	POSTAMBLE
}

} // extern "C"
